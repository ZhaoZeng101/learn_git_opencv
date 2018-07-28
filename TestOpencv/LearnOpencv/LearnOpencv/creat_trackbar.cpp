#if 0
#if 0
#include <iostream>
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//�����˴�����
string winName = "��ֵ��";

//TrackBar�����ı�Ļص�����
void onChangeTrackBar(int pos, void* userdata);

//������
int main()
{
	//trackbar��
	string trackBarName = "pos";
	//ͼ���ļ���
	string imgName = "../images\\Lena512.bmp";
	//trackbar��ֵ
	int posTrackBar = 0;
	//trackbar�����ֵ
	int maxValue = 255;

	Mat img;
	//����ͼ���ԻҶ�ͼ��ʽ����
	img = imread(imgName, 0);
	//�½�����
	namedWindow(winName);
	imshow(winName, img);
	//����trackbar�����ǰ�img��Ϊ���ݴ����ص�������
	createTrackbar(trackBarName, winName,
		&posTrackBar, maxValue,
		onChangeTrackBar, &img);

	waitKey();
	return 0;
}

// �ص�����
void onChangeTrackBar(int pos, void* usrdata)
{
	// ǿ������ת��
	Mat src = *(Mat*)(usrdata);

	Mat dst;
	// ��ֵ��
	threshold(src, dst, pos, 255, 0);

	imshow(winName, dst);
}
#endif // 0


/** �ڶ�������*/
#include <iostream>
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

/// ȫ�ֱ������������ʼ��
const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;

/// �����洢ͼ��ı���
Mat src1;
Mat src2;
Mat dst;

/**
* @function on_trackbar
* @������Ӧ�������Ļص�����
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
	/// ����ͼ�� (��ͼ��Ĵ�С������Ҫ��ͬ)
	src1 = imread("../images/lena512.bmp");
	src2 = imread("../images/lena512color.tiff");

	if (!src1.data) { printf("Error loading src1 \n"); return -1; }
	if (!src2.data) { printf("Error loading src2 \n"); return -1; }

	/// ��ʼ��Ϊ��
	alpha_slider = 0;

	/// ��������
	namedWindow("Linear Blend", 1);

	/// �ڴ����Ĵ����д���һ���������ؼ�
	char TrackbarName[50];
	//	sprintf(TrackbarName, "Alpha x %d", alpha_slider_max);

	createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar);

	/// ����ڻص���������ʾ
	on_trackbar(alpha_slider, 0);

	/// ��������˳�
	waitKey(0);
	return 0;
}
#endif // 0
