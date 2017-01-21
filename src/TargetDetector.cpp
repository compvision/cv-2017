#include "TargetDetector.hpp"
#include "Target.hpp"
#include <iostream>
//public methods

TargetDetector::TargetDetector() {
    //do derpy things
}

Target* TargetDetector::processImage(Mat input, bool tar) {
    GaussianBlur(input,input,Size(3,3),1,1);
    input = thresholdImage(input, 0, 102, 227, 255);
    imshow("Thresholded",input);
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

std::vector<std::vector<Point> > TargetDetector::filterContours(std::vector<std::vector<Point> > contours, bool tar)
{
//bool tar: true = gears, false = boiler
    for(unsigned int j = 0; j < contours.size(); j++)
    {
        std::vector<Point> outputContour;

        approxPolyDP(contours[j], outputContour, (cv::arcLength(cv::Mat(contours.at(j)), true) * 0.01), true);


        if (contourArea(outputContour) > 100 && outputContour.size() == 4) {
            double maxCosine = 0;
            int tarNum = 0;
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
              int tarNum = 0;
              std::vector<cv::Point> outputContour1;
              std::vector<cv::Point> outputContour2;
              // Left and Right Outputs
              //std::vector<cv::Point> left; /*for testing*/
              //std::vector<cv::Point> right;
              std::vector<std::vector<cv::Point> > tempVV;
              //std::vector<cv::Point> tempV; /*for testing*/
              //tempV = outputContour;
              tempVV.push_back (outputContour);
              tempVV.push_back (outputContour);
              // TempV is put in twice to the tempVV vector to serve as a placeholder

              Target* target = new Target(tempVV);
              // TempVV is temporary and is used to call getType
              /* if the target called by the main is the same as the one found
                 and if it is gears on first time*/
              if (tar == target->getType() && tar == true && tarNum == 0) {
                outputContour1 = outputContour;
                tarNum += 1;
              }
              // Gears on second try
              else if (tar == target->getType() && tar == true && tarNum == 1) {
                outputContour2 = outputContour;
                tarNum = 0;
                // whichever has the least has the left most points
                if (outputContour1[1].x > outputContour2[1].x) {
                  fullContour.push_back (outputContour2); // first will be left, then right
                  fullContour.push_back (outputContour1);
                } else {
                  fullContour.push_back (outputContour1);
                  fullContour.push_back (outputContour2);
                }

                return fullContour;
              }
              //if boiler was called
              if (tar == target->getType() && tar == false) {

                fullContour.push_back (outputContour);
                fullContour.push_back (outputContour);
                // Both contours are the same as to not need two target constructors

                return fullContour;
              }

            }

        }
        //
    }


    return std::vector<std::vector<Point> >();
}
