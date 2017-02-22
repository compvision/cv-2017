#include <cstdlib>
#include <string>

#include "AppConfig.hpp"

AppConfig::AppConfig() :
gearDeviceID(0),
boilerDeviceID(0),
isFile(0),
isGearDevice(0),
isBoilerDevice(0),
isHeadless(0),
isNetworking(1),
isDebug(0)
{
}

// Getters
std::string AppConfig::getFileName()
{
    return fileName;
}

int AppConfig::getGearDeviceID()
{
    return gearDeviceID;
}

int AppConfig::getBoilerDeviceID()
{
    return boilerDeviceID;
}

int AppConfig::getIsFile()
{
    return isFile;
}

int AppConfig::getIsGearDevice()
{
    return isGearDevice;
}

int AppConfig::getIsBoilerDevice()
{
    return isBoilerDevice;
}

int AppConfig::getIsHeadless()
{
    return isHeadless;
}

int AppConfig::getIsNetworking()
{
    return isNetworking;
}

int AppConfig::getIsDebug()
{
    return isDebug;
}

// Setters
void AppConfig::setFileName(std::string inputFileName)
{
    fileName = inputFileName;
}

void AppConfig::setGearDeviceID(int inputDeviceID)
{
    gearDeviceID = inputDeviceID;
}

void AppConfig::setBoilerDeviceID(int inputDeviceID)
{
    boilerDeviceID = inputDeviceID;
}

void AppConfig::setIsFile(int inputIsFile)
{
    isFile = inputIsFile;
}

void AppConfig::setIsGearDevice(int inputIsDevice)
{
    isGearDevice = inputIsDevice;
}

void AppConfig::setIsBoilerDevice(int inputIsDevice)
{
    isBoilerDevice = inputIsDevice;
}

void AppConfig::setIsHeadless(int inputIsHeadless)
{
    isHeadless = inputIsHeadless;
}

void AppConfig::setIsNetworking(int inputIsNetworking)
{
    isNetworking = inputIsNetworking;
}

void AppConfig::setIsDebug(int debug)
{
    isDebug = debug;
}
