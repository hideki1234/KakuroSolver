#!/usr/bin/env python
"""
Shows images in a directory specified by an env var, KAKURO_CELL_DIR
You enter clue values for each image. Values are 0, or 3 to 45. 0 means no clue.
The result is a csv file. Each row has an image file name, lower left clue,
and upper right clue.
"""

import os
import csv
import shutil
from glob import glob
from Tkinter import *

def_num = '0'

class Application(Frame):

    def set_value(self):
        lb = self.num_lb.get()
        ur = self.num_ur.get()
        # validate values
        try:
            lb_v = int(lb)
            ur_v = int(ur)
            if not (lb_v == 0 or (lb_v >=3 and lb_v <= 45)):
                raise ValueError('lower left value out of range')
            if not (ur_v == 0 or (ur_v >=3 and ur_v <= 45)):
                raise ValueError('upper right value out of range')
        except ValueError as e:
            print e
            self.set_initial_input_state()
        else:
            print "{}: {},{}".format(self.image_name, lb, ur)
            self.writer.writerow([self.image_name, lb_v, ur_v])
            self.move_done()
            self.show_image()

    def createWidgets(self):
        # Quit button
        self.QUIT = Button(self)
        self.QUIT["text"] = 'Quit'
        self.QUIT["fg"] = 'red'
        self.QUIT['command'] = self.quit

        self.QUIT.pack({'side': 'left'})

        # image
        self.image_data = PhotoImage(width=32, height=32)
        self.image_frame = Label(self, image = self.image_data)
        self.image_frame.pack({'side': 'left'})

        # lower left clue
        self.num_lb = Entry(self)
        self.num_lb.insert(0, def_num)

        self.num_lb.pack({'side': 'left'})

        # upper right clue
        self.num_ur = Entry(self)
        self.num_ur.insert(0, def_num)

        self.num_ur.pack({'side': 'left'})

        self.hi_there = Button(self)
        self.hi_there['text'] = 'Set'
        self.hi_there['command'] = self.set_value

        self.hi_there.pack({'side': 'left'})

        # show the first image
        self.show_image()

    def set_initial_input_state(self):
        lb = self.num_lb.get()
        ur = self.num_ur.get()
        self.num_lb.delete(0, len(lb))
        self.num_lb.insert(0, def_num)
        self.num_ur.delete(0, len(ur))
        self.num_ur.insert(0, def_num)
        self.num_lb.focus_set()
        self.num_lb.select_range(0,len(def_num))

    def move_done(self):
        shutil.move(self.image_name, self.dn_done)

    def show_image(self):
        try:
            self.image_name = self.image_i.next()
            self.image_data = PhotoImage(file = self.image_name)
            self.image_frame.configure(image = self.image_data)
            self.set_initial_input_state()
        except StopIteration:
            self.quit()

    def __init__(self, master, images, f_out, dn_done):
        Frame.__init__(self, master)
        self.dn_done = dn_done
        self.image_i = images.__iter__()
        self.writer = csv.writer(f_out)
        self.master.title('Make training data')
        self.pack()
        self.createWidgets()

# set up the directory to store training images
cell_dir_name = 'KAKURO_CELL_DIR'
cell_data_dir = os.getenv(cell_dir_name)
if cell_data_dir == None:
    print 'Please set an environment variable: ', cell_dir_name
    sys.exit(1)
flist = glob(os.path.join(cell_data_dir, '*.pgm'))
fn_out = os.path.join(cell_data_dir, '0_train_set.csv')
dn_done = os.path.join(cell_data_dir, 'done')

filemode = 'ab' if os.path.exists(fn_out) else 'wb'
with open(fn_out, filemode) as f_out:
    root = Tk()
    app = Application(root, flist, f_out, dn_done)
    app.mainloop()
    root.destroy()
