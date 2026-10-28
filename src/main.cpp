#include <QApplication>
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <DatabaseManager.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    static DatabaseManager& db_manager = DatabaseManager::instance();
    if(!db_manager.conn_open("test.db")){
        return 1;
    }
    qDebug() << "Correct initalization";
    db_manager.initSchema();
    
    Vehicle vehicleInstance1(1,"Audi", "A4", "PO 89812", Vehicle::Category::Economy, 200.0, Vehicle::Status::Rented);
    Vehicle vehicleInstance2(1,"Skoda", "Octavia", "PY 54332", Vehicle::Category::Economy, 200.0, Vehicle::Status::InService);
    Vehicle vehicleInstance3(1,"Porsche", "911", "CT 65643", Vehicle::Category::Premium, 200.0, Vehicle::Status::Available);

    db_manager.addVehicle(vehicleInstance1);
    db_manager.addVehicle(vehicleInstance2);
    db_manager.addVehicle(vehicleInstance3);

    QList<Vehicle> vehicles = db_manager.getAllVehicles();

    for(const Vehicle& v : vehicles){
        qDebug() << "Marka: " << v.brand() << ", Model: " << v.model() << ", Blachy: " << v.plate();
    }

    db_manager.close();

    QWidget window;
    window.resize(400, 300);
    window.show();
    
    return app.exec();
}