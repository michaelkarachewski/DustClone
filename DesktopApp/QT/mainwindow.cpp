#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtBluetooth>

//used to execute system call
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

using namespace std;

//helper function for Linux terminal system call
std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::find_attached_devices(){
    std::string arpstring = "arp -a";
    const char *command = arpstring.c_str();
    std::string output = exec(command);

    map<std::string,std::string> attached;

    while(1){
        int index = output.find("\n");
        std::string curline = output.substr(0,index);
        if(curline.size() == 0){
            break;
        }

        int a = curline.find(" ");
        std::string name = curline.substr(0,a);
        int b = curline.find(" ", a+1);
        std::string address = curline.substr(a+2, b-a-3);
        attached[name]=address;
        output = output.substr(index+1);
    }

    attached_devices = attached;
    return;
}

//scan for attached devices
void MainWindow::on_find_clicked()
{
    ui->listWidget->clear();
    find_attached_devices();
    for (std::pair<std::string, std::string> element : attached_devices) {
        std::string name = element.first;
        QString qname = QString::fromStdString(name);
        qDebug()<<qname;
        ui->listWidget->addItem(qname);
    }

    ui->listWidget->show();

    return;
}

//download from selected device and upload to DB
void MainWindow::on_on_clicked()
{
    ui->progressBar->setValue(0);

    connect(&d, SIGNAL( inprogress(int)) , ui->progressBar, SLOT( setValue(int) ) );
    d.retrieve_data(selected_address);

}

//image display gallery
void MainWindow::on_off_clicked()
{
    this->hide();
    searchfilter = new SearchFilter(this);
    searchfilter->show();
}



void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString selected_device =item->text();
    selected_address = attached_devices[selected_device.toStdString()];

    //TODO remove
    selected_address = "192.168.1.78";

    return;
}
