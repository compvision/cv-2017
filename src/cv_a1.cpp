#include <opencv2/opencv.hpp>
#include <iostream>

#include "TargetDetector.hpp"
#include "VideoDevice.hpp"

//using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
  VideoDevice* camera = new VideoDevice();
  //int dev = atoi(argv[1]);
  camera->startCapture(0);
  Mat input;

  while(cv::waitKey(30) != 27)
  {
    input = camera->getImage();

    TargetDetector detector;
    Target* t = detector.processImage(input, true);
    t -> printPoints();
    delete(t);
  }
  waitKey(0);
}
