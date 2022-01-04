#ifndef DETECTIONDOWNLOADER_H
#define DETECTIONDOWNLOADER_H

#include <QDialog>
#include <dust_interface.h>
#include <database_connection.h>

//for multithreading to not block main worker GUI thread
#include <QtConcurrent>
#include <QFuture>


namespace Ui {
class DetectionDownloader;
}

class DetectionDownloader : public QDialog
{
    Q_OBJECT

public:
    explicit DetectionDownloader(QWidget *parent = 0, std::string device_address = "");
    ~DetectionDownloader();
    void download();
    int retrieve_data();

private:

    Ui::DetectionDownloader *ui;
    DatabaseConnection db_conn;
    DustInterface dust_conn;

};

#endif // DETECTIONDOWNLOADER_H
