#ifndef QT5_PRO1_H
#define QT5_PRO1_H

#include <QtWidgets/QWidget>
#include "ui_qt5_pro1.h"

class Qt5_pro1 : public QWidget
{
    Q_OBJECT

public:
    Qt5_pro1(QWidget *parent = 0);
    ~Qt5_pro1();
public slots:
void onButtonClicked( );

private:
    Ui::Qt5_pro1Class ui;
};

#endif // QT5_PRO1_H
