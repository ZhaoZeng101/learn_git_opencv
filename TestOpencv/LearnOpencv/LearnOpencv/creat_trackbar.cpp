#if 0
#if 0
#include <iostream>
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//定义了窗体名
string winName = "二值化";

//TrackBar发生改变的回调函数
void onChangeTrackBar(int pos, void* userdata);

//主函数
int main()
{
	//trackbar名
	string trackBarName = "pos";
	//图像文件名
	string imgName = "../images\\Lena512.bmp";
	//trackbar的值
	int posTrackBar = 0;
	//trackbar的最大值
	int maxValue = 255;

	Mat img;
	//读入图像，以灰度图形式读入
	img = imread(imgName, 0);
	//新建窗口
	namedWindow(winName);
	imshow(winName, img);
	//创建trackbar，我们把img作为数据传进回调函数中
	createTrackbar(trackBarName, winName,
		&posTrackBar, maxValue,
		onChangeTrackBar, &img);

	waitKey();
	return 0;
}

// 回调函数
void onChangeTrackBar(int pos, void* usrdata)
{
	// 强制类型转换
	Mat src = *(Mat*)(usrdata);

	Mat dst;
	// 二值化
	threshold(src, dst, pos, 255, 0);

	imshow(winName, dst);
}
#endif // 0


/** 第二个例程*/
#include <iostream>
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

/// 全局变量的声明与初始化
const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;

/// 声明存储图像的变量
Mat src1;
Mat src2;
Mat dst;

/**
* @function on_trackbar
* @定义响应滑动条的回调函数
*/
void on_trackbar(int, void*)
{
	alpha = (double)alpha_slider / alpha_slider_max;
	beta = (1.0 - alpha);

	addWeighted(src1, alpha, src2, beta, 0.0, dst);

	imshow("Linear Blend", dst);
}

int main(int argc, char** argv)
{
	/// 加载图像 (两图像的大小与类型要相同)
	src1 = imread("../images/lena512.bmp");
	src2 = imread("../images/lena512color.tiff");

	if (!src1.data) { printf("Error loading src1 \n"); return -1; }
	if (!src2.data) { printf("Error loading src2 \n"); return -1; }

	/// 初始化为零
	alpha_slider = 0;

	/// 创建窗体
	namedWindow("Linear Blend", 1);

	/// 在创建的窗体中创建一个滑动条控件
	char TrackbarName[50];
	//	sprintf(TrackbarName, "Alpha x %d", alpha_slider_max);

	createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar);

	/// 结果在回调函数中显示
	on_trackbar(alpha_slider, 0);

	/// 按任意键退出
	waitKey(0);
	return 0;
}
#endif // 0
