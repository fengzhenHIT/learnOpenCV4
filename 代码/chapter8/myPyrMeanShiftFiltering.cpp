#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("coins.png");
	if (!img.data)
	{
		cout << "读取图像错误，请确认图像文件是否正确" << endl;
		return -1;
	}

	//分割处理
	Mat result1, result2;
	TermCriteria T10 = TermCriteria(TermCriteria::COUNT | TermCriteria::EPS, 10, 0.1);
	pyrMeanShiftFiltering(img, result1, 20, 40, 2, T10);  //第一次分割
	pyrMeanShiftFiltering(result1, result2, 20, 40, 2, T10);  //第一次分割的结果再次分割

															  //显示分割结果
	imshow("img", img);
	imshow("result1", result1);
	imshow("result2", result2);

	//对图像提取Canny边缘
	Mat imgCanny, result1Canny, result2Canny;
	Canny(img, imgCanny, 150, 300);
	Canny(result1, result1Canny, 150, 300);
	Canny(result2, result2Canny, 150, 300);

	//显示边缘检测结果
	imshow("imgCanny", imgCanny);
	imshow("result1Canny", result1Canny);
	imshow("result2Canny", result2Canny);
	waitKey(0);
	return 0;
}
