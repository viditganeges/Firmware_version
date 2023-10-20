#include "main.h"

#define ROTATESENSOR_1  true
#define COUNTSENSOR     ROTATESENSOR_1          // ROTATESENSOR_2

#define ROTATE_SENSE1_VALID_STATE        true  /* IMPORTANT -> Timer Interrupt And Port Interrupt should be of Same priority */
#define ROTATE_SENSE2_VALID_STATE        true

#define SENSE1_NOISE_TIME_MS             10    /* Minimum Time For the Signal to be constant for it to be considered as Valid */
#define SENSE2_NOISE_TIME_MS             10    /* Minimum Time For the Signal to be constant for it to be considered as Valid */


static uint32_t rotateCount = 0;
//static uint32_t AcelrotateCount = 0;
static volatile bool rotateSense1TimeOver      = false;
static int32_t rotateSense1_timecount          = 0;
static void RotateSense1TimerOn (uint32_t setRotateSense1TimeCount_ms);
static inline void RotateSense1TimerStop (void);
static inline void IncrementRotateCount (void);
//static inline void IncrementAcelRotateCount (void);
static void EnableRotateSense1Sensor (void);
static void DisableRotateSense1Sensor (void);
static inline bool IsValidRotateSense1SensorState (void);
static void CheckAndIncrementRotateCount(void);
static void EnableInterrupt1 (void);
static void DisableInterrupt1 (void);


static uint32_t rotateCount2 = 0;
//static uint32_t Brush_rotateCount = 0;
static volatile bool rotateSense2TimeOver      = false;
static int32_t rotateSense2_timecount          = 0;
static void RotateSense2TimerOn (uint32_t setRotateSense2TimeCount_ms);
static inline void RotateSense2TimerStop (void);
static inline void IncrementRotateCount2 (void);
//static inline void IncrementBrushRotateCount (void);
static void EnableRotateSense2Sensor (void);
static void DisableRotateSense2Sensor (void);
static inline bool IsValidRotateSense2SensorState (void);
static void CheckAndIncrementRotateCount2(void);
//static void CheckAndIncrementBrushRotateCount(void);
static void EnableInterrupt2 (void);
static void DisableInterrupt2 (void);

//static bool RotateSensorEnabledState = true;

static inline bool IsValidRotateSense1SensorState (void)
{ 
   return(PROX1_PE4_Get()== ROTATE_SENSE1_VALID_STATE);
} 
static inline bool IsValidRotateSense2SensorState (void)
{
   return(PROX2_PD30_Get()== ROTATE_SENSE2_VALID_STATE);
} 

//char GetRotateSense1State()
//{
//  if(IsValidRotateSense1SensorState())
//  {
//    return 1;
//  }
//  else
//  {
//    return 0;
//  }
//}
//char GetRotateSense2State()
//{
//  if(IsValidRotateSense2SensorState())
//  {
//    return 1;
//  }
//  else
//  {
//    return 0;
//  }
//}


void Rotate_Sense1_Irq(void)
{
    if(IsValidRotateSense1SensorState())// && IsRotateSensorEnabled())
    {
      RotateSense1TimerOn(SENSE1_NOISE_TIME_MS);
    }
    else
    {
      RotateSense1TimerStop();
    }
}
void Rotate_Sense2_Irq(void)
{
    if(IsValidRotateSense2SensorState())// && IsRotateSensorEnabled())
    {
      RotateSense2TimerOn(SENSE2_NOISE_TIME_MS);
    }
    else
    {
      RotateSense2TimerStop();
    }
}




bool RotateSense1Init (void)
{
  (void)IsValidRotateSense1SensorState();
  EnableInterrupt1();
  return true;
}
bool RotateSense2Init (void)
{
  (void)IsValidRotateSense2SensorState();
  EnableInterrupt2();
  return true;
}


static void EnableInterrupt1 (void)
{
    PROX1_PE4_InterruptEnable();
}

static void DisableInterrupt1 (void)
{
    PROX1_PE4_InterruptDisable();
}

static void EnableRotateSense1Sensor (void)
{
   EnableInterrupt1(); 
}

static void DisableRotateSense1Sensor (void)
{
   DisableInterrupt1();
}

void StartRotateSense1Count (void)
{
  RotateSense1TimerStop();
  if(GetPrevPauseState() == PA_IDLE)
  {
  rotateCount = 0;
  }
  else
  {
  rotateCount = GetPrevRotateSense1Count();
  }
  EnableRotateSense1Sensor();
}

void ResumeRotateSense1Count (void)
{
  EnableRotateSense1Sensor();
}


void StopRotateSense1Count (void)
{
  DisableRotateSense1Sensor();
}

static inline void IncrementRotateCount (void)
{
  rotateCount++;
  prev_rotateCount =  rotateCount;  
}

//static inline void IncrementAcelRotateCount (void)
//{
//  AcelrotateCount++;  
//}

uint32_t GetRotateSense1CountValue (void)
{
  return rotateCount;  
}

//uint32_t GetAcelRotateCountValue (void)
//{
//  return AcelrotateCount;  
//}

void ClearRotateSense1Count (void)
{
  rotateCount = 0;  
}

//void ClearAcelRotateCount (void)
//{
//  AcelrotateCount = 0;  
//}

void SetRotateSense1Count (uint32_t count)
{
  rotateCount = count;  
}

