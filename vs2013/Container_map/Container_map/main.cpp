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
    //���캯�� 1Ĭ�� 2 ��Χ 3 ���� 4 �ƶ� 5 initializer_list
    map<int, string> m0;
    map<int, string> m1(m0.begin(), m0.end());
    map<int, string> m2(m1);
    map<int, string> m3(std::move(m2));
    map<int, string> m4{ { 1, string("tom") }, { 2, string("jack") }, { 3, string("cheng") }, { 4, string("mark") } };
    cout << "m4:";
    print(m4);
    
    //��ֵ��ʽֻ��operator= 1��ֵ 2 ��ֵ 3 initializer_list
    m0 = m4;
    cout << "m0 = m4 m0:";
    print(m0);
    m1 = map<int, string>{{5, "sam"}, { 6, "timi" }};
    m2 = map<int,string>{ make_pair<int, string>(7, string("dolan")), make_pair<int, string>(8, string("jimi")), make_pair<int, string>(9, string("corn")) };
    //TODO���޷�ʹ�ó�ʼ���б�ֵ

    //insert 1 ��[iterator hint],&/&&����ֵ��ֵ 2 ��Χ[first,last] 3 initializer_list 
    m2.insert({ 10, "hout" });//��ֵ
    cout << "m2 inserted:";
    print(m2);
    cout << "insert m4 to m2 m2:";
    m2.insert(m4.begin(), m4.end());//��Χ
    print(m2);
    //TODO:why can't use this style to insert?
    //m2.insert(make_pair<int,string>(11,string("jim")));//initializer_list �д�

    //emplace��&&�� emplace_hint(iterator,&&args);ԭλ���죬����Ϊ��ֵ
    m2.emplace(make_pair<int,string>(12,string("joker"))); //Ϊʲôm2.emplace({12,""})����д����������ǲ���ӦΪ2013��vc2013�﷨��֧�֣�
    print(m2);
    //erase 1 pos 2 ��Χ[first,last) 3 (cosnt &key)
    m2.erase(m2.find(1));
    m2.erase(2);
    m2.erase(m2.find(7), m2.end());
    cout << "atfer erased : ";
    print(m2);

    cout << "m4:";
    print(m4);
    //���� count find equal_range lower_bound upper_bound �۲��� key_comp value_comp
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