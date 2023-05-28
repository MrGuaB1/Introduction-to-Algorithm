#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

const int K = 3;
using namespace std;

struct Tuple { //每个数据的标签，这里可以理解为一个点的x，y坐标
	float attr1;
	float attr2;
};

float getDistance(Tuple t1, Tuple t2);
int clusterOfTuple(Tuple means[], Tuple tuple); //根据质心，决定当前元组属于哪个簇
float getVar(vector<Tuple> clusters[], Tuple means[]); //获得给定簇集的平方误差
Tuple getMeans(vector<Tuple> cluster); //获得当前簇的均值
void KMeans(vector<Tuple> tuples); //K-means算法

int main() {

	char fname[256];
	cout << "请输入存放数据的文件名： ";
	cin >> fname;
	cout << endl;
	ifstream infile;
	infile.open(fname, ios::in);
	if (!infile) {
		cout << "不能打开输入的文件" << fname << endl;
		return 0;
	}
	int count = 0;
	vector<Tuple> tuples;
	Tuple tuple;
	while (!infile.eof()) {
		count++;
		if (count % 2 == 1) infile >> tuple.attr1;
		else {
			infile >> tuple.attr2;
			tuples.push_back(tuple);
		}
	}

	//输出文件中的元组信息
	for (vector<Tuple>::size_type ix = 0; ix != tuples.size(); ++ix)
		cout << "(" << tuples[ix].attr1 << "," << tuples[ix].attr2 << ")" << "    ";
	cout << endl;
	KMeans(tuples);
	return 0;
}

float getDistance(Tuple t1, Tuple t2) {
	return sqrt((t1.attr1 - t2.attr1) * (t1.attr1 - t2.attr1) + (t1.attr2 - t2.attr2) * (t1.attr2 - t2.attr2));
}
int clusterOfTuple(Tuple means[], Tuple tuple) { //根据质心，决定当前元组属于哪个簇
	float dist = getDistance(means[0], tuple);
	float tmp;
	int label = 0;//标示属于哪一个簇
	for (int i = 1; i < K; i++) {
		tmp = getDistance(means[i], tuple);
		if (tmp < dist) { dist = tmp; label = i; }
	}
	return label;
}
float getVar(vector<Tuple> clusters[], Tuple means[]) {
	float var = 0;
	for (int i = 0; i < K; i++)
	{
		vector<Tuple> t = clusters[i];
		for (int j = 0; j < t.size(); j++)
			var += getDistance(t[j], means[i]);
	}
	return var;
}
Tuple getMeans(vector<Tuple> cluster) { //获得当前簇的均值

	int num = cluster.size();
	double meansX = 0, meansY = 0;
	Tuple t;
	for (int i = 0; i < num; i++)
	{
		meansX += cluster[i].attr1;
		meansY += cluster[i].attr2;
	}
	t.attr1 = meansX / num;
	t.attr2 = meansY / num;
	return t;
}
void KMeans(vector<Tuple> tuples) {
	vector<Tuple> clusters[K];
	Tuple means[K];
	int i = 0;
	//默认一开始将前K个元组的值作为k个簇的质心（均值）
	for (i = 0; i < K; i++) {
		means[i].attr1 = tuples[i].attr1;
		means[i].attr2 = tuples[i].attr2;
	}
	int lable = 0;
	//根据默认的质心给簇赋值
	for (i = 0; i != tuples.size(); ++i) {
		lable = clusterOfTuple(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
	//输出刚开始的簇
	for (lable = 0; lable < 3; lable++) {
		cout << "第" << lable + 1 << "个簇：" << endl;
		vector<Tuple> t = clusters[lable];
		for (i = 0; i < t.size(); i++)
		{
			cout << "(" << t[i].attr1 << "," << t[i].attr2 << ")" << "   ";
		}
		cout << endl;
	}
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止
	{

		for (i = 0; i < K; i++) //更新每个簇的中心点
			means[i] = getMeans(clusters[i]);
		oldVar = newVar;
		newVar = getVar(clusters, means); //计算新的准则函数值
		for (i = 0; i < K; i++) //清空每个簇
			clusters[i].clear();
		//根据新的质心获得新的簇
		for (i = 0; i != tuples.size(); ++i) {
			lable = clusterOfTuple(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
		//输出当前的簇
		for (lable = 0; lable < 3; lable++) {
			cout << "第" << lable + 1 << "个簇：" << endl;
			vector<Tuple> t = clusters[lable];
			for (i = 0; i < t.size(); i++)
				cout << "(" << t[i].attr1 << "," << t[i].attr2 << ")" << "   ";
			cout << endl;
		}
	}
}