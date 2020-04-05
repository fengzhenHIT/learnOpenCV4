#include <opencv2\opencv.hpp>
#include <opencv2\ximgproc.hpp>  //细化函数thining所在的头文件
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	//中文字进行细化
	Mat img = imread("LearnCV_black.png", IMREAD_ANYCOLOR);
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}	//英文字+实心圆和圆环细化
	Mat words = Mat::zeros(100, 200, CV_8UC1);  //创建一个黑色的背景图片
	putText(words, "Learn", Point(30, 30), 2, 1, Scalar(255), 2);  //添加英文
	putText(words, "OpenCV 4", Point(30, 60), 2, 1, Scalar(255), 2);
	circle(words, Point(80, 75), 10, Scalar(255), -1);  //添加实心圆
	circle(words, Point(130, 75), 10, Scalar(255), 3);  //添加圆环

	//进行细化
	Mat thin1, thin2;
	ximgproc::thinning(img, thin1, 0);  //注意类名
	ximgproc::thinning(words, thin2, 0);

	//显示处理结果
	imshow("thin1", thin1);
	imshow("img", img);
	namedWindow("thin2", WINDOW_NORMAL);
	imshow("thin2", thin2);
	namedWindow("words", WINDOW_NORMAL);
	imshow("words", words);
	waitKey(0);
	return 0;
}