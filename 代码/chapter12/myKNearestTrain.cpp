#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace cv::ml;
using namespace std;

int main(int argc, char** argv) 
{
	Mat img = imread("digits.png");
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	// 分割为5000个cells
	Mat images = Mat::zeros(5000, 400, CV_8UC1);
	Mat labels = Mat::zeros(5000, 1, CV_8UC1);

	int index = 0;
	Rect numberImg;
	numberImg.x = 0;
	numberImg.height = 1;
	numberImg.width = 400;
	for (int row = 0; row < 50; row++) 
	{
		//从图像中分割出20×20的图像作为独立数字图像
		int label = row / 5;
		int datay = row * 20;
		for (int col = 0; col < 100; col++) 
		{
			int datax = col * 20;
			Mat number = Mat::zeros(Size(20, 20), CV_8UC1);
			for (int x = 0; x < 20; x++) 
			{
				for (int y = 0; y < 20; y++)
				{
					number.at<uchar>(x, y) = gray.at<uchar>(x + datay, y + datax);
				}
			}
			//将二维图像数据转成行数据
			Mat row = number.reshape(1, 1);
			cout << "提取第" << index + 1 << "个数据" << endl;
			numberImg.y = index;
			//添加到总数据中
			row.copyTo(images(numberImg));
			//记录每个图像对应的数字标签
			labels.at<uchar>(index, 0) = label;
			index++;
		}
	}
	imwrite("所有数据按行排列结果.png", images);
            imwrite("标签.png", labels);

	//加载训练数据集
	images.convertTo(images, CV_32FC1);
	labels.convertTo(labels, CV_32SC1);
	Ptr<ml::TrainData> tdata = ml::TrainData::create(images, ml::ROW_SAMPLE, labels);

	//创建K近邻类
	Ptr<KNearest> knn = KNearest::create();  
	knn->setDefaultK(5);  //每个类别拿出5个数据
	knn->setIsClassifier(true);  //进行分类
	
	//训练数据
	knn->train(tdata);
	//保存训练结果
	knn->save("knn_model.yml");

	//输出运行结果提示
	cout << "已使用K近邻完成数据训练和保存" << endl;

	waitKey(0);
	return true;
}