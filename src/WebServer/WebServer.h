#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED
#include "WiFi.h" 
#include "ESPAsyncWebServer.h" 
#include "SPIFFS.h"
#include "APPSettings\CustomSettings.h"

class MyOwnWebServer
{
    private:
        bool Start(int port); 
      
    public:
        bool Started=false;
        int port=80;
        bool CheckAndStart(int port);
       MyOwnWebServer();
        MyOwnWebServer(CustomWebSettings *extSettings);
        ~MyOwnWebServer();

};

#endif

