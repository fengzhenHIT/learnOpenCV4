#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	system("color F0");  //更改输出界面颜色
	Mat img = imread("approx.png");
	if (img.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;
		return -1;
	}
	// 边缘检测
	Mat canny;
	Canny(img, canny, 80, 160, 3, false);
	//膨胀运算
	Mat kernel = getStructuringElement(0, Size(3, 3));
	dilate(canny, canny, kernel);

	// 轮廓发现
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(canny, contours, hierarchy, 0, 2, Point());

	//创建图像中的一个像素点并绘制圆形
	Point point = Point(250, 200);
	circle(img, point, 2, Scalar(0, 0, 255), 2, 8, 0);

	//多边形
	for (int t = 0; t < contours.size(); t++)
	{
		//用最小外接矩形求取轮廓中心
		RotatedRect rrect = minAreaRect(contours[t]);
		Point2f center = rrect.center;
		circle(img, center, 2, Scalar(0, 255, 0), 2, 8, 0);  //绘制圆心点
															 //轮廓外部点距离轮廓的距离
		double dis = pointPolygonTest(contours[t], point, true);
		//轮廓内部点距离轮廓的距离
		double dis2 = pointPolygonTest(contours[t], center, true);
		//输出点结果
		cout << "外部点距离轮廓距离：" << dis << endl;
		cout << "内部点距离轮廓距离：" << dis2 << endl;
	}
	return 0;
}