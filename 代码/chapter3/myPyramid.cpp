#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("lena.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	vector<Mat> Gauss, Lap;  //高斯金字塔和拉普拉斯金字塔
	int level = 3;  //高斯金字塔下采样次数
	Gauss.push_back(img);  //将原图作为高斯金字塔的第0层
						   //构建高斯金字塔
	for (int i = 0; i < level; i++)
	{
		Mat gauss;
		pyrDown(Gauss[i], gauss);  //下采样
		Gauss.push_back(gauss);
	}
	//构建拉普拉斯金字塔
	for (int i = Gauss.size() - 1; i > 0; i--)
	{
		Mat lap, upGauss;
		if (i == Gauss.size() - 1)  //如果是高斯金字塔中的最上面一层图像
		{
			Mat down;
			pyrDown(Gauss[i], down);  //上采样
			pyrUp(down, upGauss);
			lap = Gauss[i] - upGauss;
			Lap.push_back(lap);
		}
		pyrUp(Gauss[i], upGauss);
		lap = Gauss[i - 1] - upGauss;
		Lap.push_back(lap);
	}
	//查看两个金字塔中的图像
	for (int i = 0; i < Gauss.size(); i++)
	{
		string name = to_string(i);
		imshow("G" + name, Gauss[i]);
		imshow("L" + name, Lap[i]);
	}
	waitKey(0);
	return 0;
}
