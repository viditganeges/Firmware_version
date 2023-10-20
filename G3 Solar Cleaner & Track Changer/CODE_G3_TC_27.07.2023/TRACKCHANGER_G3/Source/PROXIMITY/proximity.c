#include "proximity.h"
#include "PortInterrupt.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "board.h"

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

uint16_t row_detected;

bool IsValidProximitySensorState (void)
{
   return (GPIO_PinRead(PROXIMITY_GPIO, PROXIMITY_PIN) == PROXIMITY_VALID_STATE); 
} 

void Proximity_Irq(void)
{
//  if(enableProximitySensor)
//  {
    if(IsValidProximitySensorState())
    {
      ProximityTimerOn(PROXIMITY_NOISE_TIME_MS);
    }
    else
    {
      ProximityTimerStop();
    }
    GPIO_PortClearInterruptFlags(PROXIMITY_GPIO, 1U << PROXIMITY_PIN);
 // }
}

bool ProximityInit (void)
{
  gpio_pin_config_t Proximity_config = {
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
  
  GPIO_PinInit(PROXIMITY_GPIO, PROXIMITY_PIN, &Proximity_config);
  PORT_SetPinConfig(PROXIMITY_PORT, PROXIMITY_PIN, &pull_up_Config);
  (void)IsValidProximitySensorState();
  EnableInterrupt();
  return true;
}

static void EnableInterrupt (void)
{
  PORT_SetPinInterruptConfig(PROXIMITY_PORT, PROXIMITY_PIN, kPORT_InterruptFallingEdge);
}

static void DisableInterrupt (void)
{
  PORT_SetPinInterruptConfig(PROXIMITY_PORT, PROXIMITY_PIN, kPORT_InterruptOrDMADisabled); 
}

static void EnableProximitySensor (void)
{
   //enableProximitySensor = true;
   EnableInterrupt(); 
}

static void DisableProximitySensor (void)
{
   DisableInterrupt();
   //enableProximitySensor = false;
}

void StartProximityCount (void)
{
  ProximityTimerStop();
  proximityCount = 0; 
  GPIO_PortClearInterruptFlags(PROXIMITY_GPIO, 1U << PROXIMITY_PIN);
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

//void ClearRow_DetectCount (void)
//{
//  row_detected = 0;  
//}

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

//static inline bool IsProximityTimeOver (void)
//{
//  return (proximityTimeOver);
//}
//
//static inline void ClearProximityTimeOver (void)
//{
//  proximityTimeOver = false; 
//}

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
