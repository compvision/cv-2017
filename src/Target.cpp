#include "Target.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>
#include <iostream>


Target::Target(std::vector<std::vector<cv::Point>> contour)
{
    edgeL = contour[0];
    edgeR = contour[1];
    //crap
    //std::cout << "EDGE" << edge.size();

    //crow = 1415;
}

void Target::setTar(bool tar) {

    Tar = tar;
}

double Target::getHeight()
{
    return fabs(getTopPoint().y - getBottomPoint().y);
}

double Target::getWidth()
{
    return fabs(getRightPoint().x - getLeftPoint().x);
}

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
    std::cout << "TopPoint: " << getTopPoint() << std::endl;
    std::cout << "BottomPoint: " << getBottomPoint() << std::endl;
    std::cout << "LeftPoint: " << getLeftPoint() << std::endl;
    std::cout << "RightPoint: " << getRightPoint() << std::endl;
}

cv::Point Target::getCenter()//finds center point of target
{
    cv::Point center((getLeftPoint + getRightPoint)/2, (getTopPoint + getBottomPoint)/2);
    //will discuss better changes next time we meet
    return center;
}

cv::Point Target::getTopPoint()
{
    cv::Point max(0,0);
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
    cv::Point min(10000,10000);
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
    cv::Point min(0,0);
    for(unsigned int i = 0; i < edgeL.size(); i++)
    {
        if(edgeL[i].x > min.x)
        {
            min = edgeL[i];
        }
    }
    return min;
}
cv:: Point Target::getRightPoint()
{
    cv::Point max(10000,10000);
    for(unsigned int i = 0; i < edgeR.size(); i++)
    {
        if(edgeR[i].x < max.x)
        {
            max = edgeR[i];
        }
    }
    return max;
}
