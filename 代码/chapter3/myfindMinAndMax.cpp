#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
	system("color F0");  //更改输出界面颜色
	float a[12] = { 1, 2, 3, 4, 5, 10, 6, 7, 8, 9, 10, 0 };
	Mat img = Mat(3, 4, CV_32FC1, a);  //单通道矩阵
	Mat imgs = Mat(2, 3, CV_32FC2, a);  //多通道矩阵
	double minVal, maxVal;  //用于存放矩阵中的最大值和最小值
	Point minIdx, maxIdx;  ////用于存放矩阵中的最大值和最小值在矩阵中的位置

	/*寻找单通道矩阵中的最值*/
	minMaxLoc(img, &minVal, &maxVal, &minIdx, &maxIdx);
	cout << "img中最大值是：" << maxVal << "  " << "在矩阵中的位置:" << maxIdx << endl;
	cout << "img中最小值是：" << minVal << "  " << "在矩阵中的位置:" << minIdx << endl;

	/*寻找多通道矩阵中的最值*/
	Mat imgs_re = imgs.reshape(1, 4);  //将多通道矩阵变成单通道矩阵
	minMaxLoc(imgs_re, &minVal, &maxVal, &minIdx, &maxIdx);
	cout << "imgs中最大值是：" << maxVal << "  " << "在矩阵中的位置:" << maxIdx << endl;
	cout << "imgs中最小值是：" << minVal << "  " << "在矩阵中的位置:" << minIdx << endl;
	return 0;
}