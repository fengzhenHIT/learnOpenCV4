#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) 
{
	VideoCapture capture("vtest.avi");
	Mat prevFrame, prevGray;
	if (!capture.read(prevFrame))
	{
		cout << "请确认视频文件名称是否正确" << endl;
		return -1;
	}

	//将彩色图像转换成灰度图像
	cvtColor(prevFrame, prevGray, COLOR_BGR2GRAY);
	
	while (true) 
	{	
		Mat nextFrame, nextGray;
		//所有图像处理完成后推出程序
		if (!capture.read(nextFrame))
		{
			break;
		}
		imshow("视频图像", nextFrame);

		//计算稠密光流
		cvtColor(nextFrame, nextGray, COLOR_BGR2GRAY);
		Mat_<Point2f> flow;  //两个方向的运动速度
		calcOpticalFlowFarneback(prevGray, nextGray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
	
		Mat xV = Mat::zeros(prevFrame.size(), CV_32FC1);  //x方向移动速度
		Mat yV = Mat::zeros(prevFrame.size(), CV_32FC1);  //y方向移动速度
		//提取两个方向的速度
		for (int row = 0; row < flow.rows; row++)
		{
			for (int col = 0; col < flow.cols; col++)
			{
				const Point2f& flow_xy = flow.at<Point2f>(row, col);
				xV.at<float>(row, col) = flow_xy.x;
				yV.at<float>(row, col) = flow_xy.y;
			}
		}
		
		//计算向量角度和幅值
		Mat magnitude, angle;
		cartToPolar(xV, yV, magnitude, angle);

		//讲角度转换成角度制
		angle = angle * 180.0 / CV_PI / 2.0;

		//把幅值归一化到0-255区间便于显示结果
		normalize(magnitude, magnitude, 0, 255, NORM_MINMAX);

		//计算角度和幅值的绝对值
		convertScaleAbs(magnitude, magnitude);
		convertScaleAbs(angle, angle);

		//讲运动的幅值和角度生成HSV颜色空间的图像
		Mat HSV = Mat::zeros(prevFrame.size(), prevFrame.type());
		vector<Mat> result;
		split(HSV, result);
		result[0] = angle;  //决定颜色
		result[1] = Scalar(255);
		result[2] = magnitude;  //决定形态
		//将三个多通道图像合并成三通道图像
		merge(result, HSV);
		
		//讲HSV颜色空间图像转换到RGB颜色空间中
		Mat rgbImg;
		cvtColor(HSV, rgbImg, COLOR_HSV2BGR);
		
		//显示检测结果
		imshow("运动检测结果", rgbImg);
		int ch = waitKey(5);
		if (ch == 27) 
		{
			break;
		}
	}
	waitKey(0);
	return 0;
}