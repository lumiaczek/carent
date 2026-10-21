#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    ui->labelPowitanie->setText("Hello World!");
}

MainWindow::~MainWindow() {
    delete ui;
}