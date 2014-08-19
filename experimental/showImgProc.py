#!/usr/bin/env python

import cv2
import numpy as np
import sys

def _angle_cos(p0, p1, p2):
    d1, d2 = (p0-p1).astype('float'), (p2-p1).astype('float')
    return np.dot(d1, d2) / np.sqrt( np.dot(d1, d1)*np.dot(d2, d2) )


def showImageProcess(img):
    h, w, c = img.shape
    if h > 1000:
        w = w * 1000 / h
        h = 1000
        img = cv2.resize(img, (w,h))
    cv2.imshow('Original', img)
    ch = 0xFF & cv2.waitKey()
    
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    cv2.imshow('Grayed', gray)
    ch = 0xFF & cv2.waitKey()

    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    cv2.imshow('Blurred', blur)
    ch = 0xFF & cv2.waitKey()

    dilate = cv2.dilate(blur, None)
    cv2.imshow('Dilated', dilate)
    ch = 0xFF & cv2.waitKey()

    thresh = cv2.adaptiveThreshold(
            dilate,
            maxValue=255,
            adaptiveMethod=cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
            thresholdType = cv2.THRESH_BINARY,
            blockSize =5,
            C = 1)
    cv2.imshow('Edged', thresh)
    ch = 0xFF & cv2.waitKey()

    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    print 'base contours=', len(contours)
    squares = []
    for cnt in contours:
        cnt_len = cv2.arcLength(cnt, True)
        cnt = cv2.approxPolyDP(cnt, 0.02*cnt_len, True)
        if len(cnt) == 4 and cv2.contourArea(cnt) > 1000 and cv2.isContourConvex(cnt):
            cnt2 = cnt.reshape(-1, 2)
            max_cos = np.max([abs(_angle_cos( cnt2[i], cnt2[(i+1) % 4], cnt2[(i+2) % 4] )) for i in xrange(4)])
            if max_cos < 0.1:
                squares.append(cnt)
    print 'squares=', len(squares)
    if len(squares) > 0:
        cont = img.copy()
        cv2.drawContours(cont, squares, -1, (0,0,255), 1)
        cv2.imshow('Contours', cont)
        ch = 0xFF & cv2.waitKey()

    cv2.destroyAllWindows()

if __name__ == '__main__':
    flist = sys.argv[1:]
    if len(flist) == 0:
        import os
        from glob import glob
        flist = glob(os.path.join('~', 'Documents', 'kakuro', 'testdata', '*.png'))
    for fn in flist:
        print fn
        img = cv2.imread(fn)
        showImageProcess(img)
