#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBluetoothDeviceDiscoveryAgent>
#include<QBluetoothSocket>
#include<QListWidget>
#include <map>
#include <string>
#include "uploadimage.h"
#include "detectiondownloader.h"
#include "downloader.h"
#include "database_connection.h"
#include <QDebug>
#include "gallery.h"
#include "searchfilter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void find_attached_devices();

    void on_find_clicked();

    void on_on_clicked();

    void on_off_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;

    //main window devices seen and selected
    std::map<std::string,std::string> attached_devices;
    std::string selected_address;

    //child widgets

    SearchFilter *searchfilter;
    DetectionDownloader *detectiondownloader;
    downloader d;
};

#endif // MAINWINDOW_H
