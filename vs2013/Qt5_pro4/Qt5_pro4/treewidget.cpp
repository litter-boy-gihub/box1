#include "treewidget.h"
#include <QTreeWidget>
#include <QMessageBox>
treeWidget::treeWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.tree->headerItem( )->setText( 0 ,QString::fromLocal8Bit("����"));
    ui.tree->headerItem( )->setText( 1 , QString::fromLocal8Bit( "��Ϣ" ) );

    connect( ui.tree , &QTreeWidget::itemClicked , this ,&treeWidget::set_item);//���õ����item��
    connect( ui.pushButton_2 , &QPushButton::clicked , this , &treeWidget::add );
    connect( ui.pushButton_3 , &QPushButton::clicked , this , &treeWidget::del );
    connect( ui.pushButton_4 , &QPushButton::clicked , this , &treeWidget::change );
}

treeWidget::~treeWidget()
{}


//���һ��ѧ������Ϣʱ����rootitem���ڵ�����ݣ���������text�ı�Ϊ�Ѵ���ѧ����Ϣ��
void treeWidget::set_item( QTreeWidgetItem *item , int col ) {
    if( item->parent( ) == nullptr ) {
        QMessageBox::warning( this , "warning" , "item ->parent is nullptr" );
    }
    if( item->parent( ) == nullptr ) {  //����items��parentΪ��ֵnullptr
        this->rootItem = item; 
        QMessageBox::warning( this , "set roomitem:" , rootItem->text( 0 ) );
        ui.lineEdit_1->setText( rootItem->text( 0 ) );
        ui.lineEdit_2->setText( rootItem->child( 0 )->text( 1 ) );
        ui.lineEdit_3->setText( rootItem->child( 1 )->text( 1 ) );
        return;
    }
    else rootItem = item->parent( );
    QMessageBox::warning( this , "set roomitem:" , rootItem->text( 0 ) );
    ui.lineEdit_1->setText( rootItem->text( 0 ) );
    ui.lineEdit_2->setText( rootItem->child(0)->text(1) );
    ui.lineEdit_3->setText( rootItem->child(1 )->text(1));
}

bool treeWidget::is_imput_empty( ) {
    if( !ui.lineEdit_1->text( ).isEmpty( ) && \
        !ui.lineEdit_2->text( ).isEmpty( ) && \
        !ui.lineEdit_3->text( ).isEmpty( ) )
        return false;
    return true;
}

void treeWidget::add( ) {
    if( is_imput_empty( ) )  QMessageBox::warning( this , "warning" , "more one input is empty" );
    
    QTreeWidgetItem *name = new QTreeWidgetItem( ui.tree);//
    name->setText(0,ui.lineEdit_1->text( ) );//��������
    QTreeWidgetItem *gender = new QTreeWidgetItem( name );
    gender->setText( 0 , QString::fromLocal8Bit( "�Ա�" ) );
    gender->setText(1,ui.lineEdit_2->text( ) );
    QTreeWidgetItem  *class_id = new QTreeWidgetItem( name );
    class_id->setText( 0 , QString::fromLocal8Bit( "�༶" ) );
    class_id->setText( 1 , ui.lineEdit_3->text( ) );
}

void treeWidget::del( ) {
    if( is_imput_empty( ) )  QMessageBox::warning( this , "warning" , "more one input is empty" );
    int index = ui.tree->indexOfTopLevelItem( rootItem );
    if( index == -1 ) {
        QMessageBox::warning( this , "warning" , "can not find top item" );
        return;
    }
    ui.tree->takeTopLevelItem( index );
}

void treeWidget::change( ) {
    if( is_imput_empty( ) )  QMessageBox::warning( this , "warning" , "more one input is empty" );
    rootItem->setText( 0 , ui.lineEdit_1->text( ) );
    rootItem->child( 0 )->setText( 0 , ui.lineEdit_2->text( ) );
    rootItem->child( 1 )->setText( 0 , ui.lineEdit_3->text( ) );
}

