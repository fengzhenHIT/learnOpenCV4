#include <opencv2/opencv.hpp>
#include <iostream>  

using namespace std;
using namespace cv;
using namespace cv::ml;

int main()
{
	//训练数据
	Mat samples, labls;
	FileStorage fread("point.yml", FileStorage::READ);
	fread["data"] >> samples;
	fread["labls"] >> labls;
	fread.release();

	//不同种类坐标点拥有不同的颜色
	vector<Vec3b> colors;
	colors.push_back(Vec3b(0, 255, 0));
	colors.push_back(Vec3b(0, 0, 255));

	//创建空白图像用于显示坐标点
	Mat img(480, 640, CV_8UC3, Scalar(255, 255, 255));
	Mat img2;
	img.copyTo(img2);

	//在空白图像中绘制坐标点
	for (int i = 0; i < samples.rows; i++)
	{
		Point2f point;
		point.x = samples.at<float>(i, 0);
		point.y = samples.at<float>(i, 1);
		Scalar color = colors[labls.at<int>(i, 0)];
		circle(img, point, 3, color, -1);
		circle(img2, point, 3, color, -1);
	}
	imshow("两类像素点图像", img);

	//建立模型
	Ptr<SVM> model = SVM::create();
	
	//参数设置
	model->setKernel(SVM::INTER);  //内核的模型
	model->setType(SVM::C_SVC);  //SVM的类型
	model->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 100, 0.01));
	//model->setGamma(5.383);
	//model->setC(0.01);
	//model->setDegree(3);
	
	//训练模型
	model->train(TrainData::create(samples, ROW_SAMPLE, labls));

	//用模型对图像中全部像素点进行分类
	Mat imagePoint(1, 2, CV_32FC1);
	for (int y = 0; y < img2.rows; y = y + 2)
	{
		for (int x = 0; x < img2.cols; x = x + 2)
		{
			imagePoint.at<float>(0) = (float)x;
			imagePoint.at<float>(1) = (float)y;
			int color = (int)model->predict(imagePoint);
			img2.at<Vec3b>(y, x) = colors[color];
		}
	}

	imshow("图像所有像素点分类结果", img2);
	waitKey();
	return 0;
}