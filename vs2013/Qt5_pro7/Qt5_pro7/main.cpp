#include "painter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    painter w;
    w.show();
    return a.exec();
}
