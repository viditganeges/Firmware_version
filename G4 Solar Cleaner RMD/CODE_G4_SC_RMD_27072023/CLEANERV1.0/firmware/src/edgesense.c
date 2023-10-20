#include "main.h"

#define EDGE_SENSE1_VALID_STATE        true   /* IMPORTANT -> Timer Interrupt And Port Interrupt should be of Same priority */
#define EDGE_SENSE2_VALID_STATE        true 

#define EDGESENSE1_NOISE_TIME_MS           200    /* Minimum Time For the Signal to be constant for it to be considered as Valid */
#define EDGESENSE2_NOISE_TIME_MS           200 

static bool edgeSense1Flag = false;
static bool edgeSense2Flag = false;

static volatile bool edgeSense1TimeOver      = false;
static volatile bool edgeSense2TimeOver      = false;

static int32_t edgeSense1_timecount          = 0;
static int32_t edgeSense2_timecount          = 0;

static bool edgesensor1IntState = false;
static bool edgesensor2IntState = false;

static void EdgeSense1TimerOn (uint32_t setEdgeSense1TimeCount_ms);
static inline void EdgeSense1TimerStop (void);
static inline void SetEdgeSense1Flag (void);
static inline void ClearEdgeSense1Flag (void);
static inline bool IsValidEdgeSense1SensorState (bool state);
static void CheckAndSetEdgeSense1(void);
static void EnableInterrupt1 (void);
static void DisableInterrupt1 (void);
static void UpdateEdgeSense1 (bool state);

char Forward_End = 0;char Reverse_End = 0;

static void EdgeSense2TimerOn (uint32_t setEdgeSense2TimeCount_ms);
static inline void EdgeSense2TimerStop (void);
static inline void SetEdgeSense2Flag (void);
static inline void ClearEdgeSense2Flag (void);
static inline bool IsValidEdgeSense2SensorState (bool state);
static void CheckAndSetEdgeSense2(void);
static void EnableInterrupt2 (void);
static void DisableInterrupt2 (void);
static void UpdateEdgeSense2 (bool state);

static bool edgeSensorEnabledState = true;

static inline bool IsValidEdgeSense1SensorState (bool state)
{
   return (state == EDGE_SENSE1_VALID_STATE); 
} 

static inline bool ReadEdgeSense1SensorState (void)
{
  return GPIO_PA22_LimitSW_IN1_Get();
  Forward_End = 1;
}
  
char GetEdgeSense1State()
{
  if(ReadEdgeSense1SensorState ())
  {
    return 1;
  }
  else
  {
    return 0;
  }
 }


void Edge_Sense1_Irq (void)
{
    edgesensor1IntState = ReadEdgeSense1SensorState();
    EdgeSense1TimerOn(EDGESENSE1_NOISE_TIME_MS);
}

bool EdgeSense1Init (void)
{
  DisableInterrupt1();
  UpdateSense1State();
  EnableInterrupt1();
  return true;
}

void UpdateSense1State (void)
{
  edgesensor1IntState = ReadEdgeSense1SensorState();
  UpdateEdgeSense1(edgesensor1IntState);   
}

static void EnableInterrupt1 (void)
{
  GPIO_PA22_LimitSW_IN1_InterruptEnable();
}

static void DisableInterrupt1 (void)
{
  GPIO_PA22_LimitSW_IN1_InterruptDisable();
}

static inline void SetEdgeSense1Flag (void)
{
  edgeSense1Flag = true;  
}

static inline void ClearEdgeSense1Flag (void)
{
  edgeSense1Flag = false;  
}

bool IsEdgeSense1Detected (void)
{
  return edgeSense1Flag;
}

void SetEdgeSense1State (bool state)
{
  edgeSense1Flag = state;  
}

static void UpdateEdgeSense1 (bool state)
{
  if(IsValidEdgeSense1SensorState(state))
  {
    SetEdgeSense1Flag();
  }
  else
  {
    ClearEdgeSense1Flag();
  } 
}

static void CheckAndSetEdgeSense1(void)
{
  bool curState = ReadEdgeSense1SensorState();
  if(edgesensor1IntState == curState)
  {
    UpdateEdgeSense1(curState);
  }
}

static void EdgeSense1TimerOn (uint32_t setEdgeSense1TimeCount_ms)
{
  if(setEdgeSense1TimeCount_ms == 0)
  {
    edgeSense1TimeOver = true;
    EdgeSense1TimerStop();
  }
  else
  {
    edgeSense1_timecount = setEdgeSense1TimeCount_ms;
    edgeSense1TimeOver = false;
  }
}

