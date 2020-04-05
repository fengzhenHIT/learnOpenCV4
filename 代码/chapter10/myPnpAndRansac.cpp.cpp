#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
	//读取所有图像
	Mat img = imread("left01.jpg");
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	vector<Point2f> imgPoints;
	Size boardSize = Size(9, 6);
	findChessboardCorners(gray, boardSize, imgPoints);  //计算方格标定板角点
	find4QuadCornerSubpix(gray, imgPoints, Size(5, 5));  //细化方格标定板角点坐标

	//生成棋盘格每个内角点的空间三维坐标
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

	//输入前文计算得到的内参矩阵和畸变矩阵
	Mat cameraMatrix = (Mat_<float>(3, 3) << 532.016297, 0, 332.172519,
		                                     0, 531.565159, 233.388075,
		                                     0, 0, 1);
	Mat distCoeffs = (Mat_<float>(1, 5) << -0.285188, 0.080097, 0.001274,
                                           - 0.002415, 0.106579);
	
	//用PnP算法计算旋转和平移量
	Mat rvec, tvec;
	solvePnP(PointSets, imgPoints, cameraMatrix, distCoeffs, rvec, tvec);
	cout << "世界坐标系变换到相机坐标系的旋转向量：" << rvec << endl;
	//旋转向量转换旋转矩阵
	Mat R;
	Rodrigues(rvec, R);
	cout << "旋转向量转换成旋转矩阵：" << endl << R << endl;

	//用PnP+Ransac算法计算旋转向量和平移向量
	Mat rvecRansac, tvecRansac;
	solvePnPRansac(PointSets, imgPoints, cameraMatrix, distCoeffs, rvecRansac, tvecRansac);
	Mat RRansac;
	Rodrigues(rvecRansac, RRansac);
	cout << "旋转向量转换成旋转矩阵：" << endl << RRansac << endl;
	waitKey(0);
	return 0;
}