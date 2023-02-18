#include "qt5_pro1.h"
#include<QList>
#include <QPushButton>

Qt5_pro1::Qt5_pro1(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    QList<QPushButton*> buttonlist = findChildren<QPushButton*>( );
    for (int i = 0; i < buttonlist.size() ; i++)
    {
        connect( buttonlist.at( i ) , SIGNAL( clicked( ) ) , this ,
            SLOT( onButtonClicked( ) ) );
    }
}

void Qt5_pro1::onButtonClicked( ) {
    QPushButton* clickedButton = qobject_cast<QPushButton*>( sender( ) );
    
    ui.lineEdit->setText( ui.lineEdit->text( ) + clickedButton->text( ) );
    
}

Qt5_pro1::~Qt5_pro1()
{

}
