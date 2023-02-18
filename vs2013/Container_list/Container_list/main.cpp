#include<iostream>
#include<list>
#include<string>
#include<utility> //���� pair

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
    make_pair(1,2);//����pair
    //���캯��
    list<pair<int, int>> L0{};
    list<pair<int,int>> L1(4, make_pair(1, 1));
    list<pair<int,int>> L2(4);
    list<pair<int,int>> L3{make_pair(1,1),make_pair(2,2),make_pair(3,3),make_pair(4,4)};
    list<pair<int,int>> L4(++L3.begin(), --L3.end());
    list<pair<int,int>> L5(L3);
    list<pair<int,int>> L6(list<pair<int, int> >{make_pair(1, 1), make_pair(2, 2)});
    list<pair<int,int>> L7{ make_pair(3, 3), make_pair(4, 4) };

    //insert(pos,[count],[...]) 1 ������ֵ/��ֵ 2 ����count*T&  3 ��pos,fist,last������[first,last) 4 ����initializer_list
    pair<int, int> p(1, 1);
    cout << "insert һ����ֵ����ֵ";
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

    //push_front pop_front emplace_front ��Ϊ���еĴ�ǰ��ɾ������
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

    //��������merge(�ϲ�������)������Ԫ�أ��鲢��otherΪ�գ������������У��ڶ����������бȽϺ���<����true;
    //sort unique splice remove/remove_if������������erase�Ƚ�����Ժ󿴣� �ǳ�Ա����erase �� erase_if
    cout << "sorted:";
    L0.sort(comp); //Ĭ��û�в���sort()�� < ��������бȽ�
    print(L0);
    cout << "uniqued L0:";
    L0.unique(comp_eq);//Ĭ��unique()�� == ������Ƚ�
    print(L0);
    cout << "L3:";
    print(L3);
    cout << "L3 merged to L0:";
    L0.merge(L3, comp);//Ĭ��merge�� < ������Ƚ�
    print(L0);
    cout << "if L3 is NUll: " << L3.size()<< endl;
    cout << "splice L0 to L3: ";
    //splice 1 (pos,&/&&) ת������ 2 ��pos,&/&&,iterator���ƶ�һ��Ԫ�أ���iteratorָ��� 3 ��pos,&/&&,first,last��ת��[first,last)
    L3.splice(L3.end(),L0);
    print(L3);
    cout << "if L0 is NULL:" << L0.size() << endl;
    //����ֻ��front �� back ����Ԫ��
    
    system("pause");
    return 0;
}