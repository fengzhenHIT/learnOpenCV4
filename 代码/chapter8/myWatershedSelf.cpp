#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat img, imgGray, imgMask, img_;
	Mat maskWaterShed;  // watershed()函数的参数
	img = imread("lenaw.png");  //含有标记的图像
	img_ = imread("lena.png");  //原图像
	cvtColor(img, imgGray, COLOR_BGR2GRAY);

	//二值化并开运算
	threshold(imgGray, imgMask,254,255,THRESH_BINARY);
	Mat k = getStructuringElement(0, Size(3, 3));
	morphologyEx(imgMask, imgMask, MORPH_OPEN, k);

	imshow("含有标记的图像", img);
	imshow("原图像", img_);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgMask, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	//在maskWaterShed上绘制轮廓,用于输入分水岭算法
	maskWaterShed = Mat::zeros(imgMask.size(), CV_32S);
	for (int index = 0; index < contours.size(); index++)
	{
		drawContours(maskWaterShed, contours, index, Scalar::all(index + 1), -1, 8, hierarchy, INT_MAX);
	}
	//分水岭算法   需要对原图像进行处理
	watershed(img_, maskWaterShed);
	
	vector<Vec3b> colors;  // 随机生成几种颜色
	for (int i = 0; i < contours.size(); i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	Mat resultImg = Mat(img.size(), CV_8UC3);  //显示图像
	for (int i = 0; i < imgMask.rows; i++)
	{
		for (int j = 0; j < imgMask.cols; j++)
		{
			// 绘制每个区域的颜色
			int index = maskWaterShed.at<int>(i, j);
			if (index == -1)  // 区域间的值被置为-1（边界）
			{
				resultImg.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
			else if (index <= 0 || index > contours.size())  // 没有标记清楚的区域被置为0
			{
				resultImg.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
			else  // 其他每个区域的值保持不变：1，2，…，contours.size()
			{
				resultImg.at<Vec3b>(i, j) = colors[index - 1];  // 把些区域绘制成不同颜色
			}
		}
	}
	imshow("resultImg", resultImg);
	resultImg = resultImg * 0.8 + img_ * 0.2;
	//addWeighted(resultImg, 0.8, img_, 0.2, 0, resultImg);
	imshow("分水岭结果", resultImg);
	
	//绘制每个区域的图像
	for (int n = 1; n <= contours.size(); n++)
	{
		Mat resImage1 = Mat(img.size(), CV_8UC3);  // 声明一个最后要显示的图像
		for (int i = 0; i < imgMask.rows; i++)
		{
			for (int j = 0; j < imgMask.cols; j++)
			{
				int index = maskWaterShed.at<int>(i, j);
				if (index == n)
					resImage1.at<Vec3b>(i, j) = img_.at<Vec3b>(i, j);
				else
					resImage1.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
		}
		//显示图像
		imshow(to_string(n), resImage1);
	}

	waitKey(0);
	return 0;
}