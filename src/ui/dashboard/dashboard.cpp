#include "dashboard.h"
#include "ui_dashboard.h"
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>


DashboardWindow::DashboardWindow(DashboardService *service, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DashboardWindow), service(service) {
    ui->setupUi(this);

    QList<QWidget *> cards = {
        ui->cardIncome, ui->cardCosts, ui->cardClientsKpi, ui->cardRentalsKpi,
        ui->cardVehicles, ui->cardRentalsTable, ui->cardNotifications, ui->cardClientsTable};
    for (auto *card : cards) {
        applyShadow(card);
    }

    setupTable(ui->tableRentals);
    setupTable(ui->tableClients);

    connect(service, &DashboardService::dataChanged, this, &DashboardWindow::populateTables);

    populateTables();

}

DashboardWindow::~DashboardWindow() {
    delete ui;
}

void DashboardWindow::applyShadow(QWidget *widget) {
    auto *shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 18));
    widget->setGraphicsEffect(shadow);
}

void DashboardWindow::setupTable(QTableWidget *table) {
    table->verticalHeader()->setVisible(false);
    table->setShowGrid(false);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setFocusPolicy(Qt::NoFocus);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setDefaultSectionSize(44);
}

QWidget *DashboardWindow::createBadge(const QString &text, const QString &bgColor, const QString &textColor) {
    auto *container = new QWidget(this);
    auto *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);

    auto *pill = new QLabel(text, container);
    pill->setStyleSheet(QString("background-color: %1; color: %2; border-radius: 10px; "
                                "padding: 4px 12px; font-weight: 600; font-size: 12px;")
                            .arg(bgColor, textColor));
    layout->addWidget(pill);
    return container;
}

void DashboardWindow::populateTables() {
    const DashboardData data = service->loadDashboardData();

    ui->tableRentals->clearContents();
    ui->tableRentals->setRowCount(data.rentals.size());
    struct RentalRow {
        QString car, client, plate, days, bg, fg;
    };

    for (int i = 0; i < data.rentals.size(); ++i) {
        const auto &r = data.rentals[i];
        auto carItem = new QTableWidgetItem(r.carName);

        QFont f = carItem->font();
        f.setBold(true);
        carItem->setFont(f);
        ui->tableRentals->setItem(i, 0, carItem);
        ui->tableRentals->setItem(i, 1, new QTableWidgetItem(r.clientName));
        ui->tableRentals->setItem(i, 2, new QTableWidgetItem(r.plate));
        QString bg = (r.daysLeft <= 2) ? "#fef3e6" : "#e8f4fd";
        QString fg = (r.daysLeft <= 2) ? "#d97724" : "#1e88e5";
        ui->tableRentals->setCellWidget(i, 3, createBadge(QString("%1 dni").arg(r.daysLeft), bg, fg));
    }

    ui->tableClients->clearContents();
    ui->tableClients->setRowCount(data.clients.size());
    QFont boldFont;
    boldFont.setBold(true);


    for(int i = 0; i< data.clients.size();++i){
        const auto &c = data.clients[i];

        auto *nameItem = new QTableWidgetItem(c.fullName);
        nameItem->setFont(boldFont);
        ui->tableClients->setItem(i, 0, nameItem);
        ui->tableClients->setItem(i, 1, new QTableWidgetItem(c.activePlate));

        ui->tableClients->setCellWidget(i, 2, c.hasValidLicence
            ? createBadge("Ważne", "#e8f6ef", "#2e9f63")
            : createBadge("Nieważne", "#fceaea", "#d94343"));

        ui->tableClients->setCellWidget(i, 3, c.isActive
            ? createBadge("Aktywny", "#e8f6ef", "#2e9f63")
            : createBadge("Nieaktywny", "#fceaea", "#d94343"));
    }

    int totalVehicles = data.totalVehicles();

    ui->chartPlaceholder->setText(QString("%1\nPojazdów").arg(totalVehicles));
    ui->legFree->setText(QString("🟦  Wolne - %1").arg(data.availableCount));
    ui->legRented->setText(QString("🟩  Wynajęte - %1").arg(data.rentedCount));
    ui->legRepair->setText(QString("🟧  Naprawa - %1").arg(data.serviceCount));

    ui->lblClientsKpiVal->setText(QString("%1").arg(data.clients.size()));
    ui->lblRentalsKpiVal->setText(QString("%1").arg(data.rentals.size()));
}