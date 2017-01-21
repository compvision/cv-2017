#include<opencv2/opencv.hpp>
#include<iostream>

#include "TargetProcessor.hpp"

using namespace std;

int Altitude, Azimuth, Distance;

cv::Mat processor2(cv::Mat image, int lower, int upper){

    /*
    This function uses a given image, converts the given image into HSV,
    then splits the image, and finally thresholds the hue image to the range
    of the given hue values.
    */

    cv::Mat imageHSV;
    cv::cvtColor(image, imageHSV, CV_BGR2HSV);

    vector<cv::Mat> HSVChannels;
    cv::split(imageHSV, HSVChannels);

    cv::Mat imageHue = HSVChannels.at(0).clone();

    cv::Mat imageLower, imageUpper, imageResult;
    cv::threshold(imageHue, imageLower, lower, 255, CV_THRESH_BINARY);
    cv::threshold(imageHue, imageUpper, upper, 255, CV_THRESH_BINARY_INV);
    imageResult = imageLower & imageUpper;

    return imageResult;
}


int main(){
    TargetProcessor processor;

    VideoCapture cap(0);

    if(!cap.isOpened())
        return -1;

    for(;;){
        cv::Mat rawImg;
        cap >> rawImg;

        cv::Mat processedImg = processor2(rawImg, 25, 50);
        cv::Canny(processedImg, processedImg, 20, 80);

        vector<vector<cv::Point> > contours;
        cv::findContours(processedImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

        cv::drawContours(rawImg, contours, -1, cv::Scalar(130, 250, 255));

        for(int j = 0; j < contours.size(); j++){
            vector <cv::Point> output;
            cv::approxPolyDP(contours[j], output, (cv::arcLength(cv::Mat(contours.at(j)), true) * 0.02), true);
            if(output.size() == 4 && cv::contourArea(output) > 100){
                int maxX = 0, minX = 10000, maxY = 0, minY = 10000;
                for(int i = 0; i < contours.size(); i++){
                    if(output[i].x > maxX){
                        maxX = output[i].x;
                    }
                    if(output[i].x < minX){
                        minX = output[i].x;
                    }
                    if(output[i].y > maxY){
                        maxY = output[i].y;
                    }
                    if(output[i].y < minY){
                        minY = output[i].y;
                    }
                }
                cv::Point center;
                center.x = (maxX + minX)/2;
                center.y = (maxY + minY)/2;
                processor.temporaryGetPoints((maxX - minX), (maxY - minY), center);
                Distance = processor.calculateDistance();
                Azimuth = processor.calculateAzimuth();
                Altitude = processor.calculateAltitude();
            }
        }

        ;
        cv::putText(rawImg, "Distance: " + Distance, cv::Point(10, 10), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar::all(255), 3, 8);
        cv::putText(rawImg, "Azimuth: " + Azimuth, cv::Point(10, 30), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar::all(255), 3, 8);
        cv::putText(rawImg, "Altitude: " + Altitude, cv::Point(10, 50), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar::all(255), 3, 8);
        cv::imshow("Data", rawImg);
        if(waitKey(30) >= 0)
            break;
    }
    return 0;
}
