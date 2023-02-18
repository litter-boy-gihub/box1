#include "table.h"
#include <QStringList>
#include <QTableWidgetItem> 
#include <QMessageBox>
Table::Table(QWidget *parent)
    : QWidget( parent ) , row( 0 )
{
    ui.setupUi(this);
    ui.tableWidget->setColumnCount( 3 );
    ui.tableWidget->setRowCount( 10 );
    ui.tableWidget->setHorizontalHeaderLabels( QStringList { \
        QString::fromLocal8Bit( "姓名" ) , \
        QString::fromLocal8Bit( "性别" ) , \
        QString::fromLocal8Bit( "班级" ) , } );
    
    connect( ui.pushButton , &QPushButton::clicked , this , &Table::add );
    connect( ui.pushButton_2 , &QPushButton::clicked , this , &Table::del );
    connect( ui.pushButton_3 , &QPushButton::clicked , this , &Table::change );
    connect( ui.tableWidget , &QTableWidget::itemSelectionChanged , this , &Table::update );
}

Table::~Table()
{
}

void Table::add( ) {
    QStringList data;
    data << ui.lineEdit->text( ) << ui.lineEdit_2->text( ) << ui.lineEdit_3->text( );
    for (int i = 0; i < 3 ; i++)
    {
        ui.tableWidget->setItem(  row, i ,new QTableWidgetItem( data [i] ) );
    }
    row++;
}

void Table::del( ) {
    if( set.size( ) != 1 ) {
        QMessageBox::warning( this , "" , QString::fromLocal8Bit( "选中多余一行或没有选中" ) );
        return;
    }
    ui.tableWidget->removeRow( *set.begin( ) );
    row--;
}

void Table::change( ) {
    int &&n = std::move(set.size( ));
    if( n != 1 ) {
        QMessageBox::warning( this , "" , QString::fromLocal8Bit( "选中多余一行或没有选中" ) );
        return;
    }
    ui.tableWidget->item( *set.begin() , 0 )->setText( ui.lineEdit->text( ) );
    ui.tableWidget->item( *set.begin( ) , 1 )->setText( ui.lineEdit_2->text( ) );
    ui.tableWidget->item( *set.begin( ) , 2 )->setText( ui.lineEdit_3->text( ) );
}

void Table::update( ) {
    ranges = ui.tableWidget->selectedRanges( );
    set.clear( );
    for (int i = 0; i < ranges.size() ; i++)
    {
        for (int j = ranges[i].topRow(); j <= ranges[i].bottomRow() ; j++)
        {
            set.insert( j );
        }
    }
    QStringList list;
    for( auto it = set.begin( ); it != set.end( );it++ )
    {
        list << QString::number( *it );
    }
    QMessageBox::warning( this , "" , QString("selected rows:")+list.join( '#' ));
    return;
}
