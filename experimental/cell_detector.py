#!/usr/bin/env python

import cv2
import numpy as np
from kakuro_image_parser import kakuroImageParser

class kakuroCellParser(object):
    """
    Detects cells from kakuro frames and figure out the type of the cells
    (clue cell or answer cell).
    """
    
    def __init__(self, img):
        self._img = img
        self._frames = None

    def get_frames(self):
        if self._frames == None:
            self._parse()
        return self._frames

    def _parse(self):
        self._frames = []
        frame_detector = kakuroImageParser(self._img)
        org_frames = frame_detector.get_frames()
        self._img = frame_detector.image # get an image upright
        gray_img = cv2.cvtColor(self._img, cv2.COLOR_BGR2GRAY)
        frame_detector = None

        for org_frame in org_frames:
            self._parse_a_frame(org_frame, gray_img)

        return self._frames

    def _parse_a_frame(self, org_frame, img):
        '''
        frame fromat:
            ( (top left x, tl y, bottom right x, br y),
              cell_side,
              [
                    [ (x11, y11, b_clue), (x12, y12, b_clue), ... , (x1n, y1n, b_clue) ],
                                         ...
                    [ (xm1, ym1, b_clue), (xm2, ym2, b_clue), ... , (xmn, ymn, b_clue) ]
              ]
            )
        '''
        def _cell_brightness():
            brightness = np.int64(0)
            for yofs in xrange(y, y+cell_side):
                for xofs in xrange(x, x+cell_side):
                    brightness = brightness + img[yofs][xofs]
            brightness = brightness / cell_side ** 2
            return brightness

        outer_frame, cell_side, xs, ys = org_frame
        # get dark ness of cells
        brs = []
        avg = 0
        for y in ys:
            row = []
            for x in xs:
                brightness = _cell_brightness()
                row.append( brightness )
                avg = avg + brightness
            brs.append( row )
        avg = avg / (len(xs) * len(ys))
        
        cells = [ [ (xs[j], ys[i], brs[i][j] < avg) for j in xrange(len(xs)) ]
                for i in xrange(len(ys)) ]

        self._frames.append( (outer_frame, cell_side, cells) )

    @property
    def image(self):
        return self._img


if __name__ == '__main__':
    def draw_frame(img, frame):
        print frame[0]
        # draw frame
        f = [frame[0][0], frame[0][1], frame[0][2], frame[0][3]]
        cv2.rectangle( img,
                (frame[0][0], frame[0][1]),
                (frame[0][0]+frame[0][2], frame[0][1]+frame[0][3]),
                (0, 255, 0), 3 )
        side = frame[1]
        for row in frame[2]:
            for x,y,b_clue in row:
                color, thick = ((0, 0, 255), 2) if b_clue else ((255, 0, 0), 1)
                cv2.rectangle( img, (x, y), (x + side, y + side), color, thick)

    import sys
    flist = sys.argv[1:]
    if len(flist) == 0:
        import os
        from glob import glob
        flist = glob(os.path.join('..', 'testimage', '*.*'))
    for fn in flist:
        img = cv2.imread(fn)
        detector = kakuroCellParser(img)
        frames = detector.get_frames()
        print 'Frames:'
        print frames
        img = detector.image.copy()
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


