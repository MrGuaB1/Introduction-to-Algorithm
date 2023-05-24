#include <iostream>
#include <cmath>
#include <string>
using namespace std;
int getDistance(char a, char b) {
	return abs(int(a - b));
}
int getMin(int a, int b, int c) {
	return min(a, min(b, c));
}
int OPT[2001][2];
int main() {

	string A, B; int k;
	cin >> A >> B >> k;
	int lenA = A.size();
	int lenB = B.size();
	for (int i = 1; i <=lenA; i++)
		OPT[i][0] = i * k;

	for (int j = 1; j <= lenB; j++) {
		OPT[0][1] = j * k;
		for (int i = 1; i <= lenA; i++) {
			//OPT[i][j] = getMin(abs(A[i-1]-B[j-1]) + OPT[i - 1][j - 1], k + OPT[i - 1][j], k + OPT[i][j - 1]);
			OPT[i][1] = getMin(getDistance(A[i-1], B[j-1]) + OPT[i - 1][0], k + OPT[i - 1][1], k + OPT[i][0]);
		}
		//将OPT的第一列移到第0列为下一次迭代留出空间
		for (int i = 0; i <= lenA; i++)
			OPT[i][0] = OPT[i][1];
	}
	cout << OPT[lenA][1];
	return 0;
}