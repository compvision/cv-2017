#include "Target.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>
#include <iostream>


Target::Target(std::vector<std::vector<cv::Point> > contour)
{
    edgeL = contour[0];
    edgeR = contour[1];

    //splits the inputted vector into two shapes again
}


void Target::setTar(bool tar) {

    Tar = tar;
}

bool Target::getTar() {

    return Tar;

}

double Target::getHeight()
{
    return fabs(getTopPoint().y - getBottomPoint().y);
}

double Target::getWidth()
{
    return fabs(getRightPoint().x - getLeftPoint().x);
}

//True if Gears, False if Boilers
bool Target::getType() {
  if (getHeight() > getWidth()) {
    return true;
  }
if (getHeight() < getWidth()) {
    return false;
  }
}

/*
bool Target::isInitialized()
{
    std::cout << "EDGE" << edge;
    if (edge.size() == 0) {
        return false;
    }
    return true;
}
*/

void Target::printPoints() //debugging
{
    std::cout << "TopPoint: " << getTopPoint().y << std::endl;
    std::cout << "BottomPoint: " << getBottomPoint().y << std::endl;
    std::cout << "LeftPoint: " << getLeftPoint().x << std::endl;
    std::cout << "RightPoint: " << getRightPoint().x << std::endl;
    std::cout << "LeftRightPoint: " << getLeftRightPoint().x << std::endl;
    std::cout << "RightLeftPoint: " << getRightLeftPoint().x << std::endl;
    std::cout << "Height: " << getHeight() << std::endl;
    std::cout << "Width: "  << getWidth() << std::endl;
}

cv::Point Target::getCenter()//finds center point of target
{
    cv::Point center((getLeftPoint().x + getRightPoint().x)/2, (getTopPoint().y + getBottomPoint().y)/2);
    return center;
}

cv::Point Target::getTopPoint()
{
    cv::Point max =  edgeL[0];
    for(unsigned int i = 0; i < edgeL.size(); i++)
    {
        if(edgeL[i].y > max.y)
        {
            max = edgeL[i];
        }
    }
    return max;
}
cv::Point Target::getBottomPoint()
{
    cv::Point min =  edgeL[0];
    for(unsigned int i = 0; i < edgeL.size(); i++)
    {
        if(edgeL[i].y < min.y)
        {
            min = edgeL[i];
        }
    }
    return min;
}
cv::Point Target::getLeftPoint()
{
    cv::Point min =  edgeL[0];
    for(unsigned int i = 0; i < edgeL.size(); i++)
    {
        if(edgeL[i].x < min.x)
        {
            min = edgeL[i];
        }
    }
    return min;
}
// Uses the right shape for right most point in order to pretend like it's one big shape
cv::Point Target::getRightPoint()
{
    cv::Point max = edgeR[0];
    for(unsigned int i = 0; i < edgeR.size(); i++)
    {
        if(edgeR[i].x > max.x)
        {
            max = edgeR[i];
        }
    }
    return max;
}

//Right-most point of left shape
cv::Point Target::getLeftRightPoint()
{
  cv::Point max = edgeL[0];
  for(unsigned int i = 0; i < edgeL.size(); i++)
  {
      if(edgeL[i].x > max.x)
      {
          max = edgeL[i];
      }
  }
  return max;
}

//Left-most point of right shape
cv::Point Target::getRightLeftPoint()
{
  cv::Point min = edgeR[0];
  for(unsigned int i = 0; i < edgeR.size(); i++)
  {
      if(edgeR[i].x < min.x)
      {
          min = edgeR[i];
      }
  }
  return min;
}
