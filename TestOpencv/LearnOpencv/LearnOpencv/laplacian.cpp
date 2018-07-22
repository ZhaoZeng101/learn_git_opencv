#if 0
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/** @���� main */
int main(int argc, char** argv)
{
	Mat src, src_gray, dst;
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	char* window_name = "Laplace Demo";
	char* window_sobel = "sobel Demo";

	int c;

	/// װ��ͼ��
	src = imread(argv[1]);

	if (!src.data)
	{
		return -1;
	}

	/// ʹ�ø�˹�˲���������
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// ת��Ϊ�Ҷ�ͼ
	cvtColor(src, src_gray, CV_RGB2GRAY);

	/// ������ʾ����
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	namedWindow(window_sobel, CV_WINDOW_AUTOSIZE);

	/// ʹ��Laplace����
	Mat abs_dst;

	Laplacian(src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(dst, abs_dst);

	/// ��ʾ���
	imshow(window_name, abs_dst);

	waitKey(0);
	/// ���� grad_x �� grad_y ����
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat grad;
	/// �� X�����ݶ�
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// ��Y�����ݶ�
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// �ϲ��ݶ�(����)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	imshow(window_name, grad);

	waitKey(0);
	return 0;
}
#endif // 0
