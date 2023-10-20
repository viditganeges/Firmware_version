#include "RobotControl.h"
#include "Motor1Control.h"
#include "Motor2Control.h"
#include "BrushControl.h"
#include "RotateSenseCommon.h"
#include "EdgeSenseCommon.h"
#include "CycleMode.h"
#include "Rtc.h"
#include "LTC4015_Main.h"
#include "FaultProcess.h"
#include "Serial_Debug.h"
#include "RobotOperation.h"
//#include "apiprocessing.h"
#include "EventLogger.h"
#include "proximity.h"
#include "relay.h"
#include "Motor1Control_hal.h"
#include "DefaultValuesConfig.h"
#include "cleanerdetectcommon.h"
#include "ResponseAPIProcess.h"

#include <stdbool.h>
#include <string.h>

#define DEFAULT_PWM_CHANGE_VALUE        8U                              
#define MINIMUM_PWM_DUTY                8U
#define STOP_PWM_DUTY                   0
#define IDLE_PWM_DUTY                   STOP_PWM_DUTY
#define PWM_CHANGE_TIMER_TICS_MS        1U  
#define DEFAULT_PWM_CHANGE_TIME         1U                               /* This value should not be less than the timer interrupt caller tics */
#define MINIMUM_PWM_CHANGE_VALUE        1U                               /* This value should not be zero. */
//#define DEFAULT_TARGET_COUNT_THRESHOLD  500

#if (DEFAULT_PWM_CHANGE_VALUE == 0)
#error (" Default PWM Change Value Cannot be Zero ");
#endif
#if (MINIMUM_PWM_CHANGE_VALUE == 0)
#error (" Minimum PWM Change Value Cannot be Zero ");
#endif
#if (DEFAULT_PWM_CHANGE_TIME < PWM_CHANGE_TIMER_TICS_MS)
#error (" PWM Update Time should not be less than timer tics ");
#endif

typedef enum eDecelState_def
{
  DECEL_START,
  DECEL_IN_PROGRESS,
  DECEL_COMPLETED
}eDecelState;

typedef enum eAccelState_def
{
  ACCEL_START,
  ACCEL_IN_PROGRESS,
  ACCEL_COMPLETED
}eAccelState;

static const uint32_t minimumPwmDuty             = MINIMUM_PWM_DUTY;
static const uint32_t pwmChangeTimeCount_ms     = DEFAULT_PWM_CHANGE_TIME;
static float pwm1ChangeValue             = DEFAULT_PWM_CHANGE_VALUE;
static float pwm2ChangeValue             = DEFAULT_PWM_CHANGE_VALUE;
static float currentPwm1Duty             = IDLE_PWM_DUTY;                        /* Declared as a float to enable fraction pwm Increment/decrement though its converted to integer in pwm.c */
static float currentPwm2Duty             = IDLE_PWM_DUTY;                        /* Declared as a float to enable fraction pwm Increment/decrement though its converted to integer in pwm.c */
static uint32_t pwm_ChangeTimeCount      = 0;
static eRobotState robotState                  = ROBOT_IDLE;
static eDecelState decelState                  = DECEL_START;
static eAccelState accelState                  = ACCEL_START;
static eRobotDirection nextTargetDirection     = POSITIVE;

//static uint32_t targetCountThreshold           = DEFAULT_TARGET_COUNT_THRESHOLD;
//static eRobotDirection currentDirection        = STOP;
static stCountRange countRange;

static stRobotParam currentModeRobotParam;
static stRobotParam currentModeRobotSetParam;
static stLastOperationStatus curLastOpStatus, storedLastOpStatus;


static void CheckAndStopRobotOnEdge(void);
bool IsEdgeReached (eRobotDirection dir);
static void SetDecelState (eDecelState setDecelState);
static eDecelState GetDecelState (void);
static void SetAccelState (eAccelState setAccelState);
static eAccelState GetAccelState (void);
//static float CalculatePwmIncrementValue (void);
//static float CalculatePwmDecrementValue (void);
static eDecelState ApplyDecelerationPwm (void);
static eAccelState ApplyAccelerationPwm (void);
static void PwmChangeTimerOn (uint32_t setPwmChangeTimeCount_ms);
static inline void PwmChangeTimerStop (void);
static void IncrementPwm (void);
static void IncrementPwmCompleted (void);
static void DecrementPwm (void);
static void DecrementPwmCompleted (void);
//static void MovePositive (void);
//static void MoveNegative(void);
//static void StopMotor1(void);
static float CalculateDecelerationStartCount (void);
static bool IsDecelerationStartCount (float decelCount);
//static bool IsFinalTargetCountReached (void);
static bool IsRobotReachedTargetCount (float curCount, float tarCount);
static bool IsDirectionChange (eRobotDirection tarDirection, eRobotDirection curDirection);
static eRobotDirection GetNextTargetDirection(void);
static void SetNextTargetDirection(eRobotDirection direction);
static void SendCompletedStatus (float count);
static void StoreLastOperationData(void);

