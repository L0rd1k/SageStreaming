#include "mainwin.h"
#include "ui_mainwin.h"

mainWin::mainWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWin)
{
    ui->setupUi(this);
}

mainWin::~mainWin()
{
    delete ui;
}
