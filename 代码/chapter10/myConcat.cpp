#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	//矩阵数组的横竖连接
	Mat matArray[] = {Mat(1, 2, CV_32FC1, cv::Scalar(1)),
	                  Mat(1, 2, CV_32FC1, cv::Scalar(2))};
	Mat vout,hout;
	vconcat(matArray, 2, vout);
	cout << "图像数组竖向连接：" << endl << vout << endl;
	hconcat(matArray, 2, hout);
	cout << "图像数组横向连接：" << endl << hout << endl;
	
	//矩阵的横竖拼接
	Mat A = (cv::Mat_<float>(2, 2) << 1, 7, 2, 8);
	Mat B = (cv::Mat_<float>(2, 2) << 4, 10, 5, 11);
	Mat vC,hC;
	vconcat(A, B, vC);
	cout << "多个图像竖向连接：" << endl << vC << endl;
	hconcat(A, B, hC);
	cout << "多个图像横向连接：" << endl << hC << endl;

	//读取4个子图像，00表示左上角、01表示右上角、10表示左下角、11表示右下角
	Mat img00 = imread("lena00.png");
	Mat img01 = imread("lena01.png");
	Mat img10 = imread("lena10.png");
	Mat img11 = imread("lena11.png");
	//显示4个子图像
	imshow("img00", img00);
	imshow("img01", img01);
	imshow("img10", img10);
	imshow("img11", img11);

	//图像连接
	Mat img, img0, img1;
	//图像横向连接
	hconcat(img00, img01, img0);
	hconcat(img10, img11, img1);
	//横向连接结果再进行竖向连接
	vconcat(img0, img1, img);

	//显示连接图像的结果
	imshow("img0", img0);
	imshow("img1", img1);
	imshow("img", img);
	waitKey(0);
	return 0;
}