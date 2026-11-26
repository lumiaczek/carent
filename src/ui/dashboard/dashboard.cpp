#include "dashboard.h"
#include "../svg_helper.h"
#include "ui_dashboard.h"
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QPainter>
#include <QPixmap>

using namespace carent::ui;

DashboardWindow::DashboardWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DashboardWindow) {
    ui->setupUi(this);

    QList<QWidget *> cards = {
        ui->cardIncome, ui->cardCosts, ui->cardClientsKpi, ui->cardRentalsKpi,
        ui->cardVehicles, ui->cardRentalsTable, ui->cardNotifications, ui->cardClientsTable};
    for (auto *card : cards) {
        applyShadow(card);
    }

    ui->iconIncome->setPixmap(colorizeSvgPixmap(":/icons/trend-up.svg", QColor("#2e9f63"), {20, 20}));
    ui->iconCosts->setPixmap(colorizeSvgPixmap(":/icons/fuel.svg", QColor("#d94343"), {20, 20}));
    ui->iconClientsKpi->setPixmap(colorizeSvgPixmap(":/icons/user.svg", QColor("#3b76a0"), {20, 20}));
    ui->iconRentalsKpi->setPixmap(colorizeSvgPixmap(":/icons/car-front.svg", QColor("#1e88e5"), {20, 20}));

    ui->ActionBtnBlue->setIcon(colorizeSvgIcon(":/icons/key-square.svg", Qt::white, {16, 16}));
    ui->ActionBtnGreen->setIcon(colorizeSvgIcon(":/icons/user-add.svg", Qt::white, {16, 16}));

    setupTable(ui->tableRentals);
    setupTable(ui->tableClients);
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
    ui->tableRentals->setRowCount(3);
    struct RentalRow {
        QString car, client, plate, days, bg, fg;
    };
    QList<RentalRow> rentals = {
        {"Ford Ecosport", "Beata B.", "PO 6T44J", "7 dni", "#e8f4fd", "#1e88e5"},
        {"Hyundai i30", "Tomasz K.", "PO 5A333", "4 dni", "#e8f4fd", "#1e88e5"},
        {"Iveco", "Budimex", "PO 4K21A", "2 dni", "#fef3e6", "#d97724"}};

    for (int i = 0; i < rentals.size(); ++i) {
        auto *carItem = new QTableWidgetItem(rentals[i].car);
        QFont f = carItem->font();
        f.setBold(true);
        carItem->setFont(f);
        ui->tableRentals->setItem(i, 0, carItem);
        ui->tableRentals->setItem(i, 1, new QTableWidgetItem(rentals[i].client));
        ui->tableRentals->setItem(i, 2, new QTableWidgetItem(rentals[i].plate));
        ui->tableRentals->setCellWidget(i, 3, createBadge(rentals[i].days, rentals[i].bg, rentals[i].fg));
    }

    ui->tableClients->setRowCount(2);
    QFont boldFont;
    boldFont.setBold(true);

    auto *c1 = new QTableWidgetItem("Beata Barniak");
    c1->setFont(boldFont);
    ui->tableClients->setItem(0, 0, c1);
    ui->tableClients->setItem(0, 1, new QTableWidgetItem("PO 6T44J"));
    ui->tableClients->setCellWidget(0, 2, createBadge("Ważne", "#e8f6ef", "#2e9f63"));
    ui->tableClients->setCellWidget(0, 3, createBadge("Aktywny", "#e8f6ef", "#2e9f63"));

    auto *c2 = new QTableWidgetItem("Tomasz Kolak");
    c2->setFont(boldFont);
    ui->tableClients->setItem(1, 0, c2);
    ui->tableClients->setItem(1, 1, new QTableWidgetItem("Brak"));
    ui->tableClients->setCellWidget(1, 2, createBadge("Nieważne", "#fceaea", "#d94343"));
    ui->tableClients->setCellWidget(1, 3, createBadge("Aktywny", "#e8f6ef", "#2e9f63"));
}
