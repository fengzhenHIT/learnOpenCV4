#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	system("color F0");  //更改输出界面颜色
	//对图像进行距离变换
	Mat img = imread("rice.png");
	if (img.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;
		return -1;
	}
	imshow("原图", img);
	Mat rice, riceBW;

	//将图像转成二值图像，用于统计连通域
	cvtColor(img, rice, COLOR_BGR2GRAY);
	threshold(rice, riceBW, 50, 255, THRESH_BINARY);

	//生成随机颜色，用于区分不同连通域
	RNG rng(10086);
	Mat out, stats, centroids;
	//统计图像中连通域的个数
	int number = connectedComponentsWithStats(riceBW, out, stats, centroids, 8, CV_16U);
	vector<Vec3b> colors;
	for (int i = 0; i < number; i++)
	{
		//使用均匀分布的随机数确定颜色
		Vec3b vec3 = Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		colors.push_back(vec3);
	}

	//以不同颜色标记出不同的连通域
	Mat result = Mat::zeros(rice.size(), img.type());
	int w = result.cols;
	int h = result.rows;
	for (int i = 1; i < number; i++)
	{
		// 中心位置
		int center_x = centroids.at<double>(i, 0);
		int center_y = centroids.at<double>(i, 1);
		//矩形边框
		int x = stats.at<int>(i, CC_STAT_LEFT);
		int y = stats.at<int>(i, CC_STAT_TOP);
		int w = stats.at<int>(i, CC_STAT_WIDTH);
		int h = stats.at<int>(i, CC_STAT_HEIGHT);
		int area = stats.at<int>(i, CC_STAT_AREA);

		// 中心位置绘制
		circle(img, Point(center_x, center_y), 2, Scalar(0, 255, 0), 2, 8, 0);
		// 外接矩形
		Rect rect(x, y, w, h);
		rectangle(img, rect, colors[i], 1, 8, 0);
		putText(img, format("%d", i), Point(center_x, center_y),
			FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
		cout << "number: " << i << ",area: " << area << endl;
	}
	//显示结果
	imshow("标记后的图像", img);

	waitKey(0);
	return 0;
}
