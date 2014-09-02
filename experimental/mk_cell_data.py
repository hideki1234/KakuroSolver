#!/usr/bin/env python

import sys
import os
import cv2
from clue_extract import clueExtractor

# set up the directory to store training images
cell_dir_name = 'KAKURO_CELL_DIR'
cell_data_dir = os.getenv(cell_dir_name)
if cell_data_dir == None:
    print 'Please set an environment variable: ', cell_dir_name
    sys.exit(1)

# get the source kakukro images
source_dir_name = 'KAKURO_IMAGE_DIR'

flist = sys.argv[1:]
if len(flist) == 0:
    source_dir = os.getenv(source_dir_name)
    if source_dir == None:
        print 'No source images or source directory environment specified'
        sys.exit(1)
    from glob import glob
    flist = glob(os.path.join('..', 'testimage', '*.*'))

base_name = 0
for fn in flist:
    img = cv2.imread(fn)
    print '**** ', fn, ' ****'
    for cell in clueExtractor(img):
        fn_data = str(base_name) + '.png'
        base_name = base_name + 1
        path_data = os.path.join(cell_data_dir, fn_data)
        cv2.imwrite(path_data, cell)
