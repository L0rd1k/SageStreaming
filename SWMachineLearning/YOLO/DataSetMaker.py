import cv2 as cv
import numpy as np
import os
import random
import time
import glob


class YOLODetector:
    def __init__(self):
        self.__confidence = 0.1  #Confidence threshold
        self.__threshold = 0.1  #Non-maximum suppression threshold
        self.__inpWidth, self.__inpHeight  = 416, 416     #Width of network's input image
        self.__classesNameFile = "/opt/data/Ilya/YOLO/FarmBuilding/objects.txt"
        self.__modelConfiguration = "/opt/data/Ilya/YOLO/FarmBuilding/navigation.cfg"
        self.__modelWeights = "/opt/data/Ilya/YOLO/FarmBuilding/weights/navigation_35000.weights"           
        self.__net = None
        self.__classes = None
        self.__colors = None

        #self.generateDataSetFromImage("/opt/data/Проекты/ОЭС-ОН/maps/protasovo/map17.jpg", \
        #                             "/opt/data/Ilya/Script/Images/", 1000)

        self.__passYOLOConfig()                    
    
    def generateDataSetFromImage(self, mapPath, outPath, imagesNumber):
        print("....Generate {0} images from map....".format(imagesNumber))
        originalMap = cv.imread(mapPath, cv.IMREAD_COLOR) 
        hMap, wMap, cMap = originalMap.shape
        print("Height: {0}, Width: {1}, Channel: {2}".format(hMap, wMap, cMap))
        for elem in range(imagesNumber):
            rndX, rndY = random.randint(0, wMap - 500), random.randint(0, hMap - 500)
            rndW, rndH = random.randint(100, 500), random.randint(100, 500)
            print("rndX: {0}, rndY: {1}, rndW: {2}, rndH: {3}".format(rndX, rndY, rndW, rndH))
            croppedImg = originalMap[rndY : rndY + rndH, rndX : rndX + rndW]
            # cv.imwrite('{0}{1}{2}'.format('/home/ilya/Projects/GitProjects/Script/Images/', elem, '.jpg'), croppedImg)
    
    def __extractClassesName(self):
        with open(self.__classesNameFile, 'rt') as f:
            self.__classes = f.read().rstrip('\n').split('\n')
        print(self.__classes)

    def __initColors(self):
        np.random.seed(42)
        self.__colors = np.random.randint(0, 255, size=(len(self.__classes), 3), dtype="uint8")

    def __blopingImage(self, frame):
        ln = self.__net.getLayerNames()
        ln = [ln[i[0] - 1] for i in self.__net.getUnconnectedOutLayers()]
        blob = cv.dnn.blobFromImage(frame, 1/255, (self.__inpWidth, self.__inpHeight), [0,0,0], swapRB=True, crop=False)
        self.__net.setInput(blob) 
        # Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
        start = time.time()
        layerOutputs = self.__net.forward(ln)
        print("[INFO] YOLO took {:.6f} seconds".format(time.time() - start))
        return layerOutputs

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

    def __annotateFrame(self, idxs, boxes, confidences, classIDs, frame, filename):
        if len(idxs) > 0:
            os.mkdir("./WrongDetect")
            for i in idxs.flatten():
                (x, y) = (boxes[i][0], boxes[i][1])
                (w, h) = (boxes[i][2], boxes[i][3])
                color = [int(c) for c in self.__colors[classIDs[i]]]
                cv.rectangle(frame, (x, y), (x + w, y + h), color, 2)
                text = "{}: {:.4f}".format(self.__classes[classIDs[i]], confidences[i])
                cv.putText(frame, text, (x, y - 5), cv.FONT_HERSHEY_SIMPLEX,0.5, color, 2)
            cv.imwrite('{0}{1}'.format('./WrongDetect/', filename), frame)
        #cv.imshow("Image", frame)
        cv.waitKey(1)        

    def __passYOLOConfig(self):
        self.__extractClassesName()
        self.__initColors()
        self.__net = cv.dnn.readNetFromDarknet(self.__modelConfiguration, self.__modelWeights)
        self.__net.setPreferableBackend(cv.dnn.DNN_BACKEND_OPENCV)
        self.__net.setPreferableTarget(cv.dnn.DNN_TARGET_CPU)


        folder = "/opt/data/Ilya/Script/Images"
        for filename in os.listdir(folder):
            print(folder + "/" + filename)
            frame = cv.imread(folder + "/" + filename, cv.IMREAD_COLOR)
            (H, W) = frame.shape[:2]
        
        
            layerOutputs = self.__blopingImage(frame)
            idxs, boxes, confidences, classIDs = self.__initBoundingBox(layerOutputs, W, H)
            self.__annotateFrame(idxs, boxes, confidences, classIDs, frame , filename)
            

yolo = YOLODetector()
cv.destroyAllWindows()
