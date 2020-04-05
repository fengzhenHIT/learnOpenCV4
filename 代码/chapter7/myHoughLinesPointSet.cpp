#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	system("color F0");  //更改输出界面颜色
	Mat lines;  //存放检测直线结果的矩阵
	vector<Vec3d> line3d;  //换一种结果存放形式
	vector<Point2f> point;  //待检测是否存在直线的所有点
	const static float Points[20][2] = {
		{ 0.0f,   369.0f },{ 10.0f,  364.0f },{ 20.0f,  358.0f },{ 30.0f,  352.0f },
		{ 40.0f,  346.0f },{ 50.0f,  341.0f },{ 60.0f,  335.0f },{ 70.0f,  329.0f },
		{ 80.0f,  323.0f },{ 90.0f,  318.0f },{ 100.0f, 312.0f },{ 110.0f, 306.0f },
		{ 120.0f, 300.0f },{ 130.0f, 295.0f },{ 140.0f, 289.0f },{ 150.0f, 284.0f },
		{ 160.0f, 277.0f },{ 170.0f, 271.0f },{ 180.0f, 266.0f },{ 190.0f, 260.0f }
	};
	//将所有点存放在vector中，用于输入函数中
	for (int i = 0; i < 20; i++)
	{
		point.push_back(Point2f(Points[i][0], Points[i][1]));
	}
	//参数设置
	double rhoMin = 0.0f;  //最小长度
	double rhoMax = 360.0f;  //最大长度
	double rhoStep = 1;  //离散化单位距离长度
	double thetaMin = 0.0f;  //最小角度
	double thetaMax = CV_PI / 2.0f;  //最大角度
	double thetaStep = CV_PI / 180.0f;  ////离散化单位角度弧度
	HoughLinesPointSet(point, lines, 20, 1, rhoMin, rhoMax, rhoStep,
		thetaMin, thetaMax, thetaStep);
	lines.copyTo(line3d);

	//输出结果
	for (int i = 0; i < line3d.size(); i++)
	{
		cout << "votes:" << (int)line3d.at(i).val[0] << ", "
			<< "rho:" << line3d.at(i).val[1] << ", "
			<< "theta:" << line3d.at(i).val[2] << endl;
	}
	return 0;
}