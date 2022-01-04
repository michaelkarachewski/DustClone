#ifndef IMAGEGALLERY_H
#define IMAGEGALLERY_H

#include <QDialog>

namespace Ui {
class ImageGallery;
}

class ImageGallery : public QDialog
{
    Q_OBJECT

public:
    explicit ImageGallery(QWidget *parent = 0);
    ~ImageGallery();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_homebutton_clicked();

    void on_searchbutton_clicked();

private:
    Ui::ImageGallery *ui;
};

#endif // IMAGEGALLERY_H
