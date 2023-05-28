#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

const int K = 3;
using namespace std;

struct Tuple { //ÿ�����ݵı�ǩ������������Ϊһ�����x��y����
	float attr1;
	float attr2;
};

float getDistance(Tuple t1, Tuple t2);
int clusterOfTuple(Tuple means[], Tuple tuple); //�������ģ�������ǰԪ�������ĸ���
float getVar(vector<Tuple> clusters[], Tuple means[]); //��ø����ؼ���ƽ�����
Tuple getMeans(vector<Tuple> cluster); //��õ�ǰ�صľ�ֵ
void KMeans(vector<Tuple> tuples); //K-means�㷨

int main() {

	char fname[256];
	cout << "�����������ݵ��ļ����� ";
	cin >> fname;
	cout << endl;
	ifstream infile;
	infile.open(fname, ios::in);
	if (!infile) {
		cout << "���ܴ�������ļ�" << fname << endl;
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

	//����ļ��е�Ԫ����Ϣ
	for (vector<Tuple>::size_type ix = 0; ix != tuples.size(); ++ix)
		cout << "(" << tuples[ix].attr1 << "," << tuples[ix].attr2 << ")" << "    ";
	cout << endl;
	KMeans(tuples);
	return 0;
}

float getDistance(Tuple t1, Tuple t2) {
	return sqrt((t1.attr1 - t2.attr1) * (t1.attr1 - t2.attr1) + (t1.attr2 - t2.attr2) * (t1.attr2 - t2.attr2));
}
int clusterOfTuple(Tuple means[], Tuple tuple) { //�������ģ�������ǰԪ�������ĸ���
	float dist = getDistance(means[0], tuple);
	float tmp;
	int label = 0;//��ʾ������һ����
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
Tuple getMeans(vector<Tuple> cluster) { //��õ�ǰ�صľ�ֵ

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
	//Ĭ��һ��ʼ��ǰK��Ԫ���ֵ��Ϊk���ص����ģ���ֵ��
	for (i = 0; i < K; i++) {
		means[i].attr1 = tuples[i].attr1;
		means[i].attr2 = tuples[i].attr2;
	}
	int lable = 0;
	//����Ĭ�ϵ����ĸ��ظ�ֵ
	for (i = 0; i != tuples.size(); ++i) {
		lable = clusterOfTuple(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
	//����տ�ʼ�Ĵ�
	for (lable = 0; lable < 3; lable++) {
		cout << "��" << lable + 1 << "���أ�" << endl;
		vector<Tuple> t = clusters[lable];
		for (i = 0; i < t.size(); i++)
		{
			cout << "(" << t[i].attr1 << "," << t[i].attr2 << ")" << "   ";
		}
		cout << endl;
	}
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //���¾ɺ���ֵ����1��׼����ֵ���������Ա仯ʱ���㷨��ֹ
	{

		for (i = 0; i < K; i++) //����ÿ���ص����ĵ�
			means[i] = getMeans(clusters[i]);
		oldVar = newVar;
		newVar = getVar(clusters, means); //�����µ�׼����ֵ
		for (i = 0; i < K; i++) //���ÿ����
			clusters[i].clear();
		//�����µ����Ļ���µĴ�
		for (i = 0; i != tuples.size(); ++i) {
			lable = clusterOfTuple(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
		//�����ǰ�Ĵ�
		for (lable = 0; lable < 3; lable++) {
			cout << "��" << lable + 1 << "���أ�" << endl;
			vector<Tuple> t = clusters[lable];
			for (i = 0; i < t.size(); i++)
				cout << "(" << t[i].attr1 << "," << t[i].attr2 << ")" << "   ";
			cout << endl;
		}
	}
}