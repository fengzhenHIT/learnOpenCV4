#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat lena = imread("lena.png");
	Mat equalLena = imread("equalLena.png", IMREAD_ANYDEPTH);
	if (lena.empty() || equalLena.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;		return -1;
	}
	//生成与原图像同尺寸、数据类型和通道数的矩阵
	Mat lena_noise = Mat::zeros(lena.rows, lena.cols, lena.type());
	Mat equalLena_noise = Mat::zeros(lena.rows, lena.cols, equalLena.type());
	imshow("lena原图", lena);
	imshow("equalLena原图", equalLena);
	RNG rng;  //创建一个RNG类
	rng.fill(lena_noise, RNG::NORMAL, 10, 20);  //生成三通道的高斯分布随机数
	rng.fill(equalLena_noise, RNG::NORMAL, 15, 30);  //生成三通道的高斯分布随机数
	imshow("三通道高斯噪声", lena_noise);
	imshow("单通道高斯噪声", equalLena_noise);
	lena = lena + lena_noise;  //在彩色图像中添加高斯噪声
	equalLena = equalLena + equalLena_noise;  //在灰度图像中添加高斯噪声
											  //显示添加高斯噪声后的图像
	imshow("lena添加噪声", lena);
	imshow("equalLena添加噪声", equalLena);
	waitKey(0);
	return 0;
}