char Robot_Error_Flag = 0;
//extern uint8_t numberOfRows;
//extern uint32_t proximityCount;

extern int CYCLE_OCCURRED;
extern char SCHEDULE_LOG;

bool RobotInit (void)
{
  Motor1Init();  /* Added in New Code *********************************************************/
  //Motor2Init();  /* Added in New Code *********************************************************/
  //BrushInit();  /* Added in New Code *********************************************************/
  return true;
}

void SetCurrentModeRobotParameter (stRobotParam *robotParam)
{
  currentModeRobotSetParam = *robotParam;  
  SetNextTargetDirection(currentModeRobotSetParam.direction);
}

static void SetNextTargetDirection(eRobotDirection direction)
{
  nextTargetDirection = direction;  
}

static eRobotDirection GetNextTargetDirection(void)
{
  return (nextTargetDirection);  
}

//static void SetCurrentDirection(eRobotDirection direction)
//{
//  currentDirection = direction;  
//}
//
//static eRobotDirection GetCurrentDirection(void)
//{
//  return (currentDirection);  
//}

static void GetCurrentModeRobotParameter (void)
{
  currentModeRobotParam = currentModeRobotSetParam;  
  Serial_Debug("\n Current Mode Connection Params *** ");
  Serial_Debug("\n Target Count : ");
  Serial_Debug_Num(currentModeRobotParam.count); 
  Serial_Debug("\n Target Direction : ");  
  Serial_Debug_Num(currentModeRobotParam.direction); 
  Serial_Debug("\n Decel Diff Count : "); 
  Serial_Debug_Float(currentModeRobotParam.RobotPwmParam.decelStartCountDiff, 3); 
  Serial_Debug("\n Accel Time : "); 
  Serial_Debug_Num(currentModeRobotParam.RobotPwmParam.accelTime); 
  Serial_Debug("\n Decel Time : "); 
  Serial_Debug_Num(currentModeRobotParam.RobotPwmParam.decelTime); 
  Serial_Debug("\n Approach PWM1 : "); 
  Serial_Debug_Num(currentModeRobotParam.RobotPwmParam.approachPwm1);
  Serial_Debug("\n Steady PWM1 : ");
  Serial_Debug_Num(currentModeRobotParam.RobotPwmParam.steadyPwm1);
  Serial_Debug("\n Approach PWM2 : "); 
  Serial_Debug_Num(currentModeRobotParam.RobotPwmParam.approachPwm2);
  Serial_Debug("\n Steady PWM2 : ");
  Serial_Debug_Num(currentModeRobotParam.RobotPwmParam.steadyPwm2);
}

static void MoveRobotPositive (void)
{
  SetMotor1TargetState(MOTOR1_POSITIVE); 
  SetMotor2TargetState(MOTOR2_POSITIVE); 
  //  SetCurrentDirection(MOTOR_POSITIVE);
}

static void MoveRobotNegative(void)
{
  SetMotor1TargetState(MOTOR1_NEGATIVE); 
  SetMotor2TargetState(MOTOR2_NEGATIVE); 
  //  SetCurrentDirection(MOTOR_NEGATIVE);
}

static void StopRobot(void)
{
  SetMotor1TargetState(MOTOR1_STOP); 
  SetMotor2TargetState(MOTOR2_STOP); 
  // SetCurrentDirection(MOTOR_STOP);
}

void SetRobotState (eRobotState setRobotState)
{
  robotState = setRobotState;  
}

eRobotState GetRobotState (void)
{
  return(robotState);  
}

static bool IsDirectionChange (eRobotDirection tarDirection, eRobotDirection curDirection)
{
  if(curDirection == POSITIVE)
  {
    if(tarDirection == NEGATIVE)
    {
      return true;
    }
  }
  else if(curDirection == NEGATIVE)
  {
    if(tarDirection == POSITIVE)
    {
      return true;
    }
  }
  else if(curDirection == UNKNOWN) 
  {
    if(tarDirection == POSITIVE || tarDirection == NEGATIVE)
    {
      return true;
    }
  }
  return false;  
}



eRobotDirection GetRobotActualDirection (void)
{
  eRobotDirection robotCurDirection = UNKNOWN;
  eMotor1State mot1Dir = GetMotor1State();
  eMotor2State mot2Dir = GetMotor2State();
  switch (mot1Dir)
  {
  case MOTOR1_POSITIVE:
    if(mot2Dir == MOTOR2_NEGATIVE)
    {
      robotCurDirection = UNKNOWN;                              /* It should Not come to This */
    }
    else
    {
      robotCurDirection = POSITIVE;
    }
    break;
  case MOTOR1_NEGATIVE:
    if(mot2Dir == MOTOR2_POSITIVE)
    {
      robotCurDirection = UNKNOWN;                              /* It should Not come to This */
    }
    else
    {
      robotCurDirection = NEGATIVE;
    }
    break;
  case MOTOR1_STOP:
  case MOTOR1_IDLE:
    if(mot2Dir == MOTOR2_POSITIVE)
    {
      robotCurDirection = POSITIVE;                             
    }
    else if(mot2Dir == MOTOR2_NEGATIVE)
    {
      robotCurDirection = NEGATIVE;
    }
    else
    {
      robotCurDirection = STOP;
    }
    break;
  default:
    break;
  }
  return robotCurDirection;
}

