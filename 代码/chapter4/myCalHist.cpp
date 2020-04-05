#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("apple.jpg");
	if (img.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;
		return -1;
	}
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	//设置提取直方图的相关变量
	Mat hist;  //用于存放直方图计算结果
	const int channels[1] = { 0 };  //通道索引
	float inRanges[2] = { 0,255 };
	const float* ranges[1] = { inRanges };  //像素灰度值范围
	const int bins[1] = { 256 };  //直方图的维度，其实就是像素灰度值的最大值
	calcHist(&gray, 1, channels, Mat(), hist, 1, bins, ranges);  //计算图像直方图
																//准备绘制直方图
	int hist_w = 512;
	int hist_h = 400;
	int width = 2;
	Mat histImage = Mat::zeros(hist_h, hist_w, CV_8UC3);
	for (int i = 1; i <= hist.rows; i++)
	{
		rectangle(histImage, Point(width*(i - 1), hist_h - 1),
			Point(width*i - 1, hist_h - cvRound(hist.at<float>(i - 1) / 15)),
			Scalar(255, 255, 255), -1);
	}
	namedWindow("histImage", WINDOW_AUTOSIZE);
	imshow("histImage", histImage);
	imshow("gray", gray);
	waitKey(0);
	return 0;
}