#pragma once
#include <QDate>

class Rental{
    public:
        enum class Status {
            Active, 
            Completed
        };

        Rental() = default;
        Rental(int id, int customerId, int vehicleId, const QDateTime& start, const QDateTime& end, double totalCost, Status status = Status::Active);

        int id() const {return m_id;}
        int customerId() const {return m_customerId;}
        int vehicleId() const {return m_vehicleId;}
        QDate startDate() const {return m_startDate;}
        QDate endDate() const {return m_endDate;}
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

    private:
        int m_id = 0;
        int m_customerId = 0;
        int m_vehicleId = 0;
        QDate m_startDate;
        QDate m_endDate;
        Status m_status = Status::Active;

};