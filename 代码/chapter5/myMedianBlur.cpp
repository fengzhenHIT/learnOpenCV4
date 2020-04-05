#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat gray = imread("equalLena_salt.png", IMREAD_ANYCOLOR);
	Mat img = imread("lena_salt.png", IMREAD_ANYCOLOR);
	if (gray.empty() || img.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;		return -1;
	}
	Mat imgResult3, grayResult3, imgResult9, grayResult9;
	//分别对含有椒盐噪声的彩色和灰度图像进行滤波，滤波模板为3×3
	medianBlur(img, imgResult3, 3);
	medianBlur(gray, grayResult3, 3);
	//加大滤波模板，图像滤波结果会变模糊
	medianBlur(img, imgResult9, 9);
	medianBlur(gray, grayResult9, 9);
	//显示滤波处理结果
	imshow("img", img);
	imshow("gray", gray);
	imshow("imgResult3", imgResult3);
	imshow("grayResult3", grayResult3);
	imshow("imgResult9", imgResult9);
	imshow("grayResult9", grayResult9);
	waitKey(0);
	return 0;
}
