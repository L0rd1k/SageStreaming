import cv2 as cv
import numpy as np
import os
from shutil import copyfile
import random

def copyImages():
    src_directory = "/home/ilya/baisNet/seg_voskresensk2/JPEGImages"
    dst_directory = "/home/ilya/baisNet/FoldersForTrain/val"
    folder = "/home/ilya/baisNet/seg_voskresensk2/SegmentationClass"
    itr = 309 # 1 77 196

    for filename in os.listdir(folder):
        copyfile(src_directory + "/" + filename[0:-3] + "jpg", dst_directory + "/{0}.jpg".format(itr))
       
        copyfile(folder + "/" + filename, "/home/ilya/baisNet/FoldersForTrain/val_labels" + "/{0}.png".format(itr))
        print(filename)
        itr+=1

def getRandomImages():
    imgList = list()
    randNum = None
    for itr in range(20):
        while True:
            randNum = random.randint(1, 427)
            if randNum is not imgList:
                imgList.append(randNum)
                copyfile("/home/ilya/baisNet/FoldersForTrain/train/{0}.jpg".format(randNum), "/home/ilya/baisNet/FoldersForTrain/val/{0}.jpg".format(randNum))
                copyfile("/home/ilya/baisNet/FoldersForTrain/train_labels/{0}.png".format(randNum), "/home/ilya/baisNet/FoldersForTrain/val_labels/{0}.png".format(randNum))
                os.remove("/home/ilya/baisNet/FoldersForTrain/train/{0}.jpg".format(randNum))
                os.remove("/home/ilya/baisNet/FoldersForTrain/train_labels/{0}.png".format(randNum))
                break
        print(randNum)

getRandomImages()
#copyImages()
