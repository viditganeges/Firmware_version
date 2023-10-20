#include "cleanerdetect2.h"
#include "PortInterrupt.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "board.h"

/* IMPORTANT -> Timer Interrupt And Port Interrupt should be of Same priority */

#define CLEANER_DETECT_2_VALID_STATE        false

#define CLEANER_DETECT_2_NOISE_TIME_MS             100    /* Minimum Time For the Signal to be constant for it to be considered as Valid */

static uint32_t cleanerDetect2Count = 0;
//static bool enableCleanerDetect2Sensor = true;
static volatile bool cleanerDetect2TimeOver      = false;
static int32_t cleanerDetect2_timecount          = 0;

static void CleanerDetect2TimerOn (uint32_t setCleanerDetect2TimeCount_ms);
static inline void CleanerDetect2TimerStop (void);
//static inline bool IsCleanerDetect2TimeOver (void);
//static inline void ClearCleanerDetect2TimeOver (void);
static inline void IncrementCleanerDetect2Count (void);
static void EnableCleanerDetect2Sensor (void);
static void DisableCleanerDetect2Sensor (void);
static inline bool IsValidCleanerDetect2SensorState (void);
static void CheckAndIncrementCleanerDetect2Count(void);
static void EnableInterrupt (void);
static void DisableInterrupt (void);

static inline bool IsValidCleanerDetect2SensorState (void)
{
   return (GPIO_PinRead(CLEANER_DETECT_2_GPIO, CLEANER_DETECT_2_PIN) == CLEANER_DETECT_2_VALID_STATE); 
} 

bool GetCleanerDetect2PinState(void)
{
return (GPIO_PinRead(CLEANER_DETECT_2_GPIO, CLEANER_DETECT_2_PIN) == CLEANER_DETECT_2_VALID_STATE); 
}


void CleanerDetect2_Irq(void)
{
//  if(enableCleanerDetect2Sensor)
//  {
    if(IsValidCleanerDetect2SensorState())
    {
      CleanerDetect2TimerOn(CLEANER_DETECT_2_NOISE_TIME_MS);
    }
    else
    {
      CleanerDetect2TimerStop();
    }
    GPIO_PortClearInterruptFlags(CLEANER_DETECT_2_GPIO, 1U << CLEANER_DETECT_2_PIN);
 // }
}

bool CleanerDetect2Init (void)
{
  gpio_pin_config_t CleanerDetect2_config = {
    kGPIO_DigitalInput, 0,
  };
  
  port_pin_config_t pull_up_Config = {
    kPORT_PullUp,
    kPORT_SlowSlewRate,
    kPORT_PassiveFilterDisable,
    kPORT_OpenDrainDisable,
    kPORT_HighDriveStrength,
    kPORT_MuxAsGpio,
    kPORT_UnlockRegister,     
  };
  
  GPIO_PinInit(CLEANER_DETECT_2_GPIO, CLEANER_DETECT_2_PIN, &CleanerDetect2_config);
  PORT_SetPinConfig(CLEANER_DETECT_2_PORT, CLEANER_DETECT_2_PIN, &pull_up_Config);
  (void)IsValidCleanerDetect2SensorState();
 // EnableInterrupt();
  return true;
}

static void EnableInterrupt (void)
{
  PORT_SetPinInterruptConfig(CLEANER_DETECT_2_PORT, CLEANER_DETECT_2_PIN, kPORT_InterruptFallingEdge);
}

static void DisableInterrupt (void)
{
  PORT_SetPinInterruptConfig(CLEANER_DETECT_2_PORT, CLEANER_DETECT_2_PIN, kPORT_InterruptOrDMADisabled); 
}

static void EnableCleanerDetect2Sensor (void)
{
   //enableCleanerDetect2Sensor = true;
   EnableInterrupt(); 
}

static void DisableCleanerDetect2Sensor (void)
{
   DisableInterrupt();
   //enableCleanerDetect2Sensor = false;
}

void StartCleanerDetect2Count (void)
{
  CleanerDetect2TimerStop();
  cleanerDetect2Count = 0; 
  GPIO_PortClearInterruptFlags(CLEANER_DETECT_2_GPIO, 1U << CLEANER_DETECT_2_PIN);
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

//static inline bool IsCleanerDetect2TimeOver (void)
//{
//  return (cleanerDetect2TimeOver);
//}
//
//static inline void ClearCleanerDetect2TimeOver (void)
//{
//  cleanerDetect2TimeOver = false; 
//}

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
