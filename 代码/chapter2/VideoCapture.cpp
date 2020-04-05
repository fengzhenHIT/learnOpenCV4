#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	system("color F0");  //更改输出界面颜色
	VideoCapture video("cup.mp4");
	if (video.isOpened())
	{
		cout << "视频中图像的宽度=" << video.get(CAP_PROP_FRAME_WIDTH) << endl;
		cout << "视频中图像的高度=" << video.get(CAP_PROP_FRAME_HEIGHT) << endl;
		cout << "视频帧率=" << video.get(CAP_PROP_FPS) << endl;
		cout << "视频的总帧数=" << video.get(CAP_PROP_FRAME_COUNT);
	}
	else
	{
		cout << "请确认视频文件名称是否正确" << endl;
		return -1;
	}
	while (1)
	{
		Mat frame;
		video >> frame;
		if (frame.empty())
		{
			break;
		}
		imshow("video", frame);
		waitKey(1000 / video.get(CAP_PROP_FPS));
	}
	waitKey();
	return 0;
}