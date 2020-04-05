#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("lena.png");
	if (!img.data)  //防止错误读取图像
	{
		cout<<"读取图像错误，请确认图像文件是否正确" << endl; 
		return 0;
	}

	//绘制矩形
	Mat imgRect;
	img.copyTo(imgRect);  //备份图像，方式绘制矩形框对结果产生影响
	Rect rect(80, 30, 340, 390);
	rectangle(imgRect, rect, Scalar(255, 255, 255),2);
	imshow("选择的矩形区域", imgRect);

	//进行分割
	Mat bgdmod = Mat::zeros(1, 65, CV_64FC1);
	Mat fgdmod = Mat::zeros(1, 65, CV_64FC1);
	Mat mask = Mat::zeros(img.size(), CV_8UC1);
	grabCut(img, mask, rect, bgdmod, fgdmod, 5, GC_INIT_WITH_RECT);
	
	//将分割出的前景绘制回来
	Mat result;
	for (int row = 0; row < mask.rows; row++) 
	{
		for (int col = 0; col < mask.cols; col++) 
		{
			int n = mask.at<uchar>(row, col);
			//将明显是前景和可能是前景的区域都保留
			if (n == 1 || n == 3) 
			{
				mask.at<uchar>(row, col) = 255;
			}
			//将明显是背景和可能是背景的区域都删除
			else 
			{
				mask.at<uchar>(row, col) = 0;
			}
		}
	}
	bitwise_and(img, img, result, mask);
	imshow("分割结果", result);
	waitKey(0);
	return 0;
}