import jetson.inference
import jetson.utils
import time
import cv2
from time import gmtime, strftime
from threading import Thread

from http.server import BaseHTTPRequestHandler, HTTPServer
import logging
import json


#global
detect_session = None

class Image:
    def __init__(self,filename, time, objects):
        self.filename = filename
        self.time = time
        self.objects = objects

class ObjectDetector:
    def __init__(self, threshold):
        self.net = jetson.inference.detectNet("ssd-mobilenet-v2", threshold=0.5)
        self.camera = jetson.utils.gstCamera(1280,720,"/dev/video0")      # '/dev/video0' for V4L2
        self.display = jetson.utils.glDisplay() # 'my_video.mp4' for file
        self.threshold = threshold
        return 

    def get_detections(self):
        if self.display.IsOpen():
            print("analyzing new image...")
            img,width,height = self.camera.CaptureRGBA(zeroCopy=1)
            detections = self.net.Detect(img,width,height)


            #valid_detections = []
            valid_detections = ""
            for detect in detections:

                ID=detect.ClassID
                confidence = detect.Confidence
                top=detect.Top
                left=detect.Left
                bottom=detect.Bottom
                right=detect.Right
                item=self.net.GetClassDesc(ID)
                
                if confidence > self.threshold and item not in valid_detections:
                    #valid_detections.append(item)
                    valid_detections = valid_detections + item + " "
            img_arr = jetson.utils.cudaToNumpy(img,width,height,4)
            return img_arr, valid_detections[:-1]
    def release_camera(): #destructors aren't needed as much in python because has garbage collector to handle memory management automatically 
        return 




class DetectSession:
    def __init__(self):
        self.saved_images = {}
        self.past_detections = {}
        self.od = None
        self.runningthread = None 
        return 
    def process_detections(self, detections): 
        valid_image = False 

        #if we pick up any item that we haven't seen in the past 5 seconds, return True
        curtime = time.time()

        for item in detections:
            if item in self.past_detections: 
                lasttime = self.past_detections[item]

                if(curtime - lasttime > 5):
                    valid_image = True
                    self.past_detections[item] = curtime
            else:
                valid_image = True
                self.past_detections[item] = curtime

        return valid_image
    
    def start(self):
        self.runningthread = Thread(target = self.start_detection)
        self.runningthread.start()

    def start_detection(self):
        self.od = ObjectDetector(0.80)
        counter=0
        while(1):
            img,detections = self.od.get_detections()
            print()
            print()
            #print("raw detections", detections)
            if len(detections) > 0:
                save_image = self.process_detections(detections)

                print(detections)
                #print(img)
                if save_image:
                    name = f"image{counter}.jpg"
                    path = f"images/{name}"
                    strtime = strftime("%Y-%m-%d-%H:%M:%S", gmtime())


                    cv2.imwrite(path,img)

                    self.saved_images[name] = strtime + " " + detections

                    print("Final label", self.saved_images)
                    
                    counter+=1
            time.sleep(1)
            
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
            
            global detect_session
            self.wfile.write(json.dumps(detect_session.saved_images).encode())
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
    global detect_session
    detect_session = DetectSession()
    detect_session.start() #runs actual detection in background thread so that we can http run server simultaneously

    logging.basicConfig(level=logging.INFO)
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logging.info('Starting httpd...\n')
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



