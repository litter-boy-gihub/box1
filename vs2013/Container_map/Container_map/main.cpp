#include<iostream>
#include<string>
#include<map>
#include<initializer_list>
#include<windows.h>

using namespace std;

template<typename M>
void print(M &m){
    std::cout << "{ ";
    for (auto& i : m){
        std::cout << i.first << ":" << i.second << " ";
    }
    std::cout << "}\n";
}



int main(){
    //构造函数 1默认 2 范围 3 复制 4 移动 5 initializer_list
    map<int, string> m0;
    map<int, string> m1(m0.begin(), m0.end());
    map<int, string> m2(m1);
    map<int, string> m3(std::move(m2));
    map<int, string> m4{ { 1, string("tom") }, { 2, string("jack") }, { 3, string("cheng") }, { 4, string("mark") } };
    cout << "m4:";
    print(m4);
    
    //赋值方式只有operator= 1左值 2 右值 3 initializer_list
    m0 = m4;
    cout << "m0 = m4 m0:";
    print(m0);
    m1 = map<int, string>{{5, "sam"}, { 6, "timi" }};
    m2 = map<int,string>{ make_pair<int, string>(7, string("dolan")), make_pair<int, string>(8, string("jimi")), make_pair<int, string>(9, string("corn")) };
    //TODO：无法使用初始化列表赋值

    //insert 1 （[iterator hint],&/&&）左值右值 2 范围[first,last] 3 initializer_list 
    m2.insert({ 10, "hout" });//右值
    cout << "m2 inserted:";
    print(m2);
    cout << "insert m4 to m2 m2:";
    m2.insert(m4.begin(), m4.end());//范围
    print(m2);
    //TODO:why can't use this style to insert?
    //m2.insert(make_pair<int,string>(11,string("jim")));//initializer_list 有错

    //emplace（&&） emplace_hint(iterator,&&args);原位构造，参数为右值
    m2.emplace(make_pair<int,string>(12,string("joker"))); //为什么m2.emplace({12,""})这种写法在这出错，是不是应为2013，vc2013语法不支持？
    print(m2);
    //erase 1 pos 2 范围[first,last) 3 (cosnt &key)
    m2.erase(m2.find(1));
    m2.erase(2);
    m2.erase(m2.find(7), m2.end());
    cout << "atfer erased : ";
    print(m2);

    cout << "m4:";
    print(m4);
    //查找 count find equal_range lower_bound upper_bound 观察器 key_comp value_comp
    cout << "equal_range 3:";
    auto p1 = m4.equal_range(3);
    cout << (p1.first)->first << "  " << (p1.first)->second<<" ";
    cout << (p1.second)->first << "  " << (p1.second)->second << endl;
    cout << "lower_bound 2: ";
    auto p2 = m4.lower_bound(2);
    cout << p2->first << "  " << p2->second << endl;
    cout << "upper_bound 2: ";
    auto p3 = m4.upper_bound(2);
    cout << p3->first << "  " << p3->second << endl;

    system("pause");
    return 0;
}