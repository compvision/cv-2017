#include "TargetDetector.hpp"
#include "Target.hpp"
#include <iostream>
//public methods

TargetDetector::TargetDetector() {
    //do derpy things
}

Target* TargetDetector::processImage(Mat input, bool tar) {
  Target* toReturn = new Target(finalContour);
  toReturn -> setTar(tar);
    input = thresholdImage(input, 0, 102, 227, 255);
    dilate(input, input, Mat());

    std::vector<std::vector<Point> > contours = contour(input);
    // std::cout << "not contours" << std::endl;
    std::vector<std::vector<Point> > finalContour = filterContours(contours, tar);
    // std::cout << "not filterContours" << std::endl;

    if (finalContour.size() == 0) {
        return NULL;
    }
    else {
        Target* toReturn = new Target(finalContour);
        toReturn -> setTar(tar);
        return toReturn;
    }
}

Mat TargetDetector::canny(Mat input) {
    Canny(input, input, 0, 20, 3);
    return input;
}


Mat TargetDetector::thresholdImage(Mat input, int minHue, int maxHue, int minVal, int maxVal) {
    //defining variables
    Mat threshLow;
    Mat threshHigh;
    Mat hueResult;
    Mat valResult;

    //seperating image into HSV (or in this case, really just HV)
    cv::Mat cvted;
    cv::cvtColor(input, cvted, CV_BGR2HSV);

    std::vector<cv::Mat> separated(3);
    cv::split(cvted, separated);
    Mat hue = separated.at(0).clone();
    Mat val = separated.at(2).clone();

    //Hue
    threshold(hue, threshLow, minHue, 25, THRESH_BINARY);
    threshold(hue, threshHigh, maxHue, 50, THRESH_BINARY_INV);
    hueResult = threshLow & threshHigh;

    //Value
    threshold(val, threshLow, minVal, 255, THRESH_BINARY);
    threshold(val, threshHigh, maxVal, 255, THRESH_BINARY_INV);
    valResult = threshLow & threshHigh;

    Mat combined = hueResult & valResult;

    return combined;
}

std::vector<std::vector<Point> > TargetDetector::contour(Mat input) {
    std::vector<std::vector<Point> > contours;

    findContours(input, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    return contours;
}

double TargetDetector::angle(cv::Point p1, cv::Point p2, cv::Point p0) {
    double dx1 = p1.x - p0.x;
    double dy1 = p1.y - p0.y;
    double dx2 = p2.x - p0.x;
    double dy2 = p2.y - p0.y;
    return atan(dy1/dx1)-atan(dy2/dx2); //in rad
}

std::vector<Point> TargetDetector::filterContours(std::vector<std::vector<Point> > contours, bool tar) {

    for(unsigned int j = 0; j < contours.size(); j++)
    {
        std::vector<Point> outputContour;

        approxPolyDP(contours[j], outputContour, (cv::arcLength(cv::Mat(contours.at(j)), true) * 0.01), true);


        if (contourArea(outputContour) > 100 && outputContour.size() == 4) {
            double maxCosine = 0;
            int tarNum = 0
            for(int j = 2; j <=4; j++)
            {
                double cosine;
                try {
                    cosine = fabs(cos(angle(outputContour.at(j%4), outputContour.at(j-2), outputContour.at(j-1))));
                }
                catch(std::exception e){
                    std:: cout << e.what();
                }
                maxCosine = MAX(maxCosine, cosine);
            }
            //filters out contours that don't have only 90deg anlges
            if(maxCosine < .2)

            {
              std::vector<cv::Point> outputContour1
              std::vector<cv::Point> outputContour2
              std::Vector<cv::Point> * left;
              std::Vector<cv::Point> * right;
              Target target(outputContour);
              if (tar = target.getType() && tar = true && tarNum = 0) {
                outputContour1 = outputContour;
                tarNum += 1;
              } else if (tar = target.getType() && tar = true && tarNum = 1) {
                outputContour2 = outputContour;

                if (outputContour1[1].x > outputContour2[1].x) {
                  left = &outputContour2
                  right = &outputContour1
                } else {
                  left = &outputContour1
                  right = &outputContour2
                }

                fullContour.push_back (left);
                fullContour.push_back (right);

                return fullContour;
              }
              if (tar = target.getType() && tar = false) {

                left = &outputContour
                right = &outputContour

                fullContour.push_back (left);
                fullContour.push_back (right);

                return fullContour;
              }

              }

            }

        }
        //
    }


    return std::vector<Point>();
}
