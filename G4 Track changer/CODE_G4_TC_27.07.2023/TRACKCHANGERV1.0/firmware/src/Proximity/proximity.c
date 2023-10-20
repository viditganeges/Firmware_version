#include "main.h"

/* IMPORTANT -> Timer Interrupt And Port Interrupt should be of Same priority */

#define PROXIMITY_VALID_STATE        false

#define PROXIMITY_NOISE_TIME_MS             100    /* Minimum Time For the Signal to be constant for it to be considered as Valid */

static uint32_t proximityCount = 0;    //changed

//uint32_t proximityCount = 0;
//static bool enableProximitySensor = true;
static volatile bool proximityTimeOver      = false;
static int32_t proximity_timecount          = 0;

static void ProximityTimerOn (uint32_t setProximityTimeCount_ms);
static inline void ProximityTimerStop (void);
//static inline bool IsProximityTimeOver (void);
//static inline void ClearProximityTimeOver (void);
static inline void IncrementProximityCount (void);
static void EnableProximitySensor (void);
static void DisableProximitySensor (void);
static void CheckAndIncrementProximityCount(void);
static void EnableInterrupt (void);
static void DisableInterrupt (void);



bool IsValidProximitySensorState (void)
{ 
    return (PROXI24V_PA19_BLACK3_Get() == PROXIMITY_VALID_STATE);
} 

void Proximity_Irq(void)
{
    if(IsValidProximitySensorState())
    {
      ProximityTimerOn(PROXIMITY_NOISE_TIME_MS);
    }
    else
    {
      ProximityTimerStop();
    }
}

bool ProximityInit (void)
{
 (void)IsValidProximitySensorState();
  EnableInterrupt();
  return true;
}

static void EnableInterrupt (void)
{
  PROXI24V_PA19_BLACK3_InterruptEnable();
}

static void DisableInterrupt (void)
{
  PROXI24V_PA19_BLACK3_InterruptDisable();  
}

static void EnableProximitySensor (void)
{
   EnableInterrupt(); 
}

static void DisableProximitySensor (void)
{
   DisableInterrupt();
}

void StartProximityCount (void)
{
  ProximityTimerStop();
  proximityCount = 0; 
  EnableProximitySensor();
}

void ResumeProximityCount (void)
{
  EnableProximitySensor();
}


void StopProximityCount (void)
{
  DisableProximitySensor();
}

static inline void IncrementProximityCount (void)
{
  proximityCount++;  
  row_detected++;
}

uint32_t GetProximityCount (void)
{
  return proximityCount;  
}

void ClearProximityCount (void)
{
  proximityCount = 0;  
}

void SetProximityCount (uint32_t count)
{
  proximityCount = count;  
}

static void CheckAndIncrementProximityCount(void)
{
  if(IsValidProximitySensorState())
  {
    IncrementProximityCount();
  }  
}

static void ProximityTimerOn (uint32_t setProximityTimeCount_ms)
{
  if(setProximityTimeCount_ms == 0)
  {
    proximityTimeOver = true;
    ProximityTimerStop();
  }
  else
  {
    proximity_timecount = setProximityTimeCount_ms;
    proximityTimeOver = false;
  }
}

static inline void ProximityTimerStop (void)
{
  proximity_timecount = 0;
}

void ProximityTimeIncrement_ms (void)
{
  if(proximity_timecount)
  {
    if(--proximity_timecount <= 0)
    {
      proximityTimeOver = true;
      CheckAndIncrementProximityCount();
      ProximityTimerStop();
    }
    else
    {
      proximityTimeOver = false;
    }
  }
}
