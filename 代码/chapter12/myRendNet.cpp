#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace cv::dnn;
using namespace std;

int main()
{
	system("color F0");
	string model = "bvlc_googlenet.caffemodel";
	string config = "bvlc_googlenet.prototxt";

	//加载模型
	Net net = dnn::readNet(model, config);
	if (net.empty())
	{
		cout << "请确认是否输入空的模型文件" << endl;
		return -1;
	}

	// 获取各层信息
	vector<String> layerNames = net.getLayerNames();
	for (int i = 0; i < layerNames.size(); i++)
	{
		//读取每层网络的ID
		int ID = net.getLayerId(layerNames[i]);
		//读取每层网络的信息
		Ptr<Layer> layer = net.getLayer(ID);
		//输出网络信息
		cout << "网络层数：" << ID << "  网络层名称：" << layerNames[i] << endl
			<< "网络层类型：" << layer->type.c_str() << endl;
	}
	return 0;
}
