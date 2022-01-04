#include "searchfilter.h"
#include "ui_searchfilter.h"

SearchFilter::SearchFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchFilter)
{
    ui->setupUi(this);
    db_conn.connOpen();


    //set default filter date to current date
    QDate date = ui->calendarWidget->selectedDate();

    std::string month = std::to_string(date.month());
    if(month.size() == 1){
        month = "0" + month;
    }
    std::string day = std::to_string(date.day());
    if(day.size() == 1){
        day = "0" + day;
    }
    std::string year = std::to_string(date.year());

    std::string date_full = year + "_" + month + "_" + day;

    filter_date = QString::fromStdString(date_full);




    //populate detect labels dropdown
    ui->comboBox->addItem("Any");
    std::set<QString> labels = db_conn.get_detected_labels();
    for (std::set<QString>::iterator it=labels.begin(); it!=labels.end(); ++it){
        QString l = *it;
        ui->comboBox->addItem(l);
    }
}

SearchFilter::~SearchFilter()
{
    delete ui;
}

void SearchFilter::on_calendarWidget_clicked(const QDate &date)
{

    //format = 2021_12_30
    std::string month = std::to_string(date.month());
    if(month.size() == 1){
        month = "0" + month;
    }
    std::string day = std::to_string(date.day());
    if(day.size() == 1){
        day = "0" + day;
    }
    std::string year = std::to_string(date.year());

    std::string date_full = year + "_" + month + "_" + day;

    filter_date = QString::fromStdString(date_full);
    qDebug() << filter_date;
}

void SearchFilter::on_listWidget_clicked(const QModelIndex &index)
{
}

void SearchFilter::on_comboBox_activated(const QString &arg1)
{
    QString selection = arg1;
    filter_label = selection;
}

void SearchFilter::on_pushButton_4_clicked()
{
    ui->listWidget->clear();
    queried_detections = db_conn.get_detections_in_filter(filter_date, filter_label);

    for(std::vector<QString> detection : queried_detections){
        std::string todisplay = detection[0].toStdString();
        std::string hour = todisplay.substr(11,2);
        std::string min = todisplay.substr(14,2);
        std::string sec = todisplay.substr(17,2);
        QString display = QString::fromStdString(hour) + " : " + QString::fromStdString(min) + " : " + QString::fromStdString(sec) + " , " + detection[1];

        ui->listWidget->addItem(display);
    }
}

void SearchFilter::on_pushButton_3_clicked()
{
    qDebug()<<selected <<" "<<filter_date;
    std::string d = filter_date.toStdString();
    std::string t = selected.toStdString();

    std::string start_time_id_str = d + "_" + t.substr(0,2) + "_" + t.substr(5,2) + "_" + t.substr(10,2);
    std::string detect_str = t.substr(15);

    QString start_time_id = QString::fromStdString(start_time_id_str);
    QString detect = QString::fromStdString(detect_str);
    qDebug()<<start_time_id;

    this->hide();
    gallery = new Gallery(this, start_time_id, detect);

    gallery->show();

}

void SearchFilter::on_pushButton_clicked()
{
    this->hide();
    QWidget *parent = this->parentWidget();
    parent->show();
    return;
}

void SearchFilter::on_listWidget_itemClicked(QListWidgetItem *item)
{
    selected =item->text();
}
