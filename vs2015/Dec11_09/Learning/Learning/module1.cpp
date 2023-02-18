#include<iostream>
//���ֲ���˼���������Ļ���
//1-5 6-10 11-15 16-20
//�����������ʼֵ 1,6,11,16,21

//��������������ֲ��ҳɹ���ֱ�ӷ���mid �� �� ���ֲ���ʧ�ܣ�������begin

using namespace std;
static int arr[] = { 1,6,11,16,21 };//��Ҫ�漰begin��Сֵ��end�����ֵ�Ƿ�Ϸ�

							 //������
int find(int n, int begin, int end) {
	if (n<0 || n>20 || begin > end || begin < 0 || end > 4) return -1;
	int mid;
	while (begin <= end) {
		mid = begin + (end - begin) / 2;//ƽ��ֵ�����д�� 
		if (n < arr[mid]) { end = mid - 1; continue; }
		if (n > arr[mid]) { begin = mid + 1; continue; }
		if (n == arr[mid]) return n; //���ҳɹ�
	}
	return arr[begin - 1];//����ʧ��
}

//�ݹ鷨
int find_recursive(int n, int begin, int end) {
	if (begin == end + 1) return arr[begin - 1];//����ʧ��
	int mid = begin + (end - begin) / 2;
	if (n < arr[mid]) return find_recursive(n, begin, mid - 1);
	if (n > arr[mid]) return find_recursive(n, mid + 1, end);
	return n; //���ҳɹ�
}

int find_invoke(int n, int begin, int end) {
	if (n<0 || n>20 || begin >= end || begin < 0 || end > 4) return -1;
	return find_recursive(n, begin, end);
}

/*��������
begin   end		element	  ����ֵ
-1      N					-1
4       2					-1
0       5					-1
0       4		  12        11
0       4         5         1
0       4         4         1
*/

int module1() {

	cout << find(0, -1, 1) << endl;
	cout << find(0, 4, 2) << endl;
	cout << find(0, 0, 5) << endl;
	cout << find(12, 0, 4) << endl;
	cout << find(5, 0, 4) << endl;
	cout << find(4, 0, 4) << endl;
	cout << find_invoke(0, -1, 1) << endl;
	cout << find_invoke(0, 4, 2) << endl;
	cout << find_invoke(0, 0, 5) << endl;
	cout << find_invoke(12, 0, 4) << endl;
	cout << find_invoke(5, 0, 4) << endl;
	cout << find_invoke(4, 0, 4) << endl;
	return 0;
}