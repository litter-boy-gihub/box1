#include<iostream>
#include<vector>

using namespace std;

inline void print(vector<int> & v){
    for (auto i : v) cout << i << " ";
    cout << endl;
}


int main(){
    //���캯�������������������ã�
    vector<int> v0;
    vector<int> v1(4);//4��Ĭ�ϣ�������и���
    vector<int> v2(4, 2);//4��2
    vector<int> v3{ 7, 5, 16, 8,6 };
    vector<int> v4(v3.begin() + 1, v3.begin() + 4);
    vector<int> v5(v4);//���ƹ���
    vector<int> v6(vector<int>{1, 2, 3, 4});//�ƶ�����

    //operator = 1 ���Ƹ�ֵ 2 �ƶ���ֵ 3 ��ʼ���б�
    v0 = v3;
    cout << "v3 v0 ��ֵ��ֵ: ";
    print(v3);
    print(v0);
    cout<<"�ƶ���ֵ:";
    v0 = vector<int>{1, 2, 3};
    print(v0);
    cout << "after initializer_list:";
    v0 = { 4, 5, 6 };
    print(v0);
    //assign() 1count*T 2 [first,last) 3��ʼ���б�
    v0.assign(2, 1);
    cout << "assign n*T:";
    print(v0);
    cout <<"assign itetator:";
    v0.assign(v6.begin(), v6.end());
    print(v0);
    cout <<"assign initializer_list:";
    v0.assign({ 8, 8, 8, 8 });
    print(v0);

    //insert ֻ��ʹ�õ�����ָ������pos��1 ������ֵ 2������ֵ 3pos ����count����ֵ 4����pos [first,last) 5�����ʼ���б�
    int i = 2;
    cout << "insert: " << endl;
    v0.insert(v0.begin(),i );//��ֵ
    v0.insert(v0.begin(), 1);//��ֵ
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
    //emplace(pos,Args&&...args) ������ ��pos ������Ԫ��,��insert�������ǻ���ù��캯����forwardת���������args
    //emplace_backͬ��������β��ԭλ���� emplace���insert�����ܸߣ��ƶ����죬����һ�ο���
    v0.emplace(v0.begin(), 1);
    cout << "v0 emplace 1 to begin:";
    print(v0);
    v0.emplace_back(3);
    cout << "v0 emplace_back 3:";
    print(v0);
    system("pause");
    return 0;
}