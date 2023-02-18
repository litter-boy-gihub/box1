#include "q8.h"
#include <QtWidgets/QApplication>
#include "thread.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
    Q8 w;
    w.show();
    return a.exec();
}
