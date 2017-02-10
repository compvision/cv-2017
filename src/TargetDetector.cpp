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

    dilate(input, input, Mat());

    std::vector<std::vector<Point> > contours = contour(input);
    // std::cout << "not contours" << std::endl;
    std::vector<std::vector<Point> > finalContour = filterContours(contours, input, tar);




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
	
  double maxRY = 0;
  double minRY = 1000;
  double maxLY = 0;
  double minLY = 1000;
  std::vector<cv::Point> output;

  for (int i = 0; i < input.size(); i++)
  {
    if (input[i].x > max)
    {
      max = input[i].x;
    }
    else if (input[i].x < min)
    {
      min = input[i].x;
    }
  }

  //cv::Point newPt(0,0);
  for (int i = 0; i < input.size(); i++)
  {
    if (input[i].x > max - 7 && input[i].x < max + 7)
    {
      //newPt = input[i];
      if(input[i].y < minRY)
      {
        minRY = input[i].y;
      }
      else if(input[i].y > maxRY)
      {
        maxRY = input[i].y;
      }
      output.push_back(input[i]);
    }
    else if (input[i].x > min - 7 && input[i].x < min + 7)
    {
      //newPt = input[i];
      if(input[i].y < minRY)
      {
        minRY = input[i].y;
      }
      else if(input[i].y > maxRY)
      {
        maxRY = input[i].y;
      }
      output.push_back(input[i]);
    }

  }

  int diff = maxRY-minRY;
  int diff2 = maxLY - minLY;
  if( output.size() > 0 && (diff - diff2 < 20))
  {
    return output;
  }
  else
  { 
    return std::vector<cv::Point>();
  }
}

std::vector<std::vector<Point> > TargetDetector::filterContours(std::vector<std::vector<Point> > contours, cv::Mat img, bool tar)
{
    //bool tar: true = gears, false = boiler
    std::vector<cv::Point> arcContour;
    std::vector<cv::Point> outputContour;
    std::vector<cv::Point> outputContour1;
    std::vector<cv::Point> outputContour2;
    std::vector<std::vector<cv::Point> > gearVector;
    std::vector<std::vector<cv::Point> > boilerVector;

    int tarNum = 0;
    Target* first;
	
    if (tar == false)
    { 
	for(int j = 0; j< contours.size(); j++)
        {
            arcContour = arcCheck1(contours[j]);
          
            if(arcContour.size() > 0)
            {
                approxPolyDP(arcContour, outputContour, (cv::arcLength(cv::Mat(contours.at(j)), true) * 0.01), true);    
                boilerVector.push_back(outputContour);
   
            }

            int maxArea = 0;
            int index = 0;

            for(int i = 0; i < boilerVector.size(); i++)
            {
                if(contourArea(boilerVector[i]) > maxArea)
                {
                    maxArea = contourArea(boilerVector[i]);    
                    index = i;
                }
            }
	    
 	    std::vector<std::vector<cv::Point> > returnVector;
            returnVector.push_back(boilerVector[index]);
            
	    if(returnVector.size() > 0)
                return returnVector;
	}
    } 
    else
    {
        for(int j = 0; j < contours.size(); j++)
        {
            approxPolyDP(contours[j], outputContour, (cv::arcLength(cv::Mat(contours.at(j)), true) * 0.01), true);
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
                    gearVector.push_back(outputContour);
                }
            }

	    for( int i = 0; i < gearVector.size(); i++)
            {
                Target* tempOne = new Target(gearVector[i]);
                for(int k = i; k < gearVector.size()-1; k++)
                {
                     Target* tempTwo = new Target(gearVector[k+1]);

                    if(abs(tempOne->getCenter().y - tempTwo->getCenter().y) < 13)
                    {
                        std::vector<std::vector<cv::Point> > returnVector;
                        returnVector.push_back(gearVector[i]);
                        returnVector.push_back(gearVector[k+1]);
                        return returnVector;
                    }
                }   
            }
        }
    }
  
    Scalar color(255,0,0);
    Scalar color2(0,0,255);
    cv::drawContours(img, gearVector, -1, color, 10);
    cv::drawContours(img, boilerVector, -1, color2, 10);
    
    return std::vector<std::vector<cv::Point> >();
}
