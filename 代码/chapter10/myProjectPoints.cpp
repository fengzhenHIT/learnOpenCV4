#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
	/**********本程序中用到的图像是代码清单10-10中相机标定时的第一张图像**********/
	/***************各项参数都是标定时得到的*****************/

	//输入前文计算得到的内参矩阵和畸变矩阵
	Mat cameraMatrix = (Mat_<float>(3, 3) << 532.016297, 0, 332.172519,
		                                     0, 531.565159, 233.388075,
		                                     0, 0, 1);
	Mat distCoeffs = (Mat_<float>(1, 5) << -0.285188, 0.080097, 0.001274,
                                           - 0.002415, 0.106579);
	//代码清单10-10中计算的第一张图像相机坐标系与世界坐标系之间的关系
	Mat rvec = (Mat_<float>(1, 3) <<-1.977853, -2.002220, 0.130029);
	Mat tvec = (Mat_<float>(1, 3) << -26.88155,-42.79936, 159.19703);

	//生成第一张图像中内角点的三维世界坐标
	Size boardSize = Size(9, 6);
	Size squareSize = Size(10, 10);  //棋盘格每个方格的真实尺寸
	vector<Point3f> PointSets;
	for (int j = 0; j < boardSize.height; j++)
	{
		for (int k = 0; k < boardSize.width; k++)
		{
			Point3f realPoint;
			// 假设标定板为世界坐标系的z平面，即z=0
			realPoint.x = j*squareSize.width;
			realPoint.y = k*squareSize.height;
			realPoint.z = 0;
			PointSets.push_back(realPoint);
		}
	}
	
	//根据三维坐标和相机与世界坐标系时间的关系估计内角点像素坐标
	vector<Point2f> imagePoints;
	projectPoints(PointSets, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);


	/***********计算图像中内角点的真实坐标误差******************/
	Mat img = imread("left01.jpg");
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	vector<Point2f> imgPoints;
	findChessboardCorners(gray, boardSize, imgPoints);  //计算方格标定板角点
	find4QuadCornerSubpix(gray, imgPoints, Size(5, 5));  //细化方格标定板角点坐标

	//计算估计值和图像中计算的真实时之间的平均误差
	float e = 0;
	for (int i = 0; i < imagePoints.size(); i++)
	{
		float eX = pow(imagePoints[i].x - imgPoints[i].x, 2);
		float eY = pow(imagePoints[i].y - imgPoints[i].y, 2);
		e = e + sqrt(eX + eY);
	}
	e = e / imagePoints.size();
	cout << "估计坐标与真实坐标之间的误差" << e << endl;
	waitKey(0);
	return 0;
}