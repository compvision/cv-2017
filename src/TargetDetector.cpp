#include "TargetDetector.hpp"
#include "Target.hpp"
#include <iostream>
//public methods

TargetDetector::TargetDetector() {
    //do derpy things
}

Target* TargetDetector::processImage(Mat input, bool tar) {
    GaussianBlur(input,input,Size(3,3),31);
    //input = canny(thresholdImage(input,53,58,0,255,228,238));
    input = thresholdImage(input,53,58,0,255,228,238);
    
    imshow("threshold", input);
    input = canny(input);
    imshow("canny", input);
    dilate(input, input, Mat());

    std::vector<std::vector<Point> > contours = contour(input);
    // std::cout << "not contours" << std::endl;
    std::vector<std::vector<Point> > finalContour = filterContours(contours, tar);




    imshow("Contours",input);
    // std::cout << "not filterContours" << std::endl;

    if (&finalContour[0] == NULL || &finalContour[1] == NULL) {
        //std::cout << "Null" << std::endl;
        return NULL;
    }
    else {
        //std::cout << "Making Target" << std::endl;
        Target* toReturn = new Target(finalContour);
        //std::cout << "Made Target" << std::endl;
        toReturn -> setTar(tar);
        return toReturn;
    }
}

Mat TargetDetector::canny(Mat input) {
    Canny(input, input, 0, 20, 3);
    return input;
}


