#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	//创建一个16×16全为1的矩阵，因为256=16×16
	Mat img = Mat::ones(16, 16, CV_32FC1);

	//在图像中加入随机噪声
	RNG rng(10086);
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			float d = rng.uniform(-0.5, 0.5);
			img.at<float>(y, x) = img.at<float>(y, x) + d;
		}
	}
	
	//计算标准求和积分
	Mat sum;
	integral(img, sum);
	//为了便于显示，转成CV_8U格式
	Mat sum8U = Mat_<uchar>(sum);

	//计算平方求和积分
	Mat sqsum;
	integral(img, sum, sqsum);
	//为了便于显示，转成CV_8U格式
	Mat sqsum8U = Mat_<uchar>(sqsum);

	//计算倾斜求和积分
	Mat tilted;
	integral(img, sum, sqsum, tilted);
	//为了便于显示，转成CV_8U格式
	Mat tilted8U = Mat_<uchar>(tilted);

	//输出结果
	namedWindow("sum8U", WINDOW_NORMAL);
	namedWindow("sqsum8U", WINDOW_NORMAL);
	namedWindow("tilted8U", WINDOW_NORMAL);
	imshow("sum8U", sum8U);
	imshow("sqsum8U", sqsum8U);
	imshow("tilted8U", tilted8U);

	waitKey();
	return 0;
}