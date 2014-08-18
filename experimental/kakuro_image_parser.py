import cv2
import numpy as np
import math

def _angle_cos(p0, p1, p2):
    d1, d2 = (p0-p1).astype('float'), (p2-p1).astype('float')
    return np.dot(d1, d2) / np.sqrt( np.dot(d1, d1)*np.dot(d2, d2) )


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
                max_cos = np.max([abs(_angle_cos( cnt2[i], cnt2[(i+1) % 4], cnt2[(i+2) % 4] )) for i in xrange(4)])
                if max_cos < 0.1:
                    squares.append(cnt)
        return squares

    def _get_slant(self):
        squares = self._find_squares()
        square_size = [ (cv2.contourArea(square), square) for square in squares ]
        square_size = sorted(square_size, key = lambda s: s[0], reverse=True)
        # take the second largest as the largest may be a image frame
        square = square_size[1][1]
        p1 = square[0][0]
        p2 = square[1][0]
        print '**** _get_slant'
        print '\t', p1, p2
        if abs(p1[0] - p2[0]) <= abs(p1[1] - p2[1]):
            # vertical
            if p2[1] < p1[1]:
                tmp = p1
                p1 = p2
                p2 = tmp
            v = np.array([p1[0], p2[1]])
            slant = _angle_cos(v, p1, p2)
        else:
            # horizontal
            if p2[0] < p1[0]:
                tmp = p1
                p1 = p2
                p2 = tmp
            v = np.array([p2[0], p1[1]])
            slant = _angle_cos(v, p1, p2)
        print '\t', slant
        slant = math.degrees(math.acos(slant))
        print '\t', slant
        return slant

    def _rotate_img(self, degree):
        h, w, c  = self._img.shape
        print '\t', w, h, c
        rotmat = cv2.getRotationMatrix2D((w/2, h/2), degree, 1.0)
        self._img = cv2.warpAffine(self._img, rotmat, (w, h))

    def _get_possible_frames(self):
        raw_squares = self._find_squares()
        squares = [(cv2.contourArea(raw_square), cv2.boundingRect(raw_square)) for raw_square in raw_squares]
        squares = sorted(squares, key = lambda s: s[0], reverse=True)
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
        for f in frames.keys():
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
        degree = self._get_slant()
        self._rotate_img(-degree)
        frames = self._get_possible_frames()
        first = frames.keys()[0][1]
        print first
        cos = _angle_cos(
                np.array([first[0], first[3]]),
                np.array([first[0], first[1]]),
                np.array([first[2], first[1]]) )
        print 'Angle: ', cos
        print 'frames: ', len(frames)
        for f in frames:
            print '\t', f, len(frames[f])
        self._normalize_frames(frames)

    def get_frames(self):
        if not self._b_parsed:
            self._parse()
            self._b_parsed = True
        return self._frames

    @property
    def image(self):
        return self._img

if __name__ == '__main__':
    def draw_frame(img, frame):
        print frame[0]
        f = [frame[0][0], frame[0][1], frame[0][2], frame[0][3]]
        cv2.rectangle( img,
                (frame[0][0], frame[0][1]),
                (frame[0][0]+frame[0][2], frame[0][1]+frame[0][3]),
                (255, 0, 0), 3 )
        side = frame[1]
        for x in frame[2]:
            for y in frame[3]:
                cv2.rectangle( img, (x, y), (x + side, y + side), (0, 0, 255), 1)

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
        img = parser.image.copy()
        for frame in frames:
            draw_frame(img, frame)
        h, w, c = img.shape
        if h > 1000:
            w = w * 1000 / h
            h = 1000
            img = cv2.resize(img, (w,h))
        cv2.imshow('Kakuro', img)
        ch = 0xFF & cv2.waitKey()
        cv2.destroyAllWindows()

