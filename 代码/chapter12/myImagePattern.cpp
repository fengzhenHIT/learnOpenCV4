#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace cv::dnn;
using namespace std;

int main() 
{
	Mat img = imread("airplane.jpg");
	if (img.empty())
	{
		printf("could not load image...\n");
		return -1;
	}

	//读取分类种类名称
	String typeListFile = "imagenet_comp_graph_label_strings.txt";
	vector<String> typeList;
	ifstream file(typeListFile);
	if (!file.is_open())
	{
		printf("请确认分类种类名称是否正确");
		return -1;
	}
	
	std::string type;
	while (!file.eof())
	{
		//读取名称
		getline(file, type);
		if (type.length())
			typeList.push_back(type);
	}
	file.close();

	// 加载网络
	String tf_pb_file = "tensorflow_inception_graph.pb";
	Net net = readNet(tf_pb_file);
	if (net.empty()) 
	{
		printf("请确认模型文件是否为空文件");
		return -1;
	}

	//对输入图像数据进行处理
	Mat blob = blobFromImage(img, 1.0f, Size(224, 224), Scalar(), true, false);

	//进行图像种类预测
	Mat prob;
	net.setInput(blob, "input");
	prob = net.forward("softmax2");

	// 得到最可能分类输出
	Mat probMat = prob.reshape(1, 1);
	Point classNumber;  
	double classProb;  //最大可能性
	minMaxLoc(probMat, NULL, &classProb, NULL, &classNumber);

	string typeName = typeList.at(classNumber.x).c_str();
	cout << "图像中物体可能为：" << typeName << "  可能性为：" << classProb;
		
	//检测内容
	string str = typeName + " possibility:" + to_string(classProb);
	putText(img, str, Point(50, 50), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 255), 2, 8);
	imshow("图像判断结果", img);
	waitKey(0);
	return 0;
}