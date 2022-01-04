#include "imagegallery.h"
#include "ui_imagegallery.h"
#include <QDebug>

ImageGallery::ImageGallery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageGallery)
{
    ui->setupUi(this);
}

ImageGallery::~ImageGallery()
{
    delete ui;
}



void ImageGallery::on_homebutton_clicked()
{
    qDebug() << "Here";
    this->hide();
    QWidget *parent = this->parentWidget();
    parent->show();
    return;
}

void ImageGallery::on_searchbutton_clicked()
{

}
