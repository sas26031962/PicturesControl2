#include "mainwindow.h"
#include <QApplication>
#include <QString>

//-----------------------------------------------------------------------------
// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //20251123
    QString sName = w.windowTitle();
    sName += PROGRAMM_SUFFICS;
    w.setWindowTitle(sName);

    w.raise();
    w.activateWindow();;
    w.show();

    int x = a.exec();

    return x;
}
