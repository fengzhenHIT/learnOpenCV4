#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;
//绘制包含区域函数
void drawState(Mat &img, int number, Mat centroids, Mat stats, String str) {
	RNG rng(10086);
	vector<Vec3b> colors;
	for (int i = 0; i < number; i++)
	{
		//使用均匀分布的随机数确定颜色
		Vec3b vec3 = Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		colors.push_back(vec3);
	}

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

		// 中心位置绘制
		circle(img, Point(center_x, center_y), 2, Scalar(0, 255, 0), 2, 8, 0);
		// 外接矩形
		Rect rect(x, y, w, h);
		rectangle(img, rect, colors[i], 1, 8, 0);
		putText(img, format("%d", i), Point(center_x, center_y),
			FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
	}
	imshow(str, img);
}

int main()
{
	//生成用于腐蚀的原图像
	Mat src = (Mat_<uchar>(6, 6) << 0, 0, 0, 0, 255, 0,
		0, 255, 255, 255, 255, 255,
		0, 255, 255, 255, 255, 0,
		0, 255, 255, 255, 255, 0,
		0, 255, 255, 255, 255, 0,
		0, 0, 0, 0, 0, 0);
	Mat struct1, struct2;
	struct1 = getStructuringElement(0, Size(3, 3));  //矩形结构元素
	struct2 = getStructuringElement(1, Size(3, 3));  //十字结构元素

	Mat erodeSrc;  //存放腐蚀后的图像
	erode(src, erodeSrc, struct2);
	namedWindow("src", WINDOW_GUI_NORMAL);
	namedWindow("erodeSrc", WINDOW_GUI_NORMAL);
	imshow("src", src);
	imshow("erodeSrc", erodeSrc);

	Mat LearnCV_black = imread("LearnCV_black.png", IMREAD_ANYCOLOR);
	Mat LearnCV_write = imread("LearnCV_write.png", IMREAD_ANYCOLOR);
	Mat erode_black1, erode_black2, erode_write1, erode_write2;
	//黑背景图像腐蚀
	erode(LearnCV_black, erode_black1, struct1);
	erode(LearnCV_black, erode_black2, struct2);
	imshow("LearnCV_black", LearnCV_black);
	imshow("erode_black1", erode_black1);
	imshow("erode_black2", erode_black2);

	//白背景腐蚀
	erode(LearnCV_write, erode_write1, struct1);
	erode(LearnCV_write, erode_write2, struct2);
	imshow("LearnCV_write", LearnCV_write);
	imshow("erode_write1", erode_write1);
	imshow("erode_write2", erode_write2);

	//验证腐蚀对小连通域的去除
	Mat img = imread("rice.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat img2;
	copyTo(img, img2, img);  //克隆一个单独的图像，用于后期图像绘制
	Mat rice, riceBW;

	//将图像转成二值图像，用于统计连通域
	cvtColor(img, rice, COLOR_BGR2GRAY);
	threshold(rice, riceBW, 50, 255, THRESH_BINARY);

	Mat out, stats, centroids;
	//统计图像中连通域的个数
	int number = connectedComponentsWithStats(riceBW, out, stats, centroids, 8, CV_16U);
	drawState(img, number, centroids, stats, "未腐蚀时统计连通域");  //绘制图像

	erode(riceBW, riceBW, struct1);  //对图像进行腐蚀
	number = connectedComponentsWithStats(riceBW, out, stats, centroids, 8, CV_16U);
	drawState(img2, number, centroids, stats, "腐蚀后统计连通域");  //绘制图像

	waitKey(0);
	return 0;
}
