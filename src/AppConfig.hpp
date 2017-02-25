#ifndef AppConfig_hpp
#define AppConfig_hpp

#include <string>

class AppConfig
{
    public:
        AppConfig();
        void setFileName(std::string fileName);
        void setGearDeviceID(int deviceID);
        void setBoilerDeviceID(int deviceID);
        void setIsFile(int isFile);
        void setIsGearDevice(int inputIsDevice);
        void setIsBoilerDevice(int inputIsDevice);
        void setIsHeadless(int isHeadless);
        void setIsNetworking(int isNetworking);
        void setIsDebug(int debug);
        int getGearDeviceID();
        int getBoilerDeviceID();
        int getIsFile();
        int getIsGearDevice();
        int getIsBoilerDevice();
        int getIsHeadless();
        int getIsNetworking();
        int getIsDebug();
        std::string getFileName();
    private:
        std::string fileName;
        int gearDeviceID;
        int boilerDeviceID;
        int isFile;
        int isGearDevice;
        int isBoilerDevice;
        int isHeadless;
        int isNetworking;
        int isDebug;
};

#endif /* AppConfig_hpp */
