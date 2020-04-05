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

	//创建决策树
	Ptr<DTrees> DTmodel = DTrees::create();
	//参数设置
	DTmodel->setMaxDepth(8);  //树的最大深度
	DTmodel->setCVFolds(0);  //交叉验证次数

	//下面四个参数可以缺省，但是缺省会降低一定的精度
	//RTmodel->setUseSurrogates(false);  //是否建立替代分裂点
	//RTmodel->setMinSampleCount(2);  	//节点最小样本数量
	//RTmodel->setUse1SERule(false);  //是否严格修剪
	//RTmodel->setTruncatePrunedTree(false);  //分支是否完全移除
	
	Ptr<ml::TrainData> trainData = ml::TrainData::create(data, ml::ROW_SAMPLE, labels);
	DTmodel->train(trainData);
	DTmodel->save("DTrees_model.yml");

	//利用原数据进行测试
	Mat result;
	DTmodel->predict(data, result);
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
	DTmodel->predict(testdata, result2);

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