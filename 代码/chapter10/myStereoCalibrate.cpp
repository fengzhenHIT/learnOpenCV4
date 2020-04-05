#include <opencv2\opencv.hpp>
#include <fstream> 
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

//检测棋盘格内角点在图像中坐标的函数
void getImgsPoints(vector<Mat> imgs, vector<vector<Point2f>> &Points, Size boardSize)
{
	for (int i = 0; i < imgs.size(); i++)
	{
		Mat img1 = imgs[i];
		Mat gray1;
		cvtColor(img1, gray1, COLOR_BGR2GRAY);
		vector<Point2f> img1_points;
		findChessboardCorners(gray1, boardSize, img1_points);  //计算方格标定板角点
		find4QuadCornerSubpix(gray1, img1_points, Size(5, 5));  //细化方格标定板角点坐标
		Points.push_back(img1_points);
	}
}

int main()
{
	//读取所有图像
	vector<Mat> imgLs;
	vector<Mat> imgRs;
	string imgLName;
	string imgRName;
	ifstream finL("steroCalibDataL.txt");
	ifstream finR("steroCalibDataR.txt");
	while (getline(finL, imgLName) && getline(finR, imgRName))
	{
		Mat imgL = imread(imgLName);
		Mat imgR = imread(imgRName);
		if (!imgL.data && !imgR.data)
		{
			cout << "请确是否输入正确的图像文件" << endl;
			return -1;
		}
		imgLs.push_back(imgL);
		imgRs.push_back(imgR);
	}

	//提取棋盘格内角点在两个相机图像中的坐标
	Size board_size = Size(9, 6);  //方格标定板内角点数目（行，列）
	vector<vector<Point2f>> imgLsPoints;
	vector<vector<Point2f>> imgRsPoints;
	getImgsPoints(imgLs, imgLsPoints, board_size);  //调用子函数
	getImgsPoints(imgRs, imgRsPoints, board_size);  //调用子函数


													//生成棋盘格每个内角点的空间三维坐标
	Size squareSize = Size(10, 10);  //棋盘格每个方格的真实尺寸
	vector<vector<Point3f>> objectPoints;
	for (int i = 0; i < imgLsPoints.size(); i++)
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

	//图像尺寸
	Size imageSize;
	imageSize.width = imgLs[0].cols;
	imageSize.height = imgLs[0].rows;

	Mat Matrix1, dist1, Matrix2, dist2, rvecs, tvecs;
	calibrateCamera(objectPoints, imgLsPoints, imageSize, Matrix1, dist1, rvecs, tvecs, 0);
	calibrateCamera(objectPoints, imgRsPoints, imageSize, Matrix2, dist2, rvecs, tvecs, 0);
	
	//进行标定
	Mat R, T, E, F;  //旋转矩阵、平移向量、本征矩阵、基本矩阵
	stereoCalibrate(objectPoints, imgLsPoints, imgRsPoints, Matrix1, dist1, Matrix2, dist2, imageSize, R, T, E, F, CALIB_USE_INTRINSIC_GUESS);

	cout << "两个相机坐标系的旋转矩阵：" << endl << R << endl;
	cout << "两个相机坐标系的平移向量：" << endl << T << endl;
	waitKey(0);
	return 0;
}