stCountRange *GetSetCountRange(void)
{
  return(&countRange);
}

stCountRange *GetCountRange(void)
{
  return(&countRange);
}

static void StoreLastOperationData(void)
{
  storedLastOpStatus = curLastOpStatus;
  storedLastOpStatus.endSOC = GetSOCAsPercentage();
  storedLastOpStatus.endTime = GetRTCDateTimeSeconds();
  storedLastOpStatus.errorCode = GetMotorFaultCode();  
}

stLastOperationStatus * GetLastOperationData (void)
{
  return  &storedLastOpStatus;  
}

void RobotFSM (void)
{
  static float decelStartCount = 0;
  eRobotState presentRobotState = GetRobotState();
  static bool dirChangeFlag = false;
  static bool normalStopFlag = false;
  switch (presentRobotState)
  {
  case ROBOT_IDLE:
    currentPwm1Duty = IDLE_PWM_DUTY;  
    currentPwm2Duty = IDLE_PWM_DUTY;
    StopRotateSenseCount();
    //   prevCount = GetRotateSenseCount();
    SetMotor1AllPwm(currentPwm1Duty);
    SetMotor2AllPwm(currentPwm2Duty);
    break;
  case ROBOT_START:
    //    SetLogEvent (EV_LOG_MOT_STATE, ROBOT_START);                                 /* No break for this */
    SetLogEvent(EV_LOG_MOT_CMD,EV_MOT_START);
    SetLogEvent(EV_LOG_OP_MODE,GetOperationMode());
//    if((GetManualState() == MAN_TRACKCHANGER) && (proximityCount > numberOfRows))
//    {
      StartProximityCount();                   //changed
//    }
#if (HARDWARE_VERSION == HVER_1_0)
    if((GetOperationMode()==MANUAL)&&GetManualState()!=MAN_TRACKCHANGER)
    {
    SetRobotState(ROBOT_WAIT_FOR_RELAY_ON);
    DoRelayOn();
    }
    else
    {
    SetRobotState(ROBOT_RE_START);
    }
#else
    if((GetOperationMode()==MANUAL)&&GetManualState()!=MAN_TRACKCHANGER)
     {
       //MotorDriveOn();			  
     } 
    SetRobotState(ROBOT_RE_START);
#endif
  case ROBOT_WAIT_FOR_RELAY_ON :
    if(GetRelayStatus()==RELAY_ON) //use if needed, this is to turn on relay before motor operation
    {
      SetRobotState(ROBOT_RE_START);
    }
    break;
  case ROBOT_RE_START:         
    //   prevCount = GetRotateSenseCount();                                           
    SetAccelState(ACCEL_COMPLETED);
    SetDecelState(DECEL_COMPLETED);
    currentPwm1Duty = GetMotor1Pwm();
    currentPwm2Duty = GetMotor2Pwm();
    Serial_Debug("\n Robot Start : ");
    Serial_Debug_Float(currentPwm1Duty, 3);
    Serial_Debug(" , ");
    Serial_Debug_Float(currentPwm2Duty, 3);
    dirChangeFlag = false;
    normalStopFlag = false;
    if(IsDirectionChange(GetNextTargetDirection(), GetRobotActualDirection()) == true)
    {
      dirChangeFlag = true;
      SetRobotState(ROBOT_DIR_CHANGE_STOP);
      StartRotateSenseCount();
    }
    else
    {
      if(IsCycleModeContinution() == false)       
      {
        if(GetRotateSenseCount() != 0)
        {
          StoreLastOperationData();
        } 
        curLastOpStatus.startTime = GetRTCDateTimeSeconds();
        curLastOpStatus.startSOC = GetSOCAsPercentage();
        curLastOpStatus.totalRotateCount1 = 0;
        curLastOpStatus.totalRotateCount2 = 0;
      }
      StartRotateSenseCount();
      GetCurrentModeRobotParameter();
      decelStartCount = CalculateDecelerationStartCount();
      if(IsEdgeReached(currentModeRobotParam.direction))
      {
        Serial_Debug("\n Robot Edge Sensor Detected. Robot Not Started"); 
        SetRobotState(ROBOT_IMMEDIATE_STOP);
      }
      else
      {
        if(currentModeRobotParam.direction == POSITIVE)
        {
          MoveRobotPositive();
          StartBrush(BRUSH_NEGATIVE);
          SetRobotState(ROBOT_WAIT_FOR_VALID_STATE);
        }
        else if(currentModeRobotParam.direction == NEGATIVE)
        {
          MoveRobotNegative(); 
          StartBrush(BRUSH_POSITIVE);
          SetRobotState(ROBOT_WAIT_FOR_VALID_STATE);
        }
      }
    }
    break;
  case ROBOT_WAIT_FOR_VALID_STATE:                                                     /* Wait till motor State changes to positive/Negative . It may be because of dead time delay */
    if((GetMotor1TargetState() == GetMotor1State()) && (GetMotor2TargetState() == GetMotor2State()))
    {
      if((GetRobotActualDirection() == POSITIVE) || (GetRobotActualDirection() == NEGATIVE))
      {
        if((GetBrushState() == BRUSH_STEADYSTATE) || (GetBrushState() == BRUSH_DISABLEDSTATE))
        {
          SetRobotState(ROBOT_ACCELERATION_START);
          
         
        }
      }
    }
    break;
  case ROBOT_ACCELERATION_START:   
     if(SCHEDULE_LOG == 1)
     {
        SD_SOC_Start();
        
        SCHEDULE_LOG = 2;
     }
     if(SCHEDULE_LOG == 3)
     {
        SD_SOC_Start();
        
        SCHEDULE_LOG = 4;
     }
     
    if(IsDecelerationStartCount(decelStartCount) == true)
    {
      SetAccelState(ACCEL_COMPLETED);
      SetRobotState(ROBOT_DECELERATION_START);
    }
    if(currentPwm1Duty < minimumPwmDuty)
    {
      currentPwm1Duty = minimumPwmDuty;
      UpdateMotor1Pwm(currentPwm1Duty);
    }
    if(currentPwm2Duty < minimumPwmDuty)
    {
      currentPwm2Duty = minimumPwmDuty;
      UpdateMotor2Pwm(currentPwm2Duty);
    }
    SetAccelState(ACCEL_START);
    SetRobotState(ROBOT_ACCELERATION);
    Serial_Debug("\n Accel Start :");                                      /* No Break Necessary */
    //SetLogEvent(EV_LOG_MOT_CMD,EV_MOT_ACCEL_START);
    Serial_Debug_Float(currentPwm1Duty, 3);
    Serial_Debug(" ,");                                      /* No Break Necessary */
    Serial_Debug_Float(currentPwm2Duty, 3);
  case ROBOT_ACCELERATION:
    if(ApplyAccelerationPwm() == ACCEL_COMPLETED)
    {       
      if(Robot_Error_Flag == 1)
      {
        Robot_Error_Flag = 0;
      }
      Serial_Debug("\n Accel Completed: ");
      //SetLogEvent(EV_LOG_MOT_CMD,EV_MOT_ACCEL_COMPLETE);
      Serial_Debug_Float(currentPwm1Duty, 3);
      Serial_Debug(" ,");                                     
      Serial_Debug_Float(currentPwm2Duty, 3);
      SetRobotState(ROBOT_STEADYSTATE);
    }
    if(IsDecelerationStartCount(decelStartCount) == true)
    {
      SetAccelState(ACCEL_COMPLETED);
      SetRobotState(ROBOT_DECELERATION_START);
    }
    break;
  case ROBOT_STEADYSTATE:
    if(IsDecelerationStartCount(decelStartCount) == true)
    {
      SetRobotState(ROBOT_DECELERATION_START);
    }
    break;
  case ROBOT_NORMAL_STOP:
    normalStopFlag = true;
    dirChangeFlag = false;                                                      /* No break Necessary */
  case ROBOT_DIR_CHANGE_STOP:
    SetAccelState(ACCEL_COMPLETED);
    SetRobotState(ROBOT_DECELERATION_START);
    currentPwm1Duty = GetMotor1Pwm();
    currentPwm2Duty = GetMotor2Pwm();
    Serial_Debug("\n Motor Normal Stop");                                       /* No break Necessary */
  case ROBOT_DECELERATION_START:
    SetDecelState(DECEL_START);
    SetRobotState(ROBOT_DECELERATION);               
    Serial_Debug("\n Decel Started : ");   
    //SetLogEvent(EV_LOG_MOT_CMD,EV_MOT_DECEL_START);   
    Serial_Debug_Float(currentPwm1Duty, 3);                                      /* No Break Necessary */
    Serial_Debug(" , ");      
    Serial_Debug_Float(currentPwm2Duty, 3);
  case ROBOT_DECELERATION:
    if(ApplyDecelerationPwm() == DECEL_COMPLETED)
    {
      if((dirChangeFlag == true) || (normalStopFlag == true))
      {
        normalStopFlag = false;
        SetRobotState(ROBOT_END);
      }
      else
      {
        Serial_Debug("\n Approach Count Start :"); 
        Serial_Debug_Float(currentPwm1Duty, 3);
        Serial_Debug(" , ");      
        Serial_Debug_Float(currentPwm2Duty, 3);
        //SetLogEvent(EV_LOG_MOT_CMD,EV_MOT_DECEL_COMPLETE);
        SetRobotState(ROBOT_APPROACH_COUNT);
      }
    }
    break;
  case ROBOT_APPROACH_COUNT:
    if(IsFinalTargetCountReached() == true)
    {
      SetRobotState(ROBOT_END);
    }
    break;
  case ROBOT_IMMEDIATE_STOP:                                                  /* Clear Direction Flag (may be its set)- This prevents restart of motor.*/
    dirChangeFlag = false;                                                      /* No Break Necessary */
  case ROBOT_END:
    Serial_Debug("\n Robot End"); 
    currentPwm1Duty = STOP_PWM_DUTY;
    currentPwm2Duty = STOP_PWM_DUTY;
    SetMotor1AllPwm(currentPwm1Duty);
    SetMotor2AllPwm(currentPwm2Duty);
    StopRobot();
    
   if(SCHEDULE_LOG == 2)
   {
     SD_SOC_End();
    
     SCHEDULE_LOG = 3;
     CYCLE_OCCURRED = 1;
   }
   if(SCHEDULE_LOG == 4)
   {
    SD_SOC_End();
    SCHEDULE_LOG = 0;
    CYCLE_OCCURRED = 1;
   }
       
    StopBrush();
    SetAccelState(ACCEL_COMPLETED);
    SetDecelState(DECEL_COMPLETED);
    SetRobotState(ROBOT_WAIT_FOR_BRUSH_STOP);  
  case ROBOT_WAIT_FOR_BRUSH_STOP:
    if((GetBrushState() == BRUSH_COMPLETED) || (GetBrushState() == BRUSH_DISABLEDSTATE))
    {
      StopRotateSenseCount();
      curLastOpStatus.totalRotateCount1 += GetRotateSense1Count();
      curLastOpStatus.totalRotateCount2 += GetRotateSense2Count();
      StoreLastOperationData();
      //SetLogEvent(EV_LOG_PULSE_COUNT,curLastOpStatus.totalRotateCount1);
      //SetLogEvent(EV_LOG_PULSE_COUNT,curLastOpStatus.totalRotateCount2);
      SetLogEvent(EV_LOG_MOT_CMD,EV_MOT_STOP);
      if(dirChangeFlag == true)
      {
        dirChangeFlag = false;
        SetRobotState(ROBOT_RE_START);
        break;
      }
      else
      {
        uint32_t curCount = GetRotateSenseCount();
        SendCompletedStatus(curCount);
#if (HARDWARE_VERSION == HVER_1_0)
        if((GetOperationMode()==MANUAL)&&GetManualState()!=MAN_TRACKCHANGER)
        {
          SetRobotState(ROBOT_WAIT_FOR_RELAY_OFF); 
          DoRelayOff();
        }
        else
        {
          SetRobotState(ROBOT_COMPLETED); 
        }
#else
         if((GetOperationMode()==MANUAL)&&GetManualState()!=MAN_TRACKCHANGER)
         {
        //MotorDriveOff();				   
         }	 
        SetRobotState(ROBOT_COMPLETED);
#endif
        /* No break Necessary */
        //      SetLogEvent (EV_LOG_MOT_STATE, ROBOT_COMPLETED);
      }
    }
    break;
  case ROBOT_WAIT_FOR_RELAY_OFF :
    if(GetRelayStatus()==RELAY_OFF) // use if needed, this is to turn of relay after motor operation
    {
      SetRobotState(ROBOT_COMPLETED);
    }
    break;
//    
    case ROBOT_HOME_RETURN:                            
      
    if(IsCleanerPresent())
    {  
      unCleanerMAC *getCleanerMAC = GetSetCleanerMAC();
      
      SendCommandToCleaner(getCleanerMAC->cleanerMAC_64,STOP_CLEANER,"5,2");
      dirChangeFlag = true;
      Serial_Debug("\n Robot End"); 
      currentPwm1Duty = STOP_PWM_DUTY;
      currentPwm2Duty = STOP_PWM_DUTY;
      SetMotor1AllPwm(currentPwm1Duty);
      SetMotor2AllPwm(currentPwm2Duty);
      StopRobot();  
      StopBrush();
      SetAccelState(ACCEL_COMPLETED);
      SetDecelState(DECEL_COMPLETED);
      SetRobotState(ROBOT_WAIT_FOR_BRUSH_STOP); 
      if (GetRobotActualDirection() == POSITIVE || GetRobotActualDirection() == STOP || GetRobotActualDirection() == UNKNOWN)
      {
        SetMotionCommand(ROBOT_CMD_NEGATIVE);
        SetMotionPauseState(PA_IDLE);
      }  
    }
    break;
//      
  case ROBOT_COMPLETED:
    break;
  default:
    break;
  }
  Motor1FSM();
  Motor2FSM();
  BrushFSM();
  CheckAndStopRobotOnEdge();
  CheckAndStopRobotOnProximity();
}

