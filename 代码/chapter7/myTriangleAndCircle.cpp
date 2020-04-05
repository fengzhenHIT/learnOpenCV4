#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	Mat img(500, 500, CV_8UC3, Scalar::all(0));
	RNG& rng = theRNG();  //生成随机点

	while (true)
	{
		int i, count = rng.uniform(1, 101);
		vector<Point> points;
		//生成随机点
		for (i = 0; i < count; i++)
		{
			Point pt;
			pt.x = rng.uniform(img.cols / 4, img.cols * 3 / 4);
			pt.y = rng.uniform(img.rows / 4, img.rows * 3 / 4);
			points.push_back(pt);
		}

		//寻找包围点集的三角形 
		vector<Point2f> triangle;
		double area = minEnclosingTriangle(points, triangle);

		//寻找包围点集的圆形
		Point2f center;
		float radius = 0;
		minEnclosingCircle(points, center, radius);

		//创建两个图片用于输出结果
		img = Scalar::all(0);
		Mat img2;
		img.copyTo(img2);

		//在图像中绘制坐标点
		for (i = 0; i < count; i++)
		{
			circle(img, points[i], 3, Scalar(255, 255, 255), FILLED, LINE_AA);
			circle(img2, points[i], 3, Scalar(255, 255, 255), FILLED, LINE_AA);
		}
			
		//绘制三角形
		for (i = 0; i < 3; i++)
		{
			if (i==2)
			{
				line(img, triangle[i], triangle[0], Scalar(255, 255, 255), 1, 16);
				break;
			}
			line(img, triangle[i], triangle[i + 1], Scalar(255, 255, 255), 1, 16);
		}

		//绘制圆形
		circle(img2, center, cvRound(radius), Scalar(255, 255, 255), 1, LINE_AA);

		//输出结果
		imshow("triangle", img);
		imshow("circle", img2);

		//按q键或者ESC键退出程序
		char key = (char)waitKey();
		if (key == 27 || key == 'q' || key == 'Q')
		{
			break;
		}
	}
	return 0;
}