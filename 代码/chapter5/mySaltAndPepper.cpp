#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//盐噪声函数
void saltAndPepper(cv::Mat image, int n)
{
	for (int k = 0; k<n / 2; k++)
	{
		//随机确定图像中位置
		int i, j;
		i = std::rand() % image.cols;  //取余数运算，保证在图像的列数内 
		j = std::rand() % image.rows;  //取余数运算，保证在图像的行数内 
		int write_black = std::rand() % 2;  //判定为白色噪声还是黑色噪声的变量
		if (write_black == 0)  //添加白色噪声
		{
			if (image.type() == CV_8UC1)  //处理灰度图像
			{
				image.at<uchar>(j, i) = 255;  //白色噪声
			}
			else if (image.type() == CV_8UC3)  //处理彩色图像
			{
				image.at<cv::Vec3b>(j, i)[0] = 255; //cv::Vec3b为opencv定义的一个3个值的向量类型  
				image.at<cv::Vec3b>(j, i)[1] = 255; //[]指定通道，B:0，G:1，R:2  
				image.at<cv::Vec3b>(j, i)[2] = 255;
			}
		}
		else  //添加黑色噪声
		{
			if (image.type() == CV_8UC1)
			{
				image.at<uchar>(j, i) = 0;
			}
			else if (image.type() == CV_8UC3)
			{
				image.at<cv::Vec3b>(j, i)[0] = 0; //cv::Vec3b为opencv定义的一个3个值的向量类型  
				image.at<cv::Vec3b>(j, i)[1] = 0; //[]指定通道，B:0，G:1，R:2  
				image.at<cv::Vec3b>(j, i)[2] = 0;
			}
		}

	}
}

int main()
{
	Mat lena = imread("lena.png");
	Mat equalLena = imread("equalLena.png", IMREAD_ANYDEPTH);
	if (lena.empty() || equalLena.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;		return -1;
	}
	imshow("lena原图", lena);
	imshow("equalLena原图", equalLena);
	saltAndPepper(lena, 10000);  //彩色图像添加椒盐噪声
	saltAndPepper(equalLena, 10000);  //灰度图像添加椒盐噪声
	imshow("lena添加噪声", lena);
	imshow("equalLena添加噪声", equalLena);
	waitKey(0);
	return 0;
}
