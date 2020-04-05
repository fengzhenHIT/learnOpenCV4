#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("hand.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	// 二值化
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 105, 255, THRESH_BINARY);

	//开运算消除细小区域
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(binary, binary, MORPH_OPEN, k);
	imshow("binary", binary);

	// 轮廓发现
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, 0, 2, Point());
	for (int n = 0; n < contours.size(); n++)
	{
		//计算凸包
		vector<Point> hull;
		convexHull(contours[n], hull);
		//绘制凸包
		for (int i = 0; i < hull.size(); i++)
		{
			//绘制凸包顶点
			circle(img, hull[i], 4, Scalar(255, 0, 0), 2, 8, 0);
			//连接凸包
			if (i == hull.size() - 1)
			{
				line(img, hull[i], hull[0], Scalar(0, 0, 255), 2, 8, 0);
				break;
			}
			line(img, hull[i], hull[i + 1], Scalar(0, 0, 255), 2, 8, 0);
		}
	}
	imshow("hull", img);
	waitKey(0);
	return 0;
}