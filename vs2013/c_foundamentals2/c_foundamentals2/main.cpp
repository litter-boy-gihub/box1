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
    //重载1
    virtual void fun(int){
        cout << "Base: virtual void fun(int)" << endl;
    }
    //重载2
    void fun(int a,int b){
        cout << "Base: void fun(int,int) " << endl;
    }
    void fun(char){
        cout << "Base: fun(char)" << endl;
    }
};

class Derived:public Base{
public:
    //多态覆盖
    virtual void fun(int){
        cout << "Derived: virtual void fun(int)" << endl;
    }
    ////////////隐藏父类中的 重载2（不支持多态
    void fun(int a, int b){
        cout << "Derived: void fun(int,int)" << endl;
    }
   
    void test(){
        Base::fun(1, 2);//调用父类方法
        this->Base::fun(1, 2);//使用对象调用父类方法
    }
    //using Base::fun; 使得父类函数被隐藏的可见。怎么指定哪一个重载版本？？？Base::fun有多个版本。

    ///////////隐藏（不支持多态
    //如果基类声明被重载了，则应该在子类中重新定义子类的所有的基类版本，
    //否则派生类无法使用它们。如下void fun（char）函数应该重新定义Base::fun(int,int)的重载
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

    cout << "通过宏定义交换值" << endl;
    cout << "a: " << a << "  b: " << b << endl;
    i_swap(a, b);
    cout << "a: " << a << "  b: " << b << endl;

    cout << "通过引用函数交换值" << endl;
    f_iswap(a, b);
    cout << "a: " << a << "  b: " << b << endl;

    cout << "通过指针函数交换值" << endl;
    f_iswap(&a, &b);
    cout << "a: " << a << "  b: " << b << endl;


    getchar();
    return 0;
}
