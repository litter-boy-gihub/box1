#include<iostream>

using namespace std;

class A {
public:
	virtual void fun() = 0;
	//virtual void fun1() = 0;
	void fun2();
};

class B : public A{
public:
	virtual void fun();
};

class C :public B {
public:
	void fun2();
};

/*���麯����������Ҳû������ ������������ʵ�������оͲ����ǳ����࣬���麯��Ҳ���Ϊ�麯��*/
void A::fun() {
	cout << "A fun" << endl;
}

void A::fun2() {
	cout << "A fun2" << endl;
}

void B::fun() {
	cout << "B fun" << endl;
}

void C::fun2() {
	cout << "C fun2" << endl;
}
int module2() {
	B b;
	b.fun();
	b.fun2();
	C c;
	c.fun();
	c.fun2();
	c.A::fun2();

	return 0;
}