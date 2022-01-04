import jetson.inference
import jetson.utils
import time
import cv2
import numpy as np
from time import gmtime, strftime
from threading import Thread

from http.server import BaseHTTPRequestHandler, HTTPServer
import logging
import json


#global
surveil_session = None

class Image:
    def __init__(self,filename, time, objects):
        self.filename = filename
        self.time = time
        self.objects = objects

class ObjectTracker:
    def __init__(self):
        
        self.net = jetson.inference.detectNet("ssd-mobilenet-v2", threshold=0.5) #object detector
        self.camera = jetson.utils.gstCamera(1280,720,"/dev/video0")     
        self.tracker = cv2.TrackerCSRT_create()
        return 

    # waits to detect moving object and returns information for this single tracked object
    def track(self):

        #set default state and begin looking for new object to track 
        frame1 = None
        frame2 = None
        tracking = False

        detection = None
        start_time = None
        timestamps = []

        prev_track_x = None
        prev_track_y = None
         
        last_detection_time = time.time()
        sustained_movement = False

        while True:
            #get current frame 
            img,width,height = self.camera.CaptureRGBA(zeroCopy=1)
            if cv2.waitKey(40) == 27:
                break

            detections = None
            if not tracking:
                #get detections
                detections = self.net.Detect(img,width,height, overlay='none')

            #convert to numpy for use with cv2
            jetson.utils.cudaDeviceSynchronize ()
            img = jetson.utils.cudaToNumpy (img,width,height)
            img1 = cv2.cvtColor (img.astype(np.uint8), cv2.COLOR_RGBA2BGR)
    

            if not tracking:
                #get contours of moving pixels
                if frame1 is not None and frame2 is not None:
        
                    diff = cv2.absdiff(frame1, frame2)
                    gray = cv2.cvtColor(diff,cv2.COLOR_BGR2GRAY)
                    blur = cv2.GaussianBlur(gray, (5,5), 0)
                    _,thresh= cv2.threshold(blur,50,255,cv2.THRESH_BINARY)
                    dilated = cv2.dilate(thresh,None,iterations=3)
                    contours,_ = cv2.findContours(dilated, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            
                    for detect in detections:
                        if tracking:
                            break
                        ID=detect.ClassID
                        name=self.net.GetClassDesc(ID)
                        confidence = detect.Confidence
                        top=int(detect.Top)
                        left=int(detect.Left)
                        bottom=int(detect.Bottom)
                        right=int(detect.Right)

                        for contour in contours: 
                            (x,y,w,h) = cv2.boundingRect(contour)
                            if cv2.contourArea(contour) < 1500:
                                continue
                            cv2.rectangle(frame1, (x,y), (x+w,y+h), (0,155,0), 2)
                            if x >left and x+w < right and y > top and y+h < bottom:
                                print(f"New object to track: {name}")
                                min_x = int(left + (right-left)/4)
                                min_y = int(top + (bottom-top)/4)
                                w = int((right-left)/2)
                                h = int((bottom-top)/2)
                                self.tracker.init(frame1, [min_x,min_y,w,h])
                                start_time = strftime("%Y_%m_%d_%H_%M_%S", gmtime()) 
                                tracking = True
                                detection = name
                                break

                        cv2.rectangle(frame1, (left,top), (right,bottom), (0,255,0), 2)      
                    cv2.imshow("feed",frame1)

            else: 
                (success,box) = self.tracker.update(img1)
                if success:
                    curtime = time.time()
        
                    (x,y,w,h) = [int(a) for a in box]
                    if (prev_track_x is not None) and ((x - prev_track_x > 7) or (y - prev_track_y > 7)):
                     
                        #print(x-prev_track_x,sustained_movement)
                        sustained_movement = True
                        #print("Here",sustained_movement)

                    prev_track_x = x
                    prev_track_y = y


                    if curtime - last_detection_time > 1.4:
                        #print("sustained movement", sustained_movement)
                        if not sustained_movement:
                            print("tracking ended")
                            return detection,start_time,timestamps
                        last_detection_time = curtime
                        sustained_movement = False
                        strtime = strftime("%Y_%m_%d_%H_%M_%S", gmtime()) 
                        timestamps.append(strtime)
                        name = f"{strtime}.jpg"
                        path = f"images/{name}"
                        cv2.imwrite(path,img1)
 
                    
            
                    cv2.rectangle(img1,(x,y),(x+w,y+h),(255,255,0),2)
                    cv2.imshow("tracking...",img1)
                else: 
                    print("tracking ended")
                    return detection,start_time,timestamps
            frame1=frame2
            frame2 = img1

            def release_camera(): #destructors aren't needed as much in python because has garbage collector to handle memory management automatically 
                return 




class SurveillanceSession:
    def __init__(self):
        self.tracked_sessions = {}
        self.ot = None
        self.runningthread = None 
        return 

    def start(self):
        self.runningthread = Thread(target = self.start_tracking)
        self.runningthread.start()
        print("thread started")

    def start_tracking(self):
        self.ot = ObjectTracker()
        counter=0
        while(1):
             
            detection,start_time,timestamps = self.ot.track()
            if len(timestamps) == 0:
                continue

            serialized = detection
            for t in timestamps:
                serialized = serialized + " " + t
            self.tracked_sessions[start_time] = serialized
            print("object tracked successfullly")
            print(self.tracked_sessions)
            time.sleep(2)
            
            
    def end(self):
        #end thread , o.destroy(), save map to json file
        return 


class HttpServer(BaseHTTPRequestHandler): #inherits from Base class and overwrites GET and POST functions for our needs 
    def _set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        if self.path == "/":
            print("data requested")
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.end_headers
            
            global surveil_session
            self.wfile.write(json.dumps(surveil_session.tracked_sessions).encode())
            #self.wfile.write(json.dumps({'hello': 'world', 'received': 'ok'}).encode())
        
        elif self.path.endswith(".jpg"):
            print("requesting image")
            f = open("images" + self.path, 'rb')
            self.send_response(200)
            self.send_header('Content-type', 'image/jpg')
            self.end_headers()
            self.wfile.write(f.read())
            f.close()
        return 

    def do_POST(self):
        #destruct current detect session and create a new one 
        '''content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        post_data = self.rfile.read(content_length) # <--- Gets the data itself
        logging.info("POST request,\nPath: %s\nHeaders:\n%s\n\nBody:\n%s\n",
                str(self.path), str(self.headers), post_data.decode('utf-8'))

        self._set_response()
        self.wfile.write("POST request for {}".format(self.path).encode('utf-8'))'''



def run(server_class=HTTPServer, handler_class=HttpServer, port=8000):
    global surveil_session
    surveil_session = SurveillanceSession()
    print("starting surveil session")
    surveil_session.start() #runs actual detection in background thread so that we can http run server simultaneously

    logging.basicConfig(level=logging.INFO)
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print("starting http server")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info('Stopping httpd...\n')

if __name__ == '__main__':
    from sys import argv

    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()



