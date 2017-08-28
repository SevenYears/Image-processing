/**
* file main.cpp
* function show image
*/
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "filter/filter.h"
#include "play/play.h"

using namespace std;
using namespace cv;

void Test_Filter(const Mat& src, Mat& dst)
{
	int kernelLen = 31;
	FilterImage(src, dst, kernelLen, FILTER_TYPE::BILATERAL);
}

int main(void)
{
	cout << CV_VERSION << endl;

	Mat image = imread("../data/lena.jpg", IMREAD_COLOR);
	Mat result;

	//Test_Filter(image, result);
	//CompareFilter(image, result, 31);

	//result = Sketch(image);
	result = OilPaint(image);

	cv::imwrite("../data/res.jpg", result);

	// namedWindow("Image", WINDOW_AUTOSIZE);
	// namedWindow("Result", WINDOW_AUTOSIZE);
	// imshow("Image", image);
#ifdef _DEBUG
	imshow("Result", result);
	waitKey(0);
#endif
	
	return 0;
}