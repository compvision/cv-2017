#include <opencv2/opencv.hpp>
#include <iostream>

#include "TargetDetector.hpp"
#include "Target.hpp"
#include "VideoDevice.hpp"

//using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
  VideoDevice* camera = new VideoDevice();
  int dev = atoi(argv[1]);
  camera->startCapture(dev);
  Mat input;
  int count = 1;

  while(cv::waitKey(30) != 27)
  {

    input = camera->getImage();

    TargetDetector detector;
    Target* t = detector.processImage(input, true);
    if (t != NULL)
    {
      t -> printPoints();
    } else
    {
      std::cout << "Nothing Found" << std::endl;
    }
    std::cout << "While Loop #" << count << std::endl;
    count += 1;

    delete t;
  }
  waitKey(0);
}
