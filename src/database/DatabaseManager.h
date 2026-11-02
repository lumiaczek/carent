#pragma once
#include <QSqlDatabase>
#include <QString>
#include "../core/vehicles/Vehicle.h"
#include "../core/Customer.h"

class DatabaseManager{
    public:
        static DatabaseManager& instance();

        bool conn_open(const QString& dbPath = "carent_database.db");
        void close();
        bool initSchema();
        bool addVehicle(QSharedPointer<Vehicle> v);
        bool removeVehicle(const QString& plate);
        QList<QSharedPointer<Vehicle>> getAllVehicles();
        bool addCustomer(const Customer& customer);

    private:
        DatabaseManager() = default;
        ~DatabaseManager();
        DatabaseManager(const DatabaseManager&) = delete;
        DatabaseManager& operator=(const DatabaseManager&) = delete;
        
        QSqlDatabase db;
};