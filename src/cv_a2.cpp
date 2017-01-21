#include<opencv2/opencv.hpp>
#include<iostream>

#include "TargetProcessor.hpp"
#include "VideoDevice.hpp"

using namespace std;

double Altitude, Azimuth, Distance;

cv::Mat processor2(cv::Mat image, int minHue, int maxHue, int minVal, int maxVal){

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
    cv::Mat imageValue = HSVChannels.at(2).clone();

    cv::Mat threshLow, threshHigh, hueResult, valResult, imageResult;

    threshold(imageHue, threshLow, minHue, 255, THRESH_BINARY);
    threshold(imageHue, threshHigh, maxHue, 255, THRESH_BINARY_INV);
    hueResult = threshLow & threshHigh;

    //Value
    threshold(imageValue, threshLow, minVal, 255, THRESH_BINARY);
    threshold(imageValue, threshHigh, maxVal, 255, THRESH_BINARY_INV);
    valResult = threshLow & threshHigh;

    imageResult = hueResult & valResult;

    return imageResult;
}


int main(){
    TargetProcessor processor;

    VideoDevice* cap = new VideoDevice();
    cap -> startCapture(1);

    for(;;){
        cv::Mat rawImg;
        rawImg = cap -> getImage();

        cv::Mat processedImg = processor2(rawImg, 0, 102, 227, 255);

        cv::imshow("Thingamabob", processedImg);

        cv::Canny(processedImg, processedImg, 20, 80);

        vector<vector<cv::Point> > contours;
        cv::findContours(processedImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

        int j, i;
        for(j = 0; j < contours.size(); j++)
        {
            vector <cv::Point> output;
            cv::approxPolyDP(contours[j], output, (cv::arcLength(cv::Mat(contours.at(j)), true) * 0.02), true);
            if(output.size() == 4 && cv::contourArea(output) > 150){
                cout << "Found Target\n" << endl;
                int maxX = 0, minX = 10000, maxY = 0, minY = 10000;
                for(i = 0; i < contours.size(); i++){
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
        /*cout << "Distance: " <<Distance << endl;
        cout << "Azimuth: " << Azimuth << endl;
        cout << "Altitude: " << Altitude << endl;
        */
        string disText = "Distance: " + to_string(Distance);
        string aziText = "Azimuth: " + to_string(Azimuth);
        string altText = "Altitude: " + to_string(Altitude);

        cv::putText(rawImg, disText, cv::Point(10, 50), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar::all(0));
        cv::putText(rawImg, aziText, cv::Point(10, 100), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar::all(0));
        cv::putText(rawImg, altText, cv::Point(10, 150), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar::all(0));
        cv::imshow("Data", rawImg);
        if(cv::waitKey(30) >= 0)
            break;

    }
    return 0;
}
