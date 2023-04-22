#include "IOConfig/IOConfig.h"
#include "DoorController/DoorController.h"
#include "tools\tools.h"
using namespace TOOLS;
//******   DEFINICION FUNCIONES   ******
void SyncAbrirPuerta(DoorController * args);

void  DoorController::PrivateDoorIsRinging()
{
Serial.println("Han tocado al timbre!");
OnDoorIsRinging();
} 

void  WaittingRing(DoorController door)
{
 
  bool STAT_Timbre= digitalRead(door.IN_Ring);
  if (FP(STAT_Timbre,0).Value & !door.DoorIsOpenning)  
  {
    Serial.println("Han tocado al timbre!");
    door.OnDoorIsRinging();
  }
 
 
   vTaskDelete(NULL);
}
void  DoorController::AsyncWaittingRing()
{
   while (true)
  {
  xTaskCreate(
 (TaskFunction_t)WaittingRing,
    "WaittingRing",
    10000,
    this,
    100,&task2);  
  vTaskDelay(pdMS_TO_TICKS(100));
  }
}
DoorController::DoorController(int pin_OUT_OpenDoor,int pin_IN_Ring,int extDelay, int extTimeOpenning){
  OUT_OpenDoor=pin_OUT_OpenDoor;
  IN_Ring=pin_IN_Ring;
  delay=extDelay;
  timeOpenning=extTimeOpenning;
  pinMode(OUT_OpenDoor, OUTPUT);
  pinMode(IN_Ring, INPUT);
  DoorIsOpenning=false;
  AsyncWaittingRing();
  attachInterrupt(IN_Ring,PrivateDoorIsRinging,RISING);
};
DoorController::~DoorController(){};
DoorController::DoorController(){};

void DoorController::AsyncOpenDoor()
{
 xTaskCreate(
 (TaskFunction_t)SyncAbrirPuerta,
    "OpenDoor",
    10000,
    this,
    100,&task);   
}
void DoorController::AbrirPuerta()
{
  AsyncOpenDoor();
}
void SyncAbrirPuerta(DoorController * args)
{
  vTaskDelay(pdMS_TO_TICKS(args->delay));
  Serial.println("Abriendo puerta...");
  digitalWrite(args->OUT_OpenDoor,1);
  args->DoorIsOpenning=true;
  vTaskDelay(pdMS_TO_TICKS(args->timeOpenning));
  digitalWrite(args->OUT_OpenDoor,0);
  args->DoorIsOpenning=false;
  Serial.println("Puerta abierta");
  args->OnDoorIsOpenned();
  vTaskDelete(NULL);
}