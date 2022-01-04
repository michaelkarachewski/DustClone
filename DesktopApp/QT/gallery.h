#ifndef GALLERY_H
#define GALLERY_H

#include <QDialog>
#include "database_connection.h"

namespace Ui {
class Gallery;
}

class Gallery : public QDialog
{
    Q_OBJECT

public:
    explicit Gallery(QWidget *parent = 0, QString start_time = "", QString label = "");
    ~Gallery();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Gallery *ui;
    DatabaseConnection db_conn;

};

#endif // GALLERY_H
