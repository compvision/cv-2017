#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/lexical_cast.hpp>

#include "NetworkController.hpp"
int main(/*int argc, char* argv[]*/)
{
    // Create all the necessary objects (calls the default constructor for each)
	std::cout << "test" << std::endl;
    NetworkController networkController;
	//CmdLineInterface interface(argc, argv);		
	//AppConfig config = interface.getConfig();
	//GUIManager gui;
	std::cout << "test2" << std::endl;
    //init networking
    //if(config.getIsNetworking())
    networkController.startServer();
	std::cout << "test3" << std::endl;
    //if(!config.getIsHeadless())
     //   gui.init();
    //if(config.getIsDebug())
	// std::cout << "Im debugging! :D\n";

    while(true)
    {
		std::cout << "Im here2";
        networkController.waitForPing();

        bool foundGear = true; 
		bool foundBoiler = false;

		std::string msg = "";
		
		std::string gear, boiler;
		
		double distance = 10.0;
		double azimuth = 3.46;
		double altitude = 2.34;

		double bdistance = 7.46;
		double bazimuth  = 5.42;
		double baltitude = 1.36;

		//gear value logic
		if(foundGear)
		{
			gear = "true;";
		}
		else if(!foundGear)
		{
			gear = "false;";
		}

		//boiler value logic
		if(foundBoiler)
		{
			boiler = "true;";
		}
		else if(!foundBoiler)
		{
			boiler = "false;";
		}

		msg.append(gear + boiler + boost::lexical_cast<std::string> (distance) + ";" + boost::lexical_cast<std::string> (azimuth) + ";" + boost::lexical_cast<std::string> (altitude) + ";" + boost::lexical_cast<std::string> (bdistance) + ";" + boost::lexical_cast<std::string> (bazimuth) + ";" + boost::lexical_cast<std::string> (baltitude) + ";");

		//std::cout << msg << std::endl;
        if(foundGear || foundBoiler)
        {

            networkController.sendMessage(msg);
			//determines whether or not to send the distance of the boiler or gear            
			if(foundGear)
			{    
            	std::cout << "Gear Found! Distance: " << distance;
			}
			else if (foundBoiler)
			{
				std::cout << "Boiler Found! Distance: " << bdistance;
			}
        }
        else
        {
            networkController.sendMessage("false;");
        }
    }

    return 0;
}
