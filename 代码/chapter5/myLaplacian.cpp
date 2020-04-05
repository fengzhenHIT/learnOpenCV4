#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	//读取图像，黑白图像边缘检测结果较为明显
	Mat img = imread("equalLena.png", IMREAD_ANYDEPTH);
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}	Mat result, result_g, result_G;

	//未滤波提取边缘
	Laplacian(img, result, CV_16S, 3, 1, 0);
	convertScaleAbs(result, result);

	//滤波后提取Laplacian边缘
	GaussianBlur(img, result_g, Size(3, 3), 5, 0);  //高斯滤波
	Laplacian(result_g, result_G, CV_16S, 3, 1, 0);
	convertScaleAbs(result_G, result_G);

	//显示图像
	imshow("result", result);
	imshow("result_G", result_G);
	waitKey(0);
	return 0;
}
