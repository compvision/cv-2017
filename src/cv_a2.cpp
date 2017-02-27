#include<opencv2/opencv.hpp>
#include<iostream>

#include "TargetProcessor.hpp"
#include "VideoDevice.hpp"
#include "TargetDetector.hpp"

using namespace std;

double Altitude, Azimuth, Distance;

int main(int argc, char *argv[]){

    TargetProcessor* processor = new TargetProcessor();
    TargetDetector* detector = new TargetDetector();

    VideoDevice* cap = new VideoDevice();
   	int dev = atoi(argv[1]);
    cap -> startCapture(dev);

    cv::Mat rawImg;

    while(cv::waitKey(30) != 27)
    {
        rawImg = cap -> getImage();
        Target* t = detector->processImage(rawImg, true);
        cv::imshow("Data", rawImg);

        if(t != NULL)
        {
          processor->loadTarget(t);
          t->printPoints();

          double distance = processor->calculateDistance();
          double azimuth = processor->calculateAzimuth();
          double altitude = processor->calculateAltitude();

          string disText = "Distance: " + to_string(distance);
          string aziText = "Azimuth: " + to_string(azimuth);
          string altText = "Altitude: " + to_string(altitude);

          //cv::putText(rawImg, disText, cv::Point(10, 50), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar::all(0));
          //cv::putText(rawImg, aziText, cv::Point(10, 100), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar::all(0));
          //cv::putText(rawImg, altText, cv::Point(10, 150), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar::all(0));

          cout << disText << endl;
          cout << aziText << endl;
          cout << altText << endl;
          cout << std::endl;

        }

    }
    delete processor;
    delete detector;
    waitKey(0);
    return 0;
}
