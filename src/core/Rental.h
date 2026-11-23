#pragma once
#include <QDate>

class Rental{
    public:
        enum class Status {
            Active, 
            Completed
        };

        Rental() = default;
        Rental(int id, int customerId, int vehicleId, const QDate& start, const QDate& end, double totalCost, Status status = Status::Active) : m_id(id), m_customerId(customerId), m_vehicleId(vehicleId), m_startDate(start), m_endDate(end), m_totalCost(totalCost), m_status(status) {};

        int id() const {return m_id;}
        int customerId() const {return m_customerId;}
        int vehicleId() const {return m_vehicleId;}
        QDate startDate() const {return m_startDate;}
        QDate endDate() const {return m_endDate;}
        double totalCost() const {return m_totalCost;}
        Status status() const {return m_status;}

        void setId(int id){
            m_id = id;
        }
        void setCustomerId(int customerId){
            m_customerId = customerId;
        }
        void setVehicleId(int vehicleId){
            m_vehicleId = vehicleId;
        }
        void setStartDate(const QDate& startDate){
            m_startDate = startDate;
        }
        void setEndDate(const QDate& endDate){
            m_endDate = endDate;
        }
        void setTotalCost(double total){
            m_totalCost = total;
        }

        static double calculateTotalCost(const QDate& start, const QDate& end, double car_daily_rate){
            int days = start.daysTo(end);
            if(days <=0) return 0.0;

            return days * car_daily_rate;
        }

    private:
        int m_id = 0;
        int m_customerId = 0;
        int m_vehicleId = 0;
        double m_totalCost = 0.0;
        QDate m_startDate;
        QDate m_endDate;
        Status m_status = Status::Active;

};