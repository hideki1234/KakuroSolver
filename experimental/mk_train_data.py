#!/usr/bin/env python
import os
from glob import glob
from Tkinter import *

def_num = '0'

class Application(Frame):

    def set_value(self):
        lb = self.num_lb.get()
        ur = self.num_ur.get()
        print "{}: {} {}".format(self.image_name, lb, ur)
        self.num_lb.delete(0, len(lb))
        self.num_lb.insert(0, def_num)
        self.num_ur.delete(0, len(ur))
        self.num_ur.insert(0, def_num)
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

    def show_image(self):
        try:
            self.image_name = self.image_i.next()
            self.image_data = PhotoImage(file = self.image_name)
            self.image_frame.configure(image = self.image_data)
            self.num_lb.focus_set()
        except StopIteration:
            self.quit()

    def __init__(self, images, master=None):
        Frame.__init__(self, master)
        self.image_i = images.__iter__()
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

root = Tk()
app = Application(flist, root)
app.mainloop()
root.destroy()
