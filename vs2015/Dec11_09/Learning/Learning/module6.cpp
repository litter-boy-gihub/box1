#include<iostream>
#include<cmath>
using namespace std;
//���ú������Ż�
//int �������ֵ
inline int abs(int x) {
	int y = x >> 31;//xΪ����yΪ ȫ0 ����Ϊ ȫ1
	return (x + y) ^ y;  //x+y yȫ1�൱�ڼ�1   //���� ȡ����һ ���� ��һȡ�� ������ò����Ӧ��ԭ��  ��ȫ1������ȡ�� ȫ0��򲻱�
}

//int �ϴ�ֵ 
inline int max(int x, int y) {
	int m = (x - y) >> 31;//ȫ1x<y  ȫ0x >= y
	return (y&m) | (x&~m);
}

//int ��Сֵ
inline int min(int x, int y) {
	int m = (x - y) >> 31;
	return (x&m) | (y&~m);
}

//int ����������
inline void swap(int x, int y) {
	x ^= y; //x = x^y
	y ^= x; //y = x
	x ^= y; //x = y
}

//int ����ƽ��
inline int average(int x, int y) {
	int temp = (x&y) + ((x^y) >> 1); //x&y��ֻ���ǽ�λ�� ((x^y) >> 1)�ǲ����ǽ�λ��
	return temp + ((temp >> 31) & 1 & (x^y));//����ͽ��Ϊ-1ʱ������Ϊ0��ż��Ϊ-1
}
//���û�б�Ҫ��ô����
//fabs ����ֵ
//�ϴ�ֵ  x+y +fabs(x-y)/ 2 //�����
//��Сֵ  x+y -fabs(x-y)/2  //�����
//ƽ��ֵ

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