#include<iostream>
//二分查找思想进行区域的划分
//1-5 6-10 11-15 16-20
//返回区域的起始值 1,6,11,16,21

//分两种情况，二分查找成功（直接返回mid ） 和 二分查找失败（）返回begin

using namespace std;
static int arr[] = { 1,6,11,16,21 };//还要涉及begin最小值，end的最大值是否合法

							 //迭代法
int find(int n, int begin, int end) {
	if (n<0 || n>20 || begin > end || begin < 0 || end > 4) return -1;
	int mid;
	while (begin <= end) {
		mid = begin + (end - begin) / 2;//平均值不溢出写法 
		if (n < arr[mid]) { end = mid - 1; continue; }
		if (n > arr[mid]) { begin = mid + 1; continue; }
		if (n == arr[mid]) return n; //查找成功
	}
	return arr[begin - 1];//查找失败
}

//递归法
int find_recursive(int n, int begin, int end) {
	if (begin == end + 1) return arr[begin - 1];//查找失败
	int mid = begin + (end - begin) / 2;
	if (n < arr[mid]) return find_recursive(n, begin, mid - 1);
	if (n > arr[mid]) return find_recursive(n, mid + 1, end);
	return n; //查找成功
}

int find_invoke(int n, int begin, int end) {
	if (n<0 || n>20 || begin >= end || begin < 0 || end > 4) return -1;
	return find_recursive(n, begin, end);
}

/*测试用例
begin   end		element	  返回值
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