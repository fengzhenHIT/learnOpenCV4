#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
	system("color F0");  //调整DOS界面颜色

	//设置两个三维坐标
	vector<Point3f> points3;
	points3.push_back(Point3f(3, 6,1.5));
	points3.push_back(Point3f(23, 32, 1));

	//非齐次坐标转齐次坐标
	Mat points4;
	convertPointsToHomogeneous(points3, points4);

	//齐次坐标转非齐次坐标
	vector<Point2f> points2;
	convertPointsFromHomogeneous(points3, points2);

	cout << "***********齐次坐标转非齐次坐标*************" << endl;
	for (int i = 0; i < points3.size(); i++)
	{
		cout << "齐次坐标：" << points3[i];
		cout<< "   非齐次坐标：" << points2[i] << endl;
	}

	cout << "***********非齐次坐标转齐次坐标*************" << endl;
	for (int i = 0; i < points3.size(); i++)
	{
		cout << "齐次坐标：" << points3[i];
		cout << "   非齐次坐标：" << points4.at<Vec4f>(i, 0) << endl;
	}

	waitKey(0);
	return 0;
}