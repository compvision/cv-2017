
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
    
	int ping = 1;
	
    while(cv::waitKey(30) != 27)
    {
		cv::Mat background(Size(1000,1000), CV_8UC1, Scalar(255, 255, 255 ));

		std::string msg;

	    std::cout << "While Loop #" << loop << std::endl;

		if (ping == 1)
		{
		
			if(config.getIsNetworking())
			{
				std::cout << "we are waiting for ping\n";
	    		networkController.waitForPing();
				std::cout << "we have revieced ping\n";
			}
			ping++;
		}

		image = camera.getImage();

	   	if(!image.data) // check if image is valid
	    {
	          if(config.getIsDebug()){
	            std::cout << "failed to read image" << std::endl;
	          }
	          return -1;
	    }
	    else {
	   	  imshow("Live Video Feed", image);     
	   }

	    if(config.getIsDebug()){
	        std::cout << "Image Read" << std::endl;
	    }
	    Target* targetG = detector.processImage(image, true); //Gears
		  //Detects if Target matches Gear reflective tape
	   	Target* targetB = detector.processImage(image, false); //Boiler
	    	//Detects if Target matches Boiler reflective tape
      	if(config.getIsDebug()){
	      std::cout << "Image Processed by Target Detector" << std::endl;
      	}
      	bool foundGear = false;
		bool foundBoiler = false;
	
        std::cout<<"right"<<std::endl;

        if (targetG != NULL || targetB != NULL) 
		{
        	std::cout<<"in if"<<std::endl;

            if(targetG != NULL) 
			{
				foundGear = targetG -> getType();
            	std::cout << "Got Type Gears: " << foundGear << std::endl;
				foundGear = true;
        	}

        	if(targetB != NULL) 
			{
			    foundBoiler = targetB -> getType();
         		std::cout << "Got Type Boiler: " << foundBoiler << std::endl;
         		foundBoiler = true; 
            }

  	  		std::cout <<"About to check the value of foundTarget" << std::endl;

			if(foundGear)
  	        {
				String gear = "true;";
               	std::cout <<"Gear was found " << std::endl;
               	if(config.getIsDebug())
               	std::cout << "Image Being Processed" << std::endl;
               	processor.loadTarget(targetG);
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

  	                cv::putText(background, dis, cv::Point(50,100),
  	                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),
  	                1);

  	                cv::putText(background, alt, cv::Point(50,200),
  	                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),
  	                1);

  	                cv::putText(background, azi, cv::Point(50,400),
  	                cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),
  	                1);
			if (config.getIsHeadless() == 0)  					//for background
  	                	imshow("General", background);
	
					std::string zero = "0;0;0;";
               if (config.getIsNetworking())
  	     	    {
  		            msg.append("true;false;" + 
  			        boost::lexical_cast<std::string> (gearDistance) + ";" +
        		   	boost::lexical_cast<std::string> (gearAzimuth) + ";" +
  		      		boost::lexical_cast<std::string> (gearAltitude) + ";" + zero);
  		        }

      			if(config.getIsDebug())
				{
           		    std::cout << "Target Found!" << std::endl;
           	    	std::cout << "Distance (Gears): " << gearDistance << std::endl;
  	            	std::cout << "Altitude (Gears): " << gearAltitude << std::endl;
  	            	std::cout << "Azimuth (Gears): " << gearAzimuth << std::endl;
  	          	}
  			    //information to send (Networking)
  	        }
  		    /*if(foundBoiler)
  			{
				String boiler = "true;";
  				std::cout <<"Boiler was found " << std::endl;
  				if(config.getIsDebug())
  					std::cout << "Image Being Processed" << std::endl;
  				
				processor.loadTarget(targetB);
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

  				std::string dis = "distance (Boiler): " + patch::to_string(boilerDistance);
  				std::string alt = "altitude (Boiler): " + patch::to_string(boilerAltitude);
  				std::string azi = "azimuth (Boiler): " + patch::to_string(boilerAzimuth);

  				cv::putText(background, dis, cv::Point(50,100),
  				cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),1);

  				cv::putText(background, alt, cv::Point(50,200),
  				cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),1);

  				cv::putText(background, azi, cv::Point(50,400),
  				cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0),1);
  				// for background
			    if (config.getIsHeadless()==0)
  				imshow("General", background);

  		    	if (config.getIsNetworking())
  				{
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

  			}*/
  			else
  			{
  				if (config.getIsNetworking())
  					msg.append("false;false;");
  			}

  			if(config.getIsNetworking())
  			{
  				networkController.sendMessage(msg);
  			}
  	        
			std::cout << "\n\nUpdated Livefeed\n\n"; 
  	        loop++;
  	        delete targetG;
       		delete targetB;
		    //refresh loop
 	   }
	  GaussianBlur(image,image,Size(3,3),31);
	    if (config.getIsHeadless()==0)
	  	imshow("Live Video Feed", image);
    }
    return 0;
}
