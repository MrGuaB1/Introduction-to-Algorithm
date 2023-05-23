#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class edge //���ò��鼯ʵ�ֵ�Kruskal�㷨
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

int parent[51]; //�����ж��Ƿ���һ����ͨ��
vector<edge> vec;

int find_root(int x) { //��ͨ���ϵĴ���Ԫ�أ��ҵ������ڵ㡱
	if (parent[x] != x) 
		parent[x] = find_root(parent[x]);
	return parent[x];
}
void unite(int x, int y) {
	int x_root = find_root(x);
	int y_root = find_root(y);
	if (x_root != y_root)  //�������ϵĴ���Ԫ�ز�ͬ���ͺϲ���������
		parent[x_root] = y_root;	
}

int kruskal(int n) {
	for (int i = 1; i <= n; i++) //��ʼ�����鼯����ÿ���ڵ�ĸ��ڵ�ָ������
		parent[i] = i;	
	int ans = 0;
	for (vector<edge>::iterator it = vec.begin(); it != vec.end(); it++) {
		if (find_root(it->v1) != find_root(it->v2)) { //�����߲���һ������֮��
			ans += it->weight;
			unite(it->v1, it->v2); //�ϲ���������
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