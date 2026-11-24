#include "DatabaseSeeder.h"
#include "DatabaseManager.h"
#include "../core/vehicles/PassengerCar.h"
#include "../core/vehicles/DeliveryVan.h"
#include "../core/Customer.h"
#include "../core/Rental.h"
#include <QDate>
#include <QDebug>

void DatabaseSeeder::seedIfEmpty() {
    auto &db = DatabaseManager::instance();

    if (!db.getAllVehicles().isEmpty()) {
        return;
    }

    qDebug() << "Wypełnianie bazy danymi testowymi...";

    auto car1 = QSharedPointer<PassengerCar>::create(
        0, "Ford", "Ecosport", "PO 6T44J",
        Vehicle::Category::Economy, 180.0, Vehicle::Status::Available,
        5, 5, true, Vehicle::Transmission::Manual, Vehicle::Fuel::Gas,
        PassengerCar::BodyClass::SUV
    );
    auto car2 = QSharedPointer<PassengerCar>::create(
        0, "Hyundai", "i30", "PO 5A333",
        Vehicle::Category::Economy, 140.0, Vehicle::Status::Available,
        5, 5, true, Vehicle::Transmission::Automatic, Vehicle::Fuel::Gas,
        PassengerCar::BodyClass::Hatchback
    );
    auto van1 = QSharedPointer<DeliveryVan>::create(
        0, "Iveco", "Daily", "PO 4K21A",
        Vehicle::Category::Economy, 320.0, Vehicle::Status::Available,
        Vehicle::Transmission::Manual, Vehicle::Fuel::Diesel,
        1200.0, 12.5, true, 2.65
    );
    auto car3 = QSharedPointer<PassengerCar>::create(
        0, "Toyota", "Corolla", "PO 9X112",
        Vehicle::Category::Economy, 160.0, Vehicle::Status::Available,
        5, 4, true, Vehicle::Transmission::Automatic, Vehicle::Fuel::Electric,
        PassengerCar::BodyClass::Sedan
    );
    auto car4 = QSharedPointer<PassengerCar>::create(
        0, "Volkswagen", "Golf VI", "PY 66212",
        Vehicle::Category::Economy, 160.0, Vehicle::Status::Available,
        5, 4, true, Vehicle::Transmission::Manual, Vehicle::Fuel::Diesel,
        PassengerCar::BodyClass::Hatchback
    );

    db.addVehicle(car1);
    db.addVehicle(car2);
    db.addVehicle(van1);
    db.addVehicle(car3);
    db.addVehicle(car4);

    Customer c1(0, "Beata", "Barniak", QDate(1990, 5, 14), "ABC12345", Customer::CustomerStatus::CanRent);
    Customer c2(0, "Tomasz", "Kolak", QDate(1985, 11, 2), "", Customer::CustomerStatus::CannotRent); // brak prawa jazdy do testu badge'a
    Customer c3(0, "Anna", "Nowak", QDate(1994, 3, 22), "XYZ98765", Customer::CustomerStatus::CanRent);
    Customer c4(0, "Robert", "Lewandowski", QDate(1994, 3, 22), "XYZ98765", Customer::CustomerStatus::CanRent);

    db.addCustomer(c1);
    db.addCustomer(c2);
    db.addCustomer(c3);
    db.addCustomer(c4);

    const QDate today = QDate::currentDate();

    Rental r1(0, c1.id(), car1->id(), today.addDays(-2), today.addDays(7), 1620.0, Rental::Status::Active);
    Rental r2(0, c4.id(), car4->id(), today.addDays(-10), today.addDays(6), 1280.0, Rental::Status::Active);
    Rental r3(0, c3.id(), van1->id(), today.addDays(-3), today.addDays(1), 1280.0, Rental::Status::Active);

    db.addRental(r1);
    db.addRental(r2);
    db.addRental(r3);
}