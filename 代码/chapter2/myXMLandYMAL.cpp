#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	system("color F0");  //修改运行程序背景和文字颜色
	//string fileName = "datas.xml";  //文件的名称
	string fileName = "datas.yaml";  //文件的名称
	//以写入的模式打开文件
	cv::FileStorage fwrite(fileName, cv::FileStorage::WRITE);
	
	//存入矩阵Mat类型的数据
	Mat mat = Mat::eye(3, 3, CV_8U);
	fwrite.write("mat", mat);  //使用write()函数写入数据
	//存入浮点型数据，节点名称为x
	float x = 100;
	fwrite << "x" << x;
	//存入字符串型数据，节点名称为str
	String str = "Learn OpenCV 4";
	fwrite << "str" << str;
	//存入数组,节点名称为number_array
	fwrite << "number_array" << "[" <<4<<5<<6<< "]";
	//存入多node节点数据,主名称为multi_nodes
	fwrite << "multi_nodes" << "{" << "month" << 8 << "day" << 28 << "year"
		<< 2019 << "time" << "[" << 0 << 1 << 2 << 3 << "]" << "}";

	//关闭文件
	fwrite.release();

	//以读取的模式打开文件
	cv::FileStorage fread(fileName, cv::FileStorage::READ);
	//判断是否成功打开文件
	if (!fread.isOpened())
	{
		cout << "打开文件失败，请确认文件名称是否正确！" << endl;
		return -1;
	}

	//读取文件中的数据
	float xRead;
	fread["x"] >> xRead;  //读取浮点型数据
	cout << "x=" << xRead << endl;

	//读取字符串数据
	string strRead;
	fread["str"] >> strRead;
	cout << "str=" << strRead << endl;

	//读取含多个数据的number_array节点
	FileNode fileNode = fread["number_array"];
	cout << "number_array=[";
	//循环遍历每个数据
	for (FileNodeIterator i = fileNode.begin(); i != fileNode.end(); i++)
	{
		float a;
		*i >> a;
		cout << a<<" ";
	}
	cout << "]" << endl;

	//读取Mat类型数据
	Mat matRead;
	fread["mat="] >> matRead;
	cout << "mat=" << mat << endl;

	//读取含有多个子节点的节点数据，不使用FileNode和迭代器进行读取
	FileNode fileNode1 = fread["multi_nodes"];
	int month = (int)fileNode1["month"];
	int day = (int)fileNode1["day"];
	int year = (int)fileNode1["year"];
	cout << "multi_nodes:" << endl 
		<< "  month=" << month << "  day=" << day << "  year=" << year;
	cout << "  time=[";
	for (int i = 0; i < 4; i++)
	{
		int a = (int)fileNode1["time"][i];
		cout << a << " ";
	}
	cout << "]" << endl;
	
	//关闭文件
	fread.release();
	return 0;
}
