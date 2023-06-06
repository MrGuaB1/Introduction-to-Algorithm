#include <iostream>
#include <xmmintrin.h> //SSE
#include <windows.h>
#include <cmath>
using namespace std;

const int N = 4000 ; //��������Ϊ��SIMD���ֵĸ������룬����16����
const int D = 2; //��ǩ��
const int K = 3; //������

void Kmeans_SIMD();

float** DATA;
float** data_align;
float centroids[K][D]; // ��������
int cluster[N];        // �������������
long long int head, tail, freq;

int main()
{
    //��ʼ�����ݺ;�������
    DATA = new float* [D];
    data_align = (float**)malloc(sizeof(float*) * D);
    for (int i = 0; i < D; i++)
        DATA[i] = new float[N + 1], data_align[i] = (float*)_aligned_malloc(sizeof(float) * N, 64);
    for (int i = 0; i < D; i++)
        for (int j = 0; j < N; j++)
            DATA[i][j + 1] = rand() * 1.0 / RAND_MAX * 100, data_align[i][j] = rand() * 1.0 / RAND_MAX * 100;
    for (int i = 0; i < K; i++)
        for (int j = 0; j < D; j++)
            centroids[i][j] = rand() * 1.0 / RAND_MAX * 100;

    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    Kmeans_SIMD();   
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);

    cout << "����ʱ�䣺" << (tail - head) * 1000.0*1000 / freq << "ms";
    return 0;
}
void Kmeans_SIMD()
{
    float min_distance[N] = { 0.0 };
    for (int j = 0; j < K; j++)
    {
        // �����㵽�����������ĵľ���
        float distanceToCent[N] = { 0.0 };
        for (int d = 0; d < D; d++)
        {
            for (int i = 0; i < N - N % 4; i += 4)
            {
                // ȡ��ԭʼ���۵ľ���
                __m128 distance = _mm_loadu_ps(&distanceToCent[i]);
                // �������ĵ�ĳһά������
                float tmp_centroid_d[4] = { centroids[j][d] };
                __m128 centroid_d = _mm_loadu_ps(tmp_centroid_d);
                // һ��ȡ���ĸ�Ԫ�ص�ĳһά������
                __m128 data_d = _mm_load_ps(&data_align[d][i]);
                // ��ÿһ���ݸ�ά�ȼ����ֵ
                __m128 delta = _mm_sub_ps(data_d, centroid_d);
                // ��ÿһ���ݸ�ά���ۼӾ���
                distance = _mm_add_ps(distance, _mm_mul_ps(delta, delta));
                // ���
                _mm_storeu_ps(&distanceToCent[i], distance);
            }
        }
        // �жϵ�ǰ��ÿһ�����ݵ������ĵľ����Ƿ�����С��
        for (int i = 0; i < N; i++)
            if (distanceToCent[i] < min_distance[i])
                min_distance[i] = distanceToCent[i], cluster[i] = j;
    }
}

