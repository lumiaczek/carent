#include "core/vehicles/DeliveryVan.h"
#include "core/vehicles/PassengerCar.h"
#include "dashboard.h"
#include <DatabaseManager.h>
#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QFont>
#include <QFontDatabase>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>
#include <QWidget>

namespace {

QString registerFontFile(const QString &path) {
    const int id = QFontDatabase::addApplicationFont(path);
    if (id == -1) {
        qWarning() << "Nie udało się załadować czcionki:" << path;
        return {};
    }
    const QStringList families = QFontDatabase::applicationFontFamilies(id);
    if (families.isEmpty()) {
        qWarning() << "Brak nazw rodzin w czcionce:" << path;
        return {};
    }
    return families.first();
}

QString registerFontDirectory(const QString &dirPath) {
    QString primaryFamily;
    QDirIterator it(dirPath, {"*.ttf", "*.otf"}, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString filePath = it.next();
        const QString family = registerFontFile(filePath);
        if (primaryFamily.isEmpty() && !family.isEmpty()) {
            primaryFamily = family;
        }
    }
    if (primaryFamily.isEmpty()) {
        qWarning() << "Nie znaleziono żadnych poprawnych plików fontów w:" << dirPath;
    }
    return primaryFamily;
}

} // namespace
#include "DatabaseSeeder.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const QString interFamily = registerFontFile(":/fonts/Inter.ttf");
    const QString poppinsFamily = registerFontDirectory(":/fonts/Poppins");

    qDebug() << "Inter family:" << interFamily;
    qDebug() << "Poppins family:" << poppinsFamily;

    if (!interFamily.isEmpty()) {
        QFont defaultFont(interFamily);
        app.setFont(defaultFont);
    }

    static DatabaseManager &db_manager = DatabaseManager::instance();
    if (!db_manager.conn_open("test.db")) {
        return 1;
    }
    qDebug() << "Correct initalization";
    db_manager.initSchema();

    DatabaseSeeder::seedIfEmpty();

    DashboardService dashboardService;

    // QList<QSharedPointer<Vehicle>> from_database = db_manager.getAllVehicles();
    // if(from_database.count() == 0) {
    //     qWarning() << "No items fetched from database";
    // }
    // else{
    //     for(const auto& vehicle : from_database){
    //         qDebug() << "Vehicle "<< vehicle->plate() << " successfully fetched from database";
    //         qDebug() << "Id: " <<vehicle->id();
    //     }
    // }

    QList<Customer> customers = db_manager.getAllCustomers();
    if (customers.count() == 0) {
        qWarning() << "No items fetched from database";
    } else {
        for (const Customer &customer : customers) {
            qDebug() << "Customer " << customer.name() << " " << customer.surname() << " successfully fetched from database";
        }
    }

    QList<Rental> rentals = db_manager.getAllRentals();
    if (rentals.count() == 0) {
        qWarning() << "No rentals fetched from database";
    } else {
        for (const Rental &rental : rentals) {
            qDebug() << "Customer " << rental.customerId() << " rented vehicle: " << rental.vehicleId() << " [record fetched from database]";
        }
    }

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        app.setStyleSheet(stream.readAll());
        styleFile.close();
    } else {
        qWarning() << "Nie udalo sie wczytac pliku style.qss!";
    }

    DashboardWindow window(&dashboardService);
    window.show();
    db_manager.close();

    return app.exec();
}