/**
* For play
*/
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

//ľ��Ч��
Mat WoodCarvings(const Mat& input, const int threshold = 30)
{ 
	//�����ǰ�������ֵ���ٽ�����һ�������ֵ������ֵ������Ĭ������Ϊ30�����ð�ɫ�������ú�ɫ  
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

//Ǧ�ʻ�
Mat PencilImage(const Mat& input, const int threshold=30)
{
	//�����ǰ�������ֵ���ٽ�����һ�������ֵ������ֵ������Ĭ������Ϊ30�����ú�ɫ�������ð�ɫ�������ľ�����෴�Ĳ�����
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

//����
Mat Sketch(const Mat& input)
{
	Mat src;
	input.copyTo(src);
	int width = src.cols;
	int heigh = src.rows;
	Mat gray0, gray1;
	cvtColor(src, gray0, CV_BGR2GRAY);   //�Ҷȴ���
	addWeighted(gray0, -1, NULL, 0, 255, gray1);    //��ɫ
	// threshold(gray0,gray1,128,255,THRESH_BINARY_INV);
	GaussianBlur(gray1, gray1, Size(11, 11), 0);   //��˹����һ��
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
			P[x] = (uchar)min((tmp0 + (tmp0*tmp1) / (256 - tmp1)), 255);   //����
		}
	}
	return dst;
}

// �ͻ�
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

	/// ͼ��ҶȻ�
	Mat gray;
	cvtColor(input, gray, COLOR_BGR2GRAY);

	/// Ŀ��ͼ��
	Mat dst = Mat::zeros(input.size(), input.type());

	for (int nY = 0; nY <height; nY++)
	{
		// �ͻ���Ⱦ��Χ���±߽�
		int top = nY - brushSize;
		int bottom = nY + brushSize + 1;

		if (top<0) top = 0;
		if (bottom >= height) bottom = height - 1;

		for (int nX = 0; nX<width; nX++)
		{
			// �ͻ���Ⱦ��Χ���ұ߽�
			int left = nX - brushSize;
			int right = nX + brushSize + 1;

			if (left<0) left = 0;
			if (right >= width) right = width - 1;

			//��ʼ������
			for (int i = 0; i <lenArray; i++)
			{
				CountIntensity[i] = 0;
				RedAverage[i] = 0;
				GreenAverage[i] = 0;
				BlueAverage[i] = 0;
			}

			// ���������ѭ������������Ĵ�ѭ��
			// Ҳ���ͻ���Ч����Ĺؼ�����
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

			// �����ֵ������¼����������
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