#include<iostream>
#include<iomanip>
#include<cmath>
#include<utility> //use pair for py
#include<vector>
#include<algorithm>
using namespace std;
class Point
{
public:
	double x;
	double y;
	Point(double x = 0, double y = 0)
	{
		this->x = x;
		this->y = y;
	}
};

Point* points; //记录点的动态数组
vector<pair<int, double> > pairs; //记录纵坐标的前后索引
const int NORROW_BAND = 12;

double getDistance(Point p1, Point p2);
bool sortBy_x(Point& p1, Point& p2); //谓词，用于sort排序
bool sortBy_y(Point& p1, Point& p2);
bool sortBy_sec(pair<int, double>& p1, pair<int, double>& p2);
double closestDistance(int* rowIndex, int* colIndex, int len);

int main()
{
	int n; cin >> n;
	points = new Point[n];
	int* rowIndex = new int[n];
	int* colIndex = new int[n];
	for (int i = 0; i < n; i++)
	{
		cin >> points[i].x >> points[i].y;
		rowIndex[i] = i;
		pairs.push_back(make_pair(i, points[i].y));
	}

	sort(points, &points[n], sortBy_x);
	sort(pairs.begin(), pairs.end(), sortBy_sec);
	for (int i = 0; i < n; i++)
		colIndex[i] = pairs[i].first;
	double result = closestDistance(rowIndex, colIndex, n);
	cout << fixed << setprecision(2) << result;
	return 0;
}
bool sortBy_x(Point& p1, Point& p2) {
	return p1.x < p2.x;
}
bool sortBy_y(Point& p1, Point& p2) {
	return p1.y < p2.y;
}
bool sortBy_sec(pair<int, double>& p1, pair<int, double>& p2) {
	return p1.second < p2.second;
}
double getDistance(Point p1, Point p2) {
	return pow(1.0 * (p1.x - p2.x), 2) + pow(1.0 * (p1.y - p2.y), 2);
}
double closestDistance(int* rowIndex, int* colIndex, int len)
{
	double min = getDistance(points[rowIndex[0]], points[rowIndex[1]]);
	if (len <= 3)
	{
		for (int i = 0; i < len - 1; i++)
		{
			for (int j = i + 1; j < len; j++)
			{
				double temp = getDistance(points[rowIndex[i]], points[rowIndex[j]]);
				min = min < temp ? min : temp;
			}
		}
		return min;
	}

	//在左右两部分寻找各自最临近的点对：
	int len_Q = len / 2;
	int len_R = len - len_Q;
	int* Qx = new int[len_Q];
	int* Qy = new int[len_Q];
	int* Rx = new int[len_R];
	int* Ry = new int[len_R];

	for (int i = 0; i < len_Q; i++)
		Qx[i] = rowIndex[i];
	for (int i = len_Q; i < len; i++)
		Rx[i - len_Q] = rowIndex[i];

	int k1 = 0, k2 = 0;
	for (int i = 0; i < len; i++)
	{
		if (colIndex[i] < Rx[0])
			Qy[k1++] = colIndex[i];
		else
			Ry[k2++] = colIndex[i];
	}

	double left = closestDistance(Qx, Qy, len_Q); //求解左子块
	double right = closestDistance(Rx, Ry, len_R); //求解右子块
	double submin = left < right ? left : right; //得到子块最小值

	//求中间窄带的距离
	int* middles = new int[len];
	double middle = points[Qx[len_Q - 1]].x;
	int num = 0;
	for (int i = 0; i < len; i++)
		if (abs(points[colIndex[i]].x - middle) < submin)
			middles[num++] = colIndex[i];
	for (int i = 0; i < num - 1; i++)
		for (int j = i + 1; j <= i + NORROW_BAND && j < num; j++)
		{
			double temp = getDistance(points[middles[i]], points[middles[j]]);
			min = min < temp ? min : temp;
		}

	return min < submin ? min : submin;
}