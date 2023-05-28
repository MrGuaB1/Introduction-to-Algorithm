#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    std::string imagePath = "idol.jpg"; // 图像文件的路径
    int clusterCount = 8; // 聚类数目

    Mat src = imread(imagePath, 1);
    if (src.empty())
    {
        std::cout << "Failed to read image file: " << imagePath << std::endl;
        return 0;
    }

    Mat samples(src.rows * src.cols, 3, CV_32F);
    for (int y = 0; y < src.rows; y++)
        for (int x = 0; x < src.cols; x++)
            for (int z = 0; z < 3; z++)
                samples.at<float>(y + x * src.rows, z) = src.at<Vec3b>(y, x)[z];

    Mat labels;
    int attempts = 5;
    Mat centers;
    kmeans(samples, clusterCount, labels, TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

    Mat new_image(src.size(), src.type());
    for (int y = 0; y < src.rows; y++)
        for (int x = 0; x < src.cols; x++)
        {
            int cluster_idx = labels.at<int>(y + x * src.rows, 0);
            new_image.at<Vec3b>(y, x)[0] = centers.at<float>(cluster_idx, 0);
            new_image.at<Vec3b>(y, x)[1] = centers.at<float>(cluster_idx, 1);
            new_image.at<Vec3b>(y, x)[2] = centers.at<float>(cluster_idx, 2);
        }

    imshow("clustered image", new_image);
    waitKey(0);
}
