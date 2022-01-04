/*#ifndef UPLOADIMAGE_H
#define UPLOADIMAGE_H

#include <QDialog>
//#include "mainwindow.h"
#include "database_connection.h"

namespace Ui {
class UploadImage;
}

class UploadImage : public QDialog
{
    Q_OBJECT

public:
    explicit UploadImage(QWidget *parent = 0);
    ~UploadImage();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::UploadImage *ui;
    DatabaseConnection db_conn;
};
#endif // UPLOADIMAGE_H
*/
