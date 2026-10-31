#include <QApplication>
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <DatabaseManager.h>
#include <QSharedPointer>
#include "core/vehicles/PassengerCar.h"
#include "core/vehicles/DeliveryVan.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    static DatabaseManager& db_manager = DatabaseManager::instance();
    if(!db_manager.conn_open("test.db")){
        return 1;
    }
    qDebug() << "Correct initalization";
    db_manager.initSchema();


    QList<QSharedPointer<Vehicle>> fleet;

    auto car1 = QSharedPointer<PassengerCar>::create(1, "Audi", "A4", "PY 87282", Vehicle::Category::Economy, 100, Vehicle::Status::Available, 5, 5, true,Vehicle::Transmission::Manual, Vehicle::Fuel::Diesel, PassengerCar::BodyClass::Hatchback);

    auto car2 = QSharedPointer<PassengerCar>::create(2, "Toyota", "Corolla", "PSL 26222", Vehicle::Category::Economy, 150, Vehicle::Status::Available, 5, 5, true,Vehicle::Transmission::Automatic, Vehicle::Fuel::Electric, PassengerCar::BodyClass::Sedan);

    auto van = QSharedPointer<DeliveryVan>::create(3, "Iveco", "Daily", "PKN 44321", Vehicle::Category::Economy, 250, Vehicle::Status::Rented, Vehicle::Transmission::Manual, Vehicle::Fuel::Diesel, 3980, 9.5, false, 2.87);

    fleet.append(car1);
    fleet.append(car2);
    fleet.append(van);

    for(const auto& vehicle : fleet){
        if(db_manager.addVehicle(vehicle)){
            qDebug() << "Vehicle "<< vehicle->plate() << " successfully added to database";
        }
    }

    db_manager.close();

    QWidget window;
    window.resize(400, 300);
    window.show();
    
    return app.exec();
}