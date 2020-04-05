#include <opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

int main() 
{
	//加载视频文件，并判断是否加载成功
	VideoCapture capture("bike.avi");
	if (!capture.isOpened()) {
		cout<<"请确认视频文件是否正确"<<endl;
		return -1;
	}

	//输出视频相关信息
	int fps = capture.get(CAP_PROP_FPS);
	int width = capture.get(CAP_PROP_FRAME_WIDTH);
	int height = capture.get(CAP_PROP_FRAME_HEIGHT);
	int num_of_frames = capture.get(CAP_PROP_FRAME_COUNT);
	cout << "视频宽度：" << width << " 视频高度：" << height << " 视频帧率：" << fps << " 视频总帧数" << num_of_frames << endl;

	//读取视频中第一帧图像作为前一帧图像，并进行灰度化
	Mat preFrame, preGray;
	capture.read(preFrame);
	cvtColor(preFrame, preGray, COLOR_BGR2GRAY);
	//对图像进行高斯滤波，减少噪声干扰
	GaussianBlur(preGray, preGray, Size(0, 0), 15);

	Mat binary;
	Mat frame, gray;
	//形态学操作的矩形模板
	Mat k = getStructuringElement(MORPH_RECT, Size(7, 7), Point(-1, -1));

	while (true) 
	{
		//视频中所有图像处理完后推出循环
		if (!capture.read(frame))
		{
			break;
		}

		//对当前帧进行灰度化
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gray, Size(0, 0), 15);

		//计算当前帧与前一帧的差值的绝对值
		absdiff(gray, preGray, binary);
		
		//对计算结果二值化并进行开运算，减少噪声的干扰
		threshold(binary, binary, 10, 255, THRESH_BINARY | THRESH_OTSU);
		morphologyEx(binary, binary, MORPH_OPEN, k);

		//显示处理结果
		imshow("input", frame);
		imshow("result", binary);
		
		//将当前帧变成前一帧，准备下一个循环，注释掉这句话为固定背景
		//gray.copyTo(preGray);

		//5毫秒延时判断是否推出程序，按ESC键退出
		char c = waitKey(5);
		if (c == 27) 
		{
			break;
		}
	}

	waitKey(0);
	return 0;
}