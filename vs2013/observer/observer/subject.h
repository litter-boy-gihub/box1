#pragma once
#include <vector>
#include "observer.h"
#include <algorithm>
class ISubject {
protected:
    std::vector<IObserver*> observers_;
public:
    virtual void attach( IObserver* ob ) = 0;
    virtual void detach( IObserver* ob ) = 0;
    virtual void notify( ) = 0;
    virtual ~ISubject( ) { };
};

class Subject : public ISubject {
public:
    Subject( int n );
    ~Subject( );
    virtual void attach( IObserver* ob );
    virtual void detach( IObserver* ob );
    virtual void notify( );
    void doSome( ) {
        notify( );
    }
private:
    int m_n;
};