#if 0

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

/// 全局变量
Mat src, hsv;
Mat hue;
int bins = 25;

/// 函数申明
void Hist_and_Backproj(int, void*);

/** @函数 main */
int main(int argc, char** argv)
{
	/// 读取图像
	src = imread("../images/lena512color.tiff", 1);
	/// 转换到 HSV 空间
	cvtColor(src, hsv, CV_BGR2HSV);

	/// 分离 Hue 通道
	hue.create(hsv.size(), hsv.depth());
	int ch[] = { 0, 0 };
	mixChannels(&hsv, 1, &hue, 1, ch, 1);

	/// 创建 Trackbar 来输入bin的数目
	char* window_image = "Source image";
	namedWindow(window_image, CV_WINDOW_AUTOSIZE);
	createTrackbar("* Hue  bins: ", window_image, &bins, 180, Hist_and_Backproj);
	Hist_and_Backproj(0, 0);

	/// 现实图像
	imshow(window_image, src);

	/// 等待用户反应
	waitKey(0);
	return 0;
}


/**
* @函数 Hist_and_Backproj
* @简介：Trackbar事件的回调函数
*/
void Hist_and_Backproj(int, void*)
{
	MatND hist;
	int histSize = MAX(bins, 2);
	float hue_range[] = { 0, 180 };
	const float* ranges = { hue_range };

	/// 计算直方图并归一化
	calcHist(&hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	/// 计算反向投影
	MatND backproj;
	calcBackProject(&hue, 1, 0, hist, backproj, &ranges, 1, true);

	/// 显示反向投影
	imshow("BackProj", backproj);

	/// 显示直方图
	int w = 400; int h = 400;
	int bin_w = cvRound((double)w / histSize);
	Mat histImg = Mat::zeros(w, h, CV_8UC3);

	for (int i = 0; i < bins; i++)
	{
		rectangle(histImg, Point(i*bin_w, h), Point((i + 1)*bin_w, h - cvRound(hist.at<float>(i)*h / 255.0)), Scalar(0, 0, 255), -1);
	}

	imshow("Histogram", histImg);
}


#endif // 0
