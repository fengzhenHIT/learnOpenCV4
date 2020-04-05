#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	system("color F0");  //更改输出界面颜色
	//用四个点表示三角形轮廓
	vector<Point> contour;
	contour.push_back(Point2f(0, 0));
	contour.push_back(Point2f(10, 0));
	contour.push_back(Point2f(10, 10));
	contour.push_back(Point2f(5, 5));

	double length0 = arcLength(contour, true);
	double length1 = arcLength(contour, false);
	cout << "length0 =" << length0 << endl;
	cout << "length1 =" << length1 << endl;

	Mat img = imread("coins.jpg");
	if (img.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;
		return -1;
	}
	imshow("原图", img);
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);  //转化成灰度图
	GaussianBlur(gray, gray, Size(9, 9), 2, 2);  //平滑滤波
	threshold(gray, binary, 170, 255, THRESH_BINARY | THRESH_OTSU);  //自适应二值化

																	 // 轮廓检测
	vector<vector<Point>> contours;  //轮廓
	vector<Vec4i> hierarchy;  //存放轮廓结构变量
	findContours(binary, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());

	//输出轮廓长度
	for (int t = 0; t < contours.size(); t++)
	{
		double length2 = arcLength(contours[t], true);
		cout << "第" << t << "个轮廓长度=" << length2 << endl;
	}
	return 0;
}