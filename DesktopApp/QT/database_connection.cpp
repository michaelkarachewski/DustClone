#include "database_connection.h"

/*bool DatabaseConnection::setupDB(){
    db = QSqlDatabase::addDatabase("QSQLITE"); //sql lite database
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    db.setDatabaseName(path + "/db.sqlite");
    if(!db.open()){
        qDebug()<<"Problem opening database";
        return false;

    }
    else{
        QSqlQuery qry;
        qry.prepare("CREATE TABLE IF NOT EXISTS detection_history (detections text, timestamp text, image BLOB);");
        if(!qry.exec()){
            qDebug() << "Failed to create new table";
        }

        qry.prepare("insert into detection_history (detections, timestamp) values (?, ?)");
        qry.bindValue(0, "cat dog" );
        qry.bindValue(1, "CUMULATIVE");
        if(!qry.exec()){
            qDebug() << "Failed to load first row";
        }

        return true;
    }
}*/

bool DatabaseConnection::setupDB(){
    db = QSqlDatabase::addDatabase("QSQLITE"); //sql lite database
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    db.setDatabaseName(path + "/db.sqlite");
    if(!db.open()){
        qDebug()<<"Problem opening database";
        return false;

    }
    else{
        QSqlQuery qry;
        qry.prepare("CREATE TABLE IF NOT EXISTS track_sessions (start_time_id text, detection text);");
        if(!qry.exec()){
            qDebug() << "Failed to create new table";
        }

        qry.prepare("CREATE TABLE IF NOT EXISTS image_data (start_time_id text, timestamp text, image BLOB);");
        if(!qry.exec()){
            qDebug() << "Failed to create new table";
        }

        return true;
    }
}

bool DatabaseConnection::connOpen()
{
    db = QSqlDatabase::addDatabase("QSQLITE"); //sql lite database
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    db.setDatabaseName(path + "/db.sqlite");
    if(!db.open()){
        qDebug()<<"Problem opening database";
        return false;

    }
    else{
        return true;
    }
}

void DatabaseConnection::connClose()
{
    db.close();
    db.removeDatabase(QSqlDatabase::defaultConnection);
    return;
}


/*void DatabaseConnection::upload_detection(std::string start, std::string detection, std::vector<std::string> image_names){
    QSqlQuery qry;

    QFile file(QString::fromStdString(filename));
    if(!file.open(QIODevice::ReadOnly)) return;
    QByteArray byte_array = file.readAll();

    qry.prepare("insert into detection_history (detections, timestamp, image) values (?, ?, ?)");
    qry.bindValue(0, QString::fromStdString(detections) );
    qry.bindValue(1, QString::fromStdString(timestamp) );
    qry.bindValue(2, byte_array );

    file.remove(); //delete from runtime directory

    if(qry.exec()){
        qDebug()<<"Data saved";
    }
    else{
        qDebug()<<"Could not save data to database";
    }
}*/

void DatabaseConnection::upload_detection(std::string start, std::string detection, std::vector<std::string> image_names){
    QSqlQuery qry;



    qry.prepare("insert into track_sessions (start_time_id, detection) values (?, ?)");
    qry.bindValue(0, QString::fromStdString(start) );
    qry.bindValue(1, QString::fromStdString(detection) );
    if(qry.exec()){
        qDebug()<<"Data saved";
    }
    else{
        qDebug()<<"Could not save data to database";
    }

    for(std::string image_name : image_names){
        std::string path = image_name + ".jpg";
        qDebug() << QString::fromStdString(path);
        qDebug() << "uploading new image";
        QFile file(QString::fromStdString(path));
        if(!file.open(QIODevice::ReadOnly)) return;

        QByteArray byte_array = file.readAll();
        qry.prepare("insert into image_data (start_time_id, timestamp, image) values (?, ?, ?)");
        qry.bindValue(0, QString::fromStdString(start) );
        qry.bindValue(1, QString::fromStdString(image_name) );
        qry.bindValue(2, byte_array );

        file.remove(); //delete from runtime directory

        if(qry.exec()){
            qDebug()<<"Data saved";
        }
        else{
            qDebug()<<"Could not save data to database";
        }
    }
}

std::vector<ImageRecord> DatabaseConnection::search(QString criteria){
    std::vector<ImageRecord> v;
    QSqlQuery query;
    query.setForwardOnly(true);
    QString command = "SELECT timestamp, image FROM image_data WHERE start_time_id LIKE '%" + criteria + "%'";
    query.exec(command);

    while(query.next())
    {
        ImageRecord r;

        QString ts = query.value(0).toString();

        QByteArray byte_array = query.value( 1 ).toByteArray();
        r.timestamp = ts;
        r.image = byte_array;
        qDebug() << "timestamp" << ts;
        v.push_back(r);
    }
    return v;
}


std::vector<std::vector<QString>> DatabaseConnection::get_detections_in_filter(QString date, QString label){
    std::vector<std::vector<QString>> start_times;

    QSqlQuery query;
    QString command;

    query.setForwardOnly(true);

    if (label == "Any"){
        command = "SELECT start_time_id, detection FROM track_sessions WHERE start_time_id LIKE '%" + date + "%'";
    }
    else{
        command = "SELECT start_time_id, detection FROM track_sessions WHERE (start_time_id LIKE '%" + date + "%' AND detection LIKE '%" + label + "%')";
    }
    qDebug() << command;
    query.exec(command);

    while(query.next())
    {

        QString id = query.value(0).toString();
        QString label = query.value(1).toString();
        start_times.push_back({id,label});
    }
    return start_times;
}


std::set<QString> DatabaseConnection::get_detected_labels(){
    std::set<QString> total_detections;
    QSqlQuery query;
    query.setForwardOnly(true);
    QString command = "SELECT detection FROM track_sessions";
    query.exec(command);
    //query.next();
    while(query.next()){
        QString ds = query.value(0).toString();
        total_detections.insert(ds);
    }

    return total_detections;
}


void DatabaseConnection::update_detected_labels(std::set<std::string> detected_labels){
    QString s = "";
    for (std::set<std::string>::iterator it=detected_labels.begin(); it!=detected_labels.end(); ++it){
       QString qs = QString::fromStdString((*it));
       s = s + qs + " ";
    }
    s.chop(1); //remove last space

    qDebug() << "Updating CUMULATIVE to ... " << s;

    QSqlQuery query;
    query.setForwardOnly(true);
    QString criteria = "CUMULATIVE";
    QString command = "UPDATE  detection_history SET detections = '" + s + "' WHERE timestamp LIKE '%" + criteria + "%'";
    qDebug() << command;
    query.exec(command);

    return;
}
