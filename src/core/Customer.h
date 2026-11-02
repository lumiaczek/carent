#pragma once
#include <QDateTime>

class Customer{
    public:
        enum class CustomerStatus {
            CanRent, 
            CannotRent
        };
        Customer() = default;
        Customer(int id, const QString& name, const QString& surname, const QDateTime& birthdate, const QString& licence, CustomerStatus status = CustomerStatus::CanRent): m_id(id), m_name(name), m_surname(surname), m_birthdate(birthdate), m_licence(licence), m_status(status) {};
        ~Customer() = default;
        int id() {return m_id;}
        QString name() {return m_name;}
        QString surname() {return m_surname;}
        QDateTime birthdate() {return m_birthdate;}
        QString licence() {return m_licence;}
        CustomerStatus status() {return m_status;}
    private:
        int m_id;
        QString m_name;
        QString m_surname;
        QDateTime m_birthdate;
        QString m_licence;
        CustomerStatus m_status;
};