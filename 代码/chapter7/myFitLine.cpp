#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	system("color F0");  //更改输出界面颜色
	Vec4f lines;  //存放你和后的直线
	vector<Point2f> point;  //待检测是否存在直线的所有点
	const static float Points[20][2] = {
		{ 0.0f,   0.0f },{ 10.0f,  11.0f },{ 21.0f,  20.0f },{ 30.0f,  30.0f },
		{ 40.0f,  42.0f },{ 50.0f,  50.0f },{ 60.0f,  60.0f },{ 70.0f,  70.0f },
		{ 80.0f,  80.0f },{ 90.0f,  92.0f },{ 100.0f, 100.0f },{ 110.0f, 110.0f },
		{ 120.0f, 120.0f },{ 136.0f, 130.0f },{ 138.0f, 140.0f },{ 150.0f, 150.0f },
		{ 160.0f, 163.0f },{ 175.0f, 170.0f },{ 181.0f, 180.0f },{ 200.0f, 190.0f }
	};
	//将所有点存放在vector中，用于输入函数中
	for (int i = 0; i < 20; i++)
	{
		point.push_back(Point2f(Points[i][0], Points[i][1]));
	}
	//参数设置
	double param = 0;  //距离模型中的数值参数C
	double reps = 0.01;  //坐标原点与直线之间的距离精度
	double aeps = 0.01;  //角度精度
	fitLine(point, lines, DIST_L1, 0, 0.01, 0.01);
	double k = lines[1] / lines[0];  //直线斜率
	cout << "直线斜率：" << k << endl;
	cout << "直线上一点坐标x：" << lines[2] << ", y::" << lines[3] << endl;
	cout << "直线解析式：y=" << k << "(x-" << lines[2] << ")+" << lines[3] << endl;
	return 0;
}