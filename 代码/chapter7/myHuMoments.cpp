#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	system("color F0");  //更改输出界面颜色
	Mat img = imread("approx.png");
	if (img.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;
		return -1;
	}
	// 二值化
	Mat gray, binary;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 105, 255, THRESH_BINARY);

	//开运算消除细小区域
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(binary, binary, MORPH_OPEN, k);

	// 轮廓发现
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, 0, 2, Point());
	for (int n = 0; n < contours.size(); n++)
	{
		Moments M;
		M = moments(contours[n], true);
		Mat hu;
		HuMoments(M, hu);  //计算Hu矩
		cout << hu << endl;
	}
	return 0;
}