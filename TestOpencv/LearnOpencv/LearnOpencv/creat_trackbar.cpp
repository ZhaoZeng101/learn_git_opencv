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
