#include "main.h"

/* IMPORTANT -> Timer Interrupt And Port Interrupt should be of Same priority */

#define CLEANER_DETECT_1_VALID_STATE        false

#define CLEANER_DETECT_1_NOISE_TIME_MS             100    /* Minimum Time For the Signal to be constant for it to be considered as Valid */

static uint32_t cleanerDetect1Count = 0;
static volatile bool cleanerDetect1TimeOver      = false;
static int32_t cleanerDetect1_timecount          = 0;

static void CleanerDetect1TimerOn (uint32_t setCleanerDetect1TimeCount_ms);
static inline void CleanerDetect1TimerStop (void);
static inline void IncrementCleanerDetect1Count (void);
static void EnableCleanerDetect1Sensor (void);
static void DisableCleanerDetect1Sensor (void);
static inline bool IsValidCleanerDetect1SensorState (void);
static void CheckAndIncrementCleanerDetect1Count(void);
static void EnableInterrupt (void);
static void DisableInterrupt (void);

static inline bool IsValidCleanerDetect1SensorState (void)
{
   return (PROXI24V_PC13_BLACK1_Get() == CLEANER_DETECT_1_VALID_STATE); 
} 

bool GetCleanerDetect1PinState(void)
{
    return (PROXI24V_PC13_BLACK1_Get() == CLEANER_DETECT_1_VALID_STATE); 
}

void CleanerDetect1_Irq(void)
{
    if(IsValidCleanerDetect1SensorState())
    {
      CleanerDetect1TimerOn(CLEANER_DETECT_1_NOISE_TIME_MS);
    }
    else
    {
      CleanerDetect1TimerStop();
    }
}

bool CleanerDetect1Init (void)
{
  (void)IsValidCleanerDetect1SensorState();
  return true;
}

static void EnableInterrupt (void)
{
  PROXI24V_PC13_BLACK1_InterruptEnable();
}

static void DisableInterrupt (void)
{
  PROXI24V_PC13_BLACK1_InterruptDisable(); 
}

static void EnableCleanerDetect1Sensor (void)
{
   EnableInterrupt(); 
}

static void DisableCleanerDetect1Sensor (void)
{
   DisableInterrupt();
}

void StartCleanerDetect1Count (void)
{
  CleanerDetect1TimerStop();
  cleanerDetect1Count = 0; 
  EnableCleanerDetect1Sensor();
}

void ResumeCleanerDetect1Count (void)
{
  EnableCleanerDetect1Sensor();
}


void StopCleanerDetect1Count (void)
{
  DisableCleanerDetect1Sensor();
}

static inline void IncrementCleanerDetect1Count (void)
{
  cleanerDetect1Count++;  
}

uint32_t GetCleanerDetect1Count (void)
{
  return cleanerDetect1Count;  
}

void ClearCleanerDetect1Count (void)
{
  cleanerDetect1Count = 0;  
}

void SetCleanerDetect1Count (uint32_t count)
{
  cleanerDetect1Count = count;  
}

static void CheckAndIncrementCleanerDetect1Count(void)
{
  if(IsValidCleanerDetect1SensorState())
  {
    IncrementCleanerDetect1Count();
  }  
}

static void CleanerDetect1TimerOn (uint32_t setCleanerDetect1TimeCount_ms)
{
  if(setCleanerDetect1TimeCount_ms == 0)
  {
    cleanerDetect1TimeOver = true;
    CleanerDetect1TimerStop();
  }
  else
  {
    cleanerDetect1_timecount = setCleanerDetect1TimeCount_ms;
    cleanerDetect1TimeOver = false;
  }
}

static inline void CleanerDetect1TimerStop (void)
{
  cleanerDetect1_timecount = 0;
}

void CleanerDetect1TimeIncrement_ms (void)
{
  if(cleanerDetect1_timecount)
  {
    if(--cleanerDetect1_timecount <= 0)
    {
      cleanerDetect1TimeOver = true;
      CheckAndIncrementCleanerDetect1Count();
      CleanerDetect1TimerStop();
    }
    else
    {
      cleanerDetect1TimeOver = false;
    }
  }
}
