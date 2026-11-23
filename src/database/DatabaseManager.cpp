#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "../core/vehicles/PassengerCar.h"
#include "../core/vehicles/DeliveryVan.h"
#include "../core/Rental.h"

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
    QSqlQuery query;

    QString sql = R"(
        SELECT v.id, v.type, v.brand, v.model, v.plate, v.category, v.daily_rate, v.status, v.transmission, v.fuel, p.seats, p.trunk_capacity, p.body_class, p.doors, p.has_isofix, c.cargo_volume, c.capacity, c.has_tail_lift, c.height FROM vehicles v LEFT JOIN passenger_cars p ON v.id = p.vehicle_id LEFT JOIN delivery_vans c ON v.id = c.vehicle_id
    )";

    if(!query.exec(sql)){
        qWarning() << "Error fetching vehicles from database" << query.lastError().text();
        return vehicles;
    }
    while(query.next()){
        int id = query.value("id").toInt();
        QString typeStr = query.value("type").toString();
        QString brand = query.value("brand").toString();
        QString model = query.value("model").toString();
        QString plate = query.value("plate").toString();
        Vehicle::Category cat = static_cast<Vehicle::Category>(query.value("category").toInt());
        Vehicle::Status stat = static_cast<Vehicle::Status>(query.value("status").toInt());
        double dailyRate = query.value("daily_rate").toDouble();
        Vehicle::Transmission transm = static_cast<Vehicle::Transmission>(query.value("transmission").toInt());
        Vehicle::Fuel fuel = static_cast<Vehicle::Fuel>(query.value("fuel").toInt());

        if(typeStr == "PassengerCar"){
            int seats = query.value("seats").toInt();
            double trunk_capacity = query.value("trunk_capacity").toDouble();
            PassengerCar::BodyClass body_class = static_cast<PassengerCar::BodyClass>(query.value("body_class").toInt());
            int doors = query.value("doors").toInt();
            bool hasIsofix = static_cast<bool>(query.value("has_isofix").toInt());

            auto car = QSharedPointer<PassengerCar>::create(id, brand, model, plate, cat, dailyRate, stat, seats, doors, hasIsofix, transm, fuel, body_class);

            vehicles.append(car);
        }
        else if(typeStr == "DeliveryVan"){
            double cargo_volume = query.value("cargo_volume").toDouble();
            double capacity = query.value("capacity").toDouble();
            bool has_tail_lift = static_cast<bool>(query.value("has_tail_lift").toInt());
            double height = query.value("height").toDouble();

            auto van = QSharedPointer<DeliveryVan>::create(id, brand, model, plate, cat, dailyRate, stat, transm, fuel, capacity, cargo_volume, has_tail_lift, height);

            vehicles.append(van);
        }
    }
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
    v->setId(newVehicleId);

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
bool DatabaseManager::addCustomer(Customer& customer){
    QSqlQuery query;

    query.prepare("INSERT INTO customers(name, surname, birthdate, licence, status) VALUES (:name, :surname, :birthdate, :licence, :status)");
    query.bindValue(":name", customer.name());
    query.bindValue(":surname", customer.surname());
    query.bindValue(":birthdate", customer.birthdate());
    query.bindValue(":licence", customer.licence());
    query.bindValue(":status", static_cast<int>(customer.status()));

    if(!query.exec()){
        qWarning() << "Cannot add customer to database";
        return false;
    }
    customer.setId(query.lastInsertId().toInt());
    qDebug() << "Customer has been successfully add to database";
    return true;
};

QList<Customer> DatabaseManager::getAllCustomers(){
    QList<Customer> customers;
    QSqlQuery query;

    QString selectStr = R"(
        SELECT id, name, surname, birthdate, licence, status FROM customers;
    )";

    if(!query.exec(selectStr)){
        qWarning() << "Cannot fetch customers from database";
        return customers;
    }

    while(query.next()){
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString surname = query.value("surname").toString();
        QDate birthdate = query.value("birthdate").toDate();
        QString licence = query.value("licence").toString();
        Customer::CustomerStatus status = static_cast<Customer::CustomerStatus>(query.value("status").toInt());

        Customer customer(id, name, surname, birthdate, licence, status);

        customers.append(customer);
    }

    return customers;

} 

