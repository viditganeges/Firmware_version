#include "cleanerdetect1.h"
#include "PortInterrupt.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "board.h"

/* IMPORTANT -> Timer Interrupt And Port Interrupt should be of Same priority */

#define CLEANER_DETECT_1_VALID_STATE        false

#define CLEANER_DETECT_1_NOISE_TIME_MS             100    /* Minimum Time For the Signal to be constant for it to be considered as Valid */

static uint32_t cleanerDetect1Count = 0;
//static bool enableCleanerDetect1Sensor = true;
static volatile bool cleanerDetect1TimeOver      = false;
static int32_t cleanerDetect1_timecount          = 0;

static void CleanerDetect1TimerOn (uint32_t setCleanerDetect1TimeCount_ms);
static inline void CleanerDetect1TimerStop (void);
//static inline bool IsCleanerDetect1TimeOver (void);
//static inline void ClearCleanerDetect1TimeOver (void);
static inline void IncrementCleanerDetect1Count (void);
static void EnableCleanerDetect1Sensor (void);
static void DisableCleanerDetect1Sensor (void);
static inline bool IsValidCleanerDetect1SensorState (void);
static void CheckAndIncrementCleanerDetect1Count(void);
static void EnableInterrupt (void);
static void DisableInterrupt (void);

static inline bool IsValidCleanerDetect1SensorState (void)
{
   return (GPIO_PinRead(CLEANER_DETECT_1_GPIO, CLEANER_DETECT_1_PIN) == CLEANER_DETECT_1_VALID_STATE); 
} 

bool GetCleanerDetect1PinState(void)
{
return (GPIO_PinRead(CLEANER_DETECT_1_GPIO, CLEANER_DETECT_1_PIN) == CLEANER_DETECT_1_VALID_STATE); 
}

void CleanerDetect1_Irq(void)
{
//  if(enableCleanerDetect1Sensor)
//  {
    if(IsValidCleanerDetect1SensorState())
    {
      CleanerDetect1TimerOn(CLEANER_DETECT_1_NOISE_TIME_MS);
    }
    else
    {
      CleanerDetect1TimerStop();
    }
    GPIO_PortClearInterruptFlags(CLEANER_DETECT_1_GPIO, 1U << CLEANER_DETECT_1_PIN);
 // }
}

bool CleanerDetect1Init (void)
{
  gpio_pin_config_t CleanerDetect1_config = {
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
  
  GPIO_PinInit(CLEANER_DETECT_1_GPIO, CLEANER_DETECT_1_PIN, &CleanerDetect1_config);
  PORT_SetPinConfig(CLEANER_DETECT_1_PORT, CLEANER_DETECT_1_PIN, &pull_up_Config);
  (void)IsValidCleanerDetect1SensorState();
 //EnableInterrupt();
  return true;
}

static void EnableInterrupt (void)
{
  PORT_SetPinInterruptConfig(CLEANER_DETECT_1_PORT, CLEANER_DETECT_1_PIN, kPORT_InterruptFallingEdge);
}

static void DisableInterrupt (void)
{
  PORT_SetPinInterruptConfig(CLEANER_DETECT_1_PORT, CLEANER_DETECT_1_PIN, kPORT_InterruptOrDMADisabled); 
}

static void EnableCleanerDetect1Sensor (void)
{
   //enableCleanerDetect1Sensor = true;
   EnableInterrupt(); 
}

static void DisableCleanerDetect1Sensor (void)
{
   DisableInterrupt();
   //enableCleanerDetect1Sensor = false;
}

void StartCleanerDetect1Count (void)
{
  CleanerDetect1TimerStop();
  cleanerDetect1Count = 0; 
  GPIO_PortClearInterruptFlags(CLEANER_DETECT_1_GPIO, 1U << CLEANER_DETECT_1_PIN);
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

//static inline bool IsCleanerDetect1TimeOver (void)
//{
//  return (cleanerDetect1TimeOver);
//}
//
//static inline void ClearCleanerDetect1TimeOver (void)
//{
//  cleanerDetect1TimeOver = false; 
//}

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
