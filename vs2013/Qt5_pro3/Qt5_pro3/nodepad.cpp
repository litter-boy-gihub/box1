#include "nodepad.h"
#include <QMessageBox>   
#include<QTextCursor>
#include<QFileDialog>
#include<QFile>
#include<QTextStream>
Nodepad::Nodepad(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect( ui.actionOpen , SIGNAL( triggered( bool )) , this , SLOT( open( bool ) ) );
    connect( ui.actionSave , SIGNAL( triggered( bool ) ) , this , SLOT( save( bool ) ) );
    connect( ui.actionEcit , SIGNAL( triggered( bool ) ) , this , SLOT( exit( bool ) ) );
    connect( ui.actionAll , SIGNAL( triggered( bool ) ) , this , SLOT( selectall( bool ) ) );
    connect( ui.actionDel , SIGNAL( triggered( bool ) ) , this , SLOT( del( bool ) ) );
    connect( ui.actionHelp , SIGNAL( triggered( bool ) ) , this , SLOT( help( bool ) ) );
    connect( ui.actionAbout , SIGNAL( triggered( bool ) ) , this , SLOT( about( bool ) ) );
    
    
}


Nodepad::~Nodepad()
{}

void Nodepad::about( bool ) {
    QMessageBox::information(this,"about","this is notepad application 1.0!" );
}

void Nodepad::help( bool ) {
    QMessageBox::information( this , "help" , "no help,it's easy to use" );
}

void Nodepad::del( bool ) {
    QTextCursor cursor = ui.textEdit->textCursor( );
    cursor.removeSelectedText( );
}

void Nodepad::selectall( bool ) {
    ui.textEdit->selectAll( );
}

void Nodepad::exit( bool ) {
    this->close( );
}

void Nodepad::save( bool ) {
    QString path = QFileDialog::getSaveFileName( this , tr( "save file" ) , "." , tr( "Text Files(*.txt)" ) );
    if( path.isEmpty( ) ) {
        QMessageBox::warning( this , "warning" , "save failed" );
        return;
    }
    QFile file;
    file.setFileName( path );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
        QMessageBox::warning( this , tr( "warning" ) , tr( "write file open failed" ) );
        return;
    }
    //路径存在，文件打开成功
    QTextStream out( &file );
    out << ui.textEdit->toPlainText( );
    file.close( );
}

void Nodepad::open( bool ) {
    QString path = QFileDialog::getOpenFileName( this , tr( "open file" ) , "." , tr( "Text Files(*.txt)" ) );
    if( path.isEmpty( ) ) {
        QMessageBox::warning( this , "warning" , "path not exsit or not select any" );
        return;
    }
    QFile file;
    file.setFileName( path );
    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        QMessageBox::warning( this , tr("warning") , tr("file open failed") );
        return;
    }
    //路径存在，文件打开成功
    QTextStream in( &file );
    ui.textEdit->setText( in.readAll( ) );
    file.close( );
    QStringList list = path.split('/' );
    fileName = list.at( list.size( )-1);
    this->setWindowTitle( fileName );
}
