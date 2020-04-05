#include <opencv2\opencv.hpp>
#include <xfeatures2d.hpp>  //SURF特征点头文件
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;  
using namespace xfeatures2d;  //SURF特征点命名空间

int main()
{
	Mat img = imread("lena.png");
	if (!img.data)
	{
		cout << "读取图像错误，请确认图像文件是否正确" << endl;
		return -1;
	}

	//创建SURF特征点类变量
	Ptr<SURF> surf = SURF::create(500,  //关键点阈值
		                            4,  //4组金字塔
		                            3,  //每组金字塔有3层
		                         true,  //使用128维描述子
		                        false);  //计算关键点方向

	//计算SURF关键点
	vector<KeyPoint> Keypoints;
	surf->detect(img, Keypoints);  //确定关键点

	//计算SURF描述子
	Mat descriptions;
	surf->compute(img, Keypoints, descriptions);  //计算描述子
	
	//绘制特征点
	Mat imgAngel;
	img.copyTo(imgAngel);
	//绘制不含角度和大小的结果
	drawKeypoints(img, Keypoints, img,Scalar(255,255,255));
	//绘制含有角度和大小的结果
	drawKeypoints(img, Keypoints, imgAngel, Scalar(255, 255, 255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//显示结果
	imshow("不含角度和大小的结果", img);
	imshow("含有角度和大小的结果", imgAngel);
	waitKey(0); 
	return 0;
}