bool DatabaseManager::addRental(Rental& rental){
    db.transaction();

    QSqlQuery vehicleQueryUpdate;

    vehicleQueryUpdate.prepare("UPDATE vehicles SET status = :status WHERE id = :id AND status = :status_available");
    vehicleQueryUpdate.bindValue(":status", static_cast<int>(Vehicle::Status::Rented));
    vehicleQueryUpdate.bindValue(":id", rental.vehicleId());
    vehicleQueryUpdate.bindValue(":status_available", static_cast<int>(Vehicle::Status::Available));

    if(!vehicleQueryUpdate.exec() || vehicleQueryUpdate.numRowsAffected() == 0){
        qWarning() << "Cannot add rental to database. Check if you are not trying to rent car that was already rented";
        db.rollback();
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO rentals(customer_id, vehicle_id, start_date, end_date, total_cost, status) VALUES (:customer_id, :vehicle_id, :start_date, :end_date, :total_cost, :status)");
    query.bindValue(":customer_id", rental.customerId());
    query.bindValue(":vehicle_id", rental.vehicleId());
    qDebug() << rental.startDate();
    query.bindValue(":start_date", rental.startDate().toString(Qt::ISODate));
    query.bindValue(":end_date", rental.endDate().toString(Qt::ISODate));
    query.bindValue(":total_cost", rental.totalCost());
    query.bindValue(":status", static_cast<int>(rental.status()));

    
    if(!query.exec()){
        qWarning() << "Cannot add rental to database" << query.lastError().text();
        db.rollback();
        return false;
    }

    rental.setId(query.lastInsertId().toInt());
    qDebug() << "Successfully rented " << rental.vehicleId() << " vehicle to " << rental.customerId() << " customer";
    return db.commit(); 
}

QList<Rental> DatabaseManager::getAllRentals(){
    QSqlQuery query;

    QList<Rental> rentals;

    QString rentalsStr = R"(
        SELECT * FROM rentals;
    )";

    if(!query.exec(rentalsStr)){
        qWarning() << "Cannot fetch rentals from database";
        return rentals;
    }

    while(query.next()){
        int id = query.value("id").toInt();
        int customer_id = query.value("customer_id").toInt();
        int vehicle_id = query.value("vehicle_id").toInt();
        QDate start_date = query.value("start_date").toDate();
        QDate end_date = query.value("end_data").toDate();

        double total_cost = query.value("total_cost").toDouble();

        Rental::Status status = static_cast<Rental::Status>(query.value("status").toInt());

        Rental r(id, customer_id, vehicle_id, start_date, end_date, total_cost, status);

        rentals.append(r);
    }

    return rentals;
}


bool DatabaseManager::initSchema(){
    QSqlQuery q;

    QString createVehicles = R"(
        CREATE TABLE IF NOT EXISTS vehicles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL,
            brand TEXT NOT NULL, 
            model TEXT NOT NULL, 
            plate TEXT UNIQUE NOT NULL, 
            category TEXT NOT NULL, 
            daily_rate REAL NOT NULL, 
            status TEXT NOT NULL DEFAULT 'Available',
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
    QString createRentals = R"(
        CREATE TABLE IF NOT EXISTS rentals (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            customer_id INTEGER NOT NULL,
            vehicle_id INTEGER NOT NULL,
            start_date TEXT NOT NULL,
            end_date TEXT NOT NULL,
            total_cost REAL NOT NULL,
            status TEXT NOT NULL DEFAULT 'ACTIVE',
            FOREIGN KEY(customer_id) REFERENCES customers(id) ON DELETE RESTRICT,
            FOREIGN KEY(vehicle_id) REFERENCES vehicles(id) ON DELETE RESTRICT
        )
    )";

    if(!q.exec(createVehicles) || !q.exec(createCustomers) || !q.exec(createPassengerCars) || !q.exec(createDeliveryVans) || !q.exec(createRentals)){
        qCritical() << "Error while creating tables" << q.lastError().text();

        return false;
    }
    return true;
}