bool IsEdgeReached (eRobotDirection dir)
{
  if(dir == POSITIVE)
  {
    return IsPositiveEdgeReached();
  }
  else if (dir == NEGATIVE)
  {
    return IsNegativeEdgeReached();  
  }
  else
  {
    return false;
  }
}

void CheckAndStopRobotOnEdge(void)
{
  eRobotState presentRobotState = GetRobotState();
  switch (presentRobotState)
  {
  case ROBOT_IDLE:
  case ROBOT_IMMEDIATE_STOP:
  case ROBOT_END:
  case ROBOT_WAIT_FOR_BRUSH_STOP:
  case ROBOT_COMPLETED:
    break;                                               /* Do Nothing For all the above states */
  default:
    if(IsEdgeReached(GetRobotActualDirection()))
    {
      Serial_Debug("\n Robot Edge Sensor Detected"); 
      SetRobotState(ROBOT_END);
    }                                                 /* Stop the motor if it reaches the end */
    break;
  } 
}

bool IsProximityDetected(void)
{
  if(GetProximityCount())
  {
    return true;
  }
  return false;
}



static void SendCompletedStatus (float count)
{
  //  char * txbuf;
  //  txbuf = FormatStatusUpdate(count);
  //  UartAllTransmit((uint8_t*)txbuf, strlen((const char *)txbuf));
}

