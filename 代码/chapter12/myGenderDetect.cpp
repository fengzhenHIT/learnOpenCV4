#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

using namespace cv;
using namespace cv::dnn;
using namespace std;

int main() 
{
	Mat img = imread("faces.jpg");
	if (img.empty())
	{
		cout << "请确定是否输入正确的图像文件" << endl;
		return -1;
	}

	//读取人脸识别模型
	String model_bin = "ch12_face_age/opencv_face_detector_uint8.pb";
	String config_text = "ch12_face_age/opencv_face_detector.pbtxt";
	Net faceNet = readNet(model_bin, config_text);

	//读取性别检测模型
	String genderProto = "ch12_face_age/gender_deploy.prototxt";
	String genderModel = "ch12_face_age/gender_net.caffemodel";
	String genderList[] = { "Male", "Female" };
	Net genderNet = readNet(genderModel, genderProto);
	if(faceNet.empty()&&genderNet.empty())
	{
		cout << "请确定是否输入正确的模型文件" << endl;
		return -1;
	}
	
	//对整幅图像进行人脸检测
	Mat blobImage = blobFromImage(img, 1.0, Size(300, 300), Scalar(), false, false);
	faceNet.setInput(blobImage, "data");
	Mat detect = faceNet.forward("detection_out");
	//人脸概率、人脸矩形区域的位置
	Mat detectionMat(detect.size[2], detect.size[3], CV_32F, detect.ptr<float>());
	
	//对每个人脸区域进行性别检测
	int exBoundray = 25;  //每个人脸区域四个方向扩充的尺寸
	float confidenceThreshold = 0.5;  //判定为人脸的概率阈值，阈值越大准确性越高
	for (int i = 0; i < detectionMat.rows; i++) 
	{
		float confidence = detectionMat.at<float>(i, 2);  //检测为人脸的概率
		//只检测概率大于阈值区域的性别
		if (confidence > confidenceThreshold)
		{
			//网络检测人脸区域大小
			int topLx = detectionMat.at<float>(i, 3) * img.cols;
			int topLy = detectionMat.at<float>(i, 4) * img.rows;
			int bottomRx = detectionMat.at<float>(i, 5) * img.cols;
			int bottomRy = detectionMat.at<float>(i, 6) * img.rows;
			Rect faceRect(topLx, topLy, bottomRx - topLx, bottomRy - topLy);

			//将网络检测出的区域尺寸进行扩充，要注意防止尺寸在图像真实尺寸之外
			Rect faceTextRect;
			faceTextRect.x = max(0, faceRect.x - exBoundray);
			faceTextRect.y = max(0, faceRect.y - exBoundray);
			faceTextRect.width = min(faceRect.width + exBoundray, img.cols - 1);
			faceTextRect.height = min(faceRect.height + exBoundray, img.rows - 1);
			Mat face = img(faceTextRect);  //扩充后的人脸图像

			//调整面部图像尺寸
			Mat faceblob = blobFromImage(face, 1.0, Size(227, 227), Scalar(), false, false);
			//将调整后的面部图像输入到性别检测网络
			genderNet.setInput(faceblob);
			//计算检测结果
			Mat genderPreds = genderNet.forward();  //两个性别的可能性

			//性别检测结果
			float male, female;
			male = genderPreds.at<float>(0, 0);
			female = genderPreds.at<float>(0, 1);
			int classID = male > female ? 0 : 1;
			String gender = genderList[classID];

			//在原图像中绘制面部轮廓和性别
			rectangle(img, faceRect, Scalar(0, 0, 255), 2, 8, 0);
			putText(img, gender.c_str(), faceRect.tl(), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 2, 8);
		}
	}
	imshow("性别检测结果", img);
	waitKey(0);
	return 0;
}