#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

void orb_features(Mat &gray, vector<KeyPoint> &keypionts, Mat &descriptions)
{
	Ptr<ORB> orb = ORB::create(1000, 1.2f);
	orb->detect(gray, keypionts);
	orb->compute(gray, keypionts, descriptions);
}

int main()
{
	Mat img1, img2;
	img1 = imread("box.png");
	img2 = imread("box_in_scene.png");

	if (!(img1.data && img2.dataend))
	{
		cout << "读取图像错误，请确认图像文件是否正确" << endl;
		return -1;
	}

	//提取ORB特征点
	vector<KeyPoint> Keypoints1, Keypoints2;
	Mat descriptions1, descriptions2;

	//计算SURF特征点
	orb_features(img1, Keypoints1, descriptions1);
	orb_features(img2, Keypoints2, descriptions2);

	//判断描述子数据类型，如果数据类型不符需要进行类型转换，主要针对ORB特征点
	if ((descriptions1.type() != CV_32F) && (descriptions2.type() != CV_32F))
	{
		descriptions1.convertTo(descriptions1, CV_32F);
		descriptions2.convertTo(descriptions2, CV_32F);
	}

	//特征点匹配
	vector<DMatch> matches;  //定义存放匹配结果的变量
	FlannBasedMatcher matcher;  //使用默认值即可
	matcher.match(descriptions1, descriptions2, matches);
	cout << "matches=" << matches.size() << endl;  //匹配成功特征点数目


												   //寻找距离最大值和最小值，如果是ORB特征点min_dist取值需要大一些
	double max_dist = 0; double min_dist = 100;
	for (int i = 0; i < descriptions1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	cout << " Max dist :" << max_dist << endl;
	cout << " Min dist :" << min_dist << endl;

	//将最大值距离的0.4倍作为最优匹配结果进行筛选
	std::vector< DMatch > good_matches;
	for (int i = 0; i < descriptions1.rows; i++)
	{
		if (matches[i].distance < 0.40 * max_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}
	cout << "good_matches=" << good_matches.size() << endl;  //匹配成功特征点数目

															 //绘制匹配结果
	Mat outimg, outimg1;
	drawMatches(img1, Keypoints1, img2, Keypoints2, matches, outimg);
	drawMatches(img1, Keypoints1, img2, Keypoints2, good_matches, outimg1);
	imshow("未筛选结果", outimg);
	imshow("筛选结果", outimg1);

	waitKey(0);
	return 0;
}