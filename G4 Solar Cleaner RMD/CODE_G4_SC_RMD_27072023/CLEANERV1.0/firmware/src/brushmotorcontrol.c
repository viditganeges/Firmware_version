#include "main.h"

typedef enum eBrushMotorDTimeState_def
{
  DTIME_IDLE,
  DTIME_START,
  DTIME_IN_PROGRESS,
  DTIME_COMPLETE  
}eBrushMotorDTimeState;

static void OperateBrushMotor_IDLE (eBrushMotorState targetState);
static void OperateBrushMotor_STOP (eBrushMotorState targetState);
static void OperateBrushMotor_POSITIVE(eBrushMotorState targetState);
static void OperateBrushMotor_NEGATIVE(eBrushMotorState targetState);
static void SetDeadTimeState (eBrushMotorDTimeState DTimeState);
static eBrushMotorDTimeState GetDeadTimeState (void);
static void DeadTimeFSM (void);
static void BrushMotorTimerOn (uint32_t setBrushMotorTimeCount_ms);
static inline void BrushMotorTimerStop (void);
static inline bool IsBrushMotorTimeOver (void);
static inline void ClearBrushMotorTimeOver (void);

static eBrushMotorState brushMotorState          = BRUSHMOTOR_IDLE;
static eBrushMotorState brushMotorTargetState    = BRUSHMOTOR_IDLE;
static eBrushMotorDTimeState deadTimeState       = DTIME_IDLE;
static volatile bool brushMotorTimeOver          = false;
static int32_t brushMotor_timecount              = 0;
uint32_t brushMotorDeadTime_ms                   = DEFAULT_BRUSHMOTOR_DEAD_TIME_MS;


void BrushMotorInit (void)  //done//
{
  BrushMotorInit_hal ();  
}

void BrushMotorFSM (void)
{
  brushMotorTargetState = GetBrushMotorTargetState();
  eBrushMotorState brushMotorPresentState = GetBrushMotorState();
  
  switch (brushMotorPresentState)
  {
   case BRUSHMOTOR_IDLE:
    OperateBrushMotor_IDLE(brushMotorTargetState);
    break;
   case BRUSHMOTOR_STOP:
    OperateBrushMotor_STOP(brushMotorTargetState);
    break;                                                      
   case BRUSHMOTOR_POSITIVE:
    OperateBrushMotor_POSITIVE(brushMotorTargetState);
    break;
   case BRUSHMOTOR_NEGATIVE:
    OperateBrushMotor_NEGATIVE(brushMotorTargetState);
    break;
   default:    
    break;
  }
}

void SetBrushMotorState (eBrushMotorState brushMotorStateValue)
{
  brushMotorState = brushMotorStateValue;
}

eBrushMotorState GetBrushMotorState (void)
{
  return (brushMotorState);
}

void SetBrushMotorTargetState (eBrushMotorState brushMotorStateValue)
{
  brushMotorTargetState = brushMotorStateValue;
}

eBrushMotorState GetBrushMotorTargetState (void)
{
  return (brushMotorTargetState);
}

static void OperateBrushMotor_IDLE (eBrushMotorState targetState)
{
  switch(targetState)
  {
   case BRUSHMOTOR_IDLE:
    break;
   case BRUSHMOTOR_POSITIVE:
   case BRUSHMOTOR_NEGATIVE:
   case BRUSHMOTOR_STOP:
    BrushMotorStop();
    SetBrushMotorState(BRUSHMOTOR_STOP);
    SetDeadTimeState(DTIME_START);
    break;
   default:
    break;
  }
}

bool SetBrushMotorAllPwm(uint8_t duty)
{
  bool status = true;
  status &= SetBrushMotorForwardDutyCycle(duty);
  status &= SetBrushMotorReverseDutyCycle(duty);  
  return status;
}

bool UpdateBrushMotorPwm (uint8_t pwmDuty)
{
  bool status = true;
  if (GetBrushMotorState() == BRUSHMOTOR_POSITIVE)
  {
    status = UpdateBrushMotorPositivePwm(pwmDuty);
  }
  else if (GetBrushMotorState() == BRUSHMOTOR_NEGATIVE)
  {
    status = UpdateBrushMotorNegativePwm(pwmDuty);   
  }
  else
  {
    status = false;
  }
  return status;
}

uint8_t GetBrushMotorPwm (void)
{
  uint8_t pwmDuty;
  if (GetBrushMotorState() == BRUSHMOTOR_POSITIVE)
  {
    pwmDuty = GetBrushMotorPositivePwm();
  }
  else if (GetBrushMotorState() == BRUSHMOTOR_NEGATIVE)
  {
    pwmDuty = GetBrushMotorNegativePwm(); 
  }
  else
  {
    pwmDuty = 0;
  }
  return pwmDuty;
}

