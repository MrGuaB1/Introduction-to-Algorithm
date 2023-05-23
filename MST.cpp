#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class edge //利用并查集实现的Kruskal算法
{
public:
	int v1;
	int v2;
	int weight;
	edge(int v1, int v2, int weight) {
		this->v1 = v1;
		this->v2 = v2;
		this->weight = weight;
	}
	bool operator<(edge e) {
		return this->weight < e.weight;
	}
	friend ostream& operator<<(ostream& os, edge e) {
		os << e.v1 << " " << e.v2 << " " << e.weight;
		return os;
	}
};

int parent[51]; //用于判断是否在一个连通块
vector<edge> vec;

int find_root(int x) { //联通集合的代表元素，找到“根节点”
	if (parent[x] != x) 
		parent[x] = find_root(parent[x]);
	return parent[x];
}
void unite(int x, int y) {
	int x_root = find_root(x);
	int y_root = find_root(y);
	if (x_root != y_root)  //两个集合的代表元素不同，就合并两个集合
		parent[x_root] = y_root;	
}

int kruskal(int n) {
	for (int i = 1; i <= n; i++) //初始化并查集，将每个节点的父节点指向自身
		parent[i] = i;	
	int ans = 0;
	for (vector<edge>::iterator it = vec.begin(); it != vec.end(); it++) {
		if (find_root(it->v1) != find_root(it->v2)) { //两条边不在一个集合之中
			ans += it->weight;
			unite(it->v1, it->v2); //合并两个集合
		}
	}
	return ans;
}

int main() {

	int n, m;
	cin >> n >> m;
	int v1, v2, weight;
	for (int i = 1; i <= m; i++)
	{
		cin >> v1 >> v2 >> weight;
		vec.push_back(edge(v1, v2, weight));
	}
	sort(vec.begin(), vec.end());
	cout << kruskal(n);

	return 0;
}