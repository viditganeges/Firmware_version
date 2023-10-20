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
}
 
void RelayOffPulse (void)
{
   GPIO_PD2_MotorRelayCtrl2_Set();
   GPIO_PA29_MotorRelayCtrl1_Clear();
}
 
void RelayNoPulse (void)
{
  GPIO_PA29_MotorRelayCtrl1_Clear();
  GPIO_PD2_MotorRelayCtrl2_Clear();
}


void RelayFSM (void)
{
    if(relaycontrol==2)
    {
        RelayOffPulse();
        strelay* getrelay = GetSetrelayLimits(); 
        RelayTimerOn((getrelay->T1)*1000);
        relaycontrol = 1;
    }
    else if ((relayTimeOver == true)&&(relaycontrol == 1)) 
    {
        RelayOnPulse();
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