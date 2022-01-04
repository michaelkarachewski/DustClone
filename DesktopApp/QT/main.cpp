#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QtSql>
#include "database_connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qDebug() << "Starting application with persistent app data path = " << path;
    QDir d{path};
    if (!d.exists()){
        d.mkpath(d.absolutePath());
        QFile file(path + "/db.sqlite");
        file.open(QIODevice::ReadWrite); // Or QIODevice::ReadWrite

        DatabaseConnection d;
        d.setupDB();
    }
    else{
        qDebug() << "Database discovered from previous trial";
    }

    MainWindow w;
    w.show();

    return a.exec();
}
