#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat equalLena = imread("equalLena.png", IMREAD_ANYDEPTH);  //用于方框滤波的图像
	if (equalLena.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	//验证方框滤波算法的数据矩阵
	float points[25] = { 1,2,3,4,5,
		6,7,8,9,10,
		11,12,13,14,15,
		16,17,18,19,20,
		21,22,23,24,25 };
	Mat data(5, 5, CV_32FC1, points);
	//将CV_8U类型转换成CV_32F类型
	Mat equalLena_32F;
	equalLena.convertTo(equalLena_32F, CV_32F, 1.0 / 255);
	Mat resultNorm, result, dataSqrNorm, dataSqr, equalLena_32FSqr;
	//方框滤波boxFilter()和sqrBoxFilter()
	boxFilter(equalLena, resultNorm, -1, Size(3, 3), Point(-1, -1), true);  //进行归一化
	boxFilter(equalLena, result, -1, Size(3, 3), Point(-1, -1), false);  //不进行归一化
	sqrBoxFilter(data, dataSqrNorm, -1, Size(3, 3), Point(-1, -1),
		true, BORDER_CONSTANT);  //进行归一化
	sqrBoxFilter(data, dataSqr, -1, Size(3, 3), Point(-1, -1),
		false, BORDER_CONSTANT);  //不进行归一化
	sqrBoxFilter(equalLena_32F, equalLena_32FSqr, -1, Size(3, 3), Point(-1, -1),
		true, BORDER_CONSTANT);
	//显示处理结果
	imshow("resultNorm", resultNorm);
	imshow("result", result);
	imshow("equalLena_32FSqr", equalLena_32FSqr);
	waitKey(0);
	return 0;
}