static inline void EdgeSense1TimerStop (void)
{
  edgeSense1_timecount = 0;
}

void EdgeSense1TimeIncrement_ms (void)
{
  if(edgeSense1_timecount)
  {
    if(--edgeSense1_timecount <= 0)
    {
      edgeSense1TimeOver = true;
      CheckAndSetEdgeSense1();
      EdgeSense1TimerStop();
    }
    else
    {
      edgeSense1TimeOver = false;
    }
  }
}


static inline bool IsValidEdgeSense2SensorState (bool state)
{
   return (state == EDGE_SENSE2_VALID_STATE); 
} 

static inline bool ReadEdgeSense2SensorState (void)
{
  return GPIO_PC1_LimitSW_IN2_Get();
  Reverse_End = 1;
}
  
char GetEdgeSense2State()
{
  if(ReadEdgeSense2SensorState ())
  {
    return 1;
  }
  else
  {
    return 0;
  }
 }

void Edge_Sense2_Irq(void)
{
    edgesensor2IntState = ReadEdgeSense2SensorState();
    EdgeSense2TimerOn(EDGESENSE2_NOISE_TIME_MS);
}

bool EdgeSense2Init (void)
{
  DisableInterrupt2();
  UpdateSense2State();
  EnableInterrupt2();
  return true;
}

void UpdateSense2State (void)
{
  edgesensor2IntState = ReadEdgeSense2SensorState();
  UpdateEdgeSense2(edgesensor2IntState);   
}

static void EnableInterrupt2 (void)
{
  GPIO_PC1_LimitSW_IN2_InterruptEnable(); 
}

static void DisableInterrupt2 (void)
{
  GPIO_PC1_LimitSW_IN2_InterruptDisable();
}

static inline void SetEdgeSense2Flag (void)
{
  edgeSense2Flag = true;  
}

static inline void ClearEdgeSense2Flag (void)
{
  edgeSense2Flag = false;  
}

bool IsEdgeSense2Detected (void)
{
  return edgeSense2Flag;
}

void SetEdgeSense2State (bool state)
{
  edgeSense2Flag = state;  
}

static void UpdateEdgeSense2 (bool state)
{
  if(IsValidEdgeSense2SensorState(state))
  {
    SetEdgeSense2Flag();
  }
  else
  {
    ClearEdgeSense2Flag();
  } 
}

static void CheckAndSetEdgeSense2(void)
{
  bool curState = ReadEdgeSense2SensorState();
  if(edgesensor2IntState == curState)
  {
    UpdateEdgeSense2(curState);
  }
}

static void EdgeSense2TimerOn (uint32_t setEdgeSense2TimeCount_ms)
{
  if(setEdgeSense2TimeCount_ms == 0)
  {
    edgeSense2TimeOver = true;
    EdgeSense2TimerStop();
  }
  else
  {
    edgeSense2_timecount = setEdgeSense2TimeCount_ms;
    edgeSense2TimeOver = false;
  }
}

static inline void EdgeSense2TimerStop (void)
{
  edgeSense2_timecount = 0;
}

void EdgeSense2TimeIncrement_ms (void)
{
  if(edgeSense2_timecount)
  {
    if(--edgeSense2_timecount <= 0)
    {
      edgeSense2TimeOver = true;
      CheckAndSetEdgeSense2();
      EdgeSense2TimerStop();
    }
    else
    {
      edgeSense2TimeOver = false;
    }
  }
}

bool EdgeSenseInit (void)
{
  bool status = true;
  status &= EdgeSense1Init();
  status &= EdgeSense2Init();  
  return status;
}

bool *GetSetEdgeSensorEnabledState(void)
{
  return(&edgeSensorEnabledState);
}

bool IsEdgeSensorEnabled (void)
{
  return edgeSensorEnabledState;
}

bool IsEdgeSensor1Detected (void)
{
  if(IsEdgeSensorEnabled())
  {
    return IsEdgeSense1Detected(); 
  }
  else 
  {
    return false;
  }
}

bool IsEdgeSensor2Detected (void)
{
  if(IsEdgeSensorEnabled())
  {
    return IsEdgeSense2Detected(); 
  }
  else 
  {
    return false;
  }  
}

bool IsPositiveEdgeReached (void)
{
  return IsEdgeSensor1Detected();
}

bool IsNegativeEdgeReached (void)
{
   return IsEdgeSensor2Detected(); 
}

void EdgeSenseTimeIncrement_ms (void)
{
  EdgeSense1TimeIncrement_ms();
  EdgeSense2TimeIncrement_ms();  
}

