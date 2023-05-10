#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showList();
    w.mediaPlayerInit();
    w.show();
    return a.exec();
}
