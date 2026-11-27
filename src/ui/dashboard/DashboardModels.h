#pragma once
#include <QString>
#include <QList>

struct DashboardRentalRow
{
    QString carName;
    QString clientName;
    QString plate;
    int daysLeft = 0;
};

struct DashboardClientRow{
    QString fullName;
    QString activePlate;
    bool hasValidLicence;
    bool isActive;
};


struct DashboardData
{
    QList<DashboardClientRow> clients;
    QList<DashboardRentalRow> rentals;

    int availableCount = 0;
    int rentedCount = 0;
    int serviceCount = 0;

    int totalVehicles() const {
        return availableCount + rentedCount + serviceCount;
    }
};
