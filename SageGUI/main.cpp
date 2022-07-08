#include <iostream>

#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow _mainWin;
    _mainWin.show();
    return a.exec();
}