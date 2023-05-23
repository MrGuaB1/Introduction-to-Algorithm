#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <vector>
#include <limits>
using namespace std;

const int N = 1000;
const double INF = std::numeric_limits<double>::infinity();

struct Point {
	int x, y;
	bool operator < (const Point& p) const {
		return x < p.x;
	}
} points[N];

int sumOf_x[N]; //x���ۼӺ�
int sumOf_y[N]; //y���ۼӺ�
int sumOf_xy[N]; //x*y���ۼӺ�
int sumOf_xx[N]; //x*x���ۼӺ�

double slope[N][N]; //б��
double intercept[N][N]; //�ؾ�
double E[N][N]; //���

double OPT[N]; //���Ž��ֵ
double opt[N]; //�������Ž��ֵ

int main()
{
	int n, c;
	int i, j, k;
	cin >> n >> c;
	for (i = 1; i <= n; i++)
		cin >> points[i].x >> points[i].y;
	sort(points + 1, points + n + 1);

	for (j = 1; j <= n; j++) {
		//����a��bҪ�õ����ĸ���
		sumOf_x[j] = sumOf_x[j - 1] + points[j].x;
		sumOf_y[j] = sumOf_y[j - 1] + points[j].y;
		sumOf_xy[j] = sumOf_xy[j - 1] + points[j].x * points[j].y;
		sumOf_xx[j] = sumOf_xx[j - 1] + points[j].x * points[j].x;
		for (i = 1; i <= j; i++) {
			int interval = j - i + 1; //������ĵ�ĸ���
			//���������Ҫ�õ����ĸ���
			long long subsumOf_x = sumOf_x[j] - sumOf_x[i - 1];
			long long subsumOf_y = sumOf_y[j] - sumOf_y[i - 1];
			long long subsumOf_xy = sumOf_xy[j] - sumOf_xy[i - 1];
			long long subsumOf_xx = sumOf_xx[j] - sumOf_xx[i - 1];

			double molecular = interval * subsumOf_xy - subsumOf_x * subsumOf_y; //б�ʵķ���
			double denominator = interval * subsumOf_xx - pow(subsumOf_x, 2); //б�ʵķ�ĸ
			slope[i][j] = molecular / (1.0 * denominator);  //б��

			intercept[i][j] = (subsumOf_y - slope[i][j] * subsumOf_x) / (1.0 * interval);  //�ؾ�
			for (k = i; k <= j; k++) {
				double temp = points[k].y - slope[i][j] * points[k].x - intercept[i][j]; //�������
				E[i][j] += pow(temp, 2);
			}
		}
	}

	//��������OPT
	double minError;
	for (j = 1; j <= n; j++) {
		for (i = 1, k = 0, minError = INF; i <= j; i++) {
			double temp = E[i][j] + OPT[i - 1];
			if (temp < minError) {
				minError = temp;
				k = i;
			}
		}
		opt[j] = k;
		OPT[j] = minError + c;
	}

	cout << fixed << setprecision(2) << OPT[n]; //OPT[n]�������Ž�
	return 0;
}