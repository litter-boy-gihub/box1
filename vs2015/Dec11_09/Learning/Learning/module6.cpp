#include<iostream>
#include<cmath>
using namespace std;
//常用函数的优化
//int 计算绝对值
inline int abs(int x) {
	int y = x >> 31;//x为正数y为 全0 负数为 全1
	return (x + y) ^ y;  //x+y y全1相当于减1   //补码 取反加一 等于 减一取反 可以求得补码对应的原码  和全1亦或就是取反 全0亦或不变
}

//int 较大值 
inline int max(int x, int y) {
	int m = (x - y) >> 31;//全1x<y  全0x >= y
	return (y&m) | (x&~m);
}

//int 较小值
inline int min(int x, int y) {
	int m = (x - y) >> 31;
	return (x&m) | (y&~m);
}

//int 交换两个数
inline void swap(int x, int y) {
	x ^= y; //x = x^y
	y ^= x; //y = x
	x ^= y; //x = y
}

//int 算术平均
inline int average(int x, int y) {
	int temp = (x&y) + ((x^y) >> 1); //x&y是只考虑进位和 ((x^y) >> 1)是不考虑进位和
	return temp + ((temp >> 31) & 1 & (x^y));//当求和结果为-1时，奇数为0，偶数为-1
}
//如果没有必要这么极致
//fabs 绝对值
//较大值  x+y +fabs(x-y)/ 2 //会溢出
//较小值  x+y -fabs(x-y)/2  //会溢出
//平均值

inline int average2(int x, int y) {
	return x + fabs(x - y)/ 2;
}

int module6() {
	 
	cout << average(-4,5) << endl;
	cout << average(5,-6) << endl;
	cout << average(5, -7) << endl;
	cout << average(5, -8) << endl;
	return 0;
}