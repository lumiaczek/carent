#include "dashboard.h"
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
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "DatabaseSeeder.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    static DatabaseManager& db_manager = DatabaseManager::instance();
    if(!db_manager.conn_open("test.db")){
        return 1;
    }
    qDebug() << "Correct initalization";
    db_manager.initSchema();

    DatabaseSeeder::seedIfEmpty();

    DashboardService dashboardService;


    // QList<QSharedPointer<Vehicle>> from_database = db_manager.getAllVehicles();
    // if(from_database.count() == 0) {
    //     qWarning() << "No items fetched from database";
    // }
    // else{
    //     for(const auto& vehicle : from_database){
    //         qDebug() << "Vehicle "<< vehicle->plate() << " successfully fetched from database";
    //         qDebug() << "Id: " <<vehicle->id();
    //     }
    // }

    QList<Customer> customers = db_manager.getAllCustomers();
    if(customers.count() == 0) {
        qWarning() << "No items fetched from database";
    }
    else{
        for(const Customer& customer : customers){
            qDebug() << "Customer "<< customer.name() << " " <<customer.surname() << " successfully fetched from database";
        }
    }

    QList<Rental> rentals = db_manager.getAllRentals();
    if(rentals.count() == 0){
        qWarning() << "No rentals fetched from database";
    }
    else{
        for(const Rental& rental : rentals){
            qDebug() << "Customer "<< rental.customerId() << " rented vehicle: " <<rental.vehicleId() << " [record fetched from database]";
        }
    }

    

    
    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        app.setStyleSheet(stream.readAll());
        styleFile.close();
    } else {
        qWarning() << "Nie udalo sie wczytac pliku style.qss!";
    }

    DashboardWindow window(&dashboardService);
    window.show();

    
    return app.exec();
}