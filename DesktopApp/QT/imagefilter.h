#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include <QDialog>
#include <QDebug>
#include "database_connection.h"
#include "gallery.h"
#include<QListWidget>




namespace Ui {
class ImageFilter;
}

class ImageFilter : public QDialog
{
    Q_OBJECT

public:
    explicit ImageFilter(QWidget *parent = 0);
    ~ImageFilter();

private slots:
    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_3_clicked();



    void on_pushButton_5_clicked();


    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::ImageFilter *ui;

    DatabaseConnection db_conn;

    Gallery *gallery;

    std::vector<std::vector<QString>> queried_detections;

    QString filter_start;
    QString selected_start_time;
    QString selected_detect_label;
};

#endif // IMAGEFILTER_H
