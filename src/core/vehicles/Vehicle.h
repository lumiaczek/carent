#pragma once
#include <QString>
#include <QDateTime>

class Vehicle{
    public:
        enum class Status {
            Available,
            Rented,
            InService,
            Incident
        };
        enum class Category{
            Economy,
            Premium
        };

        Vehicle() = default;

        Vehicle(int id, const QString& brand, const QString& model, const QString& plate, Category cat, double dailyRate, Status status = Status::Available);

        int id() const { return m_id; }
        QString brand() const { return m_brand; }
        QString model() const { return m_model; }
        QString plate() const { return m_plate; }
        Category category() const { return m_category; }
        Status status() const { return m_status; }

    private:

        int m_id = 0;
        QString m_brand;
        QString m_model;
        QString m_plate;
        Category m_category = Category::Economy;
        Status m_status = Status::Available;

};