#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow Menu;
    Menu.show();
    return a.exec();
}
