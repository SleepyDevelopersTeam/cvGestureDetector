#ifndef GESTUREDETECTOR_H
#define GESTUREDETECTOR_H
#include <opencv/cv.h>

class GestureDetector
{
public:
    GestureDetector();
    bool detect(cv::Mat& frame);
    int critDeltaWidth;
    int critHeight;
    int prevWidth;
};

#endif // GESTUREDETECTOR_H
