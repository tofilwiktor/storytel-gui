#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFile file(":qdarkstyle/dark/darkstyle.qss");
    file.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(file.readAll());
    a.setStyleSheet(stylesheet);
    w.showList();
    w.mediaPlayerInit();
    w.initKeyboardShortcuts();
    w.show();
    return a.exec();
}
