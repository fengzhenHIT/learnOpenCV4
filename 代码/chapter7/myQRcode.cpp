#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("qrcode2.png");
	if (img.empty())	{		cout << "请确认图像文件名称是否正确" << endl;		return -1;	}	Mat gray, qrcode_bin;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	QRCodeDetector qrcodedetector;
	vector<Point> points;
	string information;
	bool isQRcode;
	isQRcode = qrcodedetector.detect(gray, points);  //识别二维码
	if (isQRcode)
	{
		//解码二维码
		information = qrcodedetector.decode(gray, points, qrcode_bin);
		cout << points << endl;  //输出二维码四个顶点的坐标
	}
	else
	{
		cout << "无法识别二维码，请确认图像时候含有二维码" << endl;
		return -1;
	}
	//绘制二维码的边框
	for (int i = 0; i < points.size(); i++)
	{
		if (i == points.size() - 1)
		{
			line(img, points[i], points[0], Scalar(0, 0, 255), 2, 8);
			break;
		}
		line(img, points[i], points[i + 1], Scalar(0, 0, 255), 2, 8);
	}
	//将解码内容输出到图片上
	putText(img, information.c_str(), Point(20, 30), 0, 1.0, Scalar(0, 0, 255), 2, 8);

	//利用函数直接定位二维码并解码
	string information2;
	vector<Point> points2;
	information2 = qrcodedetector.detectAndDecode(gray, points2);
	cout << points2 << endl;
	putText(img, information2.c_str(), Point(20, 55), 0, 1.0, Scalar(0, 0, 0), 2, 8);

	//输出结果
	imshow("result", img);
	namedWindow("qrcode_bin", WINDOW_NORMAL);
	imshow("qrcode_bin", qrcode_bin);
	waitKey(0);
	return 0;
}