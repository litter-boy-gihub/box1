#include <mutex>
#include <atomic>
//饿汉式
class Asingle {
private:
    Asingle( ) { };
    Asingle( const Asingle& other ) = delete;
    static Asingle* instance_;
public:
    static Asingle* getInstance( ) {
        return instance_;
    }
};

//在对类的静态私有数据成员初始化的时候，可以引用类的其他私有成员。
Asingle* Asingle::instance_ = new Asingle( );


//懒汉式  单线程
class Bsingle {
private:
    Bsingle( ) { };
    Bsingle( const Bsingle& other ) = delete;
    static Bsingle* instance_;
public:
    static Bsingle* getInstance( ) {
        if( instance_ == nullptr ) {
            instance_ = new Bsingle( );
        }
        return instance_;
    }
};

Bsingle* Bsingle::instance_ = nullptr;

//懒汉式  多线程mutex
class Csingle {
private:
    Csingle( ) { };
    Csingle( const Csingle& other ) = delete;
    static Csingle* instance_;
    static std::mutex mtx;
public:
    static Csingle* getInstance( ) {
        mtx.lock( );
        if( instance_ == nullptr ) {
            instance_ = new Csingle( );
        }
        mtx.unlock( );
        return instance_;
    }
};
std::mutex Csingle::mtx;
Csingle* Csingle::instance_ = nullptr;

//懒汉式 多线程双检查锁
 class Dsingle {
private:
    Dsingle( ) { };
    Dsingle( const Dsingle& other ) = delete;
    static Dsingle* instance_;
    static std::mutex mtx;
public:
    static Dsingle* getInstance( ) {
        Dsingle* tmp = instance_;
        std::atomic_thread_fence( std::memory_order_acquire );     //六种内存序在qq的我的电脑备忘里有详细介绍
        if( instance_  != nullptr ) {
            mtx.lock( );
            tmp = instance_;
            if( tmp == nullptr ) {
                tmp = new Dsingle( );
                std::atomic_thread_fence( std::memory_order_release );
                instance_ = tmp;
            };
            mtx.unlock( );
        }
        return instance_;
    }
};
std::mutex Dsingle::mtx;
Dsingle* Dsingle::instance_ = nullptr;