#include "Target.hpp"
#include "TargetProcessor.hpp"
#include <math.h>

TargetProcessor::TargetProcessor()
{
    GearWidth = 0.270; //meters
    BoilerWidth = 0.400; // Cjq-0fjekkom
    focalLength = 700; //varies by camera
    horizCenter = 320; //aslo varies by camera (center horizontal point on video)
    vertCenter = 240; //center vertical point on video

} //constructor

void TargetProcessor::loadTarget(Target* target){
    imageTarWidth = target->getWidth();
    imageTarHeight = target->getHeight();
    imageTarCenter = target->getCenter();
    Tar = target->getTar();
}


void TargetProcessor::temporaryGetPoints(int width, int height, cv::Point center)
{
    imageTarWidth = width;
    imageTarHeight = height;
    imageTarCenter = center;
    Tar = true;
}

double TargetProcessor::calculateDistance()
{
    if(Tar)
    {
        std::cout << "gear image width: " << imageTarWidth << std::endl;
        return GearWidth * focalLength / imageTarWidth; //returns the distance (m)
    }
    else
    {
        std::cout << "boiler image width: " << imageTarWidth << std::endl;
        return BoilerWidth * focalLength / imageTarWidth;
    }
}

double TargetProcessor::calculateAzimuth(){ //unsure if this is working properly, but is returning a reasonable looking value{
    double offset = imageTarCenter.x - horizCenter;
    return (atan(offset/focalLength))*(180/M_PI); //in degrees
}

double TargetProcessor::calculateAltitude(){ //same comment as calculateAzimuth(){
    int cameraAngle = 0; //angle the camera is pointing up from the horizon; assumes camera is level
    double offset =  vertCenter - imageTarCenter.y;
    return (atan(offset/focalLength))*(180/M_PI) + cameraAngle; //in degrees
}
