#pragma once
#include "Vehicle.h"

class PassengerCar : public Vehicle{
    public:
        enum class BodyClass{
            SUV,
            Coupe, 
            Sedan, 
            Hatchback, 
            Roadster
        };
        PassengerCar(int id, const QString& brand, const QString& model, const QString& plate, Category cat, double dailyRate, Status status = Status::Available, int seats, int doors, bool hasIsofix, Transmission transmission, Fuel fuel, BodyClass bodyClass) : Vehicle(id, brand, model, plate, cat, dailyRate, status, transmission, fuel), m_seats(seats), m_hasIsofix(hasIsofix), m_doors(doors), m_bodyClass(bodyClass){};

        int seats() {return m_seats;}
        void setSeats(int seats) {
            if(seats >=1 && seats <= 9) m_seats = seats;
        }
        int trunkCapacity() {return m_trunkCapacity;}
        bool hasIsofix() {return m_hasIsofix;}
        int doors() {return m_doors;}
        BodyClass bodyClass() const {return m_bodyClass;}


        Type type() const override {return Type::PassengerCar;}

    private:
        BodyClass m_bodyClass;
        int m_seats;
        int m_trunkCapacity = 0.0;
        bool m_hasIsofix;
        int m_doors = 5;
};