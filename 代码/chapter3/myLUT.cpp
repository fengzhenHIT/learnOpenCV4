#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	//LUT查找表第一层
	uchar lutFirst[256];
	for (int i = 0; i<256; i++)
	{
		if (i <= 100)
			lutFirst[i] = 0;
		if (i > 100 && i <= 200)
			lutFirst[i] = 100;
		if (i > 200)
			lutFirst[i] = 255;
	}
	Mat lutOne(1, 256, CV_8UC1, lutFirst);

	//LUT查找表第二层
	uchar lutSecond[256];
	for (int i = 0; i<256; i++)
	{
		if (i <= 100)
			lutSecond[i] = 0;
		if (i > 100 && i <= 150)
			lutSecond[i] = 100;
		if (i > 150 && i <= 200)
			lutSecond[i] = 150;
		if (i > 200)
			lutSecond[i] = 255;
	}
	Mat lutTwo(1, 256, CV_8UC1, lutSecond);

	//LUT查找表第三层
	uchar lutThird[256];
	for (int i = 0; i<256; i++)
	{
		if (i <= 100)
			lutThird[i] = 100;
		if (i > 100 && i <= 200)
			lutThird[i] = 200;
		if (i > 200)
			lutThird[i] = 255;
	}
	Mat lutThree(1, 256, CV_8UC1, lutThird);

	//拥有三通道的LUT查找表矩阵
	vector<Mat> mergeMats;
	mergeMats.push_back(lutOne);
	mergeMats.push_back(lutTwo);
	mergeMats.push_back(lutThree);
	Mat LutTree;
	merge(mergeMats, LutTree);

	//计算图像的查找表
	Mat img = imread("lena.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat gray, out0, out1, out2;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	LUT(gray, lutOne, out0);
	LUT(img, lutOne, out1);
	LUT(img, LutTree, out2);
	imshow("out0", out0);
	imshow("out1", out1);
	imshow("out2", out2);
	waitKey(0);
	return 0;
}
