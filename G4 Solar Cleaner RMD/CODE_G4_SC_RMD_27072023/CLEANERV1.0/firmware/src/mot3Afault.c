#include "main.h"

static volatile bool iloadTimeOver = false;
static volatile int32_t iloadmax_timecount = 0;
static volatile bool IloadMaxTimeOnFlag = false;
static volatile bool maxIloadFreqTimeOver = false;
static uint32_t ImotOverCurrentCount = 0;
static int32_t maxIloadFreq_timecount = 0;

static volatile bool mot3OverCurrentFault = false;

static void SetMot3OverCurrentFault (void);
static void IloadMaxTimerOn (uint32_t setMotorTimeCount_ms);
static inline void IloadMaxTimerStop (void);
static inline bool IsIloadMaxTimeOver (void);
static inline void ClearIloadMaxTimeOver (void);
static void ClearIloadMaxOverCount(void);
static inline bool IsIloadMaxTimeOn (void);
static void MaxIloadFreqTimerOn (uint32_t setMaxIloadFreqTimeCount_ms);
static inline void MaxIloadFreqTimerStop (void);
static inline bool IsMaxIloadFreqTimeOver (void);
static inline void ClearMaxIloadFreqTimeOver (void);
static void MaxIloadFreqTimeIncrement_ms (void);

bool IsMot3OverCurrentFault (void)
{
 return mot3OverCurrentFault;
}

void ClearMot3OverCurrentFault (void)
{
  mot3OverCurrentFault = false;
}

void SetMot3OverCurrentFault (void)
{
  mot3OverCurrentFault = true;
}

void ClearMot3MaxOverCurrentCount (void)
{
  ClearIloadMaxOverCount();
}

void CheckMot3OverLoad (void)
{
  float mot3Iload = current_read_C();
  motor3A = mot3Iload;
  stMaxIloadConfigValues maxIloadConfigValues;
  GetMaxIloadConfigValues(&maxIloadConfigValues);
  stMaxMotorCurrent motCurLimits;
  GetMaxMotorCurrentLimits(&motCurLimits);
  float mot3IloadLimit = motCurLimits.Imot3;
  if(GetMotionPauseState() == PA_IDLE)
  {
    if(mot3Iload >= mot3IloadLimit)
    {
      if(IsMot3OverCurrentFault() == false)
      {  
        if(IsIloadMaxTimeOn() == false)
        {
          IloadMaxTimerOn (maxIloadConfigValues.maxIloadNoiseTimeValue);
        }
        else
        {
          if(IsIloadMaxTimeOver() == true)
          {
            IloadMaxTimerStop();
            IloadMaxTimeOnFlag = false;
            if(++ImotOverCurrentCount >= maxIloadConfigValues.maxIloadRepeatCountValue)
            {
              Serial_Debug("\n*** Mot3 Over Current Detected. Iload: *****");
              Serial_Debug_Float(mot3Iload,2);
              SetMot3OverCurrentFault();
              ImotOverCurrentCount = 0;
              MaxIloadFreqTimerStop();
            }
            else
            {
              Serial_Debug("\n *** <Mot3 Over Current Detected - Paused.");
              MaxIloadFreqTimerOn(maxIloadConfigValues.maxIloadFreqTimeValue);
            }
          }
        }
      }
    }
    else
    {
      IloadMaxTimerStop();
      IloadMaxTimeOnFlag = false;
    }
    if(IsMaxIloadFreqTimeOver() == true)
    {
       ClearIloadMaxOverCount();
    }
  }
}

static void ClearIloadMaxOverCount (void)
{
  IloadMaxTimerStop();
  ImotOverCurrentCount = 0;
  IloadMaxTimeOnFlag = false;
  MaxIloadFreqTimerStop();
  ClearMaxIloadFreqTimeOver();
}

static void IloadMaxTimerOn (uint32_t setIloadMaxTimeCount_ms)
{
  if(setIloadMaxTimeCount_ms == 0)
  {
    iloadTimeOver = true;
    IloadMaxTimerStop();
  }
  else
  {
    iloadmax_timecount = setIloadMaxTimeCount_ms;
    IloadMaxTimeOnFlag = true;
    iloadTimeOver = false;
  }
}

static bool IsIloadMaxTimeOn (void)
{
  return(IloadMaxTimeOnFlag);
}

inline void IloadMaxTimerStop (void)
{
  iloadmax_timecount = 0;
}

static inline bool IsIloadMaxTimeOver (void)
{
  return (iloadTimeOver);
}

static inline void ClearIloadMaxTimeOver (void)
{
  iloadTimeOver = false; 
}

void IloadMot3MaxTimeIncrement_ms (void)
{
  if(iloadmax_timecount)
  {
    if(--iloadmax_timecount <= 0)
    {
      iloadTimeOver = true;
      IloadMaxTimerStop();
    }
    else
    {
      ClearIloadMaxTimeOver();
    }
  }
  MaxIloadFreqTimeIncrement_ms();
}

static void MaxIloadFreqTimerOn (uint32_t setMaxIloadFreqTimeCount_ms)
{
  if(setMaxIloadFreqTimeCount_ms == 0)
  {
    maxIloadFreqTimeOver = true;
    MaxIloadFreqTimerStop();
  }
  else
  {
    maxIloadFreq_timecount = setMaxIloadFreqTimeCount_ms;
    maxIloadFreqTimeOver = false;
  }
}

static inline void MaxIloadFreqTimerStop (void)
{
  maxIloadFreq_timecount = 0;
}

static inline bool IsMaxIloadFreqTimeOver (void)
{
  return (maxIloadFreqTimeOver);
}

static inline void ClearMaxIloadFreqTimeOver (void)
{
  maxIloadFreqTimeOver = false; 
}

static void MaxIloadFreqTimeIncrement_ms (void)
{
  if(maxIloadFreq_timecount)
  {
    if(--maxIloadFreq_timecount <= 0)
    {
      maxIloadFreqTimeOver = true;
      MaxIloadFreqTimerStop();
    }
    else
    {
      ClearMaxIloadFreqTimeOver();
    }
  }
}

void BrushMotorFaultCheck(void)
{
    if(BrushMotorFault())
    {
        BrushMotor_Fault = 1; 
    }
    else
    {
        BrushMotor_Fault = 0;
    }
}

void BrushMotorrun(void)
{
    bool drivestate3 = Isdrive3Enabled();
    if(drivestate3 == 1)
    {
     eBrushMotorState brushdriveState = GetBrushMotorState();   
     if((brushdriveState == BRUSHMOTOR_POSITIVE)||(brushdriveState == BRUSHMOTOR_NEGATIVE))
     {
      if(GetRobotState() == ROBOT_ACCELERATION_SLOW_START)    
      {
        if(motor3A <= 0)
        {
           BrushMotorrunFault = 1; 
        }
      }
     }
     else 
     {
       BrushMotorrunFault = 0; 
     }
    }
    else if(drivestate3 == 0)
    {
        if(motor3A > 0)
        {
           BrushMotorrunFault = 1; 
        }
        else 
        {
          BrushMotorrunFault = 0;   
        }
    }
    else 
    {
       BrushMotorrunFault = 0; 
    }
}