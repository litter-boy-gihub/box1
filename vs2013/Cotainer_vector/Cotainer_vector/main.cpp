#include<iostream>
#include<vector>

using namespace std;

inline void print(vector<int> & v){
    for (auto i : v) cout << i << " ";
    cout << endl;
}


int main(){
    //构造函数（不包含分配器设置）
    vector<int> v0;
    vector<int> v1(4);//4个默认，不会进行复制
    vector<int> v2(4, 2);//4个2
    vector<int> v3{ 7, 5, 16, 8,6 };
    vector<int> v4(v3.begin() + 1, v3.begin() + 4);
    vector<int> v5(v4);//复制构造
    vector<int> v6(vector<int>{1, 2, 3, 4});//移动构造

    //operator = 1 复制赋值 2 移动赋值 3 初始化列表
    v0 = v3;
    cout << "v3 v0 左值赋值: ";
    print(v3);
    print(v0);
    cout<<"移动赋值:";
    v0 = vector<int>{1, 2, 3};
    print(v0);
    cout << "after initializer_list:";
    v0 = { 4, 5, 6 };
    print(v0);
    //assign() 1count*T 2 [first,last) 3初始化列表
    v0.assign(2, 1);
    cout << "assign n*T:";
    print(v0);
    cout <<"assign itetator:";
    v0.assign(v6.begin(), v6.end());
    print(v0);
    cout <<"assign initializer_list:";
    v0.assign({ 8, 8, 8, 8 });
    print(v0);

    //insert 只能使用迭代器指定插入pos，1 插入左值 2插入右值 3pos 插入count个左值 4插入pos [first,last) 5插入初始化列表
    int i = 2;
    cout << "insert: " << endl;
    v0.insert(v0.begin(),i );//左值
    v0.insert(v0.begin(), 1);//右值
    v0.insert(v0.begin(), 3, i);
    cout << "after 1 2 3 operation v0:";
    print(v0);
    v0.insert(v0.begin(), v3.begin() + 1, v3.end() - 1);
    cout << "[f,l):";
    print(v0);
    v0.insert(v0.begin(), { 4, 4, 4, 4 });
    cout << "init_list:";
    print(v0);
    //erase 1 pos 2 [first,last)
    cout << "erase 4*begin:";
    for (int i = 0; i < 4;i++)
    {
        v0.erase(v0.begin());
    }
    print(v0);
    v0.erase(v0.begin(), v0.begin() + 3);
    cout << "erase [begin,begin+3):";
    print(v0);
    //emplace(pos,Args&&...args) 无重载 在pos 处插入元素,和insert的区别是会调用构造函数，forward转发构造参数args
    //emplace_back同理，在容器尾部原位构造 emplace相比insert的性能高，移动构造，减少一次拷贝
    v0.emplace(v0.begin(), 1);
    cout << "v0 emplace 1 to begin:";
    print(v0);
    v0.emplace_back(3);
    cout << "v0 emplace_back 3:";
    print(v0);
    system("pause");
    return 0;
}