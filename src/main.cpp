//True=gears False=boiler
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

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#include <iostream>

int main(int argc, char* argv[])
{

	TargetDetector detector;
	TargetProcessor processor;
	NetworkController networkController;
  VideoDevice camera;
	CmdLineInterface interface(argc, argv);
	AppConfig config = interface.getConfig();

	if(config.getIsDevice())
	{
		camera.startCapture(config.getDeviceID());
		if(config.getIsDebug())
		std::cout << "Camera is ready\n";
	}

    if(config.getIsNetworking())
        networkController.startServer();

    cv::Mat image;

    int loop = 1;
    cv::namedWindow("Live Video Feed", cv::WINDOW_NORMAL);
    cv::namedWindow("General", cv::WINDOW_NORMAL);

    while(cv::waitKey(30) != 27)
    {
			cv::Mat background(Size(1000,1000), CV_8UC1, Scalar(255, 255, 255 ));

			std::string msg;

	      std::cout << "While Loop #" << loop << std::endl;

				if(config.getIsNetworking())
	    	networkController.waitForPing();

				image = camera.getImage();

				if(!image.data) // check if image is valid
	      {
	          if(config.getIsDebug())
	          std::cout << "failed to read image" << std::endl;
	          return -1;
	      }

	      if(config.getIsDebug())
	      std::cout << "Image Read" << std::endl;
	      Target* targetG = detector.processImage(image, true); //Gears
				//Detects if Target matches Gear reflective tape
				Target* targetB = detector.processImage(image, false); //Boiler
	    	//Detects if Target matches Boiler reflective tape
      	if(config.getIsDebug())
	      std::cout << "Image Processed by Target Detector" << std::endl;
        bool foundGear = false;
				bool foundBoiler = false;

	        /* if ((*targetG).edgeL.size() != 0)
	        {
	            foundGear = true;
	        } */

        bool typeG;
        bool typeB;
        std::cout<<"right"<<std::endl;
        if (targetG != NULL || targetB != NULL) {
         std::cout<<"in if"<<std::endl;

         if(targetG != NULL)
         typeG = targetG -> getType();

        if(targetB != NULL)
         typeB = targetB -> getType();



  				if (typeG==true)
  				{
  						foundGear = true;
  				}
  	  		if (typeG==false)
  				{
  						foundGear = false;
  				}
  				if (typeB==true)
  				{
  						foundBoiler = true;
  				}
  				if (typeB==false)
  				{
  						foundBoiler = false;
  				}
          /*if ((*targetB).contours.size() != 0)
          {
              foundBoiler = true;
          }*/
  	  		std::cout <<"About to check the value of foundTarget" << std::endl;

  		    if(foundBoiler)
  			  {
  				std::cout <<"Boiler was found " << std::endl;
  				if(config.getIsDebug())
  					std::cout << "Image Being Processed" << std::endl;

  					processor.loadTarget(targetB);
  					// middle value should be changed to object's real width (diameter of boiler is 15 in.)

  				if(config.getIsDebug())
  				    std::cout << "Target Loaded" << std::endl;

  				double distance = processor.calculateDistance();
  				if(config.getIsDebug())
  					std::cout << "Distance Calculated" << std::endl;

  				double azimuth = processor.calculateAzimuth();
  				if(config.getIsDebug())
  					std::cout << "Azimuth Calculated" << std::endl;

  				double altitude = processor.calculateAltitude();
  				if(config.getIsDebug())
  					std::cout << "Altitude Calculated" << std::endl;

  				if(config.getIsDebug())
  					std::cout << "Image Processed by TargetProcessor" << std::endl;

  				std::string dis = "distance (Boiler): " + patch::to_string(distance);
  				std::string alt = "altitude (Boiler): " + patch::to_string(altitude);
  				std::string azi = "azimuth (Boiler): " + patch::to_string(azimuth);

  				cv::putText(background, dis, cv::Point(50,100),
  				cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),1);

  				cv::putText(background, alt, cv::Point(50,200),
  				cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),1);

  				cv::putText(background, azi, cv::Point(50,400),
  				cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),1);
  				// for background
  				imshow("General", background);

  		    if (config.getIsNetworking())
  				{
  					msg.append("true;" +
  					boost::lexical_cast<std::string> (distance) + ";" +
  					boost::lexical_cast<std::string> (azimuth) + ";" +
  					boost::lexical_cast<std::string> (altitude) + ";");
  				}

  				if(config.getIsDebug()){
  					std::cout << "Target Found! Distance (Boiler): " << distance;
  					std::cout << "Altitude (Boiler): " << altitude << std::endl;
  					std::cout << "Azimuth (Boiler): " << azimuth << std::endl;
  				}

  			}
  			else
  			{
  				if (config.getIsNetworking())
  					msg.append("false;");
  			}

  	        if(foundGear)
  	        {
               std::cout <<"Gear was found " << std::endl;
               if(config.getIsDebug())
               std::cout << "Image Being Processed" << std::endl;
               processor.loadTarget(targetG);
        			// middle value should be changed to the object's real width (The width of the Gear "rectangle is 10.25 in.)

  	          if(config.getIsDebug())
  	            std::cout << "Target Loaded" << std::endl;

  	          double distance = processor.calculateDistance();

  	          if(config.getIsDebug())
  	                std::cout << "Distance Calculated" << std::endl;

                    double azimuth = processor.calculateAzimuth();
                       if(config.getIsDebug())
                            std::cout << "Azimuth Calculated" << std::endl;

                    double altitude = processor.calculateAltitude();
                        if(config.getIsDebug())
                            std::cout << "Altitude Calculated" << std::endl;

  	            if(config.getIsDebug())
  	                std::cout << "Image Processed by TargetProcessor" << std::endl;

  	                std::string dis = "distance (Gear): " + patch::to_string(distance);
  	                std::string alt = "altitude (Gear): " + patch::to_string(altitude);
  	                std::string azi = "azimuth (Gear): " + patch::to_string(azimuth);

  	                cv::putText(background, dis, cv::Point(50,100),
  	                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),
  	                1);

  	                cv::putText(background, alt, cv::Point(50,200),
  	                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),
  	                1);

  	                cv::putText(background, azi, cv::Point(50,400),
  	                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),
  	                1);
  			//for background
  	                imshow("General", background);

              if (config.getIsNetworking())
  				    {
  			          msg.append("true;" +
  			          boost::lexical_cast<std::string> (distance) + ";" +
        					boost::lexical_cast<std::string> (azimuth) + ";" +
  		      			boost::lexical_cast<std::string> (altitude) + ";");
  					  }

      				if(config.getIsDebug()){
                	std::cout << "Target Found! Distance (Gears): " << distance;
  	              std::cout << "Altitude (Gears): " << altitude << std::endl;
  	              std::cout << "Azimuth (Gears): " << azimuth << std::endl;
  	          }
  			    //information to send (Networking)
  	        }
  	        else
  	        {
  	          if (config.getIsNetworking())
  	              msg.append("false;");
  	        }

  			if(config.getIsNetworking())
  			{
  				networkController.sendMessage(msg);
  			}

  	        imshow("Live Video Feed", image);
  	        loop++;
  	        delete targetG;
        		delete targetB;
	    //refresh loop
      }
    }
    return 0;
}
