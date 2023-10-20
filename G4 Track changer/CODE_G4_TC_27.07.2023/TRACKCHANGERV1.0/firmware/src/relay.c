#include "main.h"

static volatile bool relayTimeOver      = false;
static int32_t relay_timecount          = 0;

static void RelayTimerOn (uint32_t setRelayTimeCount_ms);
static inline void RelayTimerStop (void);


bool RelayInit (void)
{
  RelayNoPulse();
  RelayOnPulse();
  return true;
}

void RelayOnPulse (void)
{
  GPIO_PA29_MotorRelayCtrl1_Set();
  GPIO_PD2_MotorRelayCtrl2_Clear(); 
//  relaycontrol = 3;
}
 
void RelayOffPulse (void)
{
   GPIO_PD2_MotorRelayCtrl2_Set();
   GPIO_PA29_MotorRelayCtrl1_Clear();
//   relaycontrol = 4;
}
 
void RelayNoPulse (void)
{
  GPIO_PA29_MotorRelayCtrl1_Clear();
  GPIO_PD2_MotorRelayCtrl2_Clear();
//  relaycontrol = 0;
}


void RelayFSM (void)
{
//    if((GetRobotState() == ROBOT_ACCELERATION_SLOW_START)&&(relaycontrol==2)&&(GetRobotActualDirection() == POSITIVE)) //ROBOT_START)) //&& (GetRobotActualDirection()== POSITIVE))
    if(relaycontrol==2)//&&(GetRobotActualDirection() == POSITIVE))
    {
//        relaycontrol = 1;
        RelayOffPulse();
//        RelayTimerOn(RELAY_NO_PULSE_TIME_MS);
        strelay* getrelay = GetSetrelayLimits(); 
        RelayTimerOn((getrelay->T1)*1000);
        
        relaycontrol = 1;
    }
    else if ((relayTimeOver == true)&&(relaycontrol == 1)) 
    {
        RelayOnPulse();
//        relaycontrol = 0;
    }
    else
    {
//        relayTimeOver = false;
    }
}


static void RelayTimerOn (uint32_t setRelayTimeCount_ms)
{
  if(setRelayTimeCount_ms == 0)
  {
    relayTimeOver = true;
    RelayTimerStop();
  }
  else
  {
    relay_timecount = setRelayTimeCount_ms;
    relayTimeOver = false;
  }
}
static inline void RelayTimerStop (void)
{
  relay_timecount = 0;
}

//void RelayTimerOn (void)
//{
////  uint32_t setRelayTimeCount_ms = 5000;
//  if(setRelayTimeCount_ms == 0)
//  {
//    relay_timecount = 0;
//    RelayOnPulse();      //added
//  }
//  else
//  {
//    relay_timecount = setRelayTimeCount_ms;
//  }
//}

void RelayTimeIncrement_ms (void)
{
  if(relay_timecount)
  {
    if(--relay_timecount <= 0)
    {
      relayTimeOver = true;
      RelayTimerStop();
    }
    else
    {
      relayTimeOver = false; 
    }
  }
}