Mat TargetDetector::thresholdImage(Mat input, int minHue, int maxHue, int minSat, int maxSat, int minVal, int maxVal) {
    //defining variables
    Mat threshLow;
    Mat threshHigh;
    Mat hueResult;
    Mat satResult;
    Mat valResult;

    //seperating image into HSV (or in this case, really just HV)
    cv::Mat cvted;
    cv::cvtColor(input, cvted, CV_BGR2HSV);

    std::vector<cv::Mat> separated(3);
    cv::split(cvted, separated);
    Mat hue = separated.at(0).clone();
    Mat sat = separated.at(1).clone();
    Mat val = separated.at(2).clone();

    //Hue
    threshold(hue, threshLow, minHue, 255, THRESH_BINARY);
    threshold(hue, threshHigh, maxHue, 255, THRESH_BINARY_INV);
    hueResult = threshLow & threshHigh;

    //Saturation
    threshold(sat, threshLow, minSat, 255, THRESH_BINARY);
    threshold(sat, threshHigh, maxSat, 255, THRESH_BINARY_INV);
    satResult = threshLow & threshHigh;

    //Value
    threshold(val, threshLow, minVal, 255, THRESH_BINARY);
    threshold(val, threshHigh, maxVal, 255, THRESH_BINARY_INV);
    valResult = threshLow & threshHigh;

    Mat combined = hueResult & valResult & satResult;

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

std::vector<Point> TargetDetector::arcCheck1(std::vector<Point> input)
{
  double max = input[0].x;
  double min = input[0].x;
  std::vector<Point> output;

  for (int i = 0; i < input.size(); i++)
  {
    if (input[i].x > max)
    {
      max = input[i].x;
    }
    if (input[i].x < min)
    {
      min = input[i].x;
    }
  }
  cv::Point New(0,0);
  for (int i = 0; i < input.size(); i++)
  {
    if (input[i].x > max - 5 && input[i].x < max + 5)
    {
      New = input[i];
      output.push_back (New);
    }
    if (input[i].x > min - 5 && input[i].x < min + 5)
    {
      New = input[i];
      output.push_back (New);
    }

  }
  return output;
}

std::vector<std::vector<Point> > TargetDetector::filterContours(std::vector<std::vector<Point> > contours, bool tar)
{
//bool tar: true = gears, false = boiler
  std::vector<Point> arcContour;
  std::vector<Point> outputContour;
  std::vector<cv::Point> outputContour1;
  std::vector<cv::Point> outputContour2;
  std::vector<std::vector<Point> >* vPtr;

  int tarNum = 0;
  Target* first;
    for(int j = 0; j < contours.size(); j++)
    {

      if (tar == false)
      {
        arcContour = arcCheck1(contours[j]);
        approxPolyDP(arcContour, outputContour, (cv::arcLength(cv::Mat(contours.at(j)), true) * 0.01), true);
      } else
      {
        approxPolyDP(contours[j], outputContour, (cv::arcLength(cv::Mat(contours.at(j)), true) * 0.01), true);
      }

        if (contourArea(outputContour) > 100 && outputContour.size() == 4)
        {
            double maxCosine = 0;
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
              std::vector<std::vector<cv::Point> > tempVV;
              std::vector<std::vector<Point> > fullContour;
              std::vector<std::vector<Point> >* vPtr;
              vPtr = &tempVV;
              if (tempVV.size() == 0) {
              tempVV.push_back(outputContour);
              tempVV.push_back(outputContour);
            } else {
              tempVV[0] = outputContour;
              tempVV[1] = outputContour;
            }
              // TempV is put in twice to the tempVV vector to serve as a placeholder
              Target* target = new Target(tempVV);
              // TempVV is temporary and is used to call getType
              /* if the target called by the main is the same as the one found
                 and if it is gears on first time*/
              //std::cout << "Type: " << target->getType() << std::endl;
              if (tar == target->getType() && tar == true && tarNum == 1) {


                std::cout << "Found Candidate for 2" << std::endl;
                //Gives a range of error of 6 so that it doesn't detect the same contour twice

                //std::cout << "addresses:" << std::endl;
                //std::cout << first << std::endl;
                //std::cout << target << std::endl;

                //std::cout << "First: " << first->getLeftPoint() << std::endl;
                //std::cout << "Second: " << target->getLeftPoint() << std::endl;

                //std::cout << fabs(first->getRightPoint().x - target->getLeftPoint().x) << std::endl;
                //std::cout << fabs(first->getRightPoint().x - first->getLeftPoint().x) << std::endl;
                //std::cout << first->getWidth() << std::endl;

                if ((fabs(first->getRightPoint().x - first->getLeftPoint().x) + 10 < fabs(first->getRightPoint().x - target->getLeftPoint().x)) && (&outputContour1 != NULL || &outputContour2 != NULL))
                {
                  //std::cout << "First" << std::endl;
                  //first->printPoints();
                  //std::cout << "Second" << std::endl;
                  //target->printPoints();
                  outputContour2 = outputContour;
                  std::cout << "Found 2" << std::endl;
                  tarNum = 0;

                // whichever has the least has the left most points
                if (fullContour.size() == 0) {
                  if (outputContour1[1].x > outputContour2[1].x) {
                    fullContour.push_back (outputContour2); // first will be left, then right
                    fullContour.push_back (outputContour1);
                  } else {
                    fullContour.push_back (outputContour1);
                    fullContour.push_back (outputContour2);
                  }
                } else {
                  if (outputContour1[1].x > outputContour2[1].x) {
                    fullContour[0] = outputContour2; // first will be left, then right
                    fullContour[1] = outputContour1;
                  } else {
                    fullContour[0] = outputContour1;
                    fullContour[1] = outputContour2;
                  }
                }
                //std::cout << "Returning" << std::endl;
                std::vector<std::vector<Point> >* fuPtr;
                fuPtr = &fullContour;
                return fullContour;
                }

              }

              if (tar == target->getType() && tar == true && tarNum == 0) {
                std::cout << "Found 1" << std::endl;
                first = new Target(tempVV);
                outputContour1 = outputContour;
                tarNum = 1;
              }

              // Gears on second try

              //if boiler was called
              if (tar == target->getType() && tar == false) {
                if (fullContour.size() == 0){
                fullContour.push_back (outputContour);
                fullContour.push_back (outputContour);
              } else {
                fullContour[0] = outputContour;
                fullContour[1] = outputContour;
              }
                // Both contours are the same as to not need two target constructors
                std::vector<std::vector<Point> >* fuPtr;
                fuPtr = &fullContour;
                //std::cout<<"line 247";
                return fullContour;
                // delete fuPtr;
              }

            }

        }
      }
    //std::cout<<"line 257";
    //delete vPtr;

    return std::vector<std::vector<Point> >();
}
