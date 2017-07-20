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

    Mat homoDst;    
    cv::blur(src, homoDst, cv::Size(kernelLen, kernelLen), cv::Point(-1, -1));
    Mat gauDst;
    cv::GaussianBlur(src, gauDst, cv::Size(kernelLen, kernelLen), 0, 0);
    Mat medianDst;
    cv::medianBlur(src, medianDst, kernelLen);
    Mat biDst;
    cv::bilateralFilter(src, biDst, kernelLen, 2*kernelLen, kernelLen/2);

    cv::Rect homoRect(0, 0, src.cols, src.rows);
    homoDst.copyTo(dst(homoRect));

    cv::Rect gauRect(0, src.cols, src.cols, src.rows);
    gauDst.copyTo(dst(gauRect));

    cv::Rect medianRect(src.rows, 0, src.cols, src.rows);
    medianDst.copyTo(dst(medianRect));

    cv::Rect biRect(src.rows, src.cols, src.cols, src.rows);
    biDst.copyTo(dst(biRect));
}