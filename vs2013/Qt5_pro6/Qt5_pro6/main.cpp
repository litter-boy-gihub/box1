#include "list.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    List w;
    w.show();
    return a.exec();
}
