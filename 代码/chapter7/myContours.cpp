#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	system("color F0");  //更改输出界面颜色
	Mat img = imread("keys.jpg");
	if (img.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;
		return -1;
	}
	imshow("原图", img);
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);  //转化成灰度图
	GaussianBlur(gray, gray, Size(13, 13), 4, 4);  //平滑滤波
	threshold(gray, binary, 170, 255, THRESH_BINARY | THRESH_OTSU);  //自适应二值化

																	 // 轮廓发现与绘制
	vector<vector<Point>> contours;  //轮廓
	vector<Vec4i> hierarchy;  //存放轮廓结构变量
	findContours(binary, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	//绘制轮廓
	for (int t = 0; t < contours.size(); t++)
	{
		drawContours(img, contours, t, Scalar(0, 0, 255), 2, 8);
	}
	//输出轮廓结构描述子
	for (int i = 0; i < hierarchy.size(); i++)
	{
		cout << hierarchy[i] << endl;
	}

	//显示结果
	imshow("轮廓检测结果", img);
	waitKey(0);
	return 0;
}