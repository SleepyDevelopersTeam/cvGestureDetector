#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include "foneaccumulator.h"
#include "gesturedetector.h"
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
			acc.accumulate(&mat_frame);
			acc.getForegroundMask(mat_frame);
            cv::imshow("capture", *(acc.tracked));
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
