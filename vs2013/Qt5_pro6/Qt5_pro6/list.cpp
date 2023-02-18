#include "list.h"

List::List(QWidget *parent)
    : QMainWindow( parent ) , cur(nullptr )
{
    ui.setupUi(this);
    connect( ui.pushButton , &QPushButton::clicked , this , &List::add);
    connect( ui.pushButton_2 , &QPushButton::clicked , this , &List::del );
    connect( ui.pushButton_3 , &QPushButton::clicked , this , &List::change );
    connect( ui.listWidget , &QListWidget::itemClicked , this , &List::update );

}
List::~List()
{
}
void List::add( ) {
    QListWidgetItem *item = new QListWidgetItem( ui.listWidget );
    QStringList list;
    list << QStringLiteral( "姓名：" ) + ui.lineEdit->text( ) \
        << QString::fromLocal8Bit( "年龄：" ) + ui.lineEdit_2->text( ) \
        << QString::fromLocal8Bit( "班级：" ) + ui.lineEdit_3->text( );
    
    item->setText(list.join(" "));
    ui.listWidget->addItem( item );
}
void List::del( ) {
    if( cur != nullptr ) {
        int row = ui.listWidget->row( cur );
        ui.listWidget->takeItem( row );
    }
}
void List::change( ) {
    QStringList list;
    list << QString::fromLocal8Bit( "姓名：" ) + ui.lineEdit->text( ) \
        << QString::fromLocal8Bit( "性别：" ) + ui.lineEdit_2->text( ) \
        << QString::fromLocal8Bit( "班级：" ) + ui.lineEdit_3->text( );

    cur->setText( list.join( " " ) );
}
void List::update( ) {
    cur = ui.listWidget->currentItem( );
}
