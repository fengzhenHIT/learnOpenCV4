#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("dial.png");
	if (!img.data)
	{
		cout << "请检查图像文件名称是否输入正确" << endl;
		return -1;
	}

	Mat img1, img2;
	Point2f center = Point2f(img.cols / 2, img.rows/2);  //极坐标在图像中的原点
	//正极坐标变换
	warpPolar(img, img1, Size(300,600), center, center.x, INTER_LINEAR + WARP_POLAR_LINEAR);
	//逆极坐标变换
	warpPolar(img1, img2, Size(img.rows,img.cols), center, center.x, INTER_LINEAR + WARP_POLAR_LINEAR + WARP_INVERSE_MAP);

	imshow("原表盘图", img);
	imshow("表盘极坐标变换结果", img1); 
	imshow("逆变换结果", img2);
	waitKey(0);
	return 0;
}