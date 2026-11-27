#pragma once

#include <QMainWindow>
#include <QTableWidget>

#include "DashboardService.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class DashboardWindow;
}
QT_END_NAMESPACE

class DashboardWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit DashboardWindow(DashboardService *service, QWidget *parent = nullptr);
    ~DashboardWindow() override;

private:
    Ui::DashboardWindow *ui;
    void applyShadow(QWidget *widget);
    void setupTable(QTableWidget *table);
    QWidget *createBadge(const QString &text, const QString &bgColor, const QString &textColor);
    void populateTables();

    DashboardService *service;
};