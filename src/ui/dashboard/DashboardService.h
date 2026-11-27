#pragma once
#include <QObject>
#include "DashboardModels.h"

class DashboardService : public QObject {
    Q_OBJECT
public:
    explicit DashboardService(QObject *parent = nullptr);

    DashboardData loadDashboardData() const;

signals:
    void dataChanged();
};