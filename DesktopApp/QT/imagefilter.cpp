#include "imagefilter.h"
#include "ui_imagefilter.h"
#include "qdebug.h"

ImageFilter::ImageFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageFilter)
{
    ui->setupUi(this);
    db_conn.connOpen();

}

ImageFilter::~ImageFilter()
{
    delete ui;
}

void ImageFilter::on_calendarWidget_clicked(const QDate &date)
{
    QString qdate = date.toString();
    qDebug() << (qdate);

    filter_start = qdate;
    ui->listWidget->clear();
    ui->listWidget->addItem(qdate);
}


void ImageFilter::on_pushButton_6_clicked()
{
    this->hide();
    QWidget *parent = this->parentWidget();
    parent->show();
    return;
}

void ImageFilter::on_pushButton_3_clicked()
{
    qDebug()<< "Hello world";
    queried_detections = db_conn.get_detections_in_range();

    for(std::vector<QString> detection : queried_detections){
        QString todisplay = detection[0] + "," + detection[1];
        ui->listWidget->addItem(todisplay);
    }


    /*for (std::set<std::string>::iterator it=detected_labels.begin(); it!=detected_labels.end(); ++it){
       QString qs = QString::fromStdString((*it));
       qDebug() << "Detection..." << qs;
       if(qs != ""){
           ui->comboBox->addItem(qs);
       }
    }*/
}


void ImageFilter::on_pushButton_5_clicked()
{

    this->hide();
    gallery = new Gallery(this, selected_start_time, selected_detect_label);

    gallery->show();
}



void ImageFilter::on_listWidget_itemClicked(QListWidgetItem *item)
{
     std::string s = item->text().toStdString();
     int index = s.find(",");
     std::string time = s.substr(0,index);
     std::string label = s.substr(index+1);
     selected_start_time = QString::fromStdString(time);
     selected_detect_label = QString::fromStdString(label);
}
