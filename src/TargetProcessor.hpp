#ifndef TARGET_PROCESSOR_H
#define TARGET_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include "Target.hpp"

class TargetProcessor
{
public:
    TargetProcessor();
    void loadTarget(Target* target);
    void temporaryGetPoints(int Width, int Height, int Center)
    double calculateDistance();
    double calculateAzimuth();
    double calculateAltitude();

private:
    double imageTarWidth;
    double imageTarHeight;
    double GearWidth;
    double BoilerWidth;
    //double objectHeight;
    //only need width or height, not both.
    cv::Point imageTarCenter;
    double focalLength;
    double horizCenter;
    //center horizontal point of the picture
    double vertCenter;
    //center vertical point
    int Tar;

};

#endif