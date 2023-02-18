#ifndef CALC_H
#define CALC_H

#include<QWidget>
#include<QLineEdit>
#include <QPushButton>
#include<QVBoxLayout>
#include<QGridLayout>

class Calc : public QWidget
{
    Q_OBJECT

public:
    Calc(); //这里和用ui文件的不同？
    ~Calc();
public slots:
    void onButtonClicked( );
private:
    QLineEdit *lineEdit;
};

#endif // CALC_H
