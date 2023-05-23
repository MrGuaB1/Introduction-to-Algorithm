#include<iostream>
#include<queue>
#include<vector>
#include<iomanip>
using namespace std;
struct Huffnode {
	int weight;
	int depth;
	Huffnode* lchild;
	Huffnode* rchild;
	Huffnode(int w) { 
		this->depth = 0;
		this->weight = w;
		this->lchild = NULL;
		this->rchild = NULL;
	}
};
class Hufftree
{
public:
	Huffnode* root;
	Hufftree(Huffnode* t)
	{
		root = t;
		t->lchild = NULL;
		t->rchild = NULL;
	}
	Hufftree() {}
	Hufftree(Hufftree& t){this->root = create(t.root);} //复制构造函数，递归的生成子节点
	Hufftree(const Hufftree& t) { this->root = t.root; }
	void setAsLeftTree(Huffnode* root, Hufftree& t) { root->lchild = t.root; }
	void setAsRightTree(Huffnode* root, Hufftree& t) { root->rchild = t.root; }
	Huffnode* create(Huffnode* node)
	{
		Huffnode* head = NULL;
		if (node != NULL)
		{
			head = new Huffnode(node->weight);
			head->lchild = create(node->lchild);
			head->rchild = create(node->rchild);
		}
		delete node; //堆内存不delete，debug一小时，好死喵
		return head;
	}
};
int AllOfWeight(Huffnode* t) {
	int sum = 0;
	queue<Huffnode*> temp;
	while (t != NULL)
	{
		if (t->lchild != NULL)
		{
			temp.push(t->lchild);
			t->lchild->depth = t->depth + 1;
		}
		if (t->rchild != NULL)
		{
			temp.push(t->rchild);
			t->rchild->depth = t->depth + 1;
		}
		if (t->lchild == NULL && t->rchild == NULL)
			sum += t->depth * t->weight;
		if (!temp.empty())
		{
			t = temp.front();
			temp.pop();
		}
		else break;
	}
	return sum;
}
class Vector
{
public:
	vector<Hufftree> forest;
	void push(Hufftree t){forest.push_back(t);}
	int num() { return forest.size(); }
	Hufftree get(int index){return this->forest[index];}
	Hufftree remove()
	{
		int min = forest[0].root->weight;
		int flag = 0;
		for (int i = 0; i < num(); i++)
		{
			if (forest[i].root->weight < min)
			{
				min = forest[i].root->weight;
				flag = i;
			}
		}
		Hufftree MinTree(forest[flag]);
		forest.erase(forest.begin()+flag);
		return MinTree;
	}
};


int main() {

	Vector Forest;
	int n; cin >> n;
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		int w; cin >> w;sum += w;
		Forest.push(Hufftree(new Huffnode(w))); //初始时每一个节点都是一棵树
	}
	

	while (Forest.num()>=2) //每次取出两棵最小的树，合成他们并重新加入森林，重复操作直至只剩一棵树
	{
		Hufftree t1 = Forest.remove(); 
		Hufftree t2 = Forest.remove();
		Huffnode* newRoot = new Huffnode(t1.root->weight + t2.root->weight);
		Hufftree newTree(newRoot);
		newTree.setAsLeftTree(newTree.root, t1);
		newTree.setAsRightTree(newTree.root, t2);
		Forest.push(newTree);
	}

	cout <<fixed<<setprecision(2)<< 1.0 * AllOfWeight(Forest.get(0).root) / sum;
	return 0;
}