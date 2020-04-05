#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
	Mat img = imread("lena.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat rotation0, rotation1, img_warp0, img_warp1;
	double angle = 30;  //设置图像旋转的角度
	Size dst_size(img.rows, img.cols);  //设置输出图像的尺寸
	Point2f center(img.rows / 2.0, img.cols / 2.0);  //设置图像的旋转中心
	rotation0 = getRotationMatrix2D(center, angle, 1);  //计算放射变换矩阵
	warpAffine(img, img_warp0, rotation0, dst_size);  //进行仿射变换
	imshow("img_warp0", img_warp0);
	//根据定义的三个点进行仿射变换
	Point2f src_points[3];
	Point2f dst_points[3];
	src_points[0] = Point2f(0, 0);  //原始图像中的三个点
	src_points[1] = Point2f(0, (float)(img.cols - 1));
	src_points[2] = Point2f((float)(img.rows - 1), (float)(img.cols - 1));
	dst_points[0] = Point2f((float)(img.rows)*0.11, (float)(img.cols)*0.20);  //放射变换后图像中的三个点
	dst_points[1] = Point2f((float)(img.rows)*0.15, (float)(img.cols)*0.70);
	dst_points[2] = Point2f((float)(img.rows)*0.81, (float)(img.cols)*0.85);
	rotation1 = getAffineTransform(src_points, dst_points);  //根据对应点求取仿射变换矩阵
	warpAffine(img, img_warp1, rotation1, dst_size);  //进行仿射变换
	imshow("img_warp1", img_warp1);
	waitKey(0);
	return 0;
}