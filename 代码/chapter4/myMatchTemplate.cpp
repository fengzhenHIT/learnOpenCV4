#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("lena.png");
	Mat temp = imread("lena_face.png");
	if (img.empty() || temp.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;		return -1;
	}
	Mat result;
	matchTemplate(img, temp, result, TM_CCOEFF_NORMED);//模板匹配
	double maxVal, minVal;
	Point minLoc, maxLoc;
	//寻找匹配结果中的最大值和最小值以及坐标位置
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	//绘制最佳匹配区域
	rectangle(img, cv::Rect(maxLoc.x, maxLoc.y, temp.cols, temp.rows), Scalar(0, 0, 255), 2);
	imshow("原图", img);
	imshow("模板图像", temp);
	imshow("result", result);
	waitKey(0);
	return 0;
}