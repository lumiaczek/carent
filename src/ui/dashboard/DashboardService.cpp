#include "DashboardService.h"
#include "../database/DatabaseManager.h"
#include <QDate>
#include <QHash>

DashboardService::DashboardService(QObject *parent) : QObject(parent) {}

DashboardData DashboardService::loadDashboardData() const {
    DashboardData result;

    auto &db = DatabaseManager::instance();

    const auto vehicles = db.getAllVehicles();
    const auto customers = db.getAllCustomers();
    const auto rentals = db.getAllRentals();

    QHash<int, QSharedPointer<Vehicle>> vehicleById;
    for(const auto &v : vehicles){
        vehicleById.insert(v->id(), v);

        switch(v->status()){
            case Vehicle::Status::Available:
                result.availableCount++;
                break;
            case Vehicle::Status::Rented:
                result.rentedCount++;
                break;
            default:
                result.serviceCount++;
                break;
        }
    }

    QHash<int, Customer> customerById;
    QHash<int, QString> activePlateByCustomer;

    for (const auto &c : customers) {
        customerById.insert(c.id(), c);
    }
    const QDate today = QDate::currentDate();

    for(const auto &r : rentals){
        if(r.status() != Rental::Status::Active) continue;

        auto v = vehicleById.value(r.vehicleId());
        qDebug()<<"ID w DashboardService: "<<r.vehicleId();
        if(!v || !customerById.contains(r.customerId())) continue;

        const Customer &c = customerById[r.customerId()];
        activePlateByCustomer.insert(c.id(), v->plate());
        
        DashboardRentalRow row;

        row.carName = v->brand() + " " + v->model();
        row.clientName = c.name() + " " + c.surname().left(1) + ".";
        row.plate = v->plate();
        row.daysLeft = qMax(0, static_cast<int>(today.daysTo(r.endDate())));

        result.rentals.append(row);
    }

    for (const auto &c : customers) {
        DashboardClientRow row;
        row.fullName = c.name() + " " + c.surname();
        row.activePlate = activePlateByCustomer.value(c.id(), "Brak");
        row.hasValidLicence = !c.licence().isEmpty();
        row.isActive = (c.status() == Customer::CustomerStatus::CanRent);

        result.clients.append(row);
    }

    return result;

}