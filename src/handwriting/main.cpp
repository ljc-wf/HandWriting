#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QObject::tr("Handwriting"));
    w.setWindowIcon(QIcon(":/resources/icons/pen.svg"));
    w.resize(320,320);
    w.show();

    return a.exec();
}
