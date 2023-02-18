#include<iostream>
#include<algorithm>

using namespace std;
class M5 {
public:
	M5(char* c):c(c) {
		cout << "const";
	}
private:
	M5(M5& m) :c(m.c) {
		cout << "& const";
	}
	char *c;
};

int module5() {
	M5 m = "log";
	M5 m2("log2");
	return 0;
}