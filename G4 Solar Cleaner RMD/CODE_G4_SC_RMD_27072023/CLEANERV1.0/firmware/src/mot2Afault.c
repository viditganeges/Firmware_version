#include "main.h"

static volatile bool iloadTimeOver = false;
static volatile int32_t iloadmax_timecount = 0;
static volatile bool IloadMaxTimeOnFlag = false;
static volatile bool maxIloadFreqTimeOver = false;
static uint32_t ImotOverCurrentCount = 0;
static int32_t maxIloadFreq_timecount = 0;

static volatile bool mot2OverCurrentFault = false;

static void SetMot2OverCurrentFault (void);
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

bool IsMot2OverCurrentFault (void)
{
 return mot2OverCurrentFault;
}

void ClearMot2OverCurrentFault (void)
{
  mot2OverCurrentFault = false;
}

void SetMot2OverCurrentFault (void)
{
  mot2OverCurrentFault = true;
}

void ClearMot2MaxOverCurrentCount (void)
{
  ClearIloadMaxOverCount();
}

void CheckMot2OverLoad (void)
{
  float mot2Iload = current_read_B();
  motor2A = mot2Iload;
  stMaxIloadConfigValues maxIloadConfigValues;
  GetMaxIloadConfigValues(&maxIloadConfigValues);
  stMaxMotorCurrent motCurLimits;
  GetMaxMotorCurrentLimits(&motCurLimits);
  float mot2IloadLimit = motCurLimits.Imot2;
  if(GetMotionPauseState() == PA_IDLE)
  {
    if(mot2Iload >= mot2IloadLimit)
    {
      if(IsMot2OverCurrentFault() == false)
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
            if((++ImotOverCurrentCount >= maxIloadConfigValues.maxIloadRepeatCountValue))
            {
              Serial_Debug("\n*** Mot2 Over Current Detected. Iload: *****");
              Serial_Debug_Float(mot2Iload,2);
              SetMot2OverCurrentFault();
              ImotOverCurrentCount = 0;
              MaxIloadFreqTimerStop();
            }
            else
            {
              Serial_Debug("\n *** <Mot2 Over Current Detected - Paused.");
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

void IloadMot2MaxTimeIncrement_ms (void)
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

void Motor2FaultCheck(void)
{
    if(Motor2Fault())
    {
        Motor2_Fault = 1; 
    }
    else
    {
        Motor2_Fault = 0;
    }
}

void Motor2run(void)
{
    bool drivestate2 = Isdrive2Enabled();
    if(drivestate2 == 1)
    {
     eMotor2State motor2driveState = GetMotor2State();   
     if((motor2driveState == MOTOR2_POSITIVE)||(motor2driveState == MOTOR2_NEGATIVE))
     {
      if(GetRobotState() == ROBOT_ACCELERATION_SLOW_START)
      {
        if(motor2A <= 0)
        {
           Motor2runFault = 1; 
        }
      }
     }
     else 
     {
       Motor2runFault = 0; 
     }
    }
        else if(drivestate2 == 0)
    {
        if(motor2A > 0)
        {
           Motor2runFault = 1; 
        }
        else 
        {
          Motor2runFault = 0;   
        }
    }
    else 
    {
       Motor2runFault = 0; 
    }
}