#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	//构建建议矩阵，用于求取像素之间的距离
	Mat a = (Mat_<uchar>(5, 5) << 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 0, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1);
	Mat dist_L1, dist_L2, dist_C, dist_L12;

	//计算街区距离
	distanceTransform(a, dist_L1, 1, 3, CV_8U);
	cout << "街区距离：" << endl << dist_L1 << endl;

	//计算欧式距离
	distanceTransform(a, dist_L2, 2, 5, CV_8U);
	cout << "欧式距离：" << endl << dist_L2 << endl;

	//计算棋盘距离
	distanceTransform(a, dist_C, 3, 5, CV_8U);
	cout << "棋盘距离：" << endl << dist_C << endl;

	//对图像进行距离变换
	Mat rice = imread("rice.png", IMREAD_GRAYSCALE);
	if (rice.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat riceBW, riceBW_INV;

	//将图像转成二值图像，同时把黑白区域颜色呼唤
	threshold(rice, riceBW, 50, 255, THRESH_BINARY);
	threshold(rice, riceBW_INV, 50, 255, THRESH_BINARY_INV);

	//距离变换
	Mat dist, dist_INV;
	distanceTransform(riceBW, dist, 1, 3, CV_32F);  //为了显示清晰，将数据类型变成CV_32F
	distanceTransform(riceBW_INV, dist_INV, 1, 3, CV_8U);

	//显示变换结果
	imshow("riceBW", riceBW);
	imshow("dist", dist);
	imshow("riceBW_INV", riceBW_INV);
	imshow("dist_INV", dist_INV);

	waitKey(0);
	return 0;
}