static void CheckAndIncrementRotateCount(void)
{
  if(IsValidRotateSense1SensorState())
  {
    IncrementRotateCount();
//    if(GetRobotState() == ROBOT_ACCELERATION_SLOW_START)
//    {
//      IncrementAcelRotateCount();
//    }
  }  
}

static void RotateSense1TimerOn (uint32_t setRotateSense1TimeCount_ms)
{
  if(setRotateSense1TimeCount_ms == 0)
  {
    rotateSense1TimeOver = true;
    RotateSense1TimerStop();
  }
  else
  {
    rotateSense1_timecount = setRotateSense1TimeCount_ms;
    rotateSense1TimeOver = false;
  }
}

static inline void RotateSense1TimerStop (void)
{
  rotateSense1_timecount = 0;
}

void RotateSense1TimeIncrement_ms (void)
{
  if(rotateSense1_timecount)
  {
    if(--rotateSense1_timecount <= 0)
    {
      rotateSense1TimeOver = true;
      CheckAndIncrementRotateCount();
      RotateSense1TimerStop();
    }
    else
    {
      rotateSense1TimeOver = false;
    }
  }
}

void DisableRotateSense_1(void)
{
ClearRotateSense1Count();  
DisableInterrupt1();
}


static void EnableInterrupt2 (void)
{
    PROX2_PD30_InterruptEnable(); 
}

static void DisableInterrupt2 (void)
{
    PROX2_PD30_InterruptDisable();
}

static void EnableRotateSense2Sensor (void)
{
   EnableInterrupt2(); 
}

static void DisableRotateSense2Sensor (void)
{
   DisableInterrupt2();
}

void StartRotateSense2Count (void)
{
  RotateSense2TimerStop();
  
  if(GetPrevPauseState() == PA_IDLE)
  {
  rotateCount2 = 0;
  }
  else
  {

  }
  EnableRotateSense2Sensor();
}

void ResumeRotateSense2Count (void)
{
  EnableRotateSense2Sensor();
}


void StopRotateSense2Count (void)
{
  DisableRotateSense2Sensor();
}

static inline void IncrementRotateCount2 (void)
{
  rotateCount2++;  
}

//static inline void IncrementBrushRotateCount (void)
//{
//  Brush_rotateCount++;  
//}

uint32_t GetRotateSense2CountValue (void)
{
  return rotateCount2;  
}

//uint32_t GetBrushRotateSense2CountValue (void)
//{
//  return Brush_rotateCount;  
//}

void ClearRotateSense2Count (void)
{
  rotateCount2 = 0;  
}
//void ClearBrushRotateSense2Count (void)
//{
//  Brush_rotateCount = 0;  
//}

void SetRotateSense2Count (uint32_t count2)
{
  rotateCount2 = count2;  
}

static void CheckAndIncrementRotateCount2(void)
{
  if(IsValidRotateSense2SensorState())
  {
    IncrementRotateCount2();
  }  
}

static void RotateSense2TimerOn (uint32_t setRotateSense2TimeCount_ms)
{
  if(setRotateSense2TimeCount_ms == 0)
  {
    rotateSense2TimeOver = true;
    RotateSense2TimerStop();
  }
  else
  {
    rotateSense2_timecount = setRotateSense2TimeCount_ms;
    rotateSense2TimeOver = false;
  }
}

static inline void RotateSense2TimerStop (void)
{
  rotateSense2_timecount = 0;
}

void RotateSense2TimeIncrement_ms (void)
{
  if(rotateSense2_timecount)
  {
    if(--rotateSense2_timecount <= 0)
    {
      rotateSense2TimeOver = true;
      CheckAndIncrementRotateCount2();
      RotateSense2TimerStop();
    }
    else
    {
      rotateSense2TimeOver = false;
    }
  }
}

void DisableRotateSense_2(void)
{
ClearRotateSense2Count();  
DisableInterrupt2();
}

bool RotateSenseInit (void)
{
  bool status = true;
  status &= RotateSense1Init();
  status &= RotateSense2Init();
  return status;  
}

//bool *GetSetRotateSensorEnabledState(void)
//{
//  return(&RotateSensorEnabledState);
//}
//
//bool IsRotateSensorEnabled (void)
//{
//  return RotateSensorEnabledState;
//}


void SetRotateSenseCount (uint32_t count)
{
  SetRotateSense1Count(count);
  SetRotateSense2Count(count);  
}

void ClearRotateSenseCount (void)
{
  ClearRotateSense1Count();
  ClearRotateSense2Count();
}

void StartRotateSenseCount (void)
{
  StartRotateSense1Count();
  StartRotateSense2Count(); 
}

void ResumeRotateSenseCount (void)
{
  ResumeRotateSense1Count();
  ResumeRotateSense2Count();
}

void StopRotateSenseCount (void)
{
  StopRotateSense1Count();
  StopRotateSense2Count();
}

uint32_t GetRotateSenseCount (void)
{
#if COUNTSENSOR == ROTATESENSOR_1
  return GetRotateSense1CountValue();  
#else
  return GetRotateSense1CountValue(); 
#endif
}

int GetRotateSense1Count (void)
{
  return GetRotateSense1CountValue();  
}

int GetRotateSense2Count (void)
{
  return GetRotateSense2CountValue();    
}

void RotateSenseTimeIncrement_ms (void)
{
  RotateSense1TimeIncrement_ms();
  RotateSense2TimeIncrement_ms();
}

void DisableRotateSense(void)
{
    DisableRotateSense_1();
    DisableRotateSense_2();
}
