#include "main.h"

static volatile bool iloadTimeOver = false;
static volatile int32_t iloadmax_timecount = 0;
static volatile bool IloadMaxTimeOnFlag = false;
static volatile bool maxIloadFreqTimeOver = false;
static uint32_t ImotOverCurrentCount = 0;
static int32_t maxIloadFreq_timecount = 0;

static volatile bool mot1OverCurrentFault = false;

static void SetMot1OverCurrentFault (void);
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

bool IsMot1OverCurrentFault (void)
{
 return mot1OverCurrentFault;
}

void ClearMot1OverCurrentFault (void)
{
  mot1OverCurrentFault = false;
}

void SetMot1OverCurrentFault (void)
{
  mot1OverCurrentFault = true;
}

void ClearMot1MaxOverCurrentCount (void)
{
  ClearIloadMaxOverCount();
}

void CheckMot1OverLoad (void)
{
  float mot1Iload = current_read_A();
  motor1A = mot1Iload;
  stMaxIloadConfigValues maxIloadConfigValues;
  GetMaxIloadConfigValues(&maxIloadConfigValues);
  stMaxMotorCurrent motCurLimits;
  GetMaxMotorCurrentLimits(&motCurLimits);
  float mot1IloadLimit = motCurLimits.Imot1;
  if(GetMotionPauseState() == PA_IDLE)
  {
    if(mot1Iload >= mot1IloadLimit)
    {
      if(IsMot1OverCurrentFault() == false)
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
              Serial_Debug("\n*** Mot1 Over Current Detected. Iload: *****");
              Serial_Debug_Float(mot1Iload,2);
              SetMot1OverCurrentFault();
              ImotOverCurrentCount = 0;
              MaxIloadFreqTimerStop();
            }
            else
            {
              Serial_Debug("\n *** <Mot1 Over Current Detected - Paused.");
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

void IloadMot1MaxTimeIncrement_ms (void)
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

void Motor1FaultCheck(void)
{
    if(Motor1Fault())
    {
        Motor1_Fault = 1; 
    }
    else
    {
        Motor1_Fault = 0;
    }
}


void Motor1run(void)
{
    bool drivestate1 = Isdrive1Enabled();
    if(drivestate1 == 1)
    {
     eMotor1State motor1driveState = GetMotor1State();   
     if((motor1driveState == MOTOR1_POSITIVE)||(motor1driveState == MOTOR1_NEGATIVE))
     {
      if(GetRobotState() == ROBOT_ACCELERATION_SLOW_START)
      {
        if(motor1A <= 0)
        {
           Motor1runFault = 1; 
        }
      }
     }
     else 
     {
       Motor1runFault = 0; 
     }
    } 
    else if(drivestate1 == 0)
    {
        if(motor1A > 0)
        {
           Motor1runFault = 1; 
        }
        else 
        {
          Motor1runFault = 0;   
        }
    }
    else 
    {
       Motor1runFault = 0; 
    }     
}

void Motor1CurrentDurationCheck(void)
{
    if((motor1_lvl_12_flag==true)&&(Motor1Runtime_get()>=MOTOR1_LEVEL_12_TIME))
    {
        Motor1CurrrentTimerError = true;
    }
    else if((motor1_lvl_23_flag==true)&&(Motor1Runtime_get()>=MOTOR1_LEVEL_23_TIME))
    {
        Motor1CurrrentTimerError = true;
    }
    else if((motor1_lvl_34_flag==true)&&(Motor1Runtime_get()>=MOTOR1_LEVEL_34_TIME))
    {
        Motor1CurrrentTimerError = true;
    }
    else if(motor1_lvl_40_flag==true)
    {
        Motor1CurrrentTimerError = true;
    }
    else
    {
        Motor1CurrrentTimerError = false;
    }
}

void Motor2CurrentDurationCheck(void)
{
    if((motor2_lvl_12_flag==true)&&(Motor2Runtime_get()>=MOTOR2_LEVEL_12_TIME))
    {
        Motor2CurrrentTimerError = true;
    }
    else if((motor2_lvl_23_flag==true)&&(Motor2Runtime_get()>=MOTOR2_LEVEL_23_TIME))
    {
        Motor2CurrrentTimerError = true;
    }
    else if((motor2_lvl_34_flag==true)&&(Motor2Runtime_get()>=MOTOR2_LEVEL_34_TIME))
    {
        Motor2CurrrentTimerError = true;
    }
    else if(motor2_lvl_40_flag==true)
    {
        Motor2CurrrentTimerError = true;
    }
    else
    {
        Motor2CurrrentTimerError = false;
    }
}

void Motor3CurrentDurationCheck(void)
{
    if((motor3_lvl_12_flag==true)&&(Motor3Runtime_get()>=MOTOR3_LEVEL_12_TIME))
    {
        Motor3CurrrentTimerError = true;
    }
    else if((motor3_lvl_23_flag==true)&&(Motor3Runtime_get()>=MOTOR3_LEVEL_23_TIME))
    {
        Motor3CurrrentTimerError = true;
    }
    else if((motor3_lvl_34_flag==true)&&(Motor3Runtime_get()>=MOTOR3_LEVEL_34_TIME))
    {
        Motor3CurrrentTimerError = true;
    }
    else if(motor3_lvl_40_flag==true)
    {
        Motor3CurrrentTimerError = true;
    }
    else
    {
        Motor3CurrrentTimerError = false;
    }
}