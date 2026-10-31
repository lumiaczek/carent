#pragma once
#include "Vehicle.h"

class DeliveryVan : public Vehicle{
    public:
        double capacity() {return m_capacity;}
        void setCapacity(double capacity) {
            if(capacity > 0) m_capacity = capacity;
        }

        DeliveryVan(int id, const QString& brand, const QString& model, const QString& plate, Category cat, double dailyRate, Status status = Status::Available, Transmission transmission, Fuel fuel, double capacity, double cargoVolume, bool hasTailLift, double vehicleHeight) : Vehicle(id, brand, model, plate, cat, dailyRate, status, transmission, fuel), m_capacity(capacity), m_cargoVolume{cargoVolume}, m_hasTailLift(hasTailLift), m_vehicleHeight(vehicleHeight){};

        Type type() const override {return Type::DeliveryVan;}

        double cargoVolume() {return m_cargoVolume;}
        double capacity() {return m_capacity;}
        bool hasTailLift() {return m_hasTailLift;}
        double vehicleHeight() {return m_vehicleHeight;}
    private:
        double m_capacity = 0.0; 
        double m_cargoVolume;
        bool m_hasTailLift;
        double m_vehicleHeight;

};