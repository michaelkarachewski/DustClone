#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H


#include <QtSql>
#include <set>

struct ImageRecord{
    QString timestamp;
    QByteArray image;
};

class DatabaseConnection {
  public:
    QSqlDatabase db;
    bool setupDB();
    bool connOpen();
    void connClose();
    void upload_detection(std::string start, std::string detection, std::vector<std::string> image_names);
    std::set<QString> get_detected_labels(void);

    std::vector<std::vector<QString>> get_detections_in_filter(QString date, QString label);

    void update_detected_labels(std::set<std::string>);
    std::vector<ImageRecord> search(QString criteria);
};
#endif // DATABASE_CONNECTION_H
