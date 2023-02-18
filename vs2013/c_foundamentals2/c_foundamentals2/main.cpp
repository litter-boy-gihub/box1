#include<iostream>
using std::cout;
using std::endl;

#ifdef MAIN 
#endif

#define PI 3.1415926

#define err(err_str) printf("err info:  %s\n",err_str);

#define i_swap(a,b) do{\
    int temp = a;\
    a = b;\
    b = temp;\
}while(0)

void f_iswap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}

void f_iswap(int *pa, int *pb){
    int temp = *pa;
    *pa = *pb;
    *pb = temp;
}

class Base{
public:
    //����1
    virtual void fun(int){
        cout << "Base: virtual void fun(int)" << endl;
    }
    //����2
    void fun(int a,int b){
        cout << "Base: void fun(int,int) " << endl;
    }
    void fun(char){
        cout << "Base: fun(char)" << endl;
    }
};

class Derived:public Base{
public:
    //��̬����
    virtual void fun(int){
        cout << "Derived: virtual void fun(int)" << endl;
    }
    ////////////���ظ����е� ����2����֧�ֶ�̬
    void fun(int a, int b){
        cout << "Derived: void fun(int,int)" << endl;
    }
   
    void test(){
        Base::fun(1, 2);//���ø��෽��
        this->Base::fun(1, 2);//ʹ�ö�����ø��෽��
    }
    //using Base::fun; ʹ�ø��ຯ�������صĿɼ�����ôָ����һ�����ذ汾������Base::fun�ж���汾��

    ///////////���أ���֧�ֶ�̬
    //������������������ˣ���Ӧ�������������¶�����������еĻ���汾��
    //�����������޷�ʹ�����ǡ�����void fun��char������Ӧ�����¶���Base::fun(int,int)������
    void fun(char){
        cout << "Derived: void fun(char)" << endl;
    }
};


int r_sum(int arr[], int n){
    if (0 == n) return arr[0];
    return r_sum(arr, n - 1)+arr[n-1];
}

int main(int argc, char *argv[]){

    int arr[]={1, 2, 3, 4, 5};

    cout << r_sum(arr,sizeof(arr)/4) << endl;




    for (int i = 0; i < argc; i++){
        cout << argv[i] << "  ";
    }
    getchar();
    
    

    Derived d;
    Base *base = &d;
    base->fun(1, 2);
    base->fun(1);
    d.fun(1, 2);
    d.fun('a');
    



    int a = 1, b = 2;
    cout << PI << endl;
    char str[] = "there is a segementfalt";
    err(str);

    cout << "ͨ���궨�彻��ֵ" << endl;
    cout << "a: " << a << "  b: " << b << endl;
    i_swap(a, b);
    cout << "a: " << a << "  b: " << b << endl;

    cout << "ͨ�����ú�������ֵ" << endl;
    f_iswap(a, b);
    cout << "a: " << a << "  b: " << b << endl;

    cout << "ͨ��ָ�뺯������ֵ" << endl;
    f_iswap(&a, &b);
    cout << "a: " << a << "  b: " << b << endl;


    getchar();
    return 0;
}
