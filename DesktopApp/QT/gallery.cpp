#include "gallery.h"
#include "ui_gallery.h"

Gallery::Gallery(QWidget *parent, QString start_time, QString label) :
    QDialog(parent),
    ui(new Ui::Gallery)
{
    ui->setupUi(this);
    db_conn.connOpen();
    ui->listWidget_2->addItem(label);

    std::vector<ImageRecord> queries = db_conn.search(start_time);

    ui->listWidget->clear();
    ui->listWidget->setViewMode((QListWidget::IconMode));
    ui->listWidget->setIconSize(QSize(400,400));
    ui->listWidget->setResizeMode(QListWidget::Adjust);

    for (ImageRecord ir : queries){
        QString time = ir.timestamp;
        QByteArray img = ir.image;
        QPixmap image;
        QByteArray ba;
        image.loadFromData(img);
        ui->listWidget->addItem(new QListWidgetItem(image, time));
    }

}

Gallery::~Gallery()
{
    delete ui;
}

void Gallery::on_pushButton_clicked()
{
    this->hide();
    QWidget *parent = this->parentWidget();
    parent->show();
    return;
}
