#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//为了能在被调函数中使用，所以设置成全局的
int value;
void callBack(int, void*);  //滑动条回调函数
Mat img1, img2;

int main()
{
	img1 = imread("lena.png");
	if (!img1.data)
	{
		cout << "请确认是否输入正确的图像文件" << endl;
		return -1;
	}
	namedWindow("滑动条改变图像亮度");
	imshow("滑动条改变图像亮度", img1);
	value = 100;  //滑动条创建时的初值
				  //创建滑动条
	createTrackbar("亮度值百分比", "滑动条改变图像亮度", &value, 600, callBack, 0);
	waitKey();
}

static void callBack(int, void*)
{
	float a = value / 100.0;
	img2 = img1 * a;
	imshow("滑动条改变图像亮度", img2);
}