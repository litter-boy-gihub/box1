#include<iostream>
#include<set>
#include<vector>

using namespace std;

inline void print(set<int> &s){
    for (auto it = s.begin(); it != s.end(); it++)
        cout << *it << " ";
    cout << endl;
}

int main(){
    vector<int> v{ 6, 7, 8, 9 };
    //构造函数
    set<int> s0{};
    set<int> s1{ 1, 2, 3, 4, 5 };
    set<int> s2(v.begin() + 1, v.end() - 1);
    set<int> s3(s1);
    set<int> s4(set<int>{4, 6, 0});

    //insert 
    //1 pair<iterator,bool> insert(const&/&&);返回插入迭代器，和是否成功的bool
    //2 (const iterator hint,const&/&&) 3 [first,last) 4 initializer_list
    s0.insert(0);
    s0.insert(s0.begin(), 1);
    s0.insert(++s1.begin(), s1.end());
    s0.insert({ 6, 7, 8 });
    print(s0);
    //前提不存在元素 emplace emplace_hint(it hint,args)hint尽可能在hint前面
    s0.emplace(9); //写了iterator后编译错误？
    print(s0);
    //erase 1 pos (配合find)  2 [first,last)要求范围合法 3 （const key_type&) 移除键值对应元素
    s0.erase(s0.begin());
    s0.erase(s0.find(4), s0.find(6));
    s0.erase(9);
    print(s0);

    //观察器 key_comp value_comp 返回用于比较键和值的函数bool (*fun)(T&,T&);
    //查找 count find equal_range lower_range upper_range  
    cout << "count 1:" << s0.count(1) << endl;
    cout << "find failed is end():" << boolalpha << (s0.find(5) == s0.end()) << endl;
    //equal_range lower_range upper_range 
    //返回值pair<itrator,itrator> equal 返回匹配键范围 lower返回首个不小于给定键it >= upper返回首个大于给定键it
    pair<set<int>::iterator,set<int>::iterator> p1 = s0.equal_range(1);
    cout<<"equal_range 1:" << *p1.first <<"  "<< *p1.second << endl;
    auto p2 = s0.lower_bound(5);
    cout << "lower_range 5:" << *p2 << endl;
    auto p3 = s0.upper_bound(6);
    cout << "upper_range 6:" << *p3 << endl;

    system("pause");
    return 0;
}