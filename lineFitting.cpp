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

int sumOf_x[N]; //x的累加和
int sumOf_y[N]; //y的累加和
int sumOf_xy[N]; //x*y的累加和
int sumOf_xx[N]; //x*x的累加和

double slope[N][N]; //斜率
double intercept[N][N]; //截距
double E[N][N]; //误差

double OPT[N]; //最优解的值
double opt[N]; //部分最优解的值

int main()
{
	int n, c;
	int i, j, k;
	cin >> n >> c;
	for (i = 1; i <= n; i++)
		cin >> points[i].x >> points[i].y;
	sort(points + 1, points + n + 1);

	for (j = 1; j <= n; j++) {
		//计算a和b要用到的四个和
		sumOf_x[j] = sumOf_x[j - 1] + points[j].x;
		sumOf_y[j] = sumOf_y[j - 1] + points[j].y;
		sumOf_xy[j] = sumOf_xy[j - 1] + points[j].x * points[j].y;
		sumOf_xx[j] = sumOf_xx[j - 1] + points[j].x * points[j].x;
		for (i = 1; i <= j; i++) {
			int interval = j - i + 1; //子问题的点的个数
			//求解子问题要用到的四个和
			long long subsumOf_x = sumOf_x[j] - sumOf_x[i - 1];
			long long subsumOf_y = sumOf_y[j] - sumOf_y[i - 1];
			long long subsumOf_xy = sumOf_xy[j] - sumOf_xy[i - 1];
			long long subsumOf_xx = sumOf_xx[j] - sumOf_xx[i - 1];

			double molecular = interval * subsumOf_xy - subsumOf_x * subsumOf_y; //斜率的分子
			double denominator = interval * subsumOf_xx - pow(subsumOf_x, 2); //斜率的分母
			slope[i][j] = molecular / (1.0 * denominator);  //斜率

			intercept[i][j] = (subsumOf_y - slope[i][j] * subsumOf_x) / (1.0 * interval);  //截距
			for (k = i; k <= j; k++) {
				double temp = points[k].y - slope[i][j] * points[k].x - intercept[i][j]; //计算误差
				E[i][j] += pow(temp, 2);
			}
		}
	}

	//迭代计算OPT
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

	cout << fixed << setprecision(2) << OPT[n]; //OPT[n]就是最优解
	return 0;
}