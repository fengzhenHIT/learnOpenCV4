#include <opencv2/opencv.hpp>"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, const char** argv)
{
	//打开视频文件，并判断是否成功打开
	VideoCapture cap("vtest.avi");
	if (!cap.isOpened())
	{
		cout << "请确认输入的视频文件名是否正确" << endl;
		return -1;
	}

	//是否已经计算目标区域直方图标志，0表示没有计算，1表示已经计算
	int trackObject = 0;  
	
	//计算直方图和反向直方图相关参数
	int hsize = 16;
	float hranges[] = { 0,180 };
	const float* phranges = hranges;
	
	//选择目标区域
	Mat frame, hsv, hue, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
	cap.read(frame);
	Rect selection = selectROI("选择目标跟踪区域", frame, true, false);
	
	while (true)
	{
		//判断是否读取了全部图像
		if (!cap.read(frame))
		{
			break;
		}
		//将图像转化成HSV颜色空间
		cvtColor(frame, hsv, COLOR_BGR2HSV);

		//定义计算直方图和反向直方图相关数据和图像
		int ch[] = { 0, 0 };
		hue.create(hsv.size(), hsv.depth());
		mixChannels(&hsv, 1, &hue, 1, ch, 1);

		//是否已经完成跟踪目标直方图的计算
		if (trackObject <= 0)
		{
			//目标区域的HSV颜色空间
			Mat roi(hue, selection);
			//计算直方图和直方图归一化
			calcHist(&roi, 1, 0, roi, hist, 1, &hsize, &phranges);
			normalize(hist, hist, 0, 255, NORM_MINMAX);
			
			//将标志设置为1，不再计算目标区域的直方图
			trackObject = 1; // Don't set up again, unless user selects new ROI

			//显示目标区域的直方图，可以将注释掉，不影响跟踪效果
			histimg = Scalar::all(0);
			int binW = histimg.cols / hsize;
			Mat buf(1, hsize, CV_8UC3);
			for (int i = 0; i < hsize; i++)
				buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180. / hsize), 255, 255);
			cvtColor(buf, buf, COLOR_HSV2BGR);

			for (int i = 0; i < hsize; i++)
			{
				int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows / 255);
				rectangle(histimg, Point(i*binW, histimg.rows),
					Point((i + 1)*binW, histimg.rows - val),
					Scalar(buf.at<Vec3b>(i)), -1, 8);
			}
		}

		// 计算目标区域的方向直方图
		calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
		
		//均值迁移法跟踪目标
		meanShift(backproj, selection, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
		//再图像中绘制寻找到的跟踪窗口
		rectangle(frame, selection, Scalar(0, 0, 255), 3, LINE_AA);

		//显示结果
		imshow("CamShift Demo", frame);  //显示跟踪结果
		imshow("Histogram", histimg);  //显示目标区域直方图

		//按ESC键退出程序
		char c = (char)waitKey(50);
		if (c == 27)
			break;
	}

	return 0;
}