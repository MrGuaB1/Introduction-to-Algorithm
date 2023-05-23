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
		if (man_ans[i] == 0) //������˵�������Ҫ��������
		{
			for (int j = 0; j < n; j++)
			{
				if (male[i][j] != 0) //û�����Ů�˾ܾ�
				{
					if (woman_ans[male[i][j] - 1] == 0) { //���Ů��Ҳ������ֱ�����
						man_ans[i] = male[i][j];
						woman_ans[male[i][j] - 1] = i + 1;
						break;
					}
					else //Ů�˲�������Ů�˾�Ҫ����ѡ��
					{
						int index1 = Find(female[male[i][j] - 1], n, i + 1); //��ǰ�������������������
						int index2 = Find(female[male[i][j] - 1], n, woman_ans[male[i][j] - 1]); //֮ǰ��Ե�����������
						if (index1 < index2) //Ů���ҵ��˸��õĶ���
						{
							int row = woman_ans[male[i][j] - 1] - 1;
							for (int m = 0; m < n; m++) {
								if (male[row][m] == male[i][j]) {
									male[row][m] = 0; //��Ǳ��ܾ��������ô�Ϊ0								
								}
							}
							man_ans[woman_ans[male[i][j] - 1] - 1] = 0; //�ָ�����
							woman_ans[male[i][j] - 1] = i + 1; //����Ϊ��ǰ����
							man_ans[i] = male[i][j];
							break;
						}
						else { //����ǰŮ�˾ܾ�						
							male[i][j] = 0; //���һ�£��´�����	
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