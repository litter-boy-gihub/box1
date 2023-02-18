#include "calc.h"

Calc::Calc()
{
    lineEdit = new QLineEdit( );
    lineEdit->setReadOnly( true );
    QString texts [16] = {"7" , "8" , "9" , "/" ,\
        "4" , "5" , "6" , "*" ,\
        "1" , "2" , "3" , "-" ,\
        "0" , "." , "=" , "+",\
    };
    QGridLayout *gridLayout = new QGridLayout( );
    for (int i = 0; i < 16 ; ++i)
    {
        QPushButton *button = new QPushButton(texts[i]);
        connect( button , SIGNAL( clicked( ) ) , this , SLOT( onClicked( ) ) );
        gridLayout->addWidget( button , i / 4 , i % 4 );   
    }
    QVBoxLayout * vBoxLayout = new QVBoxLayout(this);//这里的this什么意思？
    vBoxLayout->addWidget( lineEdit , 0 , 0 );
    vBoxLayout->addLayout( gridLayout );
}

void Calc::onButtonClicked( ) {
    QPushButton *pushbutton = qobject_cast<QPushButton*>( sender( ) );
    lineEdit->setText(lineEdit->text()+pushbutton->text() );
}

Calc::~Calc()
{

}
