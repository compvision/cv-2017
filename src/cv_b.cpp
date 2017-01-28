#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/lexical_cast.hpp>

#include "NetworkController.hpp"
int main(/*int argc, char* argv[]*/)
{
    // Create all the necessary objects (calls the default constructor for each)
	std::cout << "start code" << std::endl;
    NetworkController networkController;
	//CmdLineInterface interface(argc, argv);		
	//AppConfig config = interface.getConfig();
	//GUIManager gui;
	std::cout << "initialized network" << std::endl;
    //init networking
    //if(config.getIsNetworking())
    networkController.startServer();
	std::cout << "started server" << std::endl;
    //if(!config.getIsHeadless())
     //   gui.init();
    //if(config.getIsDebug())
	// std::cout << "Im debugging! :D\n";

    while(true)
    {
		//std::cout << "Entered Loop";
        networkController.waitForPing();

        bool foundGear = true; 
		bool foundBoiler = false;

		std::string msg = "";
		
		std::string gear, boiler;
		
		double gearDistance = 452345;
		double gearAzimuth = 3.46;
		double gearAltitude = 2.34;

		double boilerDistance = 7.7846;
		double boilerAzimuth  = 5.42;
		double boilerAtitude = 666;

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

		msg.append(gear + boiler + boost::lexical_cast<std::string> (gearDistance) + ";" + boost::lexical_cast<std::string> (gearAzimuth) + ";" + boost::lexical_cast<std::string> (gearAltitude) + ";" + boost::lexical_cast<std::string> (boilerDistance) + ";" + boost::lexical_cast<std::string> (boilerAzimuth) + ";" + boost::lexical_cast<std::string> (boilerAtitude) + ";");

		//std::cout << msg << std::endl;
        if(foundGear || foundBoiler)
        {

            networkController.sendMessage(msg);
			//determines whether or not to send the distance of the boiler or gear            
			if(foundGear)
			{    
            	std::cout << "Gear Found! Distance: " << gearDistance << std::endl;
			}
			else if (foundBoiler)
			{
				std::cout << "Boiler Found! Distance: " << boilerDistance << std::endl;
			}
        }
        else
        {
            networkController.sendMessage("false;false;");
        }
    }

    return 0;
}