static void SetDecelState (eDecelState setDecelState)
{
  decelState = setDecelState;  
}

static eDecelState GetDecelState (void)
{
  return(decelState);  
}

static float CalculatePwm1IncrementValue (void)
{
  float pwmUpdateStep = 0;
  float diffPwm = (float)(currentModeRobotParam.RobotPwmParam.steadyPwm1 - GetMotor1Pwm());
  if(diffPwm < 0)
  {
    pwmUpdateStep = 0;
  }
  else
  {
    if(currentModeRobotParam.RobotPwmParam.accelTime < PWM_CHANGE_TIMER_TICS_MS)                             /* 1ms is the minimum update time */
    {
      pwmUpdateStep = 0;
    }
    else 
    {
      if(pwmChangeTimeCount_ms > currentModeRobotParam.RobotPwmParam.accelTime)
      {
        pwmUpdateStep = currentModeRobotParam.RobotPwmParam.steadyPwm1;
      }
      else
      {
        pwmUpdateStep = ((float)(diffPwm * pwmChangeTimeCount_ms))/currentModeRobotParam.RobotPwmParam.accelTime;
      }
    }
  }
  return pwmUpdateStep;
}

static float CalculatePwm2IncrementValue (void)
{
  float pwmUpdateStep = 0;
  float diffPwm = (float)(currentModeRobotParam.RobotPwmParam.steadyPwm2 - GetMotor2Pwm());
  if(diffPwm < 0)
  {
    pwmUpdateStep = 0;
  }
  else
  {
    if(currentModeRobotParam.RobotPwmParam.accelTime < PWM_CHANGE_TIMER_TICS_MS)                             /* 1ms is the minimum update time */
    {
      pwmUpdateStep = 0;
    }
    else 
    {
      if(pwmChangeTimeCount_ms > currentModeRobotParam.RobotPwmParam.accelTime)
      {
        pwmUpdateStep = currentModeRobotParam.RobotPwmParam.steadyPwm2;
      }
      else
      {
        pwmUpdateStep = ((float)(diffPwm * pwmChangeTimeCount_ms))/currentModeRobotParam.RobotPwmParam.accelTime;
      }
    }
  }
  return pwmUpdateStep;
}

