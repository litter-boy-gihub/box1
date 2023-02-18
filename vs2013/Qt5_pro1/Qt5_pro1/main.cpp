#include "qt5_pro1.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include<QDebug>
#include<QtCore>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qt5_pro1 w;
    w.show();
    return a.exec();
}
