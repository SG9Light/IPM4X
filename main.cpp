#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //    a.connect(&a, &QApplication::lastWindowClosed, &a, &QApplication::quit);
    //    w.setAttribute(Qt::WA_QuitOnClose,true);
    return a.exec();
}
