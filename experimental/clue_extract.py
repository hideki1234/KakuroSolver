#!/usr/bin/env python

import cv2
import numpy as np
from cell_detector import kakuroCellParser

class clue_extractor(object):
    """
    extracts clue cell images from a kakuro image
    """

    def __init__(self, img):
        parser = kakuroCellParser(img)
        self._kakuros = parser.get_frames()
        self._img = cv2.cvtColor(parser.image, cv2.COLOR_BGR2GRAY)
        self._frame_no = -1
        self._x_i = -1
        self._y_i = -1
        self._x_max = 0
        self._y_max = 0

    def __iter__(self):
        return self

    def next(self):
        while True:
            self._x_i = self._x_i + 1
            if self._x_i == self._x_max:
                self._x_i = 0
                self._y_i = self._y_i + 1
                if self._y_i == self._y_max:
                    self._y_i = 0
                    self._frame_no = self._frame_no + 1
                    if self._frame_no == len(self._kakuros):
                        raise StopIteration
                    out, self._side, self._cells = self._kakuros[self._frame_no]
                    self._y_max = len(self._cells)
                    self._x_max = len(self._cells[0])
                    print out,
                    print 'max x, y = ', self._x_max, self._y_max
            print self._x_i, self._y_i,
            x, y, b_clue = self._cells[self._y_i][self._x_i]
            print x, y
            if b_clue:
                a_cell = self._img[y:y+self._side, x:x+self._side]
                return a_cell

if __name__ == '__main__':
    import sys
    flist = sys.argv[1:]
    if len(flist) == 0:
        import os
        from glob import glob
        flist = glob(os.path.join('..', 'testimage', '*.*'))
    for fn in flist:
        img = cv2.imread(fn)
        extractor = clue_extractor(img)
        for cell in extractor:
            cv2.imshow('Kakuro', cell)
            ch = 0xFF & cv2.waitKey()
        cv2.destroyAllWindows()


