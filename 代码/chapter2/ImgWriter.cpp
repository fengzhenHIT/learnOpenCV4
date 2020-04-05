#include <iostream>
#include <opencv2\opencv.hpp>

using	namespace std;
using	namespace cv;

void AlphaMat(Mat &mat)
{
	CV_Assert(mat.channels() == 4);
	for (int i = 0; i < mat.rows; ++i)
		{
			for (int j = 0; j < mat.cols; ++j)
			{
				Vec4b& bgra = mat.at<Vec4b>(i, j);
				bgra[0] = UCHAR_MAX;  // 蓝色通道
				bgra[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX);  // 绿色通道
				bgra[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX);  // 红色通道
				bgra[3] = saturate_cast<uchar>(0.5 * (bgra[1] + bgra[2]));  // Alpha通道
			}
		}
}
int main(int agrc, char** agrv)
{
	// Create mat with alpha channel
	Mat mat(480, 640, CV_8UC4);
	AlphaMat(mat);
	vector<int> compression_params;
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);  //PNG格式图像压缩标志
	compression_params.push_back(9);  //设置最高压缩质量		
	bool result = imwrite("alpha.png", mat, compression_params);
	if (!result)
	{
		cout << "保存成PNG格式图像失败" << endl;
		return -1;
	}
	cout << "保存成功" << endl;
	return 0;
}
