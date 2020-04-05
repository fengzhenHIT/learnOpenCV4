#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img;
	VideoCapture video(0);  //使用某个摄像头

	//读取视频
	//VideoCapture video;
	//video.open("cup.mp4");  

	if (!video.isOpened())  // 判断是否调用成功
	{
		cout << "打开摄像头失败，请确实摄像头是否安装成功";
		return -1;
	}

	video >> img;  //获取图像
	//检测是否成功获取图像
	if (img.empty())   //判断有没有读取图像成功
	{
		cout << "没有获取到图像" << endl;
		return -1;
	}
	bool isColor = (img.type() == CV_8UC3);  //判断相机（视频）类型是否为彩色

	VideoWriter writer;
	int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');  // 选择编码格式
	//OpenCV 4.0版本设置编码格式
	//int codec = CV_FOURCC('M', 'J', 'P', 'G'); 

	double fps = 25.0;  //设置视频帧率 
	string filename = "live.avi";  //保存的视频文件名称
	writer.open(filename, codec, fps, img.size(), isColor);  //创建保存视频文件的视频流

	if (!writer.isOpened())   //判断视频流是否创建成功
	{
		cout << "打开视频文件失败，请确实是否为合法输入" << endl;
		return -1;
	}

	while (1)
	{
		//检测是否执行完毕
		if (!video.read(img))   //判断能都继续从摄像头或者视频文件中读出一帧图像
		{
			cout << "摄像头断开连接或者视频读取完成" << endl;
			break;
		}
		writer.write(img);  //把图像写入视频流
		//writer << img;
		imshow("Live", img);  //显示图像
		char c = waitKey(50);
		if (c == 27)  //按ESC案件退出视频保存
		{
			break;
		}
	}
	// 退出程序时刻自动关闭视频流
	//video.release();
	//writer.release();	
	return 0;
}