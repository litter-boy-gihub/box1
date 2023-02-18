
#include <QtCore/QCoreApplication>
#include<QTextStream>
#include<QFile>
#include <QDebug>
#include<QString>
#include<QMap>
#include<QList>
#include<QVector>
#include<QSet>

class Data {
public:
    Data( int x , int y ) :x( x ) , y( y ) { };
    friend QDebug operator<<( QDebug debug , const Data& d );
private:
    int x;
    int y;
};

QDebug operator<<( QDebug debug , const Data& d ) {
    QDebugStateSaver saver( debug );
    debug.nospace( ) << d.x << ',' << d.y;
    return debug;
}

int main(int argc, char *argv[])
{
    QCoreApplication a( argc , argv );
 
    qDebug( ) << "let't go";
    qDebug( ) << "output types:" << QString( "String" ) << QChar( 'C' ) << 1;
    qDebug( ) << "user define type:" << Data( 3 , 4 );
    QVector<QString> st_info( 10 );
    QFile file;
    file.setFileName( "a.txt" );
    file.open( QIODevice::Text|QIODevice::WriteOnly|QIODevice::Truncate );
    //写入文件
    file.write( "liu man 1.23\n" );
    file.write( "li woman 4.5\n" );
    file.write( "san man 3.15\n" );
    file.write( "hack woman 4.23\n" );
    file.write( "jumen man 7.28\n" );
    file.write( "hea woman 1.12\n" );
    file.write( "whilt man 2.4\n" );
    file.write( "cheng woman 7.1\n" );
    file.write( "jim woman 12.16\n" );
    file.write( "tom woman 9.18\n" );
    file.close( );

    file.setFileName( "a.txt" );
    file.open( QIODevice::ReadOnly );
    QTextStream in( &file );
    //从文件读入
    for (int i = 0; i < 10 ; i++)
    {
        in.readLineInto( &st_info [i] );
    }
    
    //存储 map list vector set
    QMap<int , QString> map;
    for (int i = 0; i < 10 ; i++)
    {
        map.insert( i , st_info [i] );
    }
    qDebug( ) << "map:";
    for (int i = 0; i < 10 ; i++)
    {
        qDebug()<<*map.find( i );
    }
    

    QList<QString> list;
    for (int i = 0; i < 10 ; i++)
    {
        list.push_back( st_info [i] );
    }
    qDebug( ) << "list:";
    for (auto it = list.begin(); it != list.end() ; it++)
    {
        qDebug( ) << *it;
    }

    QVector<QString> vector;
    for (int i = 0; i < 10 ; i++)
    {
        vector.push_back( st_info [i] );
    }
    qDebug( ) << "vector:";
    for (auto it = vector.begin(); it != vector.end() ; it++)
    {
        qDebug( ) << *it;
    }
    
    QSet<QString> set;
    for (int i = 0; i < 10 ; i++)
    {
        set.insert( st_info [i] );
    }
    qDebug( ) << "set:";
    for (int i = 0; i < 10 ; i++)
    {
        qDebug( ) << *set.find( st_info [i] );
    }
    
    
    
    return a.exec();
}
