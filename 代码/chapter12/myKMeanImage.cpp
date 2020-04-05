#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("people.jpg");
	if (!img.data)
	{
		printf("请确认图像文件是否输入正确");
		return -1;
	}
	
	Vec3b colorLut[5] = {
		Vec3b(0, 0, 255),
		Vec3b(0, 255, 0),
		Vec3b(255, 0, 0),
		Vec3b(0, 255, 255),
		Vec3b(255, 0, 255)
	};

	//图像的尺寸，用于计算图像中像素点的数目
	int width = img.cols;
	int height = img.rows;
	
	// 初始化定义
	int sampleCount = width*height;
	

	//将图像矩阵数据转换成每行一个数据的形式
	Mat sample_data = img.reshape(3, sampleCount);
	Mat data;
	sample_data.convertTo(data, CV_32F);

	//KMean函数将像素值进行分类
	int number = 3;  //分割后的颜色种类
	Mat labels;
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1);
	kmeans(data, number, labels, criteria, number, KMEANS_PP_CENTERS);

	// 显示图像分割结果
	Mat result = Mat::zeros(img.size(), img.type());
	for (int row = 0; row < height; row++) 
	{
		for (int col = 0; col < width; col++) 
		{
			int index = row*width + col;
			int label = labels.at<int>(index, 0);
			result.at<Vec3b>(row, col) = colorLut[label];
		}
	}

	imshow("原图", img);
	imshow("分割后图像", result);
	waitKey(0);
	return 0;
}