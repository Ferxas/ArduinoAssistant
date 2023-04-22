#include "WebServer/WebServer.h"
//#include "freertos/task.h"
  CustomWebSettings *settingsApp;
AsyncWebServer server(80);
String ledState=""; 
static const char *TAGLog = "WebServer";

String processor(const String& var){

 //ESP_LOGI(TAGLog,"Readding %s...",var);
    String Wkey=var;
      string skey( Wkey.begin(), Wkey.end());
 string svalue= settingsApp->ReadPreference(skey.c_str());
 Serial.println(svalue.c_str());
 String wvalue(svalue.c_str());
  Serial.println(wvalue.c_str());
  return wvalue;
}

void Getprocessor(AsyncWebServerRequest *request){
 
    String inputParam;


    size_t paramsNr = request->params();
   
    for (size_t i = 0; i < paramsNr; i++)
    {
      AsyncWebParameter *parameter= request->getParam(i);
      String Wvalue=parameter->value();
      string svalue( Wvalue.begin(), Wvalue.end());

     settingsApp->SavePreference(parameter->name().c_str(),svalue.c_str(),true);
   
       
    }
    
   // request->send(SPIFFS, "/index.html", String(), false, processor);
    
     request->send(200, "text/html", "Cambios guardados correctamente.\r\nReiniciando el dispositivo...\r\nActualice la pagina pasados 10 segundos. Gracias.");
     ESP.restart();
}
bool MyOwnWebServer::Start(int port )
{
 

server.on
  (
    "/", HTTP_GET, [](AsyncWebServerRequest *request)
      {
      request->send(SPIFFS, "/index.html", String(), false, processor);
      }
  );

server.on(
  "/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(SPIFFS, "/style.css","text/css");
    }
);
server.on
(
  "/get", HTTP_GET, [] (AsyncWebServerRequest *request) 
{
  Getprocessor(request);
}
);
   Started=true;
   server.begin();
    Serial.println("*****   HTTP_SERVER is initialed  ****** ");
    return true;
}

bool MyOwnWebServer::CheckAndStart(int serverport)
{

    if (Started)
    {
          return false;
    }
     this->port=serverport;
   if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return false;
  }
     return Start(port);;
}
 MyOwnWebServer::MyOwnWebServer(){};
MyOwnWebServer::MyOwnWebServer(CustomWebSettings *extSettings)
{
settingsApp= extSettings;
}
MyOwnWebServer::~MyOwnWebServer()
{

}

