#include "core/vehicles/DeliveryVan.h"
#include "core/vehicles/PassengerCar.h"
#include "dashboard.h"
#include <DatabaseManager.h>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    static DatabaseManager &db_manager = DatabaseManager::instance();
    if (!db_manager.conn_open("test.db")) {
        return 1;
    }
    qDebug() << "Correct initalization";
    db_manager.initSchema();

    QList<QSharedPointer<Vehicle>> fleet;

    auto car1 = QSharedPointer<PassengerCar>::create(0, "Audi", "A4", "PY 87282", Vehicle::Category::Economy, 100, Vehicle::Status::Available, 5, 5, true, Vehicle::Transmission::Manual, Vehicle::Fuel::Diesel, PassengerCar::BodyClass::Hatchback);

    auto car2 = QSharedPointer<PassengerCar>::create(0, "Toyota", "Corolla", "PSL 26222", Vehicle::Category::Economy, 150, Vehicle::Status::Available, 5, 5, true, Vehicle::Transmission::Automatic, Vehicle::Fuel::Electric, PassengerCar::BodyClass::Sedan);

    auto van = QSharedPointer<DeliveryVan>::create(0, "Iveco", "Daily", "PKN 44321", Vehicle::Category::Economy, 250, Vehicle::Status::Rented, Vehicle::Transmission::Manual, Vehicle::Fuel::Diesel, 3980, 9.5, false, 2.87);

    Customer customer1(0, "Jan", "Kowalski", QDate::fromString("1990-01-01", Qt::ISODate), "B", Customer::CustomerStatus::CanRent);
    db_manager.addCustomer(customer1);

    fleet.append(car1);
    fleet.append(car2);
    fleet.append(van);

    for (const auto &vehicle : fleet) {
        if (db_manager.addVehicle(vehicle)) {
            qDebug() << "Vehicle " << vehicle->plate() << " successfully added to database";
        }
    }

    QList<QSharedPointer<Vehicle>> from_database = db_manager.getAllVehicles();
    if (from_database.count() == 0) {
        qWarning() << "No items fetched from database";
    } else {
        for (const auto &vehicle : fleet) {
            qDebug() << "Vehicle " << vehicle->plate() << " successfully fetched from database";
        }
    }

    QList<Customer> customers = db_manager.getAllCustomers();
    if (customers.count() == 0) {
        qWarning() << "No items fetched from database";
    } else {
        for (const Customer &customer : customers) {
            qDebug() << "Customer " << customer.name() << " " << customer.surname() << " successfully fetched from database";
        }
    }
    QDate rentalStart = QDate::fromString("2026-11-03", "yyyy-MM-dd");
    QDate rentalEnd = QDate::fromString("2026-11-14", "yyyy-MM-dd");
    Rental rental1(0, customer1.id(), car1->id(), rentalStart, rentalEnd, Rental::calculateTotalCost(rentalStart, rentalEnd, car1->dailyRate()));

    db_manager.addRental(rental1);

    QList<Rental> rentals = db_manager.getAllRentals();
    if (rentals.count() == 0) {
        qWarning() << "No rentals fetched from database";
    } else {
        for (const Rental &rental : rentals) {
            qDebug() << "Customer " << rental.customerId() << " rented vehicle: " << rental.vehicleId() << " [record fetched from database]";
        }
    }

    DashboardWindow window;
    window.resize(1600, 900);
    window.show();

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        app.setStyleSheet(stream.readAll());
        styleFile.close();
    } else {
        qWarning() << "Nie udalo sie wczytac pliku style.qss!";
    }

    window.show();
    db_manager.close();

    return app.exec();
}