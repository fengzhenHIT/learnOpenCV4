#include <iostream>
#include <opencv2\opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;  

void match_min(vector<DMatch> matches, vector<DMatch> & good_matches)
{
	double min_dist = 10000, max_dist = 0;
	for (int i = 0; i < matches.size(); i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	cout << "min_dist=" << min_dist << endl;
	cout << "max_dist=" << max_dist << endl;

	for (int i = 0; i < matches.size(); i++)
		if (matches[i].distance <= max(2 * min_dist, 20.0))
			good_matches.push_back(matches[i]);
}

//RANSAC算法实现
void ransac(vector<DMatch> matches, vector<KeyPoint> queryKeyPoint, vector<KeyPoint> trainKeyPoint, vector<DMatch> &matches_ransac)
{
	//定义保存匹配点对坐标
	vector<Point2f> srcPoints(matches.size()), dstPoints(matches.size());
	//保存从关键点中提取到的匹配点对的坐标
	for (int i = 0; i<matches.size(); i++)
	{
		srcPoints[i] = queryKeyPoint[matches[i].queryIdx].pt;
		dstPoints[i] = trainKeyPoint[matches[i].trainIdx].pt;
	}
	
	//匹配点对进行RANSAC过滤
	vector<int> inliersMask(srcPoints.size());
	//Mat homography;
	//homography = findHomography(srcPoints, dstPoints, RANSAC, 5, inliersMask);
	findHomography(srcPoints, dstPoints, RANSAC, 5, inliersMask);
	//手动的保留RANSAC过滤后的匹配点对
	for (int i = 0; i<inliersMask.size(); i++)
		if (inliersMask[i])
			matches_ransac.push_back(matches[i]);
}

void orb_features(Mat &gray, vector<KeyPoint> &keypionts, Mat &descriptions)
{
	Ptr<ORB> orb = ORB::create(1000, 1.2f);
	orb->detect(gray, keypionts);
	orb->compute(gray, keypionts, descriptions);
}

int main()
{
	Mat img1 = imread("box.png");  //读取图像，根据图片所在位置填写路径即可
	Mat img2 = imread("box_in_scene.png");
	if (!(img1.data && img2.data))
	{
		cout << "读取图像错误，请确认图像文件是否正确" << endl;
		return -1;
	}

	//提取ORB特征点
	vector<KeyPoint> Keypoints1, Keypoints2;
	Mat descriptions1, descriptions2;

	//基于区域分割的ORB特征点提取
	orb_features(img1, Keypoints1, descriptions1);
	orb_features(img2, Keypoints2, descriptions2);

	//特征点匹配
	vector<DMatch> matches, good_min,good_ransac;
	BFMatcher matcher(NORM_HAMMING);
	matcher.match(descriptions1, descriptions2, matches);
	cout << "matches=" << matches.size() << endl;

	//最小汉明距离
	match_min(matches, good_min);
	cout << "good_min=" << good_min.size() << endl;

	//用ransac算法筛选匹配结果
	ransac(good_min, Keypoints1, Keypoints2, good_ransac);
	cout << "good_matches.size=" << good_ransac.size() << endl;

	//绘制匹配结果
	Mat outimg, outimg1, outimg2;
	drawMatches(img1, Keypoints1, img2, Keypoints2, matches, outimg);
	drawMatches(img1, Keypoints1, img2, Keypoints2, good_min, outimg1);
	drawMatches(img1, Keypoints1, img2, Keypoints2, good_ransac, outimg2);
	imshow("未筛选结果", outimg);
	imshow("最小汉明距离筛选", outimg1);
	imshow("ransac筛选", outimg2);
	waitKey(0);  //等待键盘输入
	return 0;  //程序结束
}