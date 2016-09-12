#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include "foneaccumulator.h"
#include "gesturedetector.h"

cv::Mat gauss_Filter(const cv::Mat& image)
{
    cv::Mat result_image;
    double sigma=3;
    cv::GaussianBlur(image,result_image,cv::Size(3*sigma,3*sigma),sigma);
    return result_image;
}

cv::Mat humanShowableImage;

int main(int argc, char* argv[])
{
	CvCapture* capture = cvCreateCameraCapture(CV_CAP_ANY); //cvCaptureFromCAM( 0 );
	double width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	double height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	
	IplImage* frame=0;
	
	cvNamedWindow("capture", CV_WINDOW_AUTOSIZE);
	
	FoneAccumulator acc = FoneAccumulator((size_t)width, (size_t)height);
    GestureDetector gestDetector = GestureDetector();
	int frame_number = 0;
	while(true)
	{
			frame = cvQueryFrame( capture );
			cv::Mat mat_frame = cv::cvarrToMat(frame);
			cv::cvtColor(mat_frame, mat_frame, CV_BGR2GRAY,1);
            mat_frame = gauss_Filter(mat_frame);
            //mat_frame.copyTo(humanShowableImage);
			acc.accumulate(&mat_frame);

            if (acc.forceFoneAccumulating)
            {
                // fone is being resetting now, let's reset body width accumulator too
                gestDetector.accumulator.forceLearn();
            }

            //acc.getForegroundMask(mat_frame);
            cv::bitwise_and(mat_frame, *(acc.tracked), mat_frame);
            //mat_frame.copyTo(mat_frame, *acc.tracked);
            cv::imshow("capture", mat_frame);
            if (gestDetector.detect(*(acc.tracked)))
            {
                qDebug()<<"Gesture detected!!!!!!!!!";
            }
			char c = cvWaitKey(33);
			if (c == 27)
			{
					break;
			}
			frame_number++;
	
	}
	// освобождаем ресурсы
	cvReleaseCapture( &capture );
	cvDestroyWindow("capture");
	return 0;
}
