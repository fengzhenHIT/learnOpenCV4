#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat img, imgPoint; //全局的图像
Point prePoint; //前一时刻鼠标的坐标，用于绘制直线
void mouse(int event, int x, int y, int flags, void*);

int main()
{
	img = imread("lena.png");
	if (!img.data)
		{
			cout << "请确认输入图像名称是否正确！ " << endl;
			return -1;
		}
	img.copyTo(imgPoint);
	imshow("图像窗口 1", img);
	imshow("图像窗口 2", imgPoint);
	setMouseCallback("图像窗口 1", mouse, 0); //鼠标影响
	waitKey(0);
	return 0;
	}

void mouse(int event, int x, int y, int flags, void*)
{
	if (event == EVENT_RBUTTONDOWN) //单击右键
	{
		cout << "点击鼠标左键才可以绘制轨迹" << endl;
	}
	if (event == EVENT_LBUTTONDOWN) //单击左键，输出坐标
	{
		prePoint = Point(x, y);
		cout << "轨迹起始坐标" << prePoint << endl;
	}
	if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON)) //鼠标按住左键移动第 3 章 图像基本操作
	{
	 //通过改变图像像素显示鼠标移动轨迹
		imgPoint.at<Vec3b>(y, x) = Vec3b(0, 0, 255);
		imgPoint.at<Vec3b>(y, x - 1) = Vec3b(0, 0, 255);
		imgPoint.at<Vec3b>(y, x + 1) = Vec3b(0, 0, 255);
		imgPoint.at<Vec3b>(y + 1, x) = Vec3b(0, 0, 255);
		imgPoint.at<Vec3b>(y + 1, x) = Vec3b(0, 0, 255);
		imshow("图像窗口 2", imgPoint);
		
	    //通过绘制直线显示鼠标移动轨迹
		Point pt(x, y);
		line(img, prePoint, pt, Scalar(0, 0, 255), 2, 5, 0);
		prePoint = pt;
		imshow("图像窗口 1", img);
	}
}