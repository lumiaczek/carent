#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "../core/vehicles/PassengerCar.h"
#include "../core/vehicles/DeliveryVan.h"

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
QList<QSharedPointer<Vehicle>> DatabaseManager::getAllVehicles(){
    QList<QSharedPointer<Vehicle>> vehicles;
    
    return vehicles;
}
bool DatabaseManager::addVehicle(QSharedPointer<Vehicle> v){
    
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery query;
    query.prepare("INSERT INTO vehicles(type, brand, model, plate, category, daily_rate, status, transmission, fuel)" "VALUES (:type, :brand, :model, :plate, :category, :dailyRate, :status, :transmission, :fuel)");
    
    QString typeStr = (v->type() == Vehicle::Type::PassengerCar) ? "PassengerCar" : (v->type() == Vehicle::Type::DeliveryVan ? "DeliveryVan" : "Scooter");


    query.bindValue(":type", typeStr);
    query.bindValue(":brand", v->brand());
    query.bindValue(":model", v->model());
    query.bindValue(":plate", v->plate());
    query.bindValue(":category", static_cast<int>(v->category()));
    query.bindValue(":dailyRate", v->dailyRate());
    query.bindValue(":status", static_cast<int>(v->status()));

    query.bindValue(":transmission", static_cast<int>(v->transmission()));
    query.bindValue(":fuel", static_cast<int>(v->fuel()));

    if(!query.exec()){
        qWarning() << "Inserting vehicle into database ERROR: " <<  query.lastError().text();
        db.rollback();
        return false;
    }
    int newVehicleId = query.lastInsertId().toInt();

    if(v->type() == Vehicle::Type::PassengerCar){
        auto car = qSharedPointerCast<PassengerCar>(v);

        QSqlQuery carQuery;
        carQuery.prepare("INSERT INTO passenger_cars (vehicle_id, seats, trunk_capacity, body_class, doors, has_isofix) VALUES (:vid, :seats, :trunk_capacity, :body_class, :doors, :has_isofix)");
        carQuery.bindValue(":vid", newVehicleId);
        carQuery.bindValue(":seats", car->seats());
        carQuery.bindValue(":trunk_capacity", car->trunkCapacity());
        carQuery.bindValue(":body_class", static_cast<int>(car->bodyClass()));
        carQuery.bindValue(":doors", car->doors());
        carQuery.bindValue(":has_isofix", static_cast<int>(car->hasIsofix()));
        
        if(!carQuery.exec()) {
            qWarning() << "Błąd zapisu do passenger_cars:" << carQuery.lastError().text();
            db.rollback();
            return false;
        }
    }
    else if(v->type() == Vehicle::Type::DeliveryVan){
        auto van = qSharedPointerCast<DeliveryVan>(v);
        
        QSqlQuery vanQuery;
        vanQuery.prepare("INSERT INTO delivery_vans (vehicle_id, cargo_volume, capacity, has_tail_lift, height) VALUES (:vid, :cargo_volume, :capacity, :has_tail_lift, :height)");
        vanQuery.bindValue(":vid", newVehicleId);
        vanQuery.bindValue(":cargo_volume", van->cargoVolume());
        vanQuery.bindValue(":capacity", van->capacity());
        vanQuery.bindValue(":has_tail_lift", static_cast<int>(van->hasTailLift()));
        vanQuery.bindValue(":height", van->vehicleHeight());
        
        if (!vanQuery.exec()) {
            qWarning() << "Błąd zapisu do delivery_vans:" << vanQuery.lastError().text();
            db.rollback();
            return false;
        }
    }

    return db.commit();

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
            status TEXT NOT NULL DEFAULT 'AVAILABLE',
            transmission TEXT NOT NULL,
            fuel TEXT NOT NULL
        );
    )";
    QString createPassengerCars = R"(
        CREATE TABLE IF NOT EXISTS passenger_cars (
            vehicle_id INTEGER PRIMARY KEY,
            seats INTEGER NOT NULL,
            trunk_capacity INTEGER NOT NULL, 
            body_class TEXT NOT NULL, 
            doors INTEGER NOT NULL, 
            has_isofix INTEGER NOT NULL,
            FOREIGN KEY(vehicle_id) REFERENCES vehicles(id) ON DELETE CASCADE
        );
    )";
    QString createDeliveryVans = R"(
        CREATE TABLE IF NOT EXISTS delivery_vans (
            vehicle_id INTEGER PRIMARY KEY, 
            cargo_volume REAL NOT NULL, 
            capacity REAL NOT NULL, 
            has_tail_lift INTEGER NOT NULL,
            height REAL NOT NULL,
            FOREIGN KEY(vehicle_id) REFERENCES vehicles(id) ON DELETE CASCADE
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

    if(!q.exec(createVehicles) || !q.exec(createCustomers) || !q.exec(createPassengerCars) || !q.exec(createDeliveryVans)){
        qCritical() << "Error while creating tables" << q.lastError().text();

        return false;
    }
    return true;
}