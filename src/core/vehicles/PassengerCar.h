#pragma once
#include "Vehicle.h"

class PassengerCar : public Vehicle{
    public:
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
        enum class BodyClass{
            SUV,
            Coupe, 
            Sedan, 
            Hatchback, 
            Roadster
        };
        PassengerCar(int id, const QString& brand, const QString& model, const QString& plate, Category cat, double dailyRate, Status status = Status::Available, int seats, int doors, bool hasIsofix, Transmission transmission, Fuel fuel, BodyClass bodyClass) : Vehicle(id, brand, model, plate, cat, dailyRate, status), m_seats(seats), m_hasIsofix(hasIsofix), m_doors(doors), m_transmission(transmission), m_bodyClass(bodyClass), m_fuel(fuel){};

        int seats() {return m_seats;}
        void setSeats(int seats) {
            if(seats >=1 && seats <= 9) m_seats = seats;
        }


        Type type() const override {return Type::PassengerCar;}

    private:
        Transmission m_transmission;
        Fuel m_fuel;
        BodyClass m_bodyClass;
        int m_seats;
        int m_trunkCapacity = 0.0;
        bool m_hasIsofix;
        int m_doors = 5;
};