#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat equalLena = imread("equalLena.png", IMREAD_ANYDEPTH);
	Mat equalLena_gauss = imread("equalLena_gauss.png", IMREAD_ANYDEPTH);
	Mat equalLena_salt = imread("equalLena_salt.png", IMREAD_ANYDEPTH);
	if (equalLena.empty() || equalLena_gauss.empty() || equalLena_salt.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;		return -1;
	}
	Mat result_5, result_9;  //存放不含噪声滤波结果，后面数字代表滤波器尺寸
	Mat result_5gauss, result_9gauss;  //存放含有高斯噪声滤波结果，后面数字代表滤波器尺寸
	Mat result_5salt, result_9salt;  ////存放含有椒盐噪声滤波结果，后面数字代表滤波器尺寸
	 //调用均值滤波函数blur()进行滤波
	GaussianBlur(equalLena, result_5, Size(5, 5), 10, 20);
	GaussianBlur(equalLena, result_9, Size(9, 9), 10, 20);
	GaussianBlur(equalLena_gauss, result_5gauss, Size(5, 5), 10, 20);
	GaussianBlur(equalLena_gauss, result_9gauss, Size(9, 9), 10, 20);
	GaussianBlur(equalLena_salt, result_5salt, Size(5, 5), 10, 20);
	GaussianBlur(equalLena_salt, result_9salt, Size(9, 9), 10, 20);
	//显示不含噪声图像
	imshow("equalLena ", equalLena);
	imshow("result_5", result_5);
	imshow("result_9", result_9);
	//显示含有高斯噪声图像
	imshow("equalLena_gauss", equalLena_gauss);
	imshow("result_5gauss", result_5gauss);
	imshow("result_9gauss", result_9gauss);
	//显示含有椒盐噪声图像
	imshow("equalLena_salt", equalLena_salt);
	imshow("result_5salt", result_5salt);
	imshow("result_9salt", result_9salt);
	waitKey(0);
	return 0;
}
