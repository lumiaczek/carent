#pragma once
#include <QSqlDatabase>
#include <QString>
#include "../core/vehicles/Vehicle.h"

class DatabaseManager{
    public:
        static DatabaseManager& instance();

        bool conn_open(const QString& dbPath = "carent_database.db");
        void close();
        bool initSchema();
        bool addVehicle(const Vehicle& v);
        bool removeVehicle(const QString& plate);
        QList<Vehicle> getAllVehicles();

    private:
        DatabaseManager() = default;
        ~DatabaseManager();
        DatabaseManager(const DatabaseManager&) = delete;
        DatabaseManager& operator=(const DatabaseManager&) = delete;
        
        QSqlDatabase db;
};