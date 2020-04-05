#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace cv::dnn;
using namespace std;

int main()
{
	Mat image = imread("lena.png");
	String models[5] = { "the_wave.t7",	"mosaic.t7", "feathers.t7", "candy.t7", "udnie.t7" };
	for (int i = 0; i < size(models); i++)
	{
		Net net = readNet(models[i]);
		imshow("原始图像", image);
		//计算图像每个通道的均值
		Scalar imgaeMean = mean(image);
		//调整图像尺寸和格式
		Mat blobImage = blobFromImage(image, 1.0, Size(256, 256), imgaeMean, false, false);

		//计算网络对原图像处理结果
		net.setInput(blobImage);
		Mat output = net.forward();

		//输出结果的尺寸和通道数
		int outputChannels = output.size[1];
		int outputRows = output.size[2];
		int outputCols = output.size[3];

		//将输出结果存放到图像中
		Mat result = Mat::zeros(Size(outputCols, outputRows), CV_32FC3);
		float* data = output.ptr<float>();
		for (int channel = 0; channel < outputChannels; channel++)
		{
			for (int row = 0; row < outputRows; row++)
			{
				for (int col = 0; col < outputCols; col++)
				{
					result.at<Vec3f>(row, col)[channel] = *data++;
				}
			}
		}

		//对迁移结果进行进一步操作处理
		//恢复图像减掉的均值
		result = result + imgaeMean;
		//对图像进行归一化，便于图像显示
		result = result / 255.0;
		//调整图像尺寸，使得与原图像尺寸相同
		resize(result, result, image.size());
		//显示结果
		imshow("第"+to_string(i)+"种风格迁移结果", result);
	}

	waitKey(0);
	return 0;
}