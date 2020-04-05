#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	//读取图像，黑白图像边缘检测结果较为明显
	Mat img = imread("equalLena.png", IMREAD_ANYDEPTH);
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat resultHigh, resultLow, resultG;

	//大阈值检测图像边缘
	Canny(img, resultHigh, 100, 200, 3);

	//小阈值检测图像边缘
	Canny(img, resultLow, 20, 40, 3);

	//高斯模糊后检测图像边缘
	GaussianBlur(img, resultG, Size(3, 3), 5);
	Canny(resultG, resultG, 100, 200, 3);

	//显示图像
	imshow("resultHigh", resultHigh);
	imshow("resultLow", resultLow);
	imshow("resultG", resultG);
	waitKey(0);
	return 0;
}
