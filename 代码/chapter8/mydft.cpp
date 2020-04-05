#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	//对矩阵进行处理，展示正逆变换的关系
	Mat a = (Mat_<float>(5, 5) << 1, 2, 3, 4, 5,
		2, 3, 4, 5, 6,
		3, 4, 5, 6, 7,
		4, 5, 6, 7, 8,
		5, 6, 7, 8, 9);
	Mat b, c, d;
	dft(a, b, DFT_COMPLEX_OUTPUT);  //正变换
	dft(b, c, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);  //逆变换只输出实数
	idft(b, d, DFT_SCALE);  //逆变换

	//对图像进行处理
	Mat img = imread("lena.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	resize(gray, gray, Size(502, 502));
	imshow("原图像", gray);

	//计算合适的离散傅里叶变换尺寸
	int rows = getOptimalDFTSize(gray.rows);
	int cols = getOptimalDFTSize(gray.cols);

	//扩展图像
	Mat appropriate;
	int T = (rows - gray.rows) / 2;  //上方扩展行数
	int B = rows - gray.rows - T;  //下方扩展行数
	int L = (cols - gray.cols) / 2;  //左侧扩展行数
	int R = cols - gray.cols - L;  //右侧扩展行数
	copyMakeBorder(gray, appropriate, T, B, L, R, BORDER_CONSTANT);
	imshow("扩展后的图像", appropriate);

	//构建离散傅里叶变换输入量
	Mat flo[2], complex;
	flo[0] = Mat_<float>(appropriate);  //实数部分
	flo[1] = Mat::zeros(appropriate.size(), CV_32F);  //虚数部分
	merge(flo, 2, complex);  //合成一个多通道矩阵

	//进行离散傅里叶变换
	Mat result;
	dft(complex, result);

	//将复数转化为幅值
	Mat resultC[2];
	split(result, resultC);  //分成实数和虚数
	Mat amplitude;
	magnitude(resultC[0], resultC[1], amplitude);

	//进行对数放缩公式为： M1 = log（1+M），保证所有数都大于0
	amplitude = amplitude + 1;
	log(amplitude, amplitude);//求自然对数

	//与原图像尺寸对应的区域								
	amplitude = amplitude(Rect(T, L, gray.cols, gray.rows));
	normalize(amplitude, amplitude, 0, 1, NORM_MINMAX);  //归一化
	imshow("傅里叶变换结果幅值图像", amplitude);  //显示结果

	//重新排列傅里叶图像中的象限，使得原点位于图像中心
	int centerX = amplitude.cols / 2;
	int centerY = amplitude.rows / 2;
	//分解成四个小区域
	Mat Qlt(amplitude, Rect(0, 0, centerX, centerY));//ROI区域的左上
	Mat Qrt(amplitude, Rect(centerX, 0, centerX, centerY));//ROI区域的右上
	Mat Qlb(amplitude, Rect(0, centerY, centerX, centerY));//ROI区域的左下
	Mat Qrb(amplitude, Rect(centerX, centerY, centerX, centerY));//ROI区域的右下

	//交换象限，左上和右下进行交换
	Mat med;
	Qlt.copyTo(med);
	Qrb.copyTo(Qlt);
	med.copyTo(Qrb);
	//交换象限，左下和右上进行交换
	Qrt.copyTo(med);
	Qlb.copyTo(Qrt);
	med.copyTo(Qlb);

	imshow("中心化后的幅值图像", amplitude);
	waitKey(0);
	return 0;
}