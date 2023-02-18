#include "nodepad.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Nodepad w;
    w.show();
    return a.exec();
}
