#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("noobcvqr.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Point2f src_points[4];
	Point2f dst_points[4];
	//通过Image Watch查看的二维码四个角点坐标
	src_points[0] = Point2f(94.0, 374.0);
	src_points[1] = Point2f(507.0, 380.0);
	src_points[2] = Point2f(1.0, 623.0);
	src_points[3] = Point2f(627.0, 627.0);
	//期望透视变换后二维码四个角点的坐标
	dst_points[0] = Point2f(0.0, 0.0);
	dst_points[1] = Point2f(627.0, 0.0);
	dst_points[2] = Point2f(0.0, 627.0);
	dst_points[3] = Point2f(627.0, 627.0);
	Mat rotation, img_warp;
	rotation = getPerspectiveTransform(src_points, dst_points);  //计算透视变换矩阵
	warpPerspective(img, img_warp, rotation, img.size());  //透视变换投影
	imshow("img", img);
	imshow("img_warp", img_warp);
	waitKey(0);
	return 0;
}