static float CalculatePwm1DecrementValue (void)
{
  float pwmUpdateStep = 0;
  float diffPwm = (float)(GetMotor1Pwm() - currentModeRobotParam.RobotPwmParam.approachPwm1);
  if(diffPwm < 0)
  {
    pwmUpdateStep = 0;
  }
  else
  {
    if(currentModeRobotParam.RobotPwmParam.decelTime < PWM_CHANGE_TIMER_TICS_MS)                             /* 1ms is the minimum update time */
    {
      pwmUpdateStep = 0;
    }
    else 
    {
      if(pwmChangeTimeCount_ms > currentModeRobotParam.RobotPwmParam.decelTime)
      {
        pwmUpdateStep = currentModeRobotParam.RobotPwmParam.approachPwm1;
      }
      else
      {
        pwmUpdateStep = ((float)(diffPwm * pwmChangeTimeCount_ms))/currentModeRobotParam.RobotPwmParam.decelTime;
      }
    }
  } 
  return pwmUpdateStep;
}

static float CalculatePwm2DecrementValue (void)
{
  float pwmUpdateStep = 0;
  float diffPwm = (float)(GetMotor2Pwm() - currentModeRobotParam.RobotPwmParam.approachPwm2);
  if(diffPwm < 0)
  {
    pwmUpdateStep = 0;
  }
  else
  {
    if(currentModeRobotParam.RobotPwmParam.decelTime < PWM_CHANGE_TIMER_TICS_MS)                             /* 1ms is the minimum update time */
    {
      pwmUpdateStep = 0;
    }
    else 
    {
      if(pwmChangeTimeCount_ms > currentModeRobotParam.RobotPwmParam.decelTime)
      {
        pwmUpdateStep = currentModeRobotParam.RobotPwmParam.approachPwm2;
      }
      else
      {
        pwmUpdateStep = ((float)(diffPwm * pwmChangeTimeCount_ms))/currentModeRobotParam.RobotPwmParam.decelTime;
      }
    }
  } 
  return pwmUpdateStep;
}

