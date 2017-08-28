/**
* For play
*/
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

//木雕效果
Mat WoodCarvings(const Mat& input, const int threshold = 30)
{ 
	//如果当前点的像素值与临近任意一点的像素值大于阈值（这里默认设置为30），置白色；否则，置黑色  
	Mat dst(input.rows, input.cols, CV_8UC1);
	Mat gray;
	cvtColor(input, gray, CV_RGB2GRAY);
	for (int i = 1; i<gray.rows - 1; i++)
	{
		uchar *current = gray.ptr<uchar>(i);
		uchar *currentBefore = gray.ptr<uchar>(i - 1);
		uchar *currentNext = gray.ptr<uchar>(i + 1);
		uchar *dstLine = dst.ptr<uchar>(i);
		for (int j = 1; j<gray.cols - 1; j++)
		{
			int a, b, c, d, e, f, g, h;
			a = abs(current[j] - currentBefore[(j - 1)]);
			b = abs(current[j] - currentBefore[j]);
			c = abs(current[j] - currentBefore[(j + 1)]);
			d = abs(current[j] - currentNext[(j - 1)]);
			e = abs(current[j] - currentNext[(j)]);
			f = abs(current[j] - currentNext[(j + 1)]);
			g = abs(current[j] - current[(j - 1)]);
			h = abs(current[j] - current[(j + 1)]);

			if (a>threshold || b>threshold || c>threshold || d>threshold || e>threshold || f>threshold || g>threshold || h>threshold)
				dstLine[j] = 255;
			else
				dstLine[j] = 0;
		}
	}

	return dst;
}

//铅笔画
Mat PencilImage(const Mat& input, const int threshold=30)
{
	//如果当前点的像素值与临近任意一点的像素值大于阈值（这里默认设置为30），置黑色；否则，置白色，这里和木雕是相反的操作。
	Mat dst(input.rows, input.cols, CV_8UC1);
	Mat gray;
	cvtColor(input, gray, CV_RGB2GRAY);
	for (int i = 1; i<gray.rows - 1; i++)
	{
		uchar *current = gray.ptr<uchar>(i);
		uchar *currentBefore = gray.ptr<uchar>(i - 1);
		uchar *currentNext = gray.ptr<uchar>(i + 1);
		uchar *dstLine = dst.ptr<uchar>(i);
		for (int j = 1; j<gray.cols - 1; j++)
		{
			int a, b, c, d, e, f, g, h;
			a = abs(current[j] - currentBefore[(j - 1)]);
			b = abs(current[j] - currentBefore[j]);
			c = abs(current[j] - currentBefore[(j + 1)]);
			d = abs(current[j] - currentNext[(j - 1)]);
			e = abs(current[j] - currentNext[(j)]);
			f = abs(current[j] - currentNext[(j + 1)]);
			g = abs(current[j] - current[(j - 1)]);
			h = abs(current[j] - current[(j + 1)]);

			if (a>threshold || b>threshold || c>threshold || d>threshold || e>threshold || f>threshold || g>threshold || h>threshold)
				dstLine[j] = 0;
			else
				dstLine[j] = 255;
		}
	}

	return dst;
}

//素描
Mat Sketch(const Mat& input)
{
	Mat src;
	input.copyTo(src);
	int width = src.cols;
	int heigh = src.rows;
	Mat gray0, gray1;
	cvtColor(src, gray0, CV_BGR2GRAY);   //灰度处理
	addWeighted(gray0, -1, NULL, 0, 255, gray1);    //反色
	// threshold(gray0,gray1,128,255,THRESH_BINARY_INV);
	GaussianBlur(gray1, gray1, Size(11, 11), 0);   //高斯处理一下
	Mat dst(gray1.size(), CV_8UC1);
	for (int y = 0; y<heigh; y++)
	{
		uchar* P0 = gray0.ptr<uchar>(y);
		uchar* P1 = gray1.ptr<uchar>(y);
		uchar* P = dst.ptr<uchar>(y);
		for (int x = 0; x<width; x++)
		{
			int tmp0 = P0[x];
			int tmp1 = P1[x];
			P[x] = (uchar)min((tmp0 + (tmp0*tmp1) / (256 - tmp1)), 255);   //计算
		}
	}
	return dst;
}

// 油画
Mat OilPaint(const Mat& input, int brushSize = 2, int coarseness = 200)
{
	if (brushSize < 1) brushSize = 1;
	if (brushSize > 8) brushSize = 8;

	if (coarseness < 1) coarseness = 1;
	if (coarseness > 255) coarseness = 255;

	int width = input.cols;
	int height = input.rows;

	int lenArray = coarseness + 1;
	int* CountIntensity = new int[lenArray];
	uint* RedAverage = new uint[lenArray];
	uint* GreenAverage = new uint[lenArray];
	uint* BlueAverage = new uint[lenArray];

	/// 图像灰度化
	Mat gray;
	cvtColor(input, gray, COLOR_BGR2GRAY);

	/// 目标图像
	Mat dst = Mat::zeros(input.size(), input.type());

	for (int nY = 0; nY <height; nY++)
	{
		// 油画渲染范围上下边界
		int top = nY - brushSize;
		int bottom = nY + brushSize + 1;

		if (top<0) top = 0;
		if (bottom >= height) bottom = height - 1;

		for (int nX = 0; nX<width; nX++)
		{
			// 油画渲染范围左右边界
			int left = nX - brushSize;
			int right = nX + brushSize + 1;

			if (left<0) left = 0;
			if (right >= width) right = width - 1;

			//初始化数组
			for (int i = 0; i <lenArray; i++)
			{
				CountIntensity[i] = 0;
				RedAverage[i] = 0;
				GreenAverage[i] = 0;
				BlueAverage[i] = 0;
			}

			// 下面这个内循环类似于外面的大循环
			// 也是油画特效处理的关键部分
			for (int j = top; j<bottom; j++)
			{
				for (int i = left; i<right; i++)
				{
					uchar intensity = static_cast<uchar>(coarseness*gray.at<uchar>(j, i) / 255.0);
					CountIntensity[intensity]++;

					RedAverage[intensity] += input.at<Vec3b>(j, i)[2];
					GreenAverage[intensity] += input.at<Vec3b>(j, i)[1];
					BlueAverage[intensity] += input.at<Vec3b>(j, i)[0];
				}
			}

			// 求最大值，并记录下数组索引
			uchar chosenIntensity = 0;
			int maxInstance = CountIntensity[0];
			for (int i = 1; i<lenArray; i++)
			{
				if (CountIntensity[i]>maxInstance)
				{
					chosenIntensity = (uchar)i;
					maxInstance = CountIntensity[i];
				}
			}

			dst.at<Vec3b>(nY, nX)[2] = static_cast<uchar>(RedAverage[chosenIntensity] / static_cast<float>(maxInstance));
			dst.at<Vec3b>(nY, nX)[1] = static_cast<uchar>(GreenAverage[chosenIntensity] / static_cast<float>(maxInstance));
			dst.at<Vec3b>(nY, nX)[0] = static_cast<uchar>(BlueAverage[chosenIntensity] / static_cast<float>(maxInstance));
		}

	}

	delete[] CountIntensity;
	delete[] RedAverage;
	delete[] GreenAverage;
	delete[] BlueAverage;

	return dst;
}