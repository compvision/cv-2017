#include <opencv2/opencv.hpp>
#include <boost/lexical_cast.hpp>

#include "CmdLineInterface.hpp"
#include "AppConfig.hpp"
#include "Target.hpp"
#include "TargetDetector.hpp"
#include "TargetProcessor.hpp"
#include "NetworkController.hpp"
#include "VideoDevice.hpp"
#include "GUIManager.hpp"
#include <string>
#include <sstream>

namespace patch{
    template < typename T > std::string to_string( const T& n ){
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#include <iostream>

int main(int argc, char* argv[]){
    TargetDetector detector;
    TargetProcessor processor;
    NetworkController networkController;
    VideoDevice gearCamera;
    VideoDevice boilerCamera;
    CmdLineInterface interface(argc, argv);
    AppConfig config = interface.getConfig();

    if(config.getIsGearDevice() && config.getIsBoilerDevice()){
        gearCamera.startCapture(config.getGearDeviceID());
        boilerCamera.startCapture(config.getBoilerDeviceID());
        if(config.getIsDebug())
            std::cout << "Cameras are ready\n";
    }

    if(config.getIsNetworking())
        networkController.startServer();

    cv::Mat gearImage, boilerImage;

    int loop = 1;
    if (!config.getIsHeadless()){
        cv::namedWindow("Live Gear Feed", cv::WINDOW_NORMAL);
        cv::namedWindow("Live Boiler Feed", cv::WINDOW_NORMAL);
        cv::namedWindow("General", cv::WINDOW_NORMAL);
        cv::namedWindow("Gear Contours", cv::WINDOW_NORMAL);
        cv::namedWindow("Boiler Contours", cv::WINDOW_NORMAL);
    }

    while(cv::waitKey(30) != 27){
        cv::Mat background(Size(1000,1000), CV_8UC1, Scalar(255, 255, 255 ));

        std::string msg;

        std::cout << "While Loop #" << loop << std::endl;

        if(config.getIsNetworking())
            networkController.waitForPing();

        gearImage = gearCamera.getImage();
        boilerImage = boilerCamera.getImage();

        if(!gearImage.data){ // check if gearCamera image is valid
            if(config.getIsDebug())
                std::cout << "Failed to read gearImage" << std::endl;
            return -1;
        }

        if(!boilerImage.data){ // check if boilerCamera image is valid
            if(config.getIsDebug())
                std::cout << "Failed to read boilerImage" << std::endl;
            return -1;
        }

        if(config.getIsDebug())
            std::cout << "Image Read" << std::endl;

        Target* gearTarget = detector.processImage(gearImage, true); //Gears
        //Detects if Target matches Gear reflective tape
        Target* boilerTarget = detector.processImage(boilerImage, false); //Boiler
        //Detects if Target matches Boiler reflective tape

        if(config.getIsDebug())
            std::cout << "Image Processed by Target Detector" << std::endl;

        bool foundGear = false;
        bool foundBoiler = false;

        if (gearTarget != NULL || boilerTarget != NULL){
            std::cout << "in if" << std::endl;

            if(boilerTarget != NULL){
                foundBoiler = boilerTarget -> getType();
                if(!foundBoiler){
                    foundBoiler = true;
                }else{
                    foundBoiler = false;
                }

                std::cout << "Got Type Boiler: " << foundBoiler << std::endl;
            }

            if(gearTarget != NULL){
                foundGear = gearTarget -> getType();
                std::cout << "Got Type Gears: " << foundGear << std::endl;
            }

            std::cout << "About to check the value of foundTarget" << std::endl;

            if(foundGear){
                String gear = "true;";
                std::cout <<"Gear was found " << std::endl;
                if(config.getIsDebug())
                    std::cout << "Image Being Processed" << std::endl;

                processor.loadTarget(gearTarget);
                // middle value should be changed to the object's real width (The width of the Gear "rectangle is 10.25 in.)

                if(config.getIsDebug())
                    std::cout << "Target Loaded" << std::endl;

                double gearDistance = processor.calculateDistance();
                if(config.getIsDebug())
                    std::cout << "Distance Calculated" << std::endl;

                double gearAzimuth = processor.calculateAzimuth();
                if(config.getIsDebug())
                    std::cout << "Azimuth Calculated" << std::endl;

                double gearAltitude = processor.calculateAltitude();
                if(config.getIsDebug())
                    std::cout << "Altitude Calculated" << std::endl;

                if(config.getIsDebug())
                    std::cout << "Image Processed by TargetProcessor" << std::endl;

                std::string dis = "distance (Gear): " + patch::to_string(gearDistance);
                std::string alt = "altitude (Gear): " + patch::to_string(gearAltitude);
                std::string azi = "azimuth (Gear): " + patch::to_string(gearAzimuth);
                if (!config.getIsHeadless()){
                    cv::putText(background, dis, cv::Point(50,100),
                                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),
                                1);

                    cv::putText(background, alt, cv::Point(50,200),
                                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),
                                1);

                    cv::putText(background, azi, cv::Point(50,300),
                                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),
                                1);
                    //for background
                    imshow("General", background);
                }
                std::string zero = "0;0;0;";
                if (config.getIsNetworking()){
                    msg.append("true;false;" +
                               boost::lexical_cast<std::string> (gearDistance) + ";" +
                               boost::lexical_cast<std::string> (gearAzimuth) + ";" +
                               boost::lexical_cast<std::string> (gearAltitude) + ";" + zero);
                }

                if(config.getIsDebug()){
                    std::cout << "Target Found!" << std::endl;
                    std::cout << "Distance (Gears): " << gearDistance << std::endl;
                    std::cout << "Altitude (Gears): " << gearAltitude << std::endl;
                    std::cout << "Azimuth (Gears): " << gearAzimuth << std::endl;
                }
                //information to send (Networking)
            }else{
                if (config.getIsNetworking())
                    msg.append("false;false;");
            }

            if(foundBoiler){
                String boiler = "true;";
                std::cout << "Boiler was found" << std::endl;
                if(config.getIsDebug())
                    std::cout << "Image Being Processed" << std::endl;

                processor.loadTarget(boilerTarget);
                // middle value should be changed to object's real width (diameter of boiler is 15 in.)

                if(config.getIsDebug())
                    std::cout << "Target Loaded" << std::endl;

                double boilerDistance = processor.calculateDistance();
                if(config.getIsDebug())
                    std::cout << "Distance Calculated" << std::endl;

                double boilerAzimuth = processor.calculateAzimuth();
                if(config.getIsDebug())
                    std::cout << "Azimuth Calculated" << std::endl;

                double boilerAltitude = processor.calculateAltitude();
                if(config.getIsDebug())
                    std::cout << "Altitude Calculated" << std::endl;

                if(config.getIsDebug())
                    std::cout << "Image Processed by TargetProcessor" << std::endl;

                std::string dis = "Distance (Boiler): " + patch::to_string(boilerDistance);
                std::string alt = "Altitude (Boiler): " + patch::to_string(boilerAltitude);
                std::string azi = "Azimuth (Boiler): " + patch::to_string(boilerAzimuth);
                if (!config.getIsHeadless()){
                    cv::putText(background, dis, cv::Point(50,400),
                                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),1);
                    cv::putText(background, alt, cv::Point(50,500),
                                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),1);
                    cv::putText(background, azi, cv::Point(50,600),
                                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),1);
                    // for background
                    imshow("General", background);
                }
                if (config.getIsNetworking()){
                    msg.append("false;true;0;0;0;" +
                               boost::lexical_cast<std::string> (boilerDistance) + ";" +
                               boost::lexical_cast<std::string> (boilerAzimuth) + ";" +
                               boost::lexical_cast<std::string> (boilerAltitude) + ";");
                }

                if(config.getIsDebug()){
                    std::cout << "Target Found!" << std::endl;
                    std::cout << "Distance (Boiler): " << boilerDistance << std::endl;
                    std::cout << "Altitude (Boiler): " << boilerAltitude << std::endl;
                    std::cout << "Azimuth (Boiler): " << boilerAzimuth << std::endl;
                }

            }else{
                if(config.getIsNetworking())
                    msg.append("false;");
            }

            if(config.getIsNetworking()){
                networkController.sendMessage(msg);
            }
        }
        GaussianBlur(gearImage, gearImage,Size(3,3),31);
        GaussianBlur(boilerImage, boilerImage,Size(3,3),31);
        if (!config.getIsHeadless()){
            imshow("Live Gear Feed", gearImage);
            imshow("Live Boiler Feed", boilerImage);
        }
        loop++;
        delete gearTarget;
        delete boilerTarget;
    }
    cv::waitKey(0);
    return 0;
}
