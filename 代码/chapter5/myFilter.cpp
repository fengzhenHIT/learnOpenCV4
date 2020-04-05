#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	//待卷积矩阵
	uchar points[25] = { 1,2,3,4,5,
		6,7,8,9,10,
		11,12,13,14,15,
		16,17,18,19,20,
		21,22,23,24,25 };
	Mat img(5, 5, CV_8UC1, points);
	//卷积模板
	Mat kernel = (Mat_<float>(3, 3) << 1, 2, 1,
		2, 0, 2,
		1, 2, 1);
	Mat kernel_norm = kernel / 12;  //卷积模板归一化
									//未归一化卷积结果和归一化卷积结果
	Mat result, result_norm;
	filter2D(img, result, CV_32F, kernel, Point(-1, -1), 2, BORDER_CONSTANT);
	filter2D(img, result_norm, CV_32F, kernel_norm, Point(-1, -1), 2, BORDER_CONSTANT);
	cout << "result:" << endl << result << endl;
	cout << "result_norm:" << endl << result_norm << endl;
	//图像卷积
	Mat lena = imread("lena.png");
	if (lena.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat lena_fillter;
	filter2D(lena, lena_fillter, -1, kernel_norm, Point(-1, -1), 2, BORDER_CONSTANT);
	imshow("lena_fillter", lena_fillter);
	imshow("lena", lena);
	waitKey(0);
	return 0;
}