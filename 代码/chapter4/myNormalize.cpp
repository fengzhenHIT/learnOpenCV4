#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	system("color F0");  //更改输出界面颜色
	vector<double> positiveData = { 2.0, 8.0, 10.0 };
	vector<double> normalized_L1, normalized_L2, normalized_Inf, normalized_L2SQR;
	//测试不同归一化方法
	normalize(positiveData, normalized_L1, 1.0, 0.0, NORM_L1);  //绝对值求和归一化
	cout << "normalized_L1=[" << normalized_L1[0] << ", "
		<< normalized_L1[1] << ", " << normalized_L1[2] << "]" << endl;
	normalize(positiveData, normalized_L2, 1.0, 0.0, NORM_L2);  //模长归一化
	cout << "normalized_L2=[" << normalized_L2[0] << ", "
		<< normalized_L2[1] << ", " << normalized_L2[2] << "]" << endl;
	normalize(positiveData, normalized_Inf, 1.0, 0.0, NORM_INF);  //最大值归一化
	cout << "normalized_Inf=[" << normalized_Inf[0] << ", "
		<< normalized_Inf[1] << ", " << normalized_Inf[2] << "]" << endl;
	normalize(positiveData, normalized_L2SQR, 1.0, 0.0, NORM_MINMAX);  //偏移归一化
	cout << "normalized_MINMAX=[" << normalized_L2SQR[0] << ", "
		<< normalized_L2SQR[1] << ", " << normalized_L2SQR[2] << "]" << endl;
	//将图像直方图归一化
	Mat img = imread("apple.jpg");
	if (img.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;
		return -1;
	}
	Mat gray, hist;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	const int channels[1] = { 0 };
	float inRanges[2] = { 0,255 };
	const float* ranges[1] = { inRanges };
	const int bins[1] = { 256 };
	calcHist(&gray, 1, channels, Mat(), hist, 1, bins, ranges);
	int hist_w = 512;
	int hist_h = 400;
	int width = 2;
	Mat histImage_L1 = Mat::zeros(hist_h, hist_w, CV_8UC3);
	Mat histImage_Inf = Mat::zeros(hist_h, hist_w, CV_8UC3);
	Mat hist_L1, hist_Inf;
	normalize(hist, hist_L1, 1, 0, NORM_L1, -1, Mat());
	for (int i = 1; i <= hist_L1.rows; i++)
	{
		rectangle(histImage_L1, Point(width*(i - 1), hist_h - 1),
			Point(width*i - 1, hist_h - cvRound(30 * hist_h*hist_L1.at<float>(i - 1)) - 1),
			Scalar(255, 255, 255), -1);
	}
	normalize(hist, hist_Inf, 1, 0, NORM_INF, -1, Mat());
	for (int i = 1; i <= hist_Inf.rows; i++)
	{
		rectangle(histImage_Inf, Point(width*(i - 1), hist_h - 1),
			Point(width*i - 1, hist_h - cvRound(hist_h*hist_Inf.at<float>(i - 1)) - 1),
			Scalar(255, 255, 255), -1);
	}
	imshow("histImage_L1", histImage_L1);
	imshow("histImage_Inf", histImage_Inf);
	waitKey(0);
	return 0;
}
