#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat kernel = (Mat_<float>(5, 5) << 1, 2, 3, 4, 5,
		                               2, 3, 4, 5, 6,
		                               3, 4, 5, 6, 7,
		                               4, 5, 6, 7, 8,
		                               5, 6, 7, 8, 9);
	Mat a, b;
	dct(kernel, a);
	idct(a, b);

	//对图像进行处理
	Mat img = imread("lena.png");
	if (!img.data)
	{
		cout << "读入图像出错,请确认图像名称是否正确" << endl;
		return -1;
	}
	imshow("原图像", img);
	
	//计算最佳变换尺寸
	int width = 2 * getOptimalDFTSize((img.cols + 1) / 2);
	int height = 2 * getOptimalDFTSize((img.rows + 1) / 2);
	
	//扩展图像尺寸
	int T = 0;
	int B = height - T - img.rows;
	int L = 0;
	int R = width - L - img.rows;
	Mat appropriate;
	copyMakeBorder(img, appropriate, T, B, L, R, BORDER_CONSTANT, Scalar(0));

	//提三个通道需要分别进行DCT变换
	vector<Mat> channels;
	split(appropriate, channels);

	//提取NGR颜色各个通道的值
	Mat one = channels.at(0);	
	Mat two = channels.at(1);
	Mat three = channels.at(2);

	//进行DCT变换
	Mat oneDCT, twoDCT, threeDCT;
	dct(Mat_<float>(one), oneDCT);
	dct(Mat_<float>(two), twoDCT);
	dct(Mat_<float>(three), threeDCT);

	//重新组成三个通道
	vector<Mat> channelsDCT;
	channelsDCT.push_back(Mat_<uchar>(oneDCT));
	channelsDCT.push_back(Mat_<uchar>(twoDCT));
	channelsDCT.push_back(Mat_<uchar>(threeDCT));

	//输出图像
	Mat result;
	merge(channelsDCT, result);
	imshow("DCT图像", result);
	waitKey();
	return 0;
}