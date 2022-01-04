#include "detectiondownloader.h"
#include "ui_detectiondownloader.h"
#include <QDebug>


#include <QThread>

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};



DetectionDownloader::DetectionDownloader(QWidget *parent, std::string device_address) :
    QDialog(parent),
    ui(new Ui::DetectionDownloader)
{
    ui->setupUi(this);

    qDebug()<<"new window";
    qDebug()<<(QString::fromStdString(device_address));

    //initialize interfaces to Dust and database
    dust_conn.device_address = device_address;
    db_conn.connOpen();

    return;
}

DetectionDownloader::~DetectionDownloader()
{
    delete ui;
}

void DetectionDownloader::download(){

    //places retrieve_data function in threadpool
    QFuture<int> future = QtConcurrent::run(this,&DetectionDownloader::retrieve_data);
    qDebug() << future.result();
    return;
}
int DetectionDownloader::retrieve_data(){
    this->show();
    QCoreApplication::processEvents();

    //get mapping of image names to timestamps and detections
    std::vector<Detection> detections = dust_conn.get_detection_info();

    //for each thing in map, download image
    for(Detection d : detections){

        //save image based on filename

        //upload everything
        //db_conn.upload_detection(d.filename, d.timestamp, d.detections);
    }
    //db_conn.connClose();

    Sleeper::sleep(5);
    this->hide();
    QWidget *parent = this->parentWidget();
    parent->show();
}
