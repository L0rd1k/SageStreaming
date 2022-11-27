#include <iostream>

#include "Main.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    Main _mainWin;
    _mainWin.show();
    return a.exec();
}