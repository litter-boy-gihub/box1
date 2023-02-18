#include<iostream>
#include<deque>
#include<stack>

using namespace std;


struct Node {
	Node* left;
	Node* right;
	int i;
};

struct BTree {
	Node* head;
};

stack<Node*> g_stack;

//��������ݹ鷨  //��������DFS�ı��壬������֯��1.ջ����ջ���� 2.�ҽڵ�ѹջ 3.��ڵ�ѹջ������ȷ��ʣ�
void LRD(Node* root) {
	if (nullptr == root) return;
	//left
	LRD(root->left);
	//right
	LRD(root->right);
	//root
	cout << root->i <<"  ";
	return;
}

//�������������� LRD �ı�������� DRL�ı�������෴
void LRD_ITERATON(Node* root) {
	Node* temp = nullptr;
	stack<Node*> stack2;
	while (!g_stack.empty()) g_stack.pop();
	g_stack.push(root);
	while (!g_stack.empty()) {
		temp = g_stack.top();
		g_stack.pop();
		stack2.push(temp);

		if (temp->left != nullptr) g_stack.push(temp->left);
		if (temp->right != nullptr) g_stack.push(temp->right);
	}
	while (!stack2.empty()) {
		cout << stack2.top()->i << "  ";
		stack2.pop();
	}
}

//�������
void LDR(Node* root) {
	if (nullptr == root) return;
	//left
	LDR(root->left);
	//root
	cout << root->i << "  ";
	//right
	LDR(root->right);
	return;
}

//�������������
void LDR_ITERATION(Node* root) {
	Node* current = root;
	Node* temp = nullptr;
	while (!g_stack.empty()) g_stack.pop();
	while (!g_stack.empty() || current != nullptr) {
		while (current != nullptr) {		//�ҵ����±ߵ���������Ϊ�գ�
			g_stack.push(current);
			current = current->left;
		}

		temp = g_stack.top();
		g_stack.pop();
		cout << temp->i << "  ";

		if (temp->right != nullptr) {
			current = temp->right;			//����current,�����ݹ顱���������� ������currentΪ�գ�˵��ջ��top���������������ˣ�������ջtop
		}
	}
}

//ǰ�����
void DLR(Node* root) {
	if (nullptr == root) return;
	//root
	cout << root->i << "  ";
	//left
	DLR(root->left);
	//right
	DLR(root->right);
	return;
}

//ǰ�����������
void DLR_ITERATION(Node* root) {
	Node* temp = nullptr;
	while (!g_stack.empty()) g_stack.pop();
	g_stack.push(root);
	while (!g_stack.empty()) {
		temp = g_stack.top();
		g_stack.pop();
		cout << temp->i << "  ";

		if (nullptr != temp->right) g_stack.push(temp->right);
		if (nullptr != temp->left) g_stack.push(temp->left);
	}
}

deque<Node*> queue;
//��α���BFT ���� deque
void BFT(Node* root) {
	queue.push_back(root);
	while (!queue.empty()) {
		if (queue.front()->left) queue.push_back(queue.front()->left);
		if (queue.front()->right) queue.push_back(queue.front()->right);
		cout << queue.front()->i << "  ";
		queue.pop_front();
	}
}
//BFS
Node* BFS(Node* root,const int i) {
	queue.push_back(root);
	while (!queue.empty()) {
		if (queue.front()->left) queue.push_back(queue.front()->left);
		if (queue.front()->right) queue.push_back(queue.front()->right);
		if (i == queue.front()->i) {
			Node* ret = queue.front();
			queue.clear();
			return ret;
		}
		else cout << "visit" << queue.front()->i << endl;
		queue.pop_front();
	}
}

//������ȱ���DFT ��ջ stack 
bool arr[7];

void DFT(Node* root) {
	memset(&arr, false, sizeof(arr));
	g_stack.push(root);
	while (!g_stack.empty()) {
		arr[g_stack.top()->i] = true;     //����
		cout << g_stack.top()->i << "  ";
		Node* temp = g_stack.top();
		g_stack.pop();

		if (temp->left) {
			arr[temp->left->i] = true;
			g_stack.push(temp->left);
		}
		if (temp->right) {
			arr[temp->right->i] = true;
			g_stack.push(temp->right);
		}
	}
	return;
}

//DFS
Node* DFS(Node* root,int i) {
	memset(&arr, false, sizeof(arr));
	while (!g_stack.empty()) g_stack.pop();
	
	g_stack.push(root);
	while (!g_stack.empty()) {
		Node* temp = g_stack.top();
		arr[temp->i] = true;     //����
		cout << temp->i << "  ";
		g_stack.pop();
		if (temp->i == i) return temp;

		if (temp->left) {
			arr[temp->left->i] = true;
			g_stack.push(temp->left);
		}
		if (temp->right) {
			arr[temp->right->i] = true;
			g_stack.push(temp->right);
		}
	}
	return nullptr;
}


//��ӽڵ㣬������ӵĽڵ�
Node* add_Node(Node* exist, Node* new_node,bool f) {
	new_node->left = nullptr;
	new_node->right = nullptr;
	if (f) exist->left = new_node;
	else exist->right = new_node;
	return new_node;
}

void Init_BTree(BTree* btree) {
	btree->head = new Node;
	Node* root = btree->head;
	root->i = 0;
	root->left = nullptr;
	root->right = nullptr;

	Node* temp = add_Node(root,new Node,true);
	temp->i = 1;

	add_Node(temp, new Node, true)->i = 3;
	add_Node(temp, new Node, false)->i = 4;

	temp = add_Node(root, new Node, false);
	temp->i = 2;
	
	add_Node(temp, new Node, true)->i = 5;
	add_Node(temp, new Node, false)->i = 6;
}

int module7() {
	cout.sync_with_stdio(false);//�ر� cout �� stdio��ͬ��
	cin.tie(0);//�ӿ�cin���ٶ�
	BTree* tree = new BTree;
	Init_BTree(tree);
	LRD(tree->head); cout << endl;
	LDR(tree->head); cout << endl;
	DLR(tree->head); cout << endl;
	BFT(tree->head); cout << endl;
	DFT(tree->head); cout << endl;
	cout << "BFS: " << BFS(tree->head, 5)->i << endl;
	cout << "DFS: " << DFS(tree->head, 3)->i << endl;
	DLR_ITERATION(tree->head); cout <<"dlr iteration" <<endl;
	LDR_ITERATION(tree->head); cout << "ldr iteration"<<endl;
	LRD_ITERATON(tree->head); cout << "lrd iteration" << endl;
	return 0;
}