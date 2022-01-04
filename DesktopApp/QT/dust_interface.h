#ifndef DUST_INTERFACE_H
#define DUST_INTERFACE_H
#include "string"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

#include <json/json.h>
#include <curl/curl.h>
/*
QNetworkAccessManager was not working because of bad message ordering between python http.server and qtlib, so moved to curl.
Since curl is an external library not part of QT, we need to link the project with this
1. sudo apt-get install libcurl4-gnutls-dev
2. locate curl.so to find install location in /usr/lib/x86_64-linux-gnu/libcurl
3. right click on DustApp and Add Library to give curl library location to link with when building!
*/

struct TrackedObject{
    std::string start;
    std::string detection;
    std::vector<std::string> timestamps;
};

class DustInterface
{
public:
    std::vector<TrackedObject> get_tracking_info(void);
    void get_image(std::string timestamp);
    std::string device_address;
};



#endif // DUST_INTERFACE_H
