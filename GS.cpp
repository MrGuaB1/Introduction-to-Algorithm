#include<iostream>
using namespace std;
int Find(int* arr,int n,int elem) 
{
	for (int k = 0; k < n; k++)
		if (arr[k] == elem)
			return k;
	return -1;
}
int main()
{
	int n; cin >> n;
	int** male = new int* [n];
	int** female = new int* [n];
	int* man_ans = new int[n];
	int* woman_ans = new int[n];

	for (int i = 0; i < n; i++) {
		male[i] = new int[n];
		man_ans[i] = 0;
		female[i] = new int[n];
		woman_ans[i] = 0;
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> male[i][j];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> female[i][j];

	for (int k = 0; k < n * (n+1); k++) 
	{
		int i = k % n;
		if (man_ans[i] == 0) //如果男人单身，他就要发出邀请
		{
			for (int j = 0; j < n; j++)
			{
				if (male[i][j] != 0) //没被这个女人拒绝
				{
					if (woman_ans[male[i][j] - 1] == 0) { //这个女人也单身，则直接配对
						man_ans[i] = male[i][j];
						woman_ans[male[i][j] - 1] = i + 1;
						break;
					}
					else //女人不单身，则女人就要做出选择
					{
						int index1 = Find(female[male[i][j] - 1], n, i + 1); //当前发出邀请的男生的索引
						int index2 = Find(female[male[i][j] - 1], n, woman_ans[male[i][j] - 1]); //之前配对的男生的索引
						if (index1 < index2) //女人找到了更好的对象
						{
							int row = woman_ans[male[i][j] - 1] - 1;
							for (int m = 0; m < n; m++) {
								if (male[row][m] == male[i][j]) {
									male[row][m] = 0; //标记被拒绝的男生该处为0								
								}
							}
							man_ans[woman_ans[male[i][j] - 1] - 1] = 0; //恢复单身
							woman_ans[male[i][j] - 1] = i + 1; //更换为当前男生
							man_ans[i] = male[i][j];
							break;
						}
						else { //被当前女人拒绝						
							male[i][j] = 0; //标记一下，下次再来	
							break;
						}
					}
				}
			}
		}		
	}
	for (int i = 0; i < n; i++)
	{
		cout << man_ans[i];
		if (i < n - 1) cout << " ";
	}

	delete[] man_ans;
	delete[] woman_ans;
	for (int i = 0; i < n; i++) {
		delete[]male[i];
		delete[]female[i];
	}
	delete[]male;
	delete[]female;

	return 0;
}