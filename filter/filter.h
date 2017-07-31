/**
* 实现各种滤波器，生成各种滤波器处理效果对比图
*/
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

enum FILTER_TYPE
{
    HOMOGENEOUS=0,
    GAUSSIAN=1,
    MEDIAN=2,
    BILATERAL=3,
};

void FilterImage(const Mat& src, Mat& dst, const int kernelLen, FILTER_TYPE type)
{
    switch(type)
    {
        case HOMOGENEOUS:
            cv::blur(src, dst, cv::Size(kernelLen, kernelLen), cv::Point(-1, -1));
            break;
        case GAUSSIAN:
            cv::GaussianBlur(src, dst, cv::Size(kernelLen, kernelLen), 0, 0);
            break;
        case MEDIAN:
            cv::medianBlur(src, dst, kernelLen);
            break;
        case BILATERAL:
            cv::bilateralFilter(src, dst, kernelLen, 2*kernelLen, kernelLen/2);
        default:
            break;
    }
}

void CompareFilter(const Mat& src, Mat& dst, const int kernelLen)
{
    Mat tmp(2*src.rows, 2*src.cols, src.type());
    dst = tmp.clone();
    cv::Point textLoc(src.cols/8, src.rows/8);

    Mat homoDst;
    cv::blur(src, homoDst, cv::Size(kernelLen, kernelLen), cv::Point(-1, -1));
    cv::putText(homoDst, "Homogeneous", textLoc, CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
    homoDst.copyTo(dst(cv::Rect(0, 0, src.cols, src.rows)));

    Mat gauDst;
    cv::GaussianBlur(src, gauDst, cv::Size(kernelLen, kernelLen), 0, 0);
    cv::putText(gauDst, "Gaussian", textLoc, CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
    gauDst.copyTo(dst(cv::Rect(0, src.cols, src.cols, src.rows)));

    Mat medianDst;
    cv::medianBlur(src, medianDst, kernelLen);
    cv::putText(medianDst, "Median", textLoc, CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
    medianDst.copyTo(dst(cv::Rect(src.rows, 0, src.cols, src.rows)));

    Mat biDst;
    cv::bilateralFilter(src, biDst, kernelLen, 2*kernelLen, kernelLen/2);
    cv::putText(biDst, "Bilateral", textLoc, CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
    biDst.copyTo(dst(cv::Rect(src.rows, src.cols, src.cols, src.rows)));
}