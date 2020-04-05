#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace cv::ml;
using namespace std;

int main()
{
	system("color F0");
	//加载测试数据
	Mat data = imread("所有数据按行排列结果.png", IMREAD_ANYDEPTH);
	Mat labels = imread("标签.png", IMREAD_ANYDEPTH);
	data.convertTo(data, CV_32FC1);
	labels.convertTo(labels, CV_32SC1);

	//构建随机森林RTrees类型变量
	Ptr<RTrees> RTmodel = RTrees::create();
	//设置迭代停止条件
	RTmodel->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 100, 0.01));

	//下列参数可以缺省以加快运行速度，但是会影响准确性
	//RTmodel->setMaxDepth(10);  //最大深度
	//RTmodel->setMinSampleCount(10);  //设置最小样本数
	//RTmodel->setRegressionAccuracy(0);  //回归算法精度
	//RTmodel->setUseSurrogates(false);  //是否使用代理
	//RTmodel->setMaxCategories(15);  //最大类别数
	//RTmodel->setPriors(Mat());  //数据类型
	//RTmodel->setCalculateVarImportance(true);  //是否需要计算Var
	//RTmodel->setActiveVarCount(4);  //设置Var的数目
	
	Ptr<TrainData> trainData = TrainData::create(data, ROW_SAMPLE, labels);
	RTmodel->train(trainData);
	RTmodel->save("RTrees_model.yml");

	//利用原数据进行测试
	Mat result;
	RTmodel->predict(data, result);
	int count = 0;
	for (int row = 0; row < result.rows; row++)
	{
		int predict = result.at<float>(row, 0);
		if (labels.at<int>(row, 0) == predict)
		{
			count = count + 1;
		}
	}
	float rate = 1.0*count / result.rows;
	cout << "分类的正确性：" << rate << endl;

	Mat testImg1 = imread("handWrite01.png", IMREAD_GRAYSCALE);
	Mat testImg2 = imread("handWrite02.png", IMREAD_GRAYSCALE);
	imshow("testImg1", testImg1);
	imshow("testImg2", testImg2);

	//缩放到20×20的尺寸
	resize(testImg1, testImg1, Size(20, 20));
	resize(testImg2, testImg2, Size(20, 20));

	//将测试数据按要求存储
	Mat testdata = Mat::zeros(2, 400, CV_8UC1);
	Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.height = 1;
	rect.width = 400;
	Mat oneDate = testImg1.reshape(1, 1);
	Mat twoData = testImg2.reshape(1, 1);
	oneDate.copyTo(testdata(rect));
	rect.y = 1;
	twoData.copyTo(testdata(rect));
	//数据类型转换
	testdata.convertTo(testdata, CV_32F);

	//进行估计识别
	Mat result2;
	RTmodel->predict(testdata, result2);

	//查看预测的结果
	for (int i = 0; i< result2.rows; i++)
	{
		int predict = result2.at<float>(i, 0);
		cout << "第" << i + 1 << "图像预测结果：" << predict
			<< "  真实结果：" << i + 1 << endl;
	}

	waitKey(0);
	return true;
}