#pragma once
//֧��C++11���ԣ���Ϊ�ڿ��ӻ������ʱ���õ���to_string,�����C++11�������ԣ�ԭ��������֧��
#pragma GCC diagnostic error "-std=c++11"  

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;
int DIM;

struct Point
{
	vector<double> p;
	double operator[](int cd) { return p[cd]; }

	bool operator==(Point a) {
		for (int i = 0; i < p.size(); i++) {
			if (p[i] != a.p[i]) {
				return false;
			}
		}
		return true;
	}
	void out() {
		for (int i = 0; i < p.size(); i++) {
			cout << p[i]<<" ";
		}
		cout << endl;
	}
};

//�Ƚ�cdά���ϵĴ�С a[cd] < b[cd] ��
int cmp_dimention;
bool cmp(Point a, Point b) {
	if (a.p.size() != b.p.size()) {
		cout << "error :: cmp :: Two Points have different DIM" << endl;
		return false;
	}
	if (cmp_dimention > a.p.size()) {
		cout << "error :: cmp :: current dimention > DIM " << endl;
		return false;
	}
	return a[cmp_dimention] < b[cmp_dimention];
}

//overload
bool cmp2(Point a, Point b,int cd) {
	if (a.p.size() != b.p.size()) {
		cout << "error :: cmp2 :: Two Points have different DIM" << endl;
		cout << "a:: " << a.p.size() << " b::" << b.p.size() << endl;
		return false;
	}
	if (cd > a.p.size()) {
		cout << "error :: cmp2 :: current dimention > DIM " << endl;
		cout << "cd ::" << cd << "current_dimention ::" << a.p.size() << endl;
		return false;
	}
	return a[cd] < b[cd];
}

int KDTree_Size = 0;

class KDNode
{
public:
	KDNode() {
		left = right = NULL;
		key = KDTree_Size;
		KDTree_Size++;
	}
	KDNode(Point X) {
		data = X;
		left = right = NULL;
		key = KDTree_Size;
		KDTree_Size++;
	}
	KDNode(Point x, KDNode *l, KDNode *r) {
		data = x;
		left = l;
		right = r;
		key = KDTree_Size;
		KDTree_Size++;
	}
	~KDNode();

public:
	KDNode *left;
	KDNode *right;
	Point data;
	int key;
};


class KDTree
{
public:
	KDTree(int dimention) {
		DIM = dimention;
		root = NULL;
		size = 0;
	}
	KDNode * init(KDNode* &t, vector<Point> &m,int cd);
	~KDTree();
	KDNode * insert(Point x, KDNode *t, int cd);
	Point findmin(KDNode *t, int FindDim ,int cd,Point &min);
	KDNode* deleteNode(Point x, KDNode *t, int cd);
	void OrderOutPut(KDNode *t);
	void OrderOutfile(KDNode *t, int depth);
	void printKDTree(KDNode *t,int depth);
	KDNode* exFind(Point x, KDNode *t, int cd,bool &f);

public:
	KDNode *root;
	int size;
	int DIM;
};

KDNode * KDTree::init(KDNode * &t, vector<Point> &m,int cd)
{
	if (m.size() == 0) {
	//	cout << "m size =0" << endl;
		return NULL;
	}
	int cutting_dim = cd % DIM;
	int mid = m.size() / 2;
	// Ѱ�ҵ�mid ��Ԫ����Ϊ�з���ֵ �����Ӷ�ΪO��n��
	//nth_element(m.begin, m.begin + mid, m.end, cmp);//������
	cmp_dimention = cutting_dim;
	sort(m.begin(),m.end(),cmp);
	t = new KDNode(m[mid]);

	//cout << "new node" << endl; t->data.out(); cout << endl;

	vector<Point> a, b;
	for (int i = 0; i < mid; i++) {
		a.push_back(m[i]);
	}
	for (int i = mid + 1; i < m.size(); i++) {
		b.push_back(m[i]);
	}

	t->left = init(t->left, a, cd + 1);
	t->right = init(t->right, b, cd + 1);
	return t;
}

//������� delete
KDTree::~KDTree()
{
}


KDNode * KDTree::insert(Point x, KDNode *t, int cd) {
	if (t == NULL) {
		t = new KDNode(x);
		size++;
	}
	else if (x == t->data) {
		cout << "error :: insert :: insert duplicate" << endl; return t;
	}
	else if (x[cd] < t->data[cd]) {
		t->left = insert(x, t->left, (cd + 1) % DIM);
	}
	else {
		t->right = insert(x, t->right, (cd + 1) % DIM);
	} 
	return t;
}

//debug :: �ڵ���findmin������ʱ��һ��Ҫ��ʼ��min��ʹ�ø��ڵ��ʼ������Ϊ����cmp������ʱ���Ƚ�min.p.size()���Ƚ�ά�ȴ�С
Point KDTree::findmin(KDNode *t, int FindDim, int cd,Point &min) {
	//����,�ݹ������������Сֵ
	if (t == NULL) {
		return min; 
	}
	//t->data��ӦFindDim�ϵ�ֵС�� ������Сֵ���滻
	if (cmp2(t->data, min,FindDim)) {
		min = t->data;
	//	cout << "min changed :: " << endl;
	//	min.out();
	}
	//����ά��d=Ŀ��ά��
	if (cd == FindDim) {
	//	cout << "myDimention" << endl;
	//	t->data.out();
		return findmin(t->left, FindDim, (cd + 1) % DIM, min);
	}
	else {
		//��ά��d ��=Ŀ��ά��
		Point leftp;
		Point rightp;
		leftp = findmin(t->left, FindDim, (cd + 1) % DIM, min);
		rightp = findmin(t->right, FindDim, (cd + 1) % DIM, min);
		if (cmp2(leftp, rightp,FindDim)) {
			//������С
			return leftp;
		}
		else { return rightp; }
	}
}

