#pragma once
#include "Vehicle.h"

class DeliveryVan : public Vehicle{
    public:
        double capacity() {return m_capacity;}
        void setCapacity(double capacity) {
            if(capacity > 0) m_capacity = capacity;
        }

        DeliveryVan(int id, const QString& brand, const QString& model, const QString& plate, Category cat, double dailyRate, Status status = Status::Available, double capacity) : Vehicle(id, brand, model, plate, cat, dailyRate, status), m_capacity(capacity){};

        Type type() const override {return Type::DeliveryVan;}
    private:
        double m_capacity = 0.0; 
};