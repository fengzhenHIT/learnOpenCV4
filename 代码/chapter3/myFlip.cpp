#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("lena.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat img_x, img_y, img_xy;
	flip(img, img_x, 0);  //沿x轴对称
	flip(img, img_y, 1);  //沿y轴对称
	flip(img, img_xy, -1);  //先x轴对称，再y轴对称
	imshow("img", img);
	imshow("img_x", img_x);
	imshow("img_y", img_y);
	imshow("img_xy", img_xy);
	waitKey(0);
	return 0;
}