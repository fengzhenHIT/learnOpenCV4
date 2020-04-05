#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("lena.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	Mat grayfloat = Mat_<float>(gray);  //更改图像数据类型为float
	Mat kernel = (Mat_<float>(5, 5) << 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1);
	//构建输出图像
	Mat result;
	int rwidth = abs(grayfloat.rows - kernel.rows) + 1;
	int rheight = abs(grayfloat.cols - kernel.cols) + 1;
	result.create(rwidth, rheight, grayfloat.type());

	// 计算最优离散傅里叶变换尺寸
	int width = getOptimalDFTSize(grayfloat.cols + kernel.cols - 1);
	int height = getOptimalDFTSize(grayfloat.rows + kernel.rows - 1);

	//改变输入图像尺寸
	Mat tempA;
	int A_T = 0;
	int A_B = width - grayfloat.rows;
	int A_L = 0;
	int A_R = height - grayfloat.cols;
	copyMakeBorder(grayfloat, tempA, 0, A_B, 0, A_R, BORDER_CONSTANT);

	//改变滤波器尺寸
	Mat tempB;
	int B_T = 0;
	int B_B = width - kernel.rows;
	int B_L = 0;
	int B_R = height - kernel.cols;
	copyMakeBorder(kernel, tempB, 0, B_B, 0, B_R, BORDER_CONSTANT);

	//分别进行离散傅里叶变换
	dft(tempA, tempA, 0, grayfloat.rows);
	dft(tempB, tempB, 0, kernel.rows);

	//多个傅里叶变换的结果相乘
	mulSpectrums(tempA, tempB, tempA, DFT_COMPLEX_OUTPUT);

	//相乘结果进行逆变换
	//dft(tempA, tempA, DFT_INVERSE | DFT_SCALE, result.rows);
	idft(tempA, tempA, DFT_SCALE, result.rows);

	//对逆变换结果进行归一化
	normalize(tempA, tempA, 0, 1, NORM_MINMAX);

	//截取部分结果作为滤波结果
	tempA(Rect(0, 0, result.cols, result.rows)).copyTo(result);

	//显示结果
	imshow("原图像", gray);
	imshow("滤波结果", result);
	waitKey(0);
}