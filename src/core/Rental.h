#pragma once
#include <QDateTime>

class Rental{
    public:
        enum class Status {
            Active, 
            Completed
        };

        Rental() = default;
        Rental(int id, int customerId, int vehicleId, const QDateTime& start, const QDateTime& end, double totalCost, Status status = Status::Active);

        int id() {return m_id;}
        int customerId() {return m_customerId;}
        int vehicleId() {return m_vehicleId;}
        QDateTime startDate() const {return m_startDate;}
        QDateTime endDate() const {return m_endDate;}
        void setId(int id){
            m_id = id;
        }
        void setCustomerId(int customerId){
            m_customerId = customerId;
        }
        void setVehicleId(int vehicleId){
            m_vehicleId = vehicleId;
        }
        void setStartDate(const QDateTime& startDate){
            m_startDate = startDate;
        }
        void setEndDate(const QDateTime& endDate){
            m_endDate = endDate;
        }

    private:
        int m_id = 0;
        int m_customerId = 0;
        int m_vehicleId = 0;
        QDateTime m_startDate;
        QDateTime m_endDate;
        Status m_status = Status::Active;

};