#if 0
#include <iostream> // for standard I/O
#include <string> //for strings
#include <iomanip> // for controlling float print precision
#include <sstream> // string to number conversion

#include <opencv2/imgproc/imgproc.hpp> // gaussian blur
#include <opencv2/core.hpp> //basic opencv structures
#include <opencv2/highgui/highgui.hpp> //opencv window i/o

using namespace std;
using namespace cv;

double getPSNR(const Mat& I1, const Mat& I2);
Scalar getMSSIM(const Mat& I1, const Mat& I2);
int main(int argc, char *argv[], char *window_name)
{
	if (argc != 5)
	{
		cout << "Not enough parameters" << endl;
		return -1;
	}
	stringstream conv;

	const string sourceReference = argv[1], sourceCompareWith = argv[2];
	int psnrTriggerValue, delay;
	conv << argv[3] << endl << argv[4]; //put in the strings
	conv >> psnrTriggerValue >> delay; // take out the numbers

	char c;
	int frameNum = -1; //Frame conter

	VideoCapture captRefrnc(sourceReference),
		captUndTst(sourceCompareWith);
	if (!captRefrnc.isOpened())
	{
		cout << "Could not open reference" << sourceReference << endl;
		return -1;
	}

	if (!captUndTst.isOpened())
	{
		cout << "Could not open case test" << sourceCompareWith << endl;
		return -1;
	}

	Size refS = Size((int)captRefrnc.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)captRefrnc.get(CV_CAP_PROP_FRAME_HEIGHT));
	Size uTSi = Size((int)captUndTst.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)captUndTst.get(CV_CAP_PROP_FRAME_HEIGHT));

	if (refS != uTSi)
	{
		cout << "Input have different size!!! Closing." << endl;
		return -1;
	}

	const char* WIN_UT = "Under Test";
	const char* WIN_RF = "Reference";

	//Windows
	namedWindow(WIN_RF, CV_WINDOW_AUTOSIZE);
	namedWindow(WIN_UT, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_RF, 0, 0);
	cvMoveWindow(WIN_UT, refS.width, 0);

	cout << "Reference frame resolution: Width =" << refS.width << "Height=" << refS.height
		<< "of nr#: " << captRefrnc.get(CV_CAP_PROP_FRAME_COUNT) << endl;

	cout << "PSNR trigger value " <<
		setiosflags(ios::fixed) << setprecision(3) << psnrTriggerValue << endl;

	Mat frameReference, frameUnderTest;
	double psnrV;
	Scalar mssimV;

	while (true) // Show the image captured in the window and repeat
	{
		captRefrnc >> frameReference;
		captUndTst >> frameUnderTest;

		if (frameReference.empty() || frameUnderTest.empty())
		{
			cout << "< < < Game over! > > >";
			getchar();
			break;
		}

		++frameNum;
		cout << "Frame:" << frameNum << "#";

		////////////////////////////PSNR///////////////////////////////////
		psnrV = getPSNR(frameReference, frameUnderTest);
		cout << setiosflags(ios::fixed) << setprecision(3) << psnrV << "dB";

		//////////////////////////////////MISSIM/////////////////////////////
		if (psnrV < psnrTriggerValue && psnrV)
		{
			mssimV = getMSSIM(frameReference, frameUnderTest);

			cout << "MSSIM:"
				<< "R" << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[2] * 100 << "%"
				<< "G" << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[1] * 100 << "%"
				<< "G" << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[0] * 100 << "%";
		}

		cout << endl;

		////////////////////////////show image ////////////////////////////////
		imshow(WIN_RF, frameReference);
		imshow(WIN_UT, frameUnderTest);

		c = cvWaitKey(delay);
		if (c == 27) break;
	}

	return 0;
}

double getPSNR(const Mat& I1, const Mat& I2)
{
	Mat s1;
	absdiff(I1, I2, s1); // I1 -I2
	s1.convertTo(s1, CV_32F);// connot make a square on 8 bits;
	s1 = s1.mul(s1); // (I1 - I2)^2

	Scalar s = sum(s1); // sum elements per channels

	double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

	if (sse < 1e-10) // for small values return zeros
		return 0;
	else
	{
		double mse = sse / (double)(I1.channels() * I1.total());
		double psnr = 10.0 * log10((255 * 255) / mse);
		return psnr;
	}
}

Scalar getMSSIM(const Mat& i1, const Mat& i2)
{
	const double C1 = 6.5025, C2 = 58.5225;
	/**********************INITS***********************/
	int d = CV_32F;

	Mat I1, I2;
	i1.convertTo(I1, d); //cannot calculate on one byte large values
	i2.convertTo(I2, d);

	Mat I2_2 = I2.mul(I2);        // I2^2
	Mat I1_2 = I1.mul(I1);        // I1^2
	Mat I1_I2 = I1.mul(I2);        // I1 * I2
	/****************END INIT************************/

	Mat mu1, mu2; // preliminary computing
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);

	Mat mu1_2 = mu1.mul(mu1);
	Mat mu2_2 = mu2.mul(mu2);
	Mat mu1_mu2 = mu1.mul(mu2);

	Mat sigma1_2, sigma2_2, sigma12;

	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;

	GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;

	GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;

	///////////////////////////////// FORMULA ////////////////////////////////
	Mat t1, t2, t3;

	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

	Mat ssim_map;
	divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;

	Scalar mssim = mean(ssim_map); // mssim = average of ssim map
	return mssim;

}
#endif // 0
