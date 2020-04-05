#include <opencv2\opencv.hpp>
#include <iostream>
#include <fstream> 
#include <vector>

using namespace std;
using namespace cv;

//使用initUndistortRectifyMap()函数和remap()函数校正图像
void initUndistAndRemap(vector<Mat> imgs,  //所有原图像向量
	                    Mat cameraMatrix,  //计算得到的相机内参
	                    Mat distCoeffs,    //计算得到的相机畸变系数
	                    Size imageSize,    //图像的尺寸
	                    vector<Mat> &undistImgs)  //校正后的输出图像
{
	//计算映射坐标矩阵
	Mat R = Mat::eye(3, 3, CV_32F);
	Mat mapx = Mat(imageSize, CV_32FC1);
	Mat mapy = Mat(imageSize, CV_32FC1);
	initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, imageSize, CV_32FC1, mapx, mapy);

	//校正图像
	for (int i = 0; i < imgs.size(); i++)
	{
		Mat undistImg;
		remap(imgs[i], undistImg, mapx, mapy, INTER_LINEAR);
		undistImgs.push_back(undistImg);
	}
}

//用undistort()函数直接计算校正图像
void undist(vector<Mat> imgs,   //所有原图像向量
	        Mat cameraMatrix,   //计算得到的相机内参
	        Mat distCoeffs,     //计算得到的相机畸变系数
	        vector<Mat> &undistImgs)  //校正后的输出图像
{
	for (int i = 0; i < imgs.size(); i++)
	{
		Mat undistImg;
		undistort(imgs[i], undistImg, cameraMatrix, distCoeffs);
		undistImgs.push_back(undistImg);
	}
}

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

	//输入前文计算得到的内参矩阵
	Mat cameraMatrix = (Mat_<float>(3, 3) << 532.016297, 0, 332.172519, 
		                                     0, 531.565159, 233.388075, 
		                                     0, 0, 1);
	//输入前文计算得到的内参矩阵
	Mat distCoeffs = (Mat_<float>(1, 5) << -0.285188, 0.080097, 0.001274, -0.002415, 0.106579);
	vector<Mat> undistImgs;
	Size imageSize;
	imageSize.width = imgs[0].cols;
	imageSize.height = imgs[0].rows;

	//使用initUndistortRectifyMap()函数和remap()函数校正图像
	initUndistAndRemap(imgs,cameraMatrix,distCoeffs,imageSize,undistImgs);

	//用undistort()函数直接计算校正图像，下一行代码取消注释即可
	//undist(imgs, cameraMatrix, distCoeffs, undistImgs);

	//显示校正前后的图像
	for (int i = 0; i < imgs.size(); i++)
	{
		string windowNumber = to_string(i);
		imshow("未校正图像"+ windowNumber, imgs[i]);
		imshow("校正后图像"+ windowNumber, undistImgs[i]);
	}

	waitKey(0);
	return 0;
}