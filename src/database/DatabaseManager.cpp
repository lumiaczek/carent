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
    db = QSqlDatabase::addDatabase("QSQLIte");
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
            licence TEXT NOT NULL
            status TEXT NOT NULL DEFAULT 'ACTIVE'
        )
    )";

    if(!q.exec(createVehicles) || !q.exec(createCustomers)){
        qCritical() << "Error while creating tables" << q.lastError().text();

        return false;
    }
    return true;
}