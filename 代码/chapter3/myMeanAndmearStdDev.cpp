#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
	system("color F0");  //更改输出界面颜色
	float a[12] = { 1, 2, 3, 4, 5, 10, 6, 7, 8, 9, 10, 0 };
	Mat img = Mat(3, 4, CV_32FC1, a);  //单通道矩阵
	Mat imgs = Mat(2, 3, CV_32FC2, a);  //多通道矩阵

	cout << "/* 用meanStdDev同时求取图像的均值和标准差 */" << endl;
	Scalar myMean;
	myMean = mean(imgs);
	cout << "imgs均值=" << myMean << endl;
	cout << "imgs第一个通道的均值=" << myMean[0] << "    "
		<< "imgs第二个通道的均值=" << myMean[1] << endl << endl;

	cout << "/* 用meanStdDev同时求取图像的均值和标准差 */" << endl;
	Mat myMeanMat, myStddevMat;

	meanStdDev(img, myMeanMat, myStddevMat);
	cout << "img均值=" << myMeanMat << "    " << endl;
	cout << "img标准差=" << myStddevMat << endl << endl;
	meanStdDev(imgs, myMeanMat, myStddevMat);
	cout << "imgs均值=" << myMeanMat << "    " << endl << endl;
	cout << "imgs标准差=" << myStddevMat << endl;
	return 0;
}