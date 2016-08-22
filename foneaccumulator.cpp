#include "foneaccumulator.h"
#include <iostream>
int FoneAccumulator::dispThreshold;
int FoneAccumulator::maxN;

FoneAccumulator::FoneAccumulator(size_t height, size_t width )
{
	dispThreshold = 60;
	maxN = 100;
	
	disp = new cv::Mat(width, height, CV_8UC1);
	n = new cv::Mat(width, height, CV_8UC1);
	accumulator = new cv::Mat(width, height, CV_32F);
	tracked = new cv::Mat(width, height, CV_8UC1);
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			accumulator->at<uchar>(i, j) = 0;
			disp->at<uchar>(i, j) = 0;
			n->at<uchar>(i, j) = 0;
			tracked->at<uchar>(i, j) = 0;
		}
	
	this->width = width;
	this->height = height;
}

void FoneAccumulator::accumulateAndTrack(cv::Mat *nextFrame)
{
	accumulate(nextFrame);
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		  {
			if (tracked->at<uchar>(i, j) == 255 && disp->at<uchar>(i, j) > dispThreshold)
			{
				disp->at<uchar>(i, j) = 0;
			}
		  }
	getForegroundMask(*tracked);
}

void FoneAccumulator::accumulate(cv::Mat *nextFrame)
{
	// each pixel
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			uint accumulated = accumulator->at<uint>(x, y);
			uchar N = n->at<uchar>(x, y);
			uint m = 0;
			if (N!=0)
			{
				m = accumulated / N;
			}
	
			uchar addition = nextFrame->at<uchar>(x, y);
			uchar tmp_disp = abs(m - addition);/*(m - addition);*/
	
	
			// TODO: use statistical approach (with N used)
			if (tmp_disp < dispThreshold || N == 0)
	
			{
				tracked->at<uchar>(x, y) = 0;
				uint newValue = accumulated + addition;
				if (N == maxN)
				{
					// normalizing accumulator
					newValue = (uint) ((float)(newValue) / (N+1) * (N+0));
				}
				else
				{
					n->at<uchar>(x, y) = N + 1;
				}
	
				accumulator->at<uint>(x, y) = newValue;
				disp->at<uchar>(x, y) = abs(m - addition);/*(m - addition);*/
			}
			else
			{
				tracked->at<uchar>(x, y) = 255;
			}
		}
	}
}

void FoneAccumulator::getForegroundMask(cv::Mat& thresholded)
{
    threshold(*disp, thresholded, dispThreshold, (uchar) 255, 0);
}

FoneAccumulator::~FoneAccumulator()
{
    delete accumulator;
    delete disp;
    delete n;
}