static void OperateBrushMotor_STOP (eBrushMotorState targetState)
{
  DeadTimeFSM();
  switch(targetState)
  {
   case BRUSHMOTOR_IDLE:
    BrushMotorStop();
    SetBrushMotorState(BRUSHMOTOR_IDLE);
    break;
   case BRUSHMOTOR_STOP:
    break;
   case BRUSHMOTOR_POSITIVE:
    if(GetDeadTimeState() == DTIME_COMPLETE)
    {
      BrushMotorMovePositive();
      SetBrushMotorState(BRUSHMOTOR_POSITIVE);
    }
    break;
   case BRUSHMOTOR_NEGATIVE:
    if(GetDeadTimeState() == DTIME_COMPLETE)
    {
      BrushMotorMoveNegative();
      SetBrushMotorState(BRUSHMOTOR_NEGATIVE);
    }
    break;    
   default:
    break;
  }
}

static void OperateBrushMotor_POSITIVE(eBrushMotorState targetState)
{
  switch(targetState)
  {
   case BRUSHMOTOR_IDLE:
    BrushMotorStop();
    SetBrushMotorState(BRUSHMOTOR_IDLE);
    SetDeadTimeState(DTIME_START);
    break;
   case BRUSHMOTOR_NEGATIVE:
   case BRUSHMOTOR_STOP:
    BrushMotorStop();
    SetBrushMotorState(BRUSHMOTOR_STOP);
    SetDeadTimeState(DTIME_START);
    break;
   case BRUSHMOTOR_POSITIVE:
    break;
   default:
    break;
  }
}

static void OperateBrushMotor_NEGATIVE(eBrushMotorState targetState)
{
  switch(targetState)
  {
   case BRUSHMOTOR_IDLE:
    BrushMotorStop();
    SetBrushMotorState(BRUSHMOTOR_IDLE);
    SetDeadTimeState(DTIME_START);
    break;
   case BRUSHMOTOR_POSITIVE:
   case BRUSHMOTOR_STOP:
    BrushMotorStop();
    SetBrushMotorState(BRUSHMOTOR_STOP);
    SetDeadTimeState(DTIME_START);
    break;
   case BRUSHMOTOR_NEGATIVE:
    break;    
   default:
    break;
  }
}

/* This Function is Only for Testing. It alters the set PWM value to Maximum */
void OperateBrushMotorForTest(eBrushMotorState targetState)
{
  switch(targetState)
  {
   case BRUSHMOTOR_IDLE:
    BrushMotorStop();
    break;
   case BRUSHMOTOR_POSITIVE:
    BrushMotorStop();
    SetBrushMotorForwardDutyCycle(99);
    SetBrushMotorReverseDutyCycle(99);
    BrushMotorMovePositive();
    break;
   case BRUSHMOTOR_STOP:
    BrushMotorStop();
    break;
   case BRUSHMOTOR_NEGATIVE:
    BrushMotorStop();
    SetBrushMotorReverseDutyCycle(99);
    SetBrushMotorForwardDutyCycle(99);
    BrushMotorMoveNegative();
    break;    
   default:
    break;
  } 
}

static void SetDeadTimeState (eBrushMotorDTimeState DTimeState)
{
  deadTimeState = DTimeState;
}

static eBrushMotorDTimeState GetDeadTimeState (void)
{
  return(deadTimeState);  
}

static void DeadTimeFSM (void)
{
  eBrushMotorDTimeState DTimeState = GetDeadTimeState();
  switch (DTimeState)
  {
   case DTIME_IDLE:
    /* No break. If called from idle State it will satrt the dead time routine */
   case DTIME_START:
    BrushMotorTimerOn(brushMotorDeadTime_ms);
    SetDeadTimeState(DTIME_IN_PROGRESS);
   case DTIME_IN_PROGRESS:                            /* No break necessary */
    if(IsBrushMotorTimeOver() == true)
    {
      BrushMotorTimerStop();
      ClearBrushMotorTimeOver();
      SetDeadTimeState(DTIME_COMPLETE);                         
    }
    break;
   case DTIME_COMPLETE:
    break;
   default:
    break;
  } 
}

static void BrushMotorTimerOn (uint32_t setBrushMotorTimeCount_ms)
{
  if(setBrushMotorTimeCount_ms == 0)
  {
    brushMotorTimeOver = true;
    BrushMotorTimerStop();
  }
  else
  {
    brushMotor_timecount = setBrushMotorTimeCount_ms;
    brushMotorTimeOver = false;
  }
}

static inline void BrushMotorTimerStop (void)
{
  brushMotor_timecount = 0;
}

static inline bool IsBrushMotorTimeOver (void)
{
  return (brushMotorTimeOver);
}

static inline void ClearBrushMotorTimeOver (void)
{
  brushMotorTimeOver = false; 
}

void BrushMotorTimeIncrement_ms (void)
{
  if(brushMotor_timecount)
  {
    if(--brushMotor_timecount <= 0)
    {
      brushMotorTimeOver = true;
      BrushMotorTimerStop();
    }
    else
    {
      brushMotorTimeOver = false;
    }
  }
}
