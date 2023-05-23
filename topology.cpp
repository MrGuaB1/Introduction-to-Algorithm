#include<iostream>
#include<stack>
using namespace std;

int vertex, edge;
int matrix[60][60];
int ans[60]; //��¼��������Ľ��
bool visited[60]; //DFS���ж��Ƿ񱻷���
int path[60]; //��¼ǰ���ڵ㣬�һ�
bool unloop = true; //�ж���Ȧ
bool only = true; //ֻ���һ���⣬�ҵ�һ�������Ͻ���

void DFS(int n);
void DFS_all();

int main()
{
	cin >> vertex >> edge;
	int remain_vertex = vertex;
	bool self = false;
	int selfv;
	for (int i = 0; i < edge; i++)
	{
		int v1, v2;
		cin >> v1 >> v2;
		if (v1 == v2) 
		{
			self = true;
			selfv = v1;
		}
		matrix[v1][v2] = 1;
	}
	if (self) { //���Ի�·��ֱ���������Ի�·����������
		cout << "NO" << endl;
		cout << selfv << "," << selfv;
		return 0;
	}
//�ж����ͼ�Ƿ���ڻ�·��
	
	DFS_all();	
	if (!unloop)
		return 0;

//���ͼ���������������У�
	int num = 0;
	cout << "YES" << endl;
	while (remain_vertex != 0)
	{
		for (int j = 1; j <= vertex; j++)
		{
			bool flag = true;
			for (int i = 1; i <= vertex; i++)			
				if (matrix[i][j] != 0)
					flag = false;		
			if (flag == true)
			{				
				num++;
				cout << j;
				if (num < vertex) cout << ",";
				if (num == vertex)
					return 0;
				remain_vertex -= 1;
				for (int k = 1; k <= vertex; k++)
				{
					matrix[j][k] = 0;
					matrix[k][j] = 0;
				}
			}
		}
	}

	return 0;
}
void DFS(int v) //�Ӽ��Žڵ㿪ʼ����
{
	stack<int> Stack; //����ջ
	Stack.push(v);
	while (!Stack.empty())
	{
		int temp = Stack.top();
		Stack.pop();
		if (!visited[temp])
			visited[temp] = true;				
		for (int i = 1; i <= vertex; i++) {
			if (matrix[temp][i] == 1 ) {
				int ago = path[i];
				path[i] = temp;
				if (!visited[i])				
					Stack.push(i);					
				else if(ago==0)
				{		
					unloop = false;
					cout << "NO" << endl;
					stack<int> loop;
					loop.push(temp);
					while (true)
					{
						loop.push(path[temp]);
						if (path[temp] == i)
						{
							while (!loop.empty()) {
								cout << loop.top() << ",";
								loop.pop();
							}
							cout << i;		
							only = false;
							return;
						}
						temp = path[temp];
					}
				}
			}
		}		
	}
}
void DFS_all()
{
	for (int v = 1; v <= vertex; v++)	
		if (!visited[v]&&only)
			DFS(v);
}