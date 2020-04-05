#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
	Mat img1 = imread("left01.jpg");
	Mat img2 = imread("circle.png");
	if (!(img1.data && img2.data))
	{
		cout << "读取图像错误，请确认图像文件是否正确" << endl;
		return -1;
	}
	Mat gray1, gray2;
	cvtColor(img1, gray1, COLOR_BGR2GRAY);
	cvtColor(img2, gray2, COLOR_BGR2GRAY);

	//定义数目尺寸
	Size board_size1 = Size(9, 6);   //方格标定板内角点数目（行，列）
	Size board_size2 = Size(7, 7);   //圆形标定板圆心数目（行，列）

	//检测角点
	vector<Point2f> img1_points, img2_points;  
	findChessboardCorners(gray1, board_size1, img1_points);  //计算方格标定板角点
	findCirclesGrid(gray2, board_size2, img2_points);  //计算圆形标定板检点

	//细化角点坐标
	find4QuadCornerSubpix(gray1, img1_points, Size(5, 5));  //细化方格标定板角点坐标
	find4QuadCornerSubpix(gray2, img2_points, Size(5, 5));  //细化圆形标定板角点坐标

	//绘制角点检测结果
	drawChessboardCorners(img1, board_size1, img1_points, true);
	drawChessboardCorners(img2, board_size2, img2_points, true);

	//显示结果
	imshow("方形标定板角点检测结果", img1);
	imshow("圆形标定板角点检测结果", img2);
	waitKey(0);
	return 0;
}