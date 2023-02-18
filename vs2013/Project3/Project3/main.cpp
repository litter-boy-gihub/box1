#include<iostream>

using std::cout;
using std::endl;
void fun(){ cout << "func pointer!" << endl; }
namespace mynames{
    int a;
    void dosome(){
        cout << "mynamespace dosome" << endl;
    }
}
template<typename T>
void func(T t){
    throw t;
}
int main(){
    //指针
    //指针数组和数组指针 多维指针
    char* p[] = { "this ", "is ", "a ", "problem!" };//指针数组
    char** pa = p;//多维指针
    int arr[2][2] = { { 1,2 }, { 3,4 } };
    int(*pc)[2] = &arr[0];//数组指针
    //函数指针
    typedef void(*FUN)();
    FUN f;
    f = fun;
    f();
    //指针运算
    int ar[]{1, 2, 3, 4, 5, 6};
    int * ptr_ = ar;//读写
    int * const ptr = ar;//常指针,指针不可移动
    const int *ptr_c = static_cast<const int*>(ar);//只读常量指针
    ptr_c++;
    ptr_[0] = 3;
    cout << "指针减差一个元素 比较：" << ptr_c - ptr <<"  "<<(ptr<ptr_c) << endl;
    //引用
    int time = 0;
    int &u = time;
    int *p_t = &time;
    int* &ref_p = p_t;
    //多维数组动态分配 除了一维以上的元素都是指针,分配需要从上到下
    int ***i_arr = new int**[4];
    for (int i = 0; i < 4; i++)
    {
        i_arr[i] = new int*[3];
        for (int j = 0; j < 3; j++){
            i_arr[i][j] = new int[2]{1,2};
        }
    }
    //命名空间：
    mynames::a;
    mynames::dosome();
    //异常
    try{
        char ch{};
        func(ch);
    }
    catch (int){
        cout << "excp is int" << endl;
    }
    catch (char){
        cout << "excp is char" << endl;
    }
    catch (...) {
        cout << "excp is happened" << endl;
    }

    system("pause");
    return 0;
}
