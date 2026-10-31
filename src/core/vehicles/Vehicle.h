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
        enum class Type{
            Generic, 
            PassengerCar,
            DeliveryVan,
            Scooter
        };
        enum class Transmission{
            Manual,
            Automatic
        };
        enum class Fuel{
            Gas,
            Diesel,
            LPG,
            Electric
        };

        Vehicle() = default;

        Vehicle(int id, const QString& brand, const QString& model, const QString& plate, Category cat, double dailyRate, Status status = Status::Available, Transmission transmission, Fuel fuel) : m_id(id), m_brand(brand), m_model(model), m_plate(plate), m_category(cat), m_status(status), m_transmission(transmission), m_fuel(fuel){}

        virtual ~Vehicle() = default;

        int id() const { return m_id; }
        QString brand() const { return m_brand; }
        QString model() const { return m_model; }
        QString plate() const { return m_plate; }
        Category category() const { return m_category; }
        Status status() const { return m_status; }
        double dailyRate() {return m_dailyRate; }
        Transmission transmission() const {return m_transmission;}
        Fuel fuel() const {return m_fuel;}


        void setId(int id){m_id = id;}
        void setBrand(const QString& brand){m_brand = brand;}
        void setModel(const QString& model){m_model = model;}
        void setPlate(const QString& plate){m_plate = plate;}
        void setCategory(Category cat){m_category = cat;}
        void setStatus(Status status){m_status = status;}


        virtual Type type() const {return Type::Generic;}

    private:
        Transmission m_transmission;
        Fuel m_fuel;
        int m_id = 0;
        QString m_brand;
        QString m_model;
        QString m_plate;
        Category m_category = Category::Economy;
        Status m_status = Status::Available;
        double m_dailyRate = 0.0;

};