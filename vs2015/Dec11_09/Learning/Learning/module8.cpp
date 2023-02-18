#include<iostream>
#include<algorithm>

using namespace std;

struct Node {
	int data;
	Node* next;
};

struct List {
	Node* head;
	unsigned int size;
};

void initList(List& list) {
	list.head = nullptr;
	list.size = 0;
	list.head = new Node;
	Node* ptr = list.head;
	for (int i = 1; i < 10; i++) {
		ptr->next = new Node;
		ptr->next->data = i;
		ptr->next->next = nullptr;
		ptr = ptr->next;
		list.size++;
	}
	return;
}

void reverse(List& list) {
	Node* beg = nullptr;
	Node* temp = list.head;
	Node* end = list.head;
	int count = 0;
	while (temp->next != nullptr) {
		++count;
		temp = temp->next;
	}
	//移动第一个到结尾
	if (count > 1) {
		list.head = list.head->next;
		end->next = nullptr;
		temp->next = end;

		temp = list.head;
		beg = end; //temp不等于end ,就将temp 放到beg后面
		while (temp != end) { //todo:: 这里顺序反了，都放在beg后面且beg不动。
			list.head = temp->next;
			beg->next = temp;
			temp->next = beg->next;
		}
	}
}

int module8() {
	List list;
	initList(list);
	reverse(list);
	Node* temp = list.head;
	while (temp != nullptr) {
		cout << temp->data << "  ";
		temp = temp->next;
	}
	return 0;
}