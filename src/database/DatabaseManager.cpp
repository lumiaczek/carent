#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager& DatabaseManager::instance(){
    static DatabaseManager inst;
    return inst;
}

DatabaseManager::~DatabaseManager(){
    close();
}

bool DatabaseManager::conn_open(const QString& dbPath){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if(!db.open()){
        qCritical() << "Error while opening database" << db.lastError().text();

        return false;
    }

    QSqlQuery query;

    if(!query.exec("PRAGMA foreign_keys = ON;")){
        qWarning() << "Cannot enable PRAGMA foreign_keys: " << db.lastError().text();
    }
    return true;

}
void DatabaseManager::close(){
    if(db.isOpen()){
        db.close();
    }
}
QList<Vehicle> DatabaseManager::getAllVehicles(){
    QList<Vehicle> vehicles;
    
    QSqlQuery query("SELECT id, brand, model, plate FROM vehicles");

    if(!query.isActive()){
        qWarning() << "Cannot get vehicles from database" << query.lastError().text();
        return vehicles;
    }
    while(query.next()){
        Vehicle v;
        v.setId(query.value("id").toInt());
        v.setBrand(query.value("brand").toString());
        v.setModel(query.value("model").toString());
        v.setPlate(query.value("plate").toString());
        vehicles.append(v);
    }
    return vehicles;
    
}
bool DatabaseManager::addVehicle(const Vehicle& v){
    QSqlQuery query;
    query.prepare("INSERT INTO vehicles (brand, model, plate, category, daily_rate, status) VALUES (:brand, :model, :plate, :category, :dailyRate, :status)");

    query.bindValue(":brand", v.brand());
    query.bindValue(":model", v.model());
    query.bindValue(":plate", v.plate());
    query.bindValue(":dailyRate", 0.0);

    query.bindValue(":category", static_cast<int>(v.category()));
    query.bindValue(":status", static_cast<int>(v.status()));

    if(!query.exec()){
        qWarning() << "Saving vehicle in database failed: " << query.lastError().text();
        return false;
    }
    qDebug() << "Successfully added vehicle to database";
    return true;
}

bool DatabaseManager::initSchema(){
    QSqlQuery q;

    QString createVehicles = R"(
        CREATE TABLE IF NOT EXISTS vehicles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            brand TEXT NOT NULL, 
            model TEXT NOT NULL, 
            plate TEXT UNIQUE NOT NULL, 
            category TEXT NOT NULL, 
            daily_rate REAL NOT NULL, 
            status TEXT NOT NULL DEFAULT 'AVAILABLE'
        );
    )";

    QString createCustomers = R"(
        CREATE TABLE IF NOT EXISTS customers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL, 
            surname TEXT NOT NULL, 
            birthdate TEXT NOT NULL, 
            licence TEXT NOT NULL,
            status TEXT NOT NULL DEFAULT 'ACTIVE'
        )
    )";

    if(!q.exec(createVehicles) || !q.exec(createCustomers)){
        qCritical() << "Error while creating tables" << q.lastError().text();

        return false;
    }
    return true;
}