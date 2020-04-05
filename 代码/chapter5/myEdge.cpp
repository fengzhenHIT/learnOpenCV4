#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	//创建边缘检测滤波器
	Mat kernel1 = (Mat_<float>(1, 2) << 1, -1);  //X方向边缘检测滤波器
	Mat kernel2 = (Mat_<float>(1, 3) << 1, 0, -1);  //X方向边缘检测滤波器
	Mat kernel3 = (Mat_<float>(3, 1) << 1, 0, -1);  //X方向边缘检测滤波器
	Mat kernelXY = (Mat_<float>(2, 2) << 1, 0, 0, -1);  //由左上到右下方向边缘检测滤波器
	Mat kernelYX = (Mat_<float>(2, 2) << 0, -1, 1, 0);  //由右上到左下方向边缘检测滤波器

														//读取图像，黑白图像边缘检测结果较为明显
	Mat img = imread("equalLena.png", IMREAD_ANYCOLOR);
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat result1, result2, result3, result4, result5, result6;

	//检测图像边缘
	//以[1 -1]检测水平方向边缘
	filter2D(img, result1, CV_16S, kernel1);
	convertScaleAbs(result1, result1);

	//以[1 0 -1]检测水平方向边缘
	filter2D(img, result2, CV_16S, kernel2);
	convertScaleAbs(result2, result2);

	//以[1 0 -1]'检测由垂直方向边缘
	filter2D(img, result3, CV_16S, kernel3);
	convertScaleAbs(result3, result3);

	//整幅图像的边缘
	result6 = result2 + result3;
	//检测由左上到右下方向边缘
	filter2D(img, result4, CV_16S, kernelXY);
	convertScaleAbs(result4, result4);

	//检测由右上到左下方向边缘
	filter2D(img, result5, CV_16S, kernelYX);
	convertScaleAbs(result5, result5);

	//显示边缘检测结果
	imshow("result1", result1);
	imshow("result2", result2);
	imshow("result3", result3);
	imshow("result4", result4);
	imshow("result5", result5);
	imshow("result6", result6);
	waitKey(0);
	return 0;
}