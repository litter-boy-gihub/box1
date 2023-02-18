#include <mutex>
#include <atomic>
//����ʽ
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

//�ڶ���ľ�̬˽�����ݳ�Ա��ʼ����ʱ�򣬿��������������˽�г�Ա��
Asingle* Asingle::instance_ = new Asingle( );


//����ʽ  ���߳�
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

//����ʽ  ���߳�mutex
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

//����ʽ ���߳�˫�����
 class Dsingle {
private:
    Dsingle( ) { };
    Dsingle( const Dsingle& other ) = delete;
    static Dsingle* instance_;
    static std::mutex mtx;
public:
    static Dsingle* getInstance( ) {
        Dsingle* tmp = instance_;
        std::atomic_thread_fence( std::memory_order_acquire );     //�����ڴ�����qq���ҵĵ��Ա���������ϸ����
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