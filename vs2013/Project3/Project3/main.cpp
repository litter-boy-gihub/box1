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
    //ָ��
    //ָ�����������ָ�� ��άָ��
    char* p[] = { "this ", "is ", "a ", "problem!" };//ָ������
    char** pa = p;//��άָ��
    int arr[2][2] = { { 1,2 }, { 3,4 } };
    int(*pc)[2] = &arr[0];//����ָ��
    //����ָ��
    typedef void(*FUN)();
    FUN f;
    f = fun;
    f();
    //ָ������
    int ar[]{1, 2, 3, 4, 5, 6};
    int * ptr_ = ar;//��д
    int * const ptr = ar;//��ָ��,ָ�벻���ƶ�
    const int *ptr_c = static_cast<const int*>(ar);//ֻ������ָ��
    ptr_c++;
    ptr_[0] = 3;
    cout << "ָ�����һ��Ԫ�� �Ƚϣ�" << ptr_c - ptr <<"  "<<(ptr<ptr_c) << endl;
    //����
    int time = 0;
    int &u = time;
    int *p_t = &time;
    int* &ref_p = p_t;
    //��ά���鶯̬���� ����һά���ϵ�Ԫ�ض���ָ��,������Ҫ���ϵ���
    int ***i_arr = new int**[4];
    for (int i = 0; i < 4; i++)
    {
        i_arr[i] = new int*[3];
        for (int j = 0; j < 3; j++){
            i_arr[i][j] = new int[2]{1,2};
        }
    }
    //�����ռ䣺
    mynames::a;
    mynames::dosome();
    //�쳣
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
