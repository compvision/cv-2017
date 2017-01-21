#ifndef TARGET_DETECTOR_H
#define TARGET_DETECTOR_H

#include "Target.hpp"
#include <opencv2/opencv.hpp>
using namespace cv;

class TargetDetector
{
public:
    TargetDetector();
    Target* processImage(Mat input, bool tar);

    //thresholds, cannies, contours, filters contours
private:
    Mat thresholdImage(Mat input, int minHue, int maxHue, int minSat, int maxSat, int minVal, int maxVal);
    Mat canny(Mat input);
    double angle(cv::Point p1, cv::Point p2, cv::Point p0);
    std::vector<std::vector<Point> >  contour(Mat input);
    std::vector<std::vector<Point> > fullContour;
    std::vector<std::vector<Point> > filterContours(std::vector<std::vector<Point> > contours, bool tar);
    std::vector<Point> arcCheck1(std::vector<Point> input);
};

#endif
