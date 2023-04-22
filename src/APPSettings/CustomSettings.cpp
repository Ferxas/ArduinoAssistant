#include "CustomSettings.h"
#include <nvs.h> 
using namespace std;
 Preferences preferences;
 static const char *TAGSettings = "SettingsManager";
 const char *params=NULL;
 
 CustomWebSettings::~CustomWebSettings(){}
CustomWebSettings::CustomWebSettings(bool start)
 {
    if (!start)return;
        //  params=param;
            preferences.begin("myApp", false); 
           
       
    //    preferences.begin("myApp", false); 
  
    //     ESP_LOGI(TAGSettings,"Writting defaults settings in %s...",params);
     
        SavePreference("Wireless_SSID","",false);
        SavePreference("Wireless_Pass","",false);
        SavePreference("MQTT_Server","",false);
         SavePreference("MQTT_User","",false);
         SavePreference("MQTT_Pass","",false);
         SavePreference("Door_delay","500",false);
         SavePreference("Door_OpenTime","2000",false); 
      
    //   //  nvs_commit(my_handle); 
    //  //   nvs_close(my_handle);
    // //   preferences.end();
    
    //     vTaskDelay(pdMS_TO_TICKS(1000));
 }


 int  CustomWebSettings::SavePreference(const char *key,const char *value, bool override)
 {
 
 ESP_LOGI(TAGSettings,"%s",key);
  if (preferences.isKey(key) && !override) return 0;

      //  ESP_LOGI(TAGSettings,"Saving %s in the key %s... String Size: %s",value.c_str(),key,value.length());
        int r=   preferences.putString(key,value);
         vTaskDelay(pdMS_TO_TICKS(100));
        return r;
  return 0;
 }


 string CustomWebSettings::ReadPreference(const char *key)
 {
       String a = preferences.getString(key);
   //  ESP_LOGI("","Wireless_SSID: %s",a);

   // ESP_LOGI(TAGSettings,"%s",key);
   if (preferences.isKey(key))
    {
     String r2=  preferences.getString(key);
  //  ESP_LOGI(TAGSettings,"%s is %s.",key,r2);
    const std::string s( r2.begin(), r2.end());
    //  ESP_LOGI(TAGSettings,"%s is %s.",key,s);
        //  ESP_LOGI(TAGSettings,"%s is %s.",key,s.data());
          const char * pp=s.data();
            ESP_LOGI(TAGSettings,"%s is %s.",key,pp);
        return s;
    }
    else
    {
     //  preferences.end();
       // nvs_close(my_handle);
        ESP_LOGE(TAGSettings,"%s is missed.",key);
        String str(key);
        
        return  str.c_str(); 
   }
 }

