#include<iostream>
#include<list>
#include<string>
#include<utility> //包含 pair

using namespace std;

ostream& operator<<(ostream& o,const pair<int,int> p){
    o << "(" << p.first << "," << p.second << ")" << " ";
    return o;
}

inline void print(list<pair<int, int> > & L){
    for (auto it = L.begin(); it != L.end(); it++){
        cout << *it;
    }
    cout << endl;
}

bool comp(pair<int, int> p1, pair<int, int> p2){
    if ((p1.first + p1.second) < (p2.first+p2.second))return true;
    return false;
}

bool comp_eq(pair<int, int> p1, pair<int, int> p2){
    if ((p1.first + p1.second) == (p2.first + p2.second))return true;
    return false;
}


int main(){
    make_pair(1,2);//构造pair
    //构造函数
    list<pair<int, int>> L0{};
    list<pair<int,int>> L1(4, make_pair(1, 1));
    list<pair<int,int>> L2(4);
    list<pair<int,int>> L3{make_pair(1,1),make_pair(2,2),make_pair(3,3),make_pair(4,4)};
    list<pair<int,int>> L4(++L3.begin(), --L3.end());
    list<pair<int,int>> L5(L3);
    list<pair<int,int>> L6(list<pair<int, int> >{make_pair(1, 1), make_pair(2, 2)});
    list<pair<int,int>> L7{ make_pair(3, 3), make_pair(4, 4) };

    //insert(pos,[count],[...]) 1 插入左值/右值 2 插入count*T&  3 （pos,fist,last）插入[first,last) 4 插入initializer_list
    pair<int, int> p(1, 1);
    cout << "insert 一个左值，右值";
    L0.insert(L0.end(),p);
    L0.insert(L0.end(), make_pair(2, 2));
    print(L0);
    L0.insert(L0.end(), L7.begin(), L7.end());
    cout << "insert [first,last):";
    print(L0);
    L0.insert(L0.end(), { make_pair(5, 5), make_pair(6, 6) });
    cout << "insert initializer_list";
    print(L0);

    //erase  1 pos 2 [fist,last)
    cout << "erase end:";
    L0.erase(--L0.end());
    print(L0);
    L0.erase(std::find(L0.begin(), L0.end(), make_pair(3, 3)), L0.end());
    cout << "erase find(3,3) to end():";
    print(L0);

    //push_front pop_front emplace_front 较为特有的从前面删除插入
    L0.push_front(make_pair(0, 0));
    cout << "push_front(0,0):";
    print(L0);
    L0.pop_front();
    cout << "pop_front:";
    print(L0);
    L0.emplace_front(make_pair(3, 3));
    L0.emplace_front(make_pair(1, 1));
    cout << "emplace_front:";
    print(L0);

    //其他操作merge(合并已排序)不复制元素，归并后other为空，序列升序排列，第二参数可以有比较函数<返回true;
    //sort unique splice remove/remove_if（这个函数相比erase比较奇怪以后看） 非成员函数erase 和 erase_if
    cout << "sorted:";
    L0.sort(comp); //默认没有参数sort()用 < 运算符进行比较
    print(L0);
    cout << "uniqued L0:";
    L0.unique(comp_eq);//默认unique()用 == 运算符比较
    print(L0);
    cout << "L3:";
    print(L3);
    cout << "L3 merged to L0:";
    L0.merge(L3, comp);//默认merge用 < 运算符比较
    print(L0);
    cout << "if L3 is NUll: " << L3.size()<< endl;
    cout << "splice L0 to L3: ";
    //splice 1 (pos,&/&&) 转移所有 2 （pos,&/&&,iterator）移动一个元素，由iterator指向的 3 （pos,&/&&,first,last）转移[first,last)
    L3.splice(L3.end(),L0);
    print(L3);
    cout << "if L0 is NULL:" << L0.size() << endl;
    //访问只有front 和 back 两个元素
    
    system("pause");
    return 0;
}