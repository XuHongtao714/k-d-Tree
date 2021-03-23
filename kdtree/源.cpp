#include "KDTree.h"
#include <vector>

using namespace std;

void preorder(KDNode *t) {
	if (t == NULL) {
		return;
	}
	t->data.out();
	preorder(t->left);
	preorder(t->right);
}


int main() {
	int k = 2;
	int initSize = 10;
	cout << "------------------Welecome to k-d tree------------------" << endl;
	cout << endl;
	cout << endl;
	cout << "   Please input the dimention k : " << endl;
	cin >> k;
	cout << "   Please input the size of initial data :  " << endl;
	cin >> initSize;

	cout << "    Please input the init data ,like 0 0  1 1  " << endl;

	KDTree Tree(k);
	vector<Point> p;
	for (int i = 0; i < initSize; i++) {
		Point temp;
		for (int j = 0; j < k; j++) {
	//		double d = rand()%100;
			double d;
			cin >> d;
			temp.p.push_back(d);
		}
		p.push_back(temp);
	}
	//test out
	cout << "okay ~, your initial sequece is :" << endl;
	for (int i = 0; i < initSize; i++) {
		for (int j = 0; j < k; j++) {
			cout << p[i][j]<<" ";
		}
		cout << endl;
	}
	//test KDTree
	Tree.size = p.size();
	Tree.root=Tree.init(Tree.root, p, 0);
	cout << "and , the KDTree has been built like this" << endl;
	Tree.printKDTree(Tree.root, 0);

	cout << "we have several options " << endl;
	cout << " 1-------------insert" << endl;
	cout << " 2------------findmin" << endl;
	cout << " 3-------------delete" << endl;
	cout << " 4------visualization" << endl;
	cout << " 5--------rangeSearch" << endl;
	cout << " 6-------------exFind" << endl;
	cout << " 7---------------exit" << endl;

	while (1) {
		int op;
		cin >> op;
		switch (op)
		{
		case 3: {
			cout << "--------------welcome to delete interface" << endl;
			cout << "you have now " << k << "dimentions tree, please insure your input correct." << endl;
			cout << "please input delete node , like 0 0 " << endl;
			
			Point y;
			int temp;
			for (int i = 0; i < k; i++) {
				cin >> temp;
				y.p.push_back(temp);
			}
			Tree.deleteNode(y, Tree.root, 0);
			cout << "your got tree like this" << endl;
			Tree.printKDTree(Tree.root, 0);
			cout << endl;
			break;
		}
		case 1: {
			cout << "--------------welcome to insert interface" << endl;
			cout << "you have now " << k << "dimentions tree, please insure your input correct." << endl;

			cout << "please input insert node ,like 0 0 " << endl;
			Point x;
			int temp;
			for (int i = 0; i < k; i++) {
				cin >> temp;
				x.p.push_back(temp);
			}
			Tree.insert(x, Tree.root, 0);
			cout << "the KDTree has been built like this" << endl;
			Tree.printKDTree(Tree.root, 0);
			break;
		}
		case 2: {
			cout << "--------------welcome to findmin interface" << endl;
			cout << "you have now " << k << "dimentions tree, please insure your input correct." << endl;
			cout << "please input the dimention you want find" << endl;
			int temp;
			cin >> temp;
			Point x;
			x = Tree.root->data;
			x = Tree.findmin(Tree.root, temp , 0 , x);
			cout << " findmin of dimention ="<<temp<<" ,from root" << endl << "the min is :" << endl;
			x.out();
			cout << endl;
			break;
		}
		case 4: {
			cout << "--------------welcome to visualization interface" << endl;
			cout << "you have now " << k << "dimentions tree, please insure your input correct." << endl;
			Tree.OrderOutfile(Tree.root, 0);
			cout << "the outputfile has been generalized successfully" << endl;
			cout << "now you can check the pic" << endl;
			break;
		}
		case 5: {
			//need to modif , k-d tree rangeSearch
			cout << "--------------welcome to rangeSearch interface" << endl;
			cout << "you have now " << k << "dimentions tree, please insure your input correct." << endl;
			cout << "please input the range ,like 0 100 0 10 ,if you have a 2-d tree and want search range  0<=x<=100 , 0<=y<=10 " << endl;
			double sx, tx, sy, ty;
			vector<Point> ans;
			cin >> sx >> tx >> sy >> ty;
			rangeSearch2D(Tree.root, sx ,tx, sy, ty, 0, ans);
			cout << "aha , those points meet your standards" << endl;
			for (int i = 0; i < ans.size(); i++) {
				ans[i].out();
			}
			cout << endl;
			break;
		}
		case 6: {
			cout << "--------------welcome to exFind" << endl;
			cout << "you have now " << k << "dimentions tree, please insure your input correct." << endl;
			cout << "Please input what you want search , like 0 0" << endl;
			KDNode *t;
			Point x;
			int temp;
			bool flag = false;
			for (int i = 0; i < k; i++) {
				cin >> temp;
				x.p.push_back(temp);
			}
			t=Tree.exFind(x, Tree.root, 0,flag);
			if (!flag) {
				cout << " not found QAQ ~" << endl;
			}
			break;
		}
		case 7: {
			system("pause");
			exit(1);
			break;
		}
		default:
			cout << "Please input the right number" << endl;
			cout << "you have now " << k << "dimentions tree, please insure your input correct." << endl;

			break;
		}
	}
/*
	cout << "test insert" << endl;
	cout << "insert (0,0)" << endl;
	Point x;
	x.p.push_back(0);
	x.p.push_back(0);
	Tree.insert(x, Tree.root, 0);
	cout << "the KDTree has been built like this" << endl;
	Tree.printKDTree(Tree.root, 0);

	cout << "insert (0,0) again" << endl;
	Tree.insert(x, Tree.root, 0);
	Tree.printKDTree(Tree.root, 0);


	cout << "test findmin" << endl;
	cout << " findmin of dimention =0 ,from root" << endl << "the min is :" << endl;
	x = Tree.root->data;
	x = Tree.findmin(Tree.root, 0,0,x);
	x.out();
	cout << endl;


	//test delete error
	cout << "test delete" << endl;
	cout << "delete (62,64) ,root Node ,has Two subTrees" << endl;
	Point y;
	y.p.push_back(62);
	y.p.push_back(64);
	Tree.deleteNode(y, Tree.root, 0);
	Tree.printKDTree(Tree.root, 0);
	cout << endl;
	cout << "delete (27,36) ,root Node ,has NO subTree" << endl;
	y.p.clear();
	y.p.push_back(27);
	y.p.push_back(36);
	Tree.deleteNode(y, Tree.root, 0);
	Tree.printKDTree(Tree.root, 0);
	cout << endl;
	cout << "delete (61,91) ,root Node ,has Only One left subTree" << endl;
	y.p.clear();
	y.p.push_back(61);
	y.p.push_back(91);
	Tree.deleteNode(y, Tree.root, 0);
	Tree.printKDTree(Tree.root, 0);
	cout << endl;




	cout << __cplusplus << endl;
	cout << endl;
	Tree.OrderOutfile(Tree.root, 0);

	vector<Point> ans;
	
	rangeSearch2D(Tree.root, 1, 60, 1, 60, 0, ans);

	for (int i = 0; i < ans.size(); i++) {
		ans[i].out();
	}
	cout << endl;
	*/
	system("pause");

	return 0;
}