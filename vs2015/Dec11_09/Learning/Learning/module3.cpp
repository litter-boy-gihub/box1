#include<iostream>
#include<memory>
using namespace std;

static void fun1(shared_ptr<int> ptr) {
	cout << "fun1" << ptr.use_count() << endl;
}

static void fun2(shared_ptr<int> &ptr) {
	cout << "fun2" << ptr.use_count()<<endl;
}

class Base {
public:
	Base() { 
		cout << "constructer" << endl; }
	~Base() { 
		cout << "destructer" << endl; }
};

class CModule3 {
public:
	shared_ptr<Base> ptr;
};


int module3() {
	shared_ptr<int> p =  make_shared<int>();
	cout << p.use_count() << endl;
	fun1(p);
	cout << p.use_count() << endl;
	fun2(p);
	cout << p.use_count() << endl;
	{
		CModule3 m;
		m.ptr = make_shared<Base>();
		CModule3 m2;
		m2.ptr = m.ptr;
	}
	return 0;
}