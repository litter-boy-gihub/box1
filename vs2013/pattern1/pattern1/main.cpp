#include<cstdio>

#include<cstdlib>
#include<string>
#include <iostream>
#include<cstddef>
//策略模式和工厂模式 税率各国
class  tax_french{
public:
    int get_tax(int i ) {
        return i+1;
    }
};

class Itax {
public:
    virtual int get_tax( int i ) = 0;
    virtual ~Itax( ) { };
};

class Tax_frence : public Itax {
    virtual int get_tax( int i ) {
        return i + 1;
    }
};

class Tax_CN :public Itax {
    virtual int get_tax( int i ) {
        return i + 5;
    }
};

class ITaxFactory {
public:
    virtual Itax* createTaxStrategy( ) = 0;
    ~ITaxFactory( ) { };
};

class Tax_frence_factory :public ITaxFactory {
public:
    virtual Itax* createTaxStrategy( ) {
        return new Tax_frence( );
    }
};

class Tax_CN_factory :public ITaxFactory {
public:
    virtual Itax* createTaxStrategy( ) {
        return new Tax_CN( );
    }
};


int main(){
    char str [32]{0};
    sscanf( "cn" , "%s" , str );
    ITaxFactory* tax_factory = nullptr;
    if( strlen( str ) > 0 )  tax_factory = new Tax_CN_factory( );
    Itax* tax1 = tax_factory->createTaxStrategy( );
    int ret = tax1->get_tax( 3 );
    printf( "%d\n" , ret );
    return 0;
}