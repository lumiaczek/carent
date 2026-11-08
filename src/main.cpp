#include "dashboard.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        app.setStyleSheet(stream.readAll());
        styleFile.close();
    } else {
        qWarning() << "Nie udalo sie wczytac pliku style.qss!";
    }

    DashboardWindow window;
    window.show();

    return app.exec();
}