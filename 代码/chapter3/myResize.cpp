#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat gray = imread("lena.png", IMREAD_GRAYSCALE);
	if (gray.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat smallImg, bigImg0, bigImg1, bigImg2;
	resize(gray, smallImg, Size(15, 15), 0, 0, INTER_AREA);  //先将图像缩小
	resize(smallImg, bigImg0, Size(30, 30), 0, 0, INTER_NEAREST);  //最近邻差值
	resize(smallImg, bigImg1, Size(30, 30), 0, 0, INTER_LINEAR);  //双线性差值
	resize(smallImg, bigImg2, Size(30, 30), 0, 0, INTER_CUBIC);  //双三次差值
	namedWindow("smallImg", WINDOW_NORMAL);  //图像尺寸太小，一定要设置可以调节窗口大小标志
	imshow("smallImg", smallImg);
	namedWindow("bigImg0", WINDOW_NORMAL);
	imshow("bigImg0", bigImg0);
	namedWindow("bigImg1", WINDOW_NORMAL);
	imshow("bigImg1", bigImg1);
	namedWindow("bigImg2", WINDOW_NORMAL);
	imshow("bigImg2", bigImg2);
	waitKey(0);
	return 0;
}