static eDecelState ApplyDecelerationPwm (void)
{
  eDecelState presentDecelState = GetDecelState();
  switch (presentDecelState)
  {
  case DECEL_START:
    pwm1ChangeValue = CalculatePwm1DecrementValue();
    pwm2ChangeValue = CalculatePwm2DecrementValue();
    PwmChangeTimerOn(pwmChangeTimeCount_ms);
    presentDecelState = DECEL_IN_PROGRESS;
    SetDecelState(presentDecelState);
    break;
  case DECEL_IN_PROGRESS:
    break;
  case DECEL_COMPLETED:
    break;
  }
  return presentDecelState;
}

static void SetAccelState (eAccelState setAccelState)
{
  accelState = setAccelState;  
}

static eAccelState GetAccelState (void)
{
  return(accelState);  
}

static float CalculateDecelerationStartCount (void)
{
  float tarCount = currentModeRobotParam.count;
  float curCount = GetRotateSenseCount();
  float decelStartCount;
  float diffCount = tarCount - curCount;
  if(diffCount <=  currentModeRobotParam.RobotPwmParam.decelStartCountDiff  )
  {       
    decelStartCount = GetRotateSenseCount();
  }
  else
  {
    decelStartCount = tarCount -  currentModeRobotParam.RobotPwmParam.decelStartCountDiff  ;
  }
  Serial_Debug("\n Decel Start Count :");
  Serial_Debug_Float(decelStartCount, 3);    
  return decelStartCount;
}

static bool IsDecelerationStartCount (float decelCount)
{
  return(IsRobotReachedTargetCount(GetRotateSenseCount(), decelCount));
}

bool IsFinalTargetCountReached (void)
{
  return(IsRobotReachedTargetCount(GetRotateSenseCount(), currentModeRobotParam.count + *(GetSetEncoderPulseTolerance())));
  //return(IsRobotReachedTargetCount(GetRotateSenseCount(), currentModeRobotParam.count));
}

static bool IsRobotReachedTargetCount (float curCount, float tarCount)
{
  if(curCount >= tarCount)
  {
    return true;
  }
  else
  {
    return false;
  } 
}

static eAccelState ApplyAccelerationPwm (void)
{
  eAccelState presentAccelState = GetAccelState();
  switch (presentAccelState)
  {
  case ACCEL_START:
    pwm1ChangeValue = CalculatePwm1IncrementValue();
    pwm2ChangeValue = CalculatePwm2IncrementValue();
    PwmChangeTimerOn(pwmChangeTimeCount_ms);
    presentAccelState = ACCEL_IN_PROGRESS;
    SetAccelState(presentAccelState);
    break;
  case ACCEL_IN_PROGRESS:
    break;
  case ACCEL_COMPLETED:
    break;
  }
  return presentAccelState;
}

static void PwmChangeTimerOn (uint32_t setPwmChangeTimeCount_ms)
{
  if(setPwmChangeTimeCount_ms == 0)
  {
    IncrementPwmCompleted();
    DecrementPwmCompleted();
  }
  else
  {
    pwm_ChangeTimeCount = setPwmChangeTimeCount_ms;
  }
}

inline void PwmChangeTimerStop (void)
{
  pwm_ChangeTimeCount = 0;
}

void PwmChangeTimeIncrement_ms (void)
{
  if(GetAccelState() == ACCEL_IN_PROGRESS)
  {
    if(--pwm_ChangeTimeCount <= 0)
    {
      IncrementPwm();
    }
  }
  else if(GetDecelState() == DECEL_IN_PROGRESS)
  {
    if(--pwm_ChangeTimeCount <= 0)
    {
      DecrementPwm();
    }
  }
}

