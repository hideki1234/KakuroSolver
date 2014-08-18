import cv2
import numpy as np

def _angle_cos(p0, p1, p2):
    d1, d2 = (p0-p1).astype('float'), (p2-p1).astype('float')
    return abs( np.dot(d1, d2) / np.sqrt( np.dot(d1, d1)*np.dot(d2, d2) ) )


def _average_x_y(squares):
    x = 0
    y = 0
    for square in squares:
        x = x + square[1][2]
        y = y + square[1][3]
    return x / len(squares), y / len(squares)


def _extract_sides(raw_sides, err):
    sum_sides = 0
    count = 0
    min_side = 0
    sides = []
    for side in raw_sides:
        if count == 0:
            min_side = side
            sum_sides = side
            count = 1
        elif side > min_side + err:
            sides.append( sum_sides/count )
            min_side = side
            sum_sides = side
            count = 1
        else:
            sum_sides = sum_sides + side
            count = count + 1
    if count != 0:
        sides.append( sum_sides/count )
    return sides

class kakuroImageParser(object):

    def __init__(self, img):
        self._img = img
        self._b_parsed = False
        self._frames = []

    def _find_squares(self):
        img = cv2.cvtColor(self._img, cv2.COLOR_BGR2GRAY)
        img = cv2.GaussianBlur(img, (3, 3), 0)
        squares = []
        img = cv2.dilate(img, None)
        img = cv2.adaptiveThreshold(
                img,
                maxValue=255,
                adaptiveMethod=cv2.ADAPTIVE_THRESH_MEAN_C,
                thresholdType = cv2.THRESH_BINARY,
                blockSize =5,
                C = 2)
        contours, hierarchy = cv2.findContours(img, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        for cnt in contours:
            cnt_len = cv2.arcLength(cnt, True)
            cnt = cv2.approxPolyDP(cnt, 0.02*cnt_len, True)
            if len(cnt) == 4 and cv2.contourArea(cnt) > 1000 and cv2.isContourConvex(cnt):
                cnt2 = cnt.reshape(-1, 2)
                max_cos = np.max([_angle_cos( cnt2[i], cnt2[(i+1) % 4], cnt2[(i+2) % 4] ) for i in xrange(4)])
                if max_cos < 0.1:
                    squares.append(cnt)
        return squares

    def _get_possible_frames(self):
        raw_squares = self._find_squares()
        squares = []
        for raw_square in raw_squares:
            squares.append( (cv2.contourArea(raw_square), cv2.boundingRect(raw_square)) )
        squares = sorted(squares, key = lambda s: s[0], reverse=True)
        print 'squares in image'
        for s in squares:
            print '\t',s
        frames = dict()
        for s in squares:
            parent = None
            for p in iter(frames):
                if ( p[1][0] <= s[1][0] and p[1][0] + p[1][2] >= s[1][0] + s[1][2] and
                        p[1][1] <= s[1][1] and p[1][1] + p[1][3] >= s[1][1] + s[1][3]):
                    parent = p
                    break
            if parent != None:
                if 3 * s[1][2] < parent[1][2] and 3 * s[1][3] < parent[1][3]:
                    frames[parent].append(s)
                    continue
                else:
                    del frames[parent]
            frames[s] = []
        for f in iter(frames):
            if len(frames[f]) == 0:
                del frames[f]
        return frames

    def _normalize_frames(self, raw_frames):
        for frame in raw_frames:
            width, height = _average_x_y(raw_frames[frame])
            wh = 2.0 * width / (width + height)
            print 'width=', width, ', height=', height, ', wh=', wh
            if wh < 1.0/1.05 or 1.05 < wh:
                continue
            side = (width + height) / 2
            err = side / 2
            xs = []
            ys = []
            for cell in raw_frames[frame]:
                xs.append(cell[1][0])
                ys.append(cell[1][1])
            xs = sorted(xs)
            ys = sorted(ys)
            print xs
            print ys
            xs = _extract_sides(xs, err)
            ys = _extract_sides(ys, err)
            print len(xs), xs
            print len(ys), ys
            self._frames.append( (frame[1], side, xs, ys) )

    def _parse(self):
        frames = self._get_possible_frames()
        print 'frames: ', len(frames)
        for f in frames:
            print '\t', f, len(frames[f])
        self._normalize_frames(frames)

    def get_frames(self):
        if not self._b_parsed:
            self._parse()
            self._b_parsed = True
        return self._frames

if __name__ == '__main__':
    import sys
    flist = sys.argv[1:]
    if len(flist) == 0:
        import os
        from glob import glob
        flist = glob(os.path.join('~', 'Documents', 'kakuro', 'testdata', '*.png'))
    for fn in flist:
        img = cv2.imread(fn)
        parser = kakuroImageParser(img)
        frames = parser.get_frames()
        print 'Frames:'
        print frames
        for frame in frames:
            print frame[0]
            f = [frame[0][0], frame[0][1], frame[0][2], frame[0][3]]
            cv2.rectangle( img,
                    (frame[0][0], frame[0][1]),
                    (frame[0][0]+frame[0][2], frame[0][1]+frame[0][3]),
                    (255, 0, 0), 3 )
            
        cv2.imshow('Kakuro', img)
        ch = 0xFF & cv2.waitKey()
        cv2.destroyAllWindows()


