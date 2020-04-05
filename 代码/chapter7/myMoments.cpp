#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("approx.png");

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
		cout << "spatial moments:" << endl
			<< "m00：" << M.m00 << " m01：" << M.m01 << " m10：" << M.m10 << endl
			<< "m11：" << M.m11 << " m02：" << M.m02 << " m20：" << M.m20 << endl
			<< "m12：" << M.m12 << " m21：" << M.m21 << " m03：" << M.m03 << " m30："<< M.m30 << endl;

		cout << "central moments:" << endl
			<< "mu20：" << M.mu20 << " mu02：" << M.mu02 << " mu11：" << M.mu11 << endl
			<< "mu30：" << M.mu30 << " mu21：" << M.mu21 << " mu12：" << M.mu12 << " mu03：" << M.mu03 << endl;

		cout << "central normalized moments:" << endl
			<< "nu20：" << M.nu20 << " nu02：" << M.nu02 << " nu11：" << M.nu11 << endl
			<< "nu30：" << M.nu30 << " nu21：" << M.nu21 << " nu12：" << M.nu12 << " nu03：" << M.nu03 << endl;
	}
	return 0;
}