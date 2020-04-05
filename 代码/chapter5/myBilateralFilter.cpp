#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	//读取两张含有人脸的图像
	Mat img1 = imread("img1.png", IMREAD_ANYCOLOR);
	Mat img2 = imread("img2.png", IMREAD_ANYCOLOR);
	if (img1.empty() || img2.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;		return -1;
	}
	Mat result1, result2, result3, result4;
	//验证不同滤波器直径的滤波效果
	bilateralFilter(img1, result1, 9, 50, 25 / 2);
	bilateralFilter(img1, result2, 25, 50, 25 / 2);
	//验证不同标准差值的滤波效果
	bilateralFilter(img2, result3, 9, 9, 9);
	bilateralFilter(img2, result4, 9, 200, 200);
	//显示原图
	imshow("img1", img1);
	imshow("img2", img2);
	//不同直径滤波结果
	imshow("result1", result1);
	imshow("result2", result2);
	//不同标准差值滤波结果
	imshow("result3 ", result3);
	imshow("result4", result4);
	waitKey(0);
	return 0;
}