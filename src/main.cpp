#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include <Arduino.h>
#include "tools/tools.h"
#include "IOConfig/IOConfig.h"
#include "DoorController/DoorController.h"
#include "WiFi.h"
#include "WebServer/WebServer.h"
#include "APPSettings\CustomSettings.h"

//#include <Preferences.h>
using namespace std;
using namespace TOOLS;

static const char *WIFITAG = "WIFI";
static const char *TAG = "MQTT_TCP";
//const char* wifi_network_ssid = ; .
//const char* wifi_network_password =  ;
const char *soft_ap_ssid = "ExternalDoor_Cnf";
//const char *soft_ap_password = NULL;
const int serverPort=80;
esp_mqtt_client_handle_t client;
DoorController buildingDoor;
MyOwnWebServer myserver;
CustomWebSettings settings =CustomWebSettings(false);
//Preferences preferences;
int Ring_IN;

static void CheckMsg(esp_mqtt_event_handle_t event)
{
      std::string topicStr(event->topic);
        std::string dataStr(event->data);
        topicStr= topicStr.substr(0,event->topic_len);
        dataStr= dataStr.substr(0,event->data_len);
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        if ( topicStr=="Assistant/BuildingDoor/action" & dataStr =="open")
        {
        buildingDoor.AbrirPuerta(); 
      
        printf("\nTOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        }
      
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    switch (event->event_id)
    {
    
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        esp_mqtt_client_subscribe(client, "Assistant/BuildingDoor/action", 0);
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_DATA:
        CheckMsg(event);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb((esp_mqtt_event_handle_t)event_data);
}

static void mqtt_app_start(void)
{
 
  
  string suri=settings.ReadPreference("MQTT_Server").c_str();
  const char * uri=suri.c_str();
//  Serial.println(uri);
 string susername=settings.ReadPreference("MQTT_User").c_str();
  const char* username=susername.c_str();
   string spassword=settings.ReadPreference("MQTT_Pass").c_str();
  const char* password=spassword.c_str();
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = uri,
        .username= username,
        .password= password,
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}
void OnWiFiEvent(WiFiEvent_t event)
{
  switch (event) 
  {
    case SYSTEM_EVENT_STA_CONNECTED:
      myserver.CheckAndStart(serverPort); 
      Serial.println("ESP32 Connected to WiFi Network");
      vTaskDelay(5000 / portTICK_PERIOD_MS);
      mqtt_app_start();
      break;
    case SYSTEM_EVENT_AP_START:
      myserver.CheckAndStart(serverPort);        
      Serial.println("ESP32 soft AP started");
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      Serial.println("Station connected to ESP32 soft AP");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      Serial.println("Station disconnected from ESP32 soft AP");
      break;
    default: break;
  }
}
 
 
void wifi_connection()
{
  WiFi.onEvent(OnWiFiEvent);
  string SSID=settings.ReadPreference("Wireless_SSID");
  string Pass=settings.ReadPreference("Wireless_Pass");
  if (SSID!= "")
  {
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.softAP(soft_ap_ssid, NULL);
    WiFi.begin(
   SSID.c_str() ,
    Pass.c_str()
   );
  }
  else
  {
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(soft_ap_ssid, NULL);
  }
}

void DoorIsOpened()
{
   esp_mqtt_client_publish(client, "BuildingDoor/event", "opened", 0, 1, 0);
}
void DoorIsRinging()
{
  Serial.println("Sendding mqtt message to server....");
  esp_mqtt_client_publish(client, "BuildingDoor/event", "ring", 0, 1, 0);
}
void setup(void)
{
  Serial.begin(115200);
  nvs_flash_init();
  settings= CustomWebSettings(true);
  string delayDoor= settings.ReadPreference("Door_delay");
  string openTimeDoor= settings.ReadPreference("Door_OpenTime");
  buildingDoor = DoorController
  (
    IO13_PIN10,
    IO12_PIN4,
    stoi(delayDoor),
    stoi(openTimeDoor)  
  );
  buildingDoor.OnDoorIsOpenned+= DoorIsOpened;
  buildingDoor.OnDoorIsRinging+= DoorIsRinging;
  myserver = MyOwnWebServer(&settings);
  wifi_connection();
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  myserver.CheckAndStart(serverPort);    
}



void loop()
{

  // bool STAT_Timbre= digitalRead(Ring_IN);
  // if (FP(STAT_Timbre,0).Value & !buildingDoor.DoorIsOpenning)  
  // {
  //   Serial.println("Han tocado al timbre!");
  //   esp_mqtt_client_publish(client, "BuildingDoor/event", "ring", 0, 1, 0);
  // }
}