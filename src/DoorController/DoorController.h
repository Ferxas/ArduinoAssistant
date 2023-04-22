#ifndef DoorController_H_INCLUDED
#define DoorController_H_INCLUDED
#include "tools\delegate.h"
#include <Arduino.h>
#include <stdio.h>
class DoorController
{
private:
    void AsyncOpenDoor(); 
   void PrivateDoorIsRinging();
  // void WaittingRing();
   void AsyncWaittingRing();
public:
    Delegate OnDoorIsOpenned;
    Delegate OnDoorIsRinging;
    int delay;
    int timeOpenning;
   TaskHandle_t task;
   TaskHandle_t task2;
    int  OUT_OpenDoor;
    int IN_Ring;
    bool DoorIsOpenning=false;
    void AbrirPuerta();
    DoorController(int pin_OUT_OpenDoor,int pin_IN_Ring,int extDelay, int extTimeOpenning);
    DoorController();
    ~DoorController();

};

#endif
