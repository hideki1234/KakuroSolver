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
        outer_frame, cell_side, xs, ys = org_frame
        print "_parse_a_frame: not implemented yet"
        print outer_frame, cell_side, len(xs), len(ys)
        # get dark ness of cells
        cells = []
        avg = 0
        for y in ys:
            row = []
            for x in xs:
                brightness = np.int64(0)
                for yofs in xrange(cell_side):
                    for xofs in xrange(cell_side):
                        brightness = brightness + img[y + yofs][x + xofs]
                brightness = brightness / cell_side ** 2
                row.append( (x, y, brightness) )
                avg = avg + brightness
            cells.append( row )
        avg = avg / (len(xw) * len(ys))


    @property
    def image(self):
        return self._img


if __name__ == '__main__':
    import sys
    flist = sys.argv[1:]
    if len(flist) == 0:
        import os
        from glob import glob
        flist = glob(os.path.join('~', 'Documents', 'kakuro', 'testdata', '*.jpg'))
    for fn in flist:
        img = cv2.imread(fn)
        detector = kakuroCellParser(img)
        frames = detector.get_frames()
        print 'Frames:'
        print frames
        img = detector.image.copy()
        h, w, c = img.shape
        if h > 1000:
            w = w * 1000 / h
            h = 1000
            img = cv2.resize(img, (w,h))
        cv2.imshow('Kakuro', img)
        ch = 0xFF & cv2.waitKey()
        cv2.destroyAllWindows()


