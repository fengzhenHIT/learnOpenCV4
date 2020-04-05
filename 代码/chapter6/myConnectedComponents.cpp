#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	//对图像进行距离变换
	Mat img = imread("rice.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat rice, riceBW;

	//将图像转成二值图像，用于统计连通域
	cvtColor(img, rice, COLOR_BGR2GRAY);
	threshold(rice, riceBW, 50, 255, THRESH_BINARY);

	//生成随机颜色，用于区分不同连通域
	RNG rng(10086);
	Mat out;
	int number = connectedComponents(riceBW, out, 8, CV_16U);  //统计图像中连通域的个数
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
	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			int label = out.at<uint16_t>(row, col);
			if (label == 0)  //背景的黑色不改变
			{
				continue;
			}
			result.at<Vec3b>(row, col) = colors[label];
		}
	}

	//显示结果
	imshow("原图", img);
	imshow("标记后的图像", result);

	waitKey(0);
	return 0;
}
