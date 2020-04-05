#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void draw_lines(Mat &image, vector<Point2f> pt1, vector<Point2f> pt2);
vector<Scalar> color_lut;  //颜色查找表

int main()
{
	VideoCapture capture("mulballs.mp4");
	Mat prevframe, prevImg;
	if (!capture.read(prevframe))
	{
		cout << "请确认输入视频文件是否正确" << endl;
		return -1;
	}
	cvtColor(prevframe, prevImg, COLOR_BGR2GRAY);

	//角点检测相关参数设置
	vector<Point2f> Points;
	double qualityLevel = 0.01;
	int minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	int Corners = 5000;
	//角点检测
	goodFeaturesToTrack(prevImg, Points, Corners, qualityLevel, minDistance, Mat(),
		blockSize, useHarrisDetector, k);

	//稀疏光流检测相关参数设置
	vector<Point2f> prevPts;  //前一帧图像角点坐标
	vector<Point2f> nextPts;  //当前帧图像角点坐标
	vector<uchar> status;  //检点检测到的状态
	vector<float> err;
	TermCriteria criteria = TermCriteria(TermCriteria::COUNT
		+ TermCriteria::EPS, 30, 0.01);
	double derivlambda = 0.5;
	int flags = 0;

	//初始状态的角点
	vector<Point2f> initPoints;
	initPoints.insert(initPoints.end(), Points.begin(), Points.end());

	//前一帧图像中的角点坐标
	prevPts.insert(prevPts.end(), Points.begin(), Points.end());

	while (true)
	{
		Mat nextframe, nextImg;
		if (!capture.read(nextframe))
		{
			break;
		}
		imshow("nextframe", nextframe);

		//光流跟踪
		cvtColor(nextframe, nextImg, COLOR_BGR2GRAY);
		calcOpticalFlowPyrLK(prevImg, nextImg, prevPts, nextPts, status, err,
			Size(31, 31), 3, criteria, derivlambda, flags);

		//判断角点是否移动，如果不移动就删除
		size_t i, k;
		for (i = k = 0; i < nextPts.size(); i++)
		{
			// 距离与状态测量
			double dist = abs(prevPts[i].x - nextPts[i].x) + abs(prevPts[i].y - nextPts[i].y);
			if (status[i] && dist > 2)
			{
				prevPts[k] = prevPts[i];
				initPoints[k] = initPoints[i];
				nextPts[k++] = nextPts[i];
				circle(nextframe, nextPts[i], 3, Scalar(0, 255, 0), -1, 8);
			}
		}

		//更新移动角点数目
		nextPts.resize(k);
		prevPts.resize(k);
		initPoints.resize(k);

		// 绘制跟踪轨迹
		draw_lines(nextframe, initPoints, nextPts);
		imshow("result", nextframe);

		char c = waitKey(50);
		if (c == 27)
		{
			break;
		}

		//更新角点坐标和前一帧图像
		std::swap(nextPts, prevPts);
		nextImg.copyTo(prevImg);

		//如果角点数目少于30，就重新检测角点
		if (initPoints.size() < 30)
		{
			goodFeaturesToTrack(prevImg, Points, Corners, qualityLevel,
				minDistance, Mat(), blockSize, useHarrisDetector, k);
			initPoints.insert(initPoints.end(), Points.begin(), Points.end());
			prevPts.insert(prevPts.end(), Points.begin(), Points.end());
			printf("total feature points : %d\n", prevPts.size());
		}

	}
	return 0;
}

void draw_lines(Mat &image, vector<Point2f> pt1, vector<Point2f> pt2)
{
	RNG rng(5000);
	if (color_lut.size() < pt1.size())
	{
		for (size_t t = 0; t < pt1.size(); t++)
		{
			color_lut.push_back(Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255)));
		}
	}
	for (size_t t = 0; t < pt1.size(); t++) {
		line(image, pt1[t], pt2[t], color_lut[t], 2, 8, 0);
	}
}
