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
    Calc(); //�������ui�ļ��Ĳ�ͬ��
    ~Calc();
public slots:
    void onButtonClicked( );
private:
    QLineEdit *lineEdit;
};

#endif // CALC_H
