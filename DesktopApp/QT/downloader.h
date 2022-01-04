#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#include <dust_interface.h>
#include <database_connection.h>
#include <QObject>
#include <QMessageBox>
#include <set>


class downloader : public QObject //only classes from QObject can have signals and slots
{
Q_OBJECT
public:
    downloader(QObject *parent=0);
    void retrieve_data(std::string ipaddr);
    DatabaseConnection db_conn;
    DustInterface dust_conn;
signals:
    void inprogress(int progr);
};

#endif // DOWNLOADER_H
