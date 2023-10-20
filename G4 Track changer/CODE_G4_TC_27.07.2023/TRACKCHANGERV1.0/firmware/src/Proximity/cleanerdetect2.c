#include "main.h"

/* IMPORTANT -> Timer Interrupt And Port Interrupt should be of Same priority */

#define CLEANER_DETECT_2_VALID_STATE        false

#define CLEANER_DETECT_2_NOISE_TIME_MS             100    /* Minimum Time For the Signal to be constant for it to be considered as Valid */

static uint32_t cleanerDetect2Count = 0;
static volatile bool cleanerDetect2TimeOver      = false;
static int32_t cleanerDetect2_timecount          = 0;

static void CleanerDetect2TimerOn (uint32_t setCleanerDetect2TimeCount_ms);
static inline void CleanerDetect2TimerStop (void);
static inline void IncrementCleanerDetect2Count (void);
static void EnableCleanerDetect2Sensor (void);
static void DisableCleanerDetect2Sensor (void);
static inline bool IsValidCleanerDetect2SensorState (void);
static void CheckAndIncrementCleanerDetect2Count(void);
static void EnableInterrupt (void);
static void DisableInterrupt (void);

static inline bool IsValidCleanerDetect2SensorState (void)
{
  // return (PROXI24V_PC15_BLACK2_Get() == CLEANER_DETECT_2_VALID_STATE);  //changed for testing
    return (PROXI24V_PC30_BLACK4_Get() == CLEANER_DETECT_2_VALID_STATE);
} 

bool GetCleanerDetect2PinState(void)
{
    return (PROXI24V_PC30_BLACK4_Get() == CLEANER_DETECT_2_VALID_STATE); 
}


void CleanerDetect2_Irq(void)
{
    if(IsValidCleanerDetect2SensorState())
    {
      CleanerDetect2TimerOn(CLEANER_DETECT_2_NOISE_TIME_MS);
    }
    else
    {
      CleanerDetect2TimerStop();
    }
}

bool CleanerDetect2Init (void)
{
  (void)IsValidCleanerDetect2SensorState();
  return true;
}

static void EnableInterrupt (void)
{
    PROXI24V_PC30_BLACK4_InterruptEnable();
}

static void DisableInterrupt (void)    
{
   PROXI24V_PC30_BLACK4_InterruptDisable(); 
}

static void EnableCleanerDetect2Sensor (void)
{
   EnableInterrupt(); 
}

static void DisableCleanerDetect2Sensor (void)
{
   DisableInterrupt();
}

void StartCleanerDetect2Count (void)
{
  CleanerDetect2TimerStop();
  cleanerDetect2Count = 0; 
  EnableCleanerDetect2Sensor();
}

void ResumeCleanerDetect2Count (void)
{
  EnableCleanerDetect2Sensor();
}


void StopCleanerDetect2Count (void)
{
  DisableCleanerDetect2Sensor();
}

static inline void IncrementCleanerDetect2Count (void)
{
  cleanerDetect2Count++;  
}

uint32_t GetCleanerDetect2Count (void)
{
  return cleanerDetect2Count;  
}

void ClearCleanerDetect2Count (void)
{
  cleanerDetect2Count = 0;  
}

void SetCleanerDetect2Count (uint32_t count)
{
  cleanerDetect2Count = count;  
}

static void CheckAndIncrementCleanerDetect2Count(void)
{
  if(IsValidCleanerDetect2SensorState())
  {
    IncrementCleanerDetect2Count();
  }  
}

static void CleanerDetect2TimerOn (uint32_t setCleanerDetect2TimeCount_ms)
{
  if(setCleanerDetect2TimeCount_ms == 0)
  {
    cleanerDetect2TimeOver = true;
    CleanerDetect2TimerStop();
  }
  else
  {
    cleanerDetect2_timecount = setCleanerDetect2TimeCount_ms;
    cleanerDetect2TimeOver = false;
  }
}

static inline void CleanerDetect2TimerStop (void)
{
  cleanerDetect2_timecount = 0;
}

void CleanerDetect2TimeIncrement_ms (void)
{
  if(cleanerDetect2_timecount)
  {
    if(--cleanerDetect2_timecount <= 0)
    {
      cleanerDetect2TimeOver = true;
      CheckAndIncrementCleanerDetect2Count();
      CleanerDetect2TimerStop();
    }
    else
    {
      cleanerDetect2TimeOver = false;
    }
  }
}
