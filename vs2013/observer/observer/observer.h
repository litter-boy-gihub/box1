#pragma  once
class IObserver {
public:
    virtual void update( ) = 0;
    virtual ~IObserver( ) { };
};

class Observer : public IObserver{
public:
    Observer( );
    ~Observer( );
    virtual void update( );
};