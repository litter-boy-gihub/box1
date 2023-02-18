#include "table.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Table w;
    w.show();
    return a.exec();
}
