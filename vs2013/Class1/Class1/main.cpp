#include<iostream>
using namespace std;

class Base{
public:
    Base(int a,int b,int c):pub(a),pro(b),pri(c){
        cout << "Base const" << endl;
    }
    virtual ~Base(){
        cout << "Base dest" << endl;
    }
    virtual void doSome(){
        cout << "Base dosome" << endl;
    }
public:
    int pub;
protected:
    int pro;
private:
    int pri;
};

class Derived:public Base{
public:
    Derived(int a,int b,int c):Base(a,b,c){
        cout << "Derived const" << endl;
        cout << "Derived can access Base pro and pub:" <<pro << "  " << pub << endl;
    }
    virtual ~Derived(){
        cout << "Derived dest" << endl;
    }
    virtual void doSome(){
        cout << "Derived dosome" << endl;
    }
public:
    Base::pub;
protected:
    Base::pro;
};

class Point2d{
public:
    Point2d(int x,int y){
        this->x = new int(x);
        this->y = new int(y);
    }
    Point2d(const Point2d& p){
        this->x = new int(*(p.x));
        this->y = new int(*(p.y));
        cout << "��������" << endl;
    }
    Point2d& operator=(const Point2d& p){
        *(this->x) = *(p.x);
        *(this->y) = *(p.y);
        cout << "������ֵ" << endl;
        return *this;
    }
    ~Point2d(){
        delete x;
        delete y;
    }
    explicit operator int(){
        return *x;
    }
private:
    int *x;
    int *y;
};


int main(){
    //reinterpret_cast
    int *pint = new int(48);
    int i4 = 48;
    //ָ��
    char* pc1 = reinterpret_cast<char*>(pint);
    cout << "int* 32 to char: *" << *pc1 << endl;
    //����
    char& ch = reinterpret_cast<char&>(i4);
    cout << "int 32 to char&: " << ch << endl;
    //ָ�������
    int temp = reinterpret_cast<int>(&i4);
    ios::fmtflags flag = cout.flags();
    cout << hex << showbase<<uppercase<< temp << endl;
    i4 = 32;
    int* ptemp = reinterpret_cast<int*>(temp);
    cout.flags(flag);
    cout << "int to int*: " << *ptemp << endl;

    int i1 = 4;
    float f = 3.0;
    char c = '0';
    void* pi = &i1;
    //static_cast 
    float f1 = static_cast<float>(i1);
    int i2 = static_cast<int>(f);
    int i3 = static_cast<int>(c);
    int *p = static_cast<int*>(pi);
    cout << "float int int ptr:" <<i1 <<" "<<f<<" "<<c<<" "<<*p<<" "<< endl;
    
    //��������ת����
    Point2d point2d(6, 7);
    int ptoi = static_cast<int>(point2d);
    cout << ptoi << endl;

    const int n = 5;
    //����int &n0 = n;
    int &n1 = const_cast<int&>(n);
    n1 = 1;
    cout << n1 << endl;
    
    int *pn = const_cast<int*>(&n); //����int *pn0 = &n;
    *pn = 2;
    cout << *pn << endl;


    //�麯��
    Base base(1, 2, 3);
    Derived *pderived =nullptr;
    Base* pb = new Derived(1, 2, 3);
    base.doSome();
    pb->doSome();
    //����ת���࣬����ȫ
    Derived* pd = static_cast<Derived*>(pb);
    //����ת���࣬��ȫ
    Base *pbase = static_cast<Base*>(pderived);
    //����ָ��ת��Ϊ������ָ�롣�ɹ������ǻ���ָ����������󣬷��򷵻�nullptr��
    if ((pderived = dynamic_cast<Derived*>(pb)) != nullptr)
        cout << "dynamci_cast succeed" << endl;
    

    Derived derived(1, 2, 3);
    
    Point2d point(3, 4);

    Point2d point1 = point;//��������

    point1 = Point2d(5, 6);//������ֵ


    system("pause");
    return 0;
}