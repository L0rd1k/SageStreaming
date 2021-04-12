import cv2 as cv
import numpy as np
import os
import random
import time
import glob
import getopt
import sys
import argparse
import numpy as np
from skimage import measure
from TimeProfiler import timeProfile

class YOLODetector:
    #--------------------------------------
    # Init main class params
    #--------------------------------------
    def __init__(self , currentRegime):
        print("Current Regime: {0}".format(currentRegime))
        self.__currentRegime = currentRegime
        self.__confidence = 0.1  #Confidence threshold
        self.__threshold = 0.1  #Non-maximum suppression threshold
        self.__inpWidth, self.__inpHeight  = 416, 416     #Width of network's input image
        self.__classesNameFile = "/opt/data/Ilya/YOLO/FarmBuilding/objects.txt"
        self.__modelConfiguration = "/opt/data/Ilya/YOLO/FarmBuilding/navigation.cfg"
        self.__modelWeights = "/opt/data/Ilya/YOLO/FarmBuilding/weights3/navigation_40000.weights"           
        self.__net = None
        self.__classes = None
        self.__colors = None
        self.__videoOut = None
        self.__testImage = cv.imread('/opt/data/Ilya/testImg.jpg',cv.IMREAD_GRAYSCALE)
        

        # self.__randomMapSlicer("/opt/data/Проекты/ОЭС-ОН/maps/voskresensk/voskr_16.jpg", "/opt/data/Ilya/Script/Images6/")
        #for elem in range(20):
        #    self.__randomMapSlicer("/opt/data/Проекты/ОЭС-ОН/maps/protasovo/map17.jpg", "/opt/data/Ilya/Script/Images6/")
        
        for elem in range(50):
            self.__cutImages()
        
        # self.generateDataSetFromImage("/opt/data/Проекты/ОЭС-ОН/maps/protasovo/map17.jpg", "/opt/data/Ilya/NaviagtionSeg/Images6/", 1000)
        # self.generateDataSetFromImage("/opt/data/Проекты/ОЭС-ОН/maps/voskresensk/voskr_16.jpg", "/opt/data/Ilya/NaviagtionSeg/Images5/", 1000)
        # self.__passYOLOConfig()   
        #                  
    @timeProfile('__cutImages time: %d ms')
    def __cutImages(self):
        folder = "/media/ilya/SSDDisk/Datasets/FarmBuilding/NEG/"
        for filename in os.listdir(folder):
            # print(filename)
            image = cv.imread(folder + filename, cv.IMREAD_COLOR)
            image = image[0 : image.shape[1], 0 : image.shape[1]]
            # cv.imshow("Image", image)
            image = cv.resize(image, (512,512))
            cv.imwrite('{0}{1}'.format('/opt/data/Ilya/Images/', filename), image)
            # cv.waitKey(0)
    

    @timeProfile('__randomMapSlicer time: %d ms')
    def __randomMapSlicer(self, mapPath, outPath):
        originalMap = cv.imread(mapPath, cv.IMREAD_COLOR) 
        hMap, wMap, cMap = originalMap.shape      
        print("Height: {0}, Width: {1}, Channel: {2}".format(hMap, wMap, cMap))
        
        pos_H, pos_W = 200, 200
        imageName = 0
        while (pos_H < hMap - 200) and (pos_W < wMap - 200):
            imageName+=1
            img_size = random.randint(500, 800)
            rand_step = random.randint(300, 400)
            croppedImg = originalMap[pos_H : pos_H + img_size, pos_W : pos_W + img_size]
            croppedImg = cv.resize(croppedImg, (512,512))
            cv.imwrite('{0}{1}{2}'.format('/opt/data/Ilya/NaviagtionSeg/Images6/', imageName, '.jpg'), croppedImg)
            pos_W = pos_W + rand_step
            if pos_W > wMap - 200:
                pos_W = 200
                if pos_H < hMap - 200:
                    pos_H = pos_H + rand_step
                else:
                    break

    #--------------------------------------
    # Generate random sized images from map
    #--------------------------------------
    def generateDataSetFromImage(self, mapPath, outPath, imagesNumber):
        print("....Generate {0} images from map....".format(imagesNumber))
        originalMap = cv.imread(mapPath, cv.IMREAD_COLOR) 
        hMap, wMap, cMap = originalMap.shape
        print("Height: {0}, Width: {1}, Channel: {2}".format(hMap, wMap, cMap))
        for elem in range(imagesNumber):
            rndX, rndY = random.randint(0, wMap - 1000), random.randint(0, hMap - 1000)
            rndW, rndH = random.randint(400, 700), random.randint(500, 700)
            print("rndX: {0}, rndY: {1}, rndW: {2}, rndH: {3}".format(rndX, rndY, rndW, rndH))
            croppedImg = originalMap[rndY : rndY + rndH, rndX : rndX + rndH]
            croppedImg = cv.resize(croppedImg, (512,512))
            cv.imwrite('{0}{1}{2}'.format('/opt/data/Ilya/NaviagtionSeg/Images6/', elem, '.jpg'), croppedImg)


    #--------------------------------------
    # Get classe's names from objects.txt file 
    #--------------------------------------
    def __extractClassesName(self):
        with open(self.__classesNameFile, 'rt') as f:
            self.__classes = f.read().rstrip('\n').split('\n')
        print(self.__classes)


    #--------------------------------------
    # Randomly generate color for bounding box of every given class
    #--------------------------------------
    def __initColors(self):
        np.random.seed(42)
        self.__colors = np.random.randint(0, 255, size=(len(self.__classes), 3), dtype="uint8")

    #--------------------------------------
    # Getting bounding boxes and associated probabilities from YOLO detector
    #--------------------------------------
    def __blopingImage(self, frame):
        ln = self.__net.getLayerNames()
        ln = [ln[i[0] - 1] for i in self.__net.getUnconnectedOutLayers()]
        blob = cv.dnn.blobFromImage(frame, 1/255, (self.__inpWidth, self.__inpHeight), [0,0,0], swapRB=True, crop=False)
        self.__net.setInput(blob) 
        start = time.time()
        layerOutputs = self.__net.forward(ln)
        print("[INFO] YOLO took {:.6f} seconds".format(time.time() - start))
        return layerOutputs
    
    #--------------------------------------
    # Filtering out bounding boxes 
    #--------------------------------------
    def __initBoundingBox(self, layerOutputs, W, H):
        boxes = []
        confidences = []
        classIDs = []
        for output in layerOutputs:
            for detection in output:
                scores = detection[5:]
                classID = np.argmax(scores)
                confidence = scores[classID]
                if confidence > self.__confidence:
                    box = detection[0:4] * np.array([W, H, W, H])
                    (centerX, centerY, width, height) = box.astype("int")
                    x = int(centerX - (width / 2))
                    y = int(centerY - (height / 2))
                    boxes.append([x, y, int(width), int(height)])
                    confidences.append(float(confidence))
                    classIDs.append(classID)
        print("Scores:{0},ClassID:{1},Confidence:{2}".format(scores,classID,confidence))
        idxs = cv.dnn.NMSBoxes(boxes, confidences, self.__confidence, self.__threshold)
        print(idxs)
        return idxs, boxes, confidences, classIDs

    #--------------------------------------
    # Drawing resulting annotation for every frame
    #--------------------------------------
    def __annotateFrame(self, idxs, boxes, confidences, classIDs, frame, filename):
        testFrame = None
        if len(idxs) > 0:
            for i in idxs.flatten():
                (x, y) = (boxes[i][0], boxes[i][1])
                (w, h) = (boxes[i][2], boxes[i][3])   
                color = [int(c) for c in self.__colors[classIDs[i]]]
                cv.rectangle(frame, (x, y), (x + w, y + h), color, 2)

                if((x >= 0 and x <= frame.shape[1]) and (y >= 0 and y <= frame.shape[0])):
                    print(frame.shape)
                    print(x,y,w,h)
                    testFrame = frame[y : y + h,x :x + w]
                text = "{}: {:.4f}".format(self.__classes[classIDs[i]], confidences[i])
                cv.putText(frame, text, (x, y - 5), cv.FONT_HERSHEY_SIMPLEX,0.5, color, 2)

            cv.imshow("DETECTED: ", frame)    
            cv.imwrite('{0}{1}{2}'.format('./WrongDetect/', filename, ".jpg"), frame)
            if testFrame is not None:
                self.__compare_image(testFrame, frame)
                grayFrame = cv.cvtColor(testFrame, cv.COLOR_RGB2GRAY)
                sift = cv.SIFT_create()
                kp1, des1 = sift.detectAndCompute(self.__testImage,None)
                kp2, des2 = sift.detectAndCompute(grayFrame,None)
                bf = cv.BFMatcher()
                matches = bf.knnMatch(des1,des2,k=2)
                good = []
                for m,n in matches:
                    if m.distance < 0.75 * n.distance:
                        good.append([m])
                img3 = cv.drawMatchesKnn(self.__testImage, kp1, grayFrame, kp2, good[:10], None, flags = cv.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
            testFrame = None
            
        print(self.__currentRegime)
        if self.__currentRegime is False:
            self.__videoOut.write(frame)

    #--------------------------------------
    # Detection by video input
    #--------------------------------------
    def __videoDetection(self):
        # cap = cv.VideoCapture("/opt/data/Ilya/run_earthMap2.mp4")
        # cap = cv.VideoCapture("/opt/data/Ilya/YOLO/FarmBuilding/out/video_2_2021-03-25 20:56:36.avi")
        cap = cv.VideoCapture("/opt/data/Ilya/YOLO/FarmBuilding/out/testFly.mp4")
    
        frameCounter = 0
        cap_width = int(cap.get(cv.CAP_PROP_FRAME_WIDTH))
        cap_height = int(cap.get(cv.CAP_PROP_FRAME_HEIGHT))
        cap_fps = cap.get(cv.CAP_PROP_FPS)
        fourcc = cv.VideoWriter_fourcc(*'XVID')
        self.__videoOut = cv.VideoWriter('navigation.avi', fourcc , cap_fps, (cap_width, cap_height))

        while cap.isOpened():
            ret, frame = cap.read()
            if ret == True:
                (H, W) = frame.shape[:2]
                frameCounter+=1
                self.__runDetector(frame, W, H, frameCounter)
                cv.waitKey(1)
            else:
                break
        cap.release()
        self.__videoOut.release()

    #--------------------------------------
    # Detection of image's dataset
    #--------------------------------------
    def __imageDetection(self):
        folder = "/opt/data/Ilya/Script/Images3"
        for filename in os.listdir(folder):
            print(folder + "/" + filename)
            frame = cv.imread(folder + "/" + filename, cv.IMREAD_COLOR)
            (H, W) = frame.shape[:2]
            self.__runDetector(frame, W, H, filename)
            cv.waitKey(0)

    #--------------------------------------
    # Main detector loop
    #--------------------------------------
    def __runDetector(self, frame, W, H, filename):
        layerOutputs = self.__blopingImage(frame)
        idxs, boxes, confidences, classIDs = self.__initBoundingBox(layerOutputs, W, H)
        self.__annotateFrame(idxs, boxes, confidences, classIDs, frame , filename)

    #--------------------------------------
    # Main detector loop
    #--------------------------------------
    def __passYOLOConfig(self):
        self.__extractClassesName()
        self.__initColors()
        self.__net = cv.dnn.readNetFromDarknet(self.__modelConfiguration, self.__modelWeights)
        self.__net.setPreferableBackend(cv.dnn.DNN_BACKEND_OPENCV)
        self.__net.setPreferableTarget(cv.dnn.DNN_TARGET_CPU)
        if self.__currentRegime:
            self.__imageDetection()
        else:
            self.__videoDetection()


    def __mse(self, img1, img2):
        error = np.sum((img1.astype("float") - img2.astype("float")) ** 2)
        error /= float(img1.shape[0] * img1.shape[1])
        return error

    def __compare_image(self, img1, img2):
        # img2 = cv.resize(img2, (img1.shape[1],img1.shape[0]))
        # m = self.__mse(img1, img2)
        # s = measure.compare_ssim(img1, img2)
        # print("MSE : {0}, SSIM: {1}".format(m, s))
        pass


if __name__ == "__main__":
    commandArgs = sys.argv
    argsList = commandArgs[1:]
    short_options = ["v:i"]
    long_options = ["image", "video"]
    try:
        arguments, values = getopt.getopt(argsList, short_options, long_options)
    except getopt.error as err:
        print(str(err))
        sys.exit(2)

    currentRegime = None
    
    for current_arguments, current_value in arguments:
        if current_arguments in ("-v", "--video"):
            print("Enable Regime: video")
            currentRegime = 0
        elif current_arguments in ("-i", "--image"):
            print("Enable Regime: image")
            currentRegime = 1
    
    yolo = YOLODetector(currentRegime)
    cv.destroyAllWindows()
