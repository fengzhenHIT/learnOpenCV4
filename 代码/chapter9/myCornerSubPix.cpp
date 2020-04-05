#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main()
{
	system("color F0");  //改变DOS界面颜色
	Mat img = imread("lena.png",IMREAD_COLOR);
	if (!img.data)
	{
		cout << "读取图像错误，请确认图像文件是否正确" << endl;
		return -1;
	}
	//彩色图像转成灰度图像
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	//提取角点
	int maxCorners = 100;  //检测角点数目
	double quality_level = 0.01;  //质量等级，或者说阈值与最佳角点的比例关系
	double minDistance = 0.04;  //两个角点之间的最小欧式距离
	vector<Point2f> corners;
	goodFeaturesToTrack(gray, corners, maxCorners, quality_level, minDistance, Mat(), 3, false);
	
	//计算亚像素级别角点坐标
	vector<Point2f> cornersSub = corners;  //角点备份，方式别函数修改
	Size winSize = Size(5, 5);
	Size zeroZone = Size(-1, -1);
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 40, 0.001);
	cornerSubPix(gray, cornersSub , winSize, zeroZone, criteria);

	//输出初始坐标和精细坐标
	for (size_t i = 0; i < corners.size(); i++)
	{
		string str = to_string(i);
		str = "第" + str + "个角点点初始坐标：";
		cout << str << corners[i] << "   精细后坐标：" << cornersSub[i] << endl;
	}
	return 0;
}