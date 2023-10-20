#include "main.h"

#ifdef SERIAL_DEBUG_CYCLEMODE
  #define Serial_Debug_CycleMode Serial_Debug
#else
  #define Serial_Debug_CycleMode (void)
#endif

static eCycleModeState cycleModeState     = CYCLE_IDLE;
static stRobotPwmParam cycleModeManualPwmParam;
static stRobotPwmParam cycleModeAutoPwmParam;
static void LoadCycleMotionParameters (eRobotDirection direction);

char Robo_Cycle;   
char sch = 1;

void SetCycleManualPwmParameters (stRobotPwmParam *motionPwmParam)
{
  cycleModeManualPwmParam =  *motionPwmParam;
}

stRobotPwmParam * GetSetCycleManualPwmParameters (void)
{
  return(&cycleModeManualPwmParam);
}

void SetCycleAutoPwmParameters (stRobotPwmParam *motionPwmParam)
{
  cycleModeAutoPwmParam =  *motionPwmParam;
}

stRobotPwmParam * GetSetCycleAutoPwmParameters (void)
{
  return(&cycleModeAutoPwmParam);
}

eCycleModeState GetCycleModeState (void)
{
  return (cycleModeState);  
}

void SetCycleModeState (eCycleModeState cycleModeSt)
{
  cycleModeState = cycleModeSt;
}

static void LoadCycleMotionParameters (eRobotDirection direction)
{
  stRobotParam robotParameter;
  if(GetOperationMode() == AUTO)
  {
    robotParameter.RobotPwmParam = cycleModeAutoPwmParam;
  }
  else
  {
    robotParameter.RobotPwmParam = cycleModeManualPwmParam;   
  }
  robotParameter.direction = direction;
  stCountRange * maxCountRange = GetCountRange();
  if(direction == POSITIVE)
  {
    robotParameter.count = maxCountRange->maxPosCountForPVsetup;
  }
  else
  {
    robotParameter.count = maxCountRange->maxNegCountForPVsetup;   
  }
  SetCurrentModeRobotParameter(&robotParameter);
}

void CycleModeFSM (void)
{
  eCycleModeState curCycleModeState = GetCycleModeState();
  switch (curCycleModeState)
  {
   case CYCLE_IDLE:
    break;
   case CYCLE_START: 
      prev_rotateCount = 0;
      Serial_Debug_CycleMode("\n Cycle Mode Started");
      SetCycleModeState(CYCLE_POS_START);                         /* No Break Necessary */
      
   case CYCLE_POS_START:
      Serial_Debug_CycleMode("\n Cycle Positive Started");
      SetRobotState(ROBOT_START);
      LoadCycleMotionParameters(POSITIVE);
      SetCycleModeState(CYCLE_WAIT_TILL_POS_END);
   break;
   case CYCLE_WAIT_TILL_POS_END:
       
     if(GetPrevPauseState() == PA_IDLE)
     {
       if(GetRobotState() != ROBOT_COMPLETED)
       {
         break;
       }
       else
       {
         SetCycleModeState(CYCLE_NEG_START);         
       }  

   case CYCLE_NEG_START:
     
      RestartCommTimer();   
      Serial_Debug_CycleMode("\n Cycle Negative Started");
      SetRobotState(ROBOT_START);
      LoadCycleMotionParameters(NEGATIVE);
      SetCycleModeState(CYCLE_WAIT_TILL_NEG_END);  
   break;
   
   case CYCLE_WAIT_TILL_NEG_END:
     
      if(GetRobotState() == ROBOT_COMPLETED)
      {
        SetCycleModeState(CYCLE_COMPLETED);
        SetRobotState(ROBOT_COMPLETED);
        Schedulestart = false; 
      }    
   break;
   case CYCLE_COMPLETED: 
     ClearRoboterror();
    break;
   default:
    break;
   }
 }
}

bool IsCycleModeContinution (void)
{
 return (GetCycleModeState() == CYCLE_WAIT_TILL_NEG_END);
}


