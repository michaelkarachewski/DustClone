/*#include "uploadimage.h"
#include "ui_uploadimage.h"
#include "database_connection.h"

UploadImage::UploadImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadImage)
{
    ui->setupUi(this);
    db_conn.connOpen();
}

UploadImage::~UploadImage()
{
    delete ui;
}


//upload button
void UploadImage::on_pushButton_clicked()
{

    //get text from input boxes
    QString image_name, timestamp, label;
    image_name = ui->lineEdit->text();
    timestamp = ui->lineEdit_2->text();
    //label = ui->lineEdit_3->text();
    qDebug()<<"HI "<<image_name<<" "<<timestamp<<" "<<label;

    //upload to db using sql query
    QSqlQuery qry;
    qry.prepare("insert into detection_history (image_name,timestamp) values ('"+image_name+"','"+timestamp+"')");
    if(qry.exec()){
        qDebug()<<"Data saved";
        db_conn.connClose();
    }
    else{
        qDebug()<<"Could not save data to database";
    }

    //return to home screen
    this->hide();
    QWidget *parent = this->parentWidget();
    parent->show();

    return;

}

//home button
void UploadImage::on_pushButton_2_clicked()
{
    this->hide();
    QWidget *parent = this->parentWidget(); //mainwindow is the parent widget of the imageuploader screen
    parent->show();
    return;
}
*/
