#pragma once
#include <QDate>

class Customer{
    public:
        enum class CustomerStatus {
            CanRent, 
            CannotRent
        };
        Customer() = default;
        Customer(const QString& name, const QString& surname, const QDate& birthdate, const QString& licence, CustomerStatus status = CustomerStatus::CanRent): m_name(name), m_surname(surname), m_birthdate(birthdate), m_licence(licence), m_status(status) {};
        ~Customer() = default;
        int id() const {return m_id;}
        QString name() const {return m_name;}
        QString surname() const {return m_surname;}
        QDate birthdate() const {return m_birthdate;}
        QString licence() const  {return m_licence;}
        CustomerStatus status() const {return m_status;}
        void setId(int id) {
            m_id = id;
        }
    private:
        int m_id;
        QString m_name;
        QString m_surname;
        QDate m_birthdate;
        QString m_licence;
        CustomerStatus m_status;
};