static bool IncrementPwm1 (void)
{
  bool completeStatus = false;
  if(pwm1ChangeValue)
  {
    float newPwm1Duty = currentPwm1Duty + pwm1ChangeValue;
    if(newPwm1Duty >= currentModeRobotParam.RobotPwmParam.steadyPwm1)
    {
      currentPwm1Duty = currentModeRobotParam.RobotPwmParam.steadyPwm1;
      UpdateMotor1Pwm(currentPwm1Duty);
      completeStatus = true;
    }
    else
    {
      currentPwm1Duty = newPwm1Duty;
      UpdateMotor1Pwm(currentPwm1Duty);
      completeStatus = false;
    }
  }
  else
  {
    currentPwm1Duty = currentModeRobotParam.RobotPwmParam.steadyPwm1;
    UpdateMotor1Pwm(currentPwm1Duty);
    completeStatus = true;
  }  
  return completeStatus;
}

static bool IncrementPwm2 (void)
{
  bool completeStatus = false;
  if(pwm2ChangeValue)
  {
    float newPwm2Duty = currentPwm2Duty + pwm2ChangeValue;
    if(newPwm2Duty >= currentModeRobotParam.RobotPwmParam.steadyPwm2)
    {
      currentPwm2Duty = currentModeRobotParam.RobotPwmParam.steadyPwm2;
      UpdateMotor2Pwm(currentPwm2Duty);
      completeStatus = true;
    }
    else
    {
      currentPwm2Duty = newPwm2Duty;
      UpdateMotor2Pwm(currentPwm2Duty);
      completeStatus = false;
    }
  }
  else
  {
    currentPwm2Duty = currentModeRobotParam.RobotPwmParam.steadyPwm2;
    UpdateMotor2Pwm(currentPwm2Duty);
    completeStatus = true;
  }  
  return completeStatus;
}

static void IncrementPwm (void)
{
  bool pwmCompleteStatus = true;
  pwmCompleteStatus &= IncrementPwm1();
  pwmCompleteStatus &= IncrementPwm2(); 
  if(pwmCompleteStatus == true)
  {
    IncrementPwmCompleted();
  }
  else
  {
    PwmChangeTimerOn(pwmChangeTimeCount_ms);
  }
}

static void IncrementPwmCompleted (void)
{
  PwmChangeTimerStop();
  SetAccelState(ACCEL_COMPLETED);
}

static bool DecrementPwm1 (void)
{
  bool completeStatus = false;
  if(pwm1ChangeValue)
  {
    float newPwm1Duty = currentPwm1Duty - pwm1ChangeValue;
    if(newPwm1Duty <= currentModeRobotParam.RobotPwmParam.approachPwm1)
    {
      currentPwm1Duty = currentModeRobotParam.RobotPwmParam.approachPwm1;
      UpdateMotor1Pwm(currentPwm1Duty);
      completeStatus = true;
    }
    else
    {
      currentPwm1Duty = newPwm1Duty;
      UpdateMotor1Pwm(currentPwm1Duty);
      completeStatus = false;
    }
  }
  else
  {
    currentPwm1Duty = currentModeRobotParam.RobotPwmParam.approachPwm1;
    UpdateMotor1Pwm(currentPwm1Duty);
    completeStatus = true;
  }
  return completeStatus;
}

static bool DecrementPwm2 (void)
{
  bool completeStatus = false;
  if(pwm2ChangeValue)
  {
    float newPwm2Duty = currentPwm2Duty - pwm2ChangeValue;
    if(newPwm2Duty <= currentModeRobotParam.RobotPwmParam.approachPwm2)
    {
      currentPwm2Duty = currentModeRobotParam.RobotPwmParam.approachPwm2;
      UpdateMotor2Pwm(currentPwm2Duty);
      completeStatus = true;
    }
    else
    {
      currentPwm2Duty = newPwm2Duty;
      UpdateMotor2Pwm(currentPwm2Duty);
      completeStatus = false;
    }
  }
  else
  {
    currentPwm2Duty = currentModeRobotParam.RobotPwmParam.approachPwm2;
    UpdateMotor2Pwm(currentPwm2Duty);
    completeStatus = true;
  }
  return completeStatus;
}

static void DecrementPwm (void)
{
  bool pwmDecCompleteStatus = true;
  pwmDecCompleteStatus &= DecrementPwm1();
  pwmDecCompleteStatus &= DecrementPwm2();
  if(pwmDecCompleteStatus == true)
  {
    DecrementPwmCompleted();
  }
  else
  {
    PwmChangeTimerOn(pwmChangeTimeCount_ms);
  }
}

static void DecrementPwmCompleted (void)
{
  PwmChangeTimerStop();
  SetDecelState(DECEL_COMPLETED);
}


void RobotTimeIncrement_ms (void)
{
  PwmChangeTimeIncrement_ms ();
  Motor1TimeIncrement_ms();
  Motor2TimeIncrement_ms(); 
  BrushPwmChangeTimeIncrement_ms();
}
