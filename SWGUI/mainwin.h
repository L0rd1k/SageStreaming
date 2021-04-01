#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>

namespace Ui {
class mainWin;
}

class mainWin : public QWidget
{
    Q_OBJECT

public:
    explicit mainWin(QWidget *parent = 0);
    ~mainWin();

private:
    Ui::mainWin *ui;
};

#endif // MAINWIN_H
