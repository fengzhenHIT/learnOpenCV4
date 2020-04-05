#include <opencv2\opencv.hpp>
#include <fstream> 
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
	//读取所有图像
	vector<Mat> imgs;
	string imageName;
	ifstream fin("calibdata.txt");
	while (getline(fin,imageName))
	{
		Mat img = imread(imageName);
		imgs.push_back(img);
	}

	Size board_size = Size(9, 6);  //方格标定板内角点数目（行，列）
	vector<vector<Point2f>> imgsPoints;
	for (int i = 0; i < imgs.size(); i++)
	{
		Mat img1 = imgs[i];
		Mat gray1;
		cvtColor(img1, gray1, COLOR_BGR2GRAY);
		vector<Point2f> img1_points;
		findChessboardCorners(gray1, board_size, img1_points);  //计算方格标定板角点
		find4QuadCornerSubpix(gray1, img1_points, Size(5, 5));  //细化方格标定板角点坐标
		imgsPoints.push_back(img1_points);
	}

	//生成棋盘格每个内角点的空间三维坐标
	Size squareSize = Size(10, 10);  //棋盘格每个方格的真实尺寸
	vector<vector<Point3f>> objectPoints;
	for (int i = 0; i < imgsPoints.size(); i++)
	{
		vector<Point3f> tempPointSet;
		for (int j = 0; j < board_size.height; j++)
		{
			for (int k = 0; k < board_size.width; k++)
			{
				Point3f realPoint;
				// 假设标定板为世界坐标系的z平面，即z=0
				realPoint.x = j*squareSize.width;
				realPoint.y = k*squareSize.height;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		objectPoints.push_back(tempPointSet);
	}

	/* 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板 */
	vector<int> point_number;
	for (int i = 0; i<imgsPoints.size(); i++)
	{
		point_number.push_back(board_size.width*board_size.height);
	}

	//图像尺寸
	Size imageSize;
	imageSize.width = imgs[0].cols;
	imageSize.height = imgs[0].rows;

	Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0));  //摄像机内参数矩阵
	Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0));  //摄像机的5个畸变系数：k1,k2,p1,p2,k3
	vector<Mat> rvecs;  //每幅图像的旋转向量
	vector<Mat> tvecs;  //每张图像的平移量
	calibrateCamera(objectPoints, imgsPoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, 0);
	cout << "相机的内参矩阵=" << endl << cameraMatrix << endl;
	cout << "相机畸变系数" << distCoeffs << endl;
	waitKey(0);
	return 0;
}