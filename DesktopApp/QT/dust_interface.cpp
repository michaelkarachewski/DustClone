#include "dust_interface.h"



/*
QNetworkAccessManager was not working because of bad message ordering between python http.server and qtlib, so moved to curl.
Since curl is an external library not part of QT, we need to link the project with this
1. sudo apt-get install libcurl4-gnutls-dev
2. locate curl.so to find install location in /usr/lib/x86_64-linux-gnu/libcurl
3. right click on DustApp and Add Library to give curl library location to link with when building!
*/

static size_t string_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


std::vector<TrackedObject> DustInterface::get_tracking_info(){

    //download using curl
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();

    std::string target = "http://" + device_address + ":8000/";
    qDebug() << (QString::fromStdString(target));

    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, target.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      qDebug() << "output";
      qDebug() << (QString::fromStdString(readBuffer));
     }

    //convert to json
    Json::Value data;
    Json::Reader jsonReader;
    std::vector<TrackedObject> detections;

    if (jsonReader.parse(readBuffer, data))
    {
        qDebug() << "Successfully parsed JSON data";

        for( Json::Value::const_iterator itr = data.begin() ; itr != data.end() ; itr++ ) {
            std::string start = itr.key().asString();
            std::string val_pair = data[start].asString();
            int splitindex = val_pair.find(" ");
            std::string label = val_pair.substr(0,splitindex);
            std::string s = val_pair.substr(splitindex + 1);
            TrackedObject t;
            t.start= start;
            t.detection = label;

            std::vector<std::string> timestamps;
            //parse to deserialize space-separated timestamps
            while(1){
                int index = s.find(" ");
                qDebug()<<index;
                std::string filename = s.substr(0,index);
                timestamps.push_back(filename);

                if(index == std::string::npos){
                    break;
                }
                else{
                    s=s.substr(index+1);
                }
            }

            t.timestamps = timestamps;

            qDebug() << (QString::fromStdString(start));
            qDebug() << (QString::fromStdString(label));

            detections.push_back(t);
        }
    }
    else
    {
        qDebug()<<"JSON parsing error";
    }
    return detections;
}

size_t jpg_callback(void *ptr, size_t size, size_t nmemb, void* userdata)
{
    FILE* stream = (FILE*)userdata;
    if (!stream)
    {
        printf("!!! No stream\n");
        return 0;
    }

    size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
    return written;
}

void DustInterface::get_image(std::string timestamp){
    std::string filename = timestamp + ".jpg";


    qDebug() << (QString::fromStdString(filename));
    std::string path = filename;
    FILE* fp = fopen(path.c_str(), "wb"); //write to runtime directory

    //download using curl
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    std::string target = "http://" + device_address + ":8000/" + filename;

    qDebug() << "Image curl request";
    qDebug() << (QString::fromStdString(target));

    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, target.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, jpg_callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
    }

    fclose(fp);

    return;
}
