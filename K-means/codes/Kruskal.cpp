#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <thread>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;

const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;
static int input = 0;

class Color {
public:
	int rank;
	int red; //颜色的三个标签
	int green;
	int blue;
	Color(int red, int green, int blue,int rank) {
		this->blue = blue;
		this->red = red;
		this->green = green;
		this->rank = rank;
	}
	Color(int red, int green, int blue) {
		this->blue = blue;
		this->red = red;
		this->green = green;
		this->rank = ++input;
	}
	int likeWhatColor() {
		int num1 = pow(this->red - 255, 2) + pow(this->green, 2) + pow(this->blue, 2);
		int num2 = pow(this->red, 2)+ pow(this->green - 255, 2) + pow(this->blue, 2);
		int num3 = pow(this->red, 2) + pow(this->green, 2) + pow(this->blue - 255, 2);
		if (num1 <= num2 && num1 <= num3)
			return RED;
		else if (num2 <= num1 && num2 <= num3)
			return GREEN;
		return BLUE;
	}
	friend ostream& operator<<(ostream& os, Color c) {
		os << "RGB：(" << c.red << "," << c.green << "," << c.blue << ")" << endl;
		return os;
	}
};

class Edge { 
public:
	int v1;
	int v2;
	int value;
	Edge(int v1, int v2, int value) {
		this->v1 = v1;
		this->v2 = v2;
		this->value = value;
	}
	friend ostream& operator<<(ostream& os, Edge e) {
		os << e.v1 << " " << e.v2 << " " << e.value <<endl;
		return os;
	}
	bool operator<(Edge e) {
		return this->value < e.value;
	}
};

const int N = 50; //数据量
const int K = 3; //聚类数
int parent[N + 1];
vector<Color> color;
vector<Edge> edge;
vector<Color>::iterator itc;
vector<Edge>::iterator ite;

int getDistance(Color c1, Color c2); //距离函数
int find_(int); //优化前的搜索函数
int find(int); //并查集函数
void unite(int, int); //并查集函数
int K_cluster(int n); //Kruskal聚类算法

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	for (int i = 1; i <= N; i++) {
		int tmp1 = int(rand()*1.31)% 255;
		this_thread::sleep_for(std::chrono::milliseconds(rand()%20)); 
		int tmp2 = (rand()*17) % 255;
		this_thread::sleep_for(std::chrono::milliseconds(rand()%10));
		int tmp3 = (rand()) % 255+1;
		color.push_back(Color(tmp1,tmp2,tmp3,i));
		for (int j = 1; j < i;j++) {
			edge.push_back(Edge(j, i, getDistance(color[j - 1], color[i - 1])));
		}
	}
	
	sort(edge.begin(), edge.end());  //按照权值递增排序
	cout << "-----------------随机生成的元素-----------------" << endl;
	for (itc = color.begin(); itc != color.end(); itc++)
		cout << *itc;
	cout << "-----------------按照权值递减后排序得到的完全图的边值-----------------" << endl;
	for (ite = edge.begin(); ite != edge.end(); ite++)
		cout << *ite;

	auto start = std::chrono::high_resolution_clock::now();
	K_cluster(N); //将N个元素分为了K类
	
	vector<int> means; 
	means.push_back(parent[1]);
	for (int i = 1; i <= N; i++) {
		bool exists = false;
		for (vector<int>::iterator it = means.begin(); it != means.end(); it++) {
			if (parent[i] == (*it)) {
				exists = true;
				break;
			}
		}
		if (!exists)
			means.push_back(parent[i]); //找到这K个聚类的代表元素
	}

	cout << "-----------------parent数组-----------------" << endl;
	for (int i = 1; i <= N; i++)
		cout << parent[i] << " ";
	cout <<endl<< "-----------------K个聚类的代表元素-----------------"<< endl;
	for (vector<int>::iterator it = means.begin(); it != means.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

	vector<Color> means1;
	vector<Color> means2;
	vector<Color> means3;
	for (itc = color.begin(); itc != color.end(); itc++) {
		if (parent[(*itc).rank] == means[0])
			means1.push_back(*itc);
		else if (parent[(*itc).rank] == means[1])
			means2.push_back(*itc);
		else
			means3.push_back(*itc);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	double elapsed = duration.count() * 1e-6; //单位变为毫秒

	cout << "-----------------K聚类结果-----------------" << endl;
	cout << "第一聚类：" << endl;
	for (itc = means1.begin(); itc != means1.end(); itc++)
		cout << *itc;
	cout << "第二聚类：" << endl;
	for (itc = means2.begin(); itc != means2.end(); itc++)
		cout << *itc;
	cout << "第三聚类：" << endl;
	for (itc = means3.begin(); itc != means3.end(); itc++)
		cout << *itc;

	cout << "程序运行时间：" << elapsed << " ms" << endl;
	return  0;
}

int getDistance(Color c1, Color c2) {
	return pow(c1.red - c2.red, 2) + pow(c1.blue - c2.blue, 2) + pow(c1.green - c2.green, 2);
}
int find_(int x) {
	while (parent[x] != x)			
		x = parent[x];
	return x;
}
int find(int x) {
	if (parent[x] != x)
		parent[x] = find(parent[x]);
	return parent[x];
}
void unite(int x, int y) {
	int fx = find(x);
	int fy = find(y);
	if (fx != fy)
		parent[fx] = fy;
}
int K_cluster(int n) {
	int k = 0; //Kruskal算法执行到n-k步结束
	for (int i = 1; i <= n; i++) //初始化每个节点的父节点为自己
		parent[i] = i;
	for (ite = edge.begin(); ite != edge.end(); ite++) {
		if (find(ite->v1) != find(ite->v2)) {
			k++;
			unite(ite->v1, ite->v2);
			if (k == n - K) {
	/*			cout << "未刷新前的parent数组：" << endl;
				for (int i = 1; i <= n; i++) {
					cout << parent[i] << " ";
				}
				cout << endl;*/
				for (int i = 1; i <= n; i++) {
					if (parent[i] != i)
						parent[i] = find(parent[i]);
				}
				break;
			}
		}
	}
	return 0;
}