#include <iostream>
#include <xmmintrin.h> //SSE
#include <windows.h>
#include <cmath>
using namespace std;

const int N = 4000 ; //数据量，为了SIMD划分的更加整齐，采用16的幂
const int D = 2; //标签数
const int K = 3; //聚类数

void Kmeans_SIMD();

float** DATA;
float** data_align;
float centroids[K][D]; // 聚类中心
int cluster[N];        // 各数据所属类别
long long int head, tail, freq;

int main()
{
    //初始化数据和聚类中心
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

    cout << "消耗时间：" << (tail - head) * 1000.0*1000 / freq << "ms";
    return 0;
}
void Kmeans_SIMD()
{
    float min_distance[N] = { 0.0 };
    for (int j = 0; j < K; j++)
    {
        // 各个点到各个聚类中心的距离
        float distanceToCent[N] = { 0.0 };
        for (int d = 0; d < D; d++)
        {
            for (int i = 0; i < N - N % 4; i += 4)
            {
                // 取出原始积累的距离
                __m128 distance = _mm_loadu_ps(&distanceToCent[i]);
                // 构造质心的某一维度数据
                float tmp_centroid_d[4] = { centroids[j][d] };
                __m128 centroid_d = _mm_loadu_ps(tmp_centroid_d);
                // 一次取出四个元素的某一维度数据
                __m128 data_d = _mm_load_ps(&data_align[d][i]);
                // 对每一数据该维度计算差值
                __m128 delta = _mm_sub_ps(data_d, centroid_d);
                // 对每一数据该维度累加距离
                distance = _mm_add_ps(distance, _mm_mul_ps(delta, delta));
                // 存回
                _mm_storeu_ps(&distanceToCent[i], distance);
            }
        }
        // 判断当前的每一个数据到该质心的距离是否是最小的
        for (int i = 0; i < N; i++)
            if (distanceToCent[i] < min_distance[i])
                min_distance[i] = distanceToCent[i], cluster[i] = j;
    }
}

