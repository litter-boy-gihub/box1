#include<iostream>
#include<fstream>
#include<chrono>

using namespace std;

int module4() {

	ostream *pre;
	ofstream ofs;
	ofs.open("test.txt");
	cout << "tie example: \n";
	*cin.tie() << "this is inserter into cout";
	pre = cin.tie(&ofs);
	*cin.tie() << "this is inserter into file";
	cin.tie(pre);
	ofs.close();
	return 0;
}