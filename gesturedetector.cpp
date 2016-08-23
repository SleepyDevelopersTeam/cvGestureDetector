#include "gesturedetector.h"

GestureDetector::GestureDetector()
{
    critDeltaWidth = 60;
    critHeight = 30;
    prevWidth = INT_MAX;
}

bool GestureDetector::detect(cv::Mat& frame)
{
    bool result = false;
    int currentHorWhite = 0;
    int MaxHorWhite = 0;
    for (int x = 0; x < frame.cols; ++x)
    {
        int maxWhite = 0;
        int currentWhite = 0;
        for (int y = 0; y < frame.rows; ++y)
        {
            if (frame.at<uchar>(y,x) > 0)
            {
                currentWhite++;
            }
            else
            {
                if (maxWhite<currentWhite)
                {
                    maxWhite = currentWhite;
                }
                currentWhite = 0;
            }
        }
        if (maxWhite > critHeight)
        {
            currentHorWhite++;
        }
        else
        {
            if (MaxHorWhite<currentHorWhite)
            {
                MaxHorWhite = currentHorWhite;
            }
            currentHorWhite = 0;
        }
    }

    if ((MaxHorWhite - prevWidth) > critDeltaWidth)
    {
        result = true;
    }

    prevWidth = MaxHorWhite;
    return result;
}