KDNode* KDTree::exFind(Point x, KDNode *t, int cd,bool &f) {
	if (t == NULL) {
		return t;
	}
	else if (x == t->data) {
		f = true;
		cout << "exfind :: find the point" << endl; return t;
	}
	else if (x[cd] < t->data[cd]) {
		t->left = exFind(x, t->left, (cd + 1) % DIM,f);
	}
	else {
		t->right = exFind(x, t->right, (cd + 1) % DIM,f);
	}
	return t;
}

KDNode* KDTree::deleteNode(Point x, KDNode *t, int cd) {
	//ɾ���㲻����
	if (t == NULL) { cout << "error :: deleteNode :: the Node is NULL" << endl; return t; }
	int next_cd = (cd + 1) % DIM; //t�ڵ����ڵ��з�ά��

	//���˽ڵ�Ϊ��ɾ���ڵ�
	if (x == t->data) { 
		//debug::
	//	cout << "find x" <<"in dimention"<<cd<<endl;
	//	t->data.out();
		//ʹ�� ��������С���滻
		if (t->right != NULL) {
			Point temp=t->right->data;
			temp= findmin(t->right, next_cd, next_cd, temp);
			t->data = temp;
	//		cout << "find rightsubtree node " << endl;
	//		t->data.out();
			t->right = deleteNode(temp, t->right, next_cd);
		}//������Ϊ��
		else
		{	//�������ǿ�
			//�������� �� ʹ���µ�����������С�ڵ�
			if (t->left != NULL) {
				Point temp = t->left->data;
				temp = findmin(t->left,next_cd,next_cd,temp);
			//	swap(t->data,temp);//����
				t->data = temp;
				t->right = deleteNode(temp, t->left, next_cd);
				t->left = NULL;
			}
			else //��ɾ���ڵ�û�к���
			{
				t = NULL;
			}
		}
	}// �˽ڵ㲻�Ǵ�ɾ���ڵ㣬 ������������
	else
	{
		//ȷ��ǰ���Ļ��ֿռ�
		if (x[cd] < t->data[cd]) t->left = deleteNode(x, t->left, next_cd);
		else t->right = deleteNode(x, t->right, next_cd);
	}
	return t;
}

void KDTree::OrderOutPut(KDNode *t) {
	if (t == NULL) { return; }
	queue<KDNode *> q;
	q.push(t);
	while (!q.empty())
	{
		KDNode *temp = q.front(); q.pop();
		temp->data.out();
		if (temp->left != NULL) { q.push(temp->left); }
		if (temp->right != NULL) { q.push(temp->right); }
	}
	return;
}


//���ڵ���⴦��
void KDTree::OrderOutfile(KDNode *t,int depth) {
	if (t == NULL) { return; }
	ofstream outfile;
	outfile.open("draw.dot", ios::out);
	outfile << "Digraph a {" << endl;
	string strroot;
	for (int i = 0; i < t->data.p.size(); i++) {
		strroot = strroot + "," + to_string(t->data.p[i]);
	}
	outfile << t->key << "[label=\"" << strroot << "\"];" << endl;
	queue<KDNode *> q;
	q.push(t);
	while (!q.empty())
	{
		KDNode *temp = q.front(); q.pop();
	//	temp->data.out();
		t = temp;
		if (temp->left != NULL) { 
			string str;
			for (int i = 0; i < t->left->data.p.size(); i++) {
				str = str + "," + to_string(t->left->data.p[i]);
			}
			//	cout<<str<<endl;
			outfile << t->left->key << "[label=\"" << str <<"\"];" << endl;
			outfile << t->key<< "->" << t->left->key << endl;
			q.push(temp->left); 
		}

		if (temp->right != NULL) { 
			string str;
			for (int i = 0; i < t->right->data.p.size(); i++) {
				str = str + "," + to_string(t->right->data.p[i]);
			}
			//	cout<<str<<endl;
			outfile << t->right->key << "[label=\"" << str << "\"];" << endl;
			outfile << t->key << "->" << t->right->key << endl;
			q.push(temp->right); 
		}
	}
	outfile << "}" << endl;
	outfile.close();
	return;
}

void KDTree::printKDTree(KDNode*t, int depth)
{
	for (int i = 0; i < depth; i++) {
		cout << "\t";
	}
	t->data.out();
	if (t->left == NULL && t->right == NULL) {
		cout << endl;
		return;
	}
	else {
		if (t->left != NULL) {
			printKDTree(t->left, depth + 1);
		}
		if (t->right != NULL) {
			printKDTree(t->right, depth + 1);
		}
	}

}

void rangeSearch2D(KDNode *t, double sx, double tx, double sy, double ty, int depth, vector<Point> &ans) {
	double x = t->data.p[0];
	double y = t->data.p[1];
	if (sx <= x && x <= tx && sy <= y &&y <= ty) {
		ans.push_back(t->data);
	}

	if (depth % 2 == 0) {
		if (t->left != NULL) {
			if (sx <= x) {
				rangeSearch2D(t->left, sx, tx, sy, ty, depth + 1, ans);
			}
		}
		if (t->right != NULL) {
			if (x <= tx) {
				rangeSearch2D(t->right, sx, tx, sy, ty, depth + 1, ans);
			}
		}
	}
	else {
		if (t->left != NULL) {
			if (sy <= y) {
				rangeSearch2D(t->left, sx, tx, sy, ty, depth + 1, ans);
			}
		}
		if (t->right != NULL) {
			if (y <= ty) {
				rangeSearch2D(t->right, sx, tx, sy, ty, depth + 1, ans);
			}
		}
	}
}