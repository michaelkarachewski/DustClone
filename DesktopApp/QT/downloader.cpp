#include "downloader.h"


class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};


downloader::downloader(QObject *parent)
{

}



void downloader::retrieve_data(std::string ipaddr){
    //initialize interfaces
    dust_conn.device_address = ipaddr;
    db_conn.connOpen();


    //get json mapping of each image name to timestamp and detection list
    std::vector<TrackedObject> tracking_info = dust_conn.get_tracking_info();


    int i = 1;

    int downloadsize = 0;
    //for each thing in map, download image
    for(TrackedObject t : tracking_info){
        downloadsize += t.timestamps.size();
    }
    for(TrackedObject t : tracking_info){
        //save image based on filename
        qDebug() << "Filename: " << (QString::fromStdString(t.start + ".jpg"));
        for(std::string timestamp : t.timestamps){
            dust_conn.get_image(timestamp);

            //update progress bar
            int percentage = float(i)/float(downloadsize) * 100;
            qDebug() << "percentage" << percentage;
            emit inprogress(percentage);
            i+=1;
            Sleeper::sleep(1);
        }


        //upload everything
        db_conn.upload_detection(t.start, t.detection, t.timestamps);
    }

    QMessageBox mb;
    mb.setText("Download Complete");
    mb.exec();
    emit inprogress(0);

    Sleeper::sleep(5);
}




/*void downloader::retrieve_data(std::string ipaddr){
    //initialize interfaces
    dust_conn.device_address = ipaddr;
    db_conn.connOpen();


    //get json mapping of each image name to timestamp and detection list
    std::vector<TrackedObject> tracking_info = dust_conn.get_tracking_info();


    int i = 1;

    //for each thing in map, download image
    for(TrackedObject t : tracking_info){
        //save image based on filename
        qDebug() << "Filename: " << (QString::fromStdString(t.start + ".jpg"));
        std::vector<std::string> image_names = dust_conn.get_images(t.timestamps);

        //upload everything
        db_conn.upload_detection(t.start, t.detection, image_names);

        //update progress bar
        int percentage = float(i)/float(tracking_info.size()) * 100;
        qDebug() << "percentage" << percentage;
        emit inprogress(percentage);
        i+=1;
        Sleeper::sleep(3);

    }

    QMessageBox mb;
    mb.setText("Download Complete");
    mb.exec();
    emit inprogress(0);

    Sleeper::sleep(5);
}*/
