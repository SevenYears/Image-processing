/**
* file main.cpp
* function show image
*/
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;


int main(void)
{
	cout << CV_VERSION << endl;
	namedWindow("Image", WINDOW_AUTOSIZE);

	/// Load the source image
	Mat image = imread("../data/lena.jpg", IMREAD_COLOR);
	imshow("Image", image);

	waitKey(0);

	return 0;
}