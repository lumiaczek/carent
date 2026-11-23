#pragma once

#include <QMainWindow>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class DashboardWindow;
}
QT_END_NAMESPACE

class DashboardWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit DashboardWindow(QWidget *parent = nullptr);
    ~DashboardWindow() override;

private:
    Ui::DashboardWindow *ui;
    void applyShadow(QWidget *widget);
    void setupTable(QTableWidget *table);
    QWidget *createBadge(const QString &text, const QString &bgColor, const QString &textColor);
    void populateTables();
};