#ifndef SEARCHFILTER_H
#define SEARCHFILTER_H

#include <QDialog>
#include <QDebug>
#include "database_connection.h"
#include "gallery.h"
#include<QListWidget>
#include <QMessageBox>

namespace Ui {
class SearchFilter;
}

class SearchFilter : public QDialog
{
    Q_OBJECT

public:
    explicit SearchFilter(QWidget *parent = 0);
    ~SearchFilter();

private slots:
    void on_calendarWidget_clicked(const QDate &date);

    void on_listWidget_clicked(const QModelIndex &index);

    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::SearchFilter *ui;
    Gallery *gallery;
    DatabaseConnection db_conn;
    std::vector<std::vector<QString>> queried_detections;
    QString filter_date = " ";
    QString filter_label = "Any";
    QString selected;
};

#endif // SEARCHFILTER_H
