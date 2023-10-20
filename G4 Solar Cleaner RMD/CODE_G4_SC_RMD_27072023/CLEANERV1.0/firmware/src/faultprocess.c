#include "main.h"

//#define BAT_SOC_PERC_HYS        2

static eErrorState errorState = ER_IDLE;
static uint32_t motorFaultData = NO_FAULT;
static stFaultTimeData faultTimeData;
static void SetMotorFault (uint32_t *faultData, eFaultFlags faultFlag);
static bool IsMotorFault (uint32_t *faultData, eFaultFlags faultFlag);
static bool CheckIfMotorToBeStopped (void);
static void ClearMotorFault (uint32_t *faultData, eFaultFlags faultFlag);
static void CheckADCFault (void);
static void ErrorCheckMotorDriverFault(void);
static void CheckMotorrunFault(void);
static void ErrorCheckMotorCurrentRunTime(void);
static void CheckBoardTempFault (void);

static void CheckClearMotorOverCurrentFaults (void);
static void CheckClearMotorDriverFaults (void);
//static void CheckClearBatteryFault(void);
//static void CheckLowBatteryFault (void);
//void CheckBatteryFault(void);
//static uint8_t setBatMinSOC;

char Error_Count(void)
{
  return Robot_Error_Count;
}

void ClearRoboterror(void)
{
  Robot_Error_Count = 0;
}

eErrorState GetErrorState (void)
{
  return errorState;
}

void SetErrorState (eErrorState errorSt)
{
  errorState = errorSt;
}

//uint8_t * GetSetMinBattSoc (void)
//{
//  return &setBatMinSOC;  
//}
 
void CheckError (void)
{
  CheckADCFault();
  
  ErrorCheckMotorDriverFault();  
  CheckMotorrunFault();
  ErrorCheckMotorCurrentRunTime();
  CheckBoardTempFault(); 
  
//  CheckLowBatteryFault();
//  CheckBatteryFault();
//  CheckMotorOverLoad();
// CheckMechanicalFault();
  
  errorState = GetErrorState();
  switch (errorState) 
  {
   case ER_IDLE:
      if(Robot_Error_Flag == 0)
        SetErrorState(ER_NO_ERROR);
      
    break;
   case ER_NO_ERROR:
    if(IsMotorFault(&motorFaultData, ALL_FAULT) == true && Robot_Error_Flag != 1)
    {
      (void)CheckIfMotorToBeStopped();
      SetErrorState(ER_CLEAR_ERROR); 
    }
    break;
   case ER_ERROR:
    //   SYSTICK_DelayUs(1);
    break;
   case ER_CLEAR_ERROR:
      SetErrorState(ER_IDLE);
   default:
    break;
  }
}   
//
//static void CheckLowBatteryFault (void)
//{
//  float BatterySoc = GetSOCAsPercentage();
//  uint8_t *minBatSoc = GetSetMinBattSoc();
//  if(BatterySoc < *minBatSoc)
//  {
//    if(IsMotorFault(&motorFaultData, MIN_BAT_VOLTAGE_FAULT) == false)
//    {
//      Serial_Debug("\n Low Voltage Detected.");
//      SetMotorFault(&motorFaultData, MIN_BAT_VOLTAGE_FAULT);
//    }
//  }
//  else if(BatterySoc > (*minBatSoc + BAT_SOC_PERC_HYS))
//  {
//    if(IsMotorFault(&motorFaultData, MIN_BAT_VOLTAGE_FAULT) == true)
//    {
//      Serial_Debug("\n Battery Voltage Normal."); 
//      ClearMotorFault(&motorFaultData, MIN_BAT_VOLTAGE_FAULT);
//    }
//  }
//}

//static void CheckBoardTempFault (void)
//{
//  float boardTemperatue = temp_read_A();
//  stBoardTemperature* boardSetTemperature = GetSetMaxBoardTemperature();
//  if(boardTemperatue > boardSetTemperature->maxTempSensorOne)
//  {
//    if(IsMotorFault(&motorFaultData, OVER_BTEMP_FAULT) == false)
//    {
//      Serial_Debug("\nCurrent Temperature-->");
//      Serial_Debug_Float(boardTemperatue,3);
//      Serial_Debug("\nSet Temperature-->");
//      Serial_Debug_Float(boardSetTemperature->maxTempSensorOne,3);
//      Serial_Debug("\n Over Board Temperature Detected.");
//      SetMotorFault(&motorFaultData, OVER_BTEMP_FAULT);
//    }
//  }
//}

//void CheckBatteryFault(void)
//{
//  uint8_t batError = GetBatteryError();
//  if(batError != 0)
//  {
//    if(IsMotorFault(&motorFaultData, BATTERY_FAULT) == false)
//    {
//      Serial_Debug("\n Battery Fault Detected.");
//      SetMotorFault(&motorFaultData, BATTERY_FAULT);
//    }    
//  }    
//}
//
static void CheckADCFault (void)
{
  if(IsMotor1OverCurrentFault() && (IsMotorFault(&motorFaultData, OVER_CURRENT_MOT1_FAULT) == false))
  {
    SetMotorFault(&motorFaultData, OVER_CURRENT_MOT1_FAULT);
  } 
  if(IsMotor2OverCurrentFault() && (IsMotorFault(&motorFaultData, OVER_CURRENT_MOT2_FAULT) == false))
  {
    SetMotorFault(&motorFaultData, OVER_CURRENT_MOT2_FAULT);
  } 
  if(IsMotor3OverCurrentFault() && (IsMotorFault(&motorFaultData, OVER_CURRENT_MOT3_FAULT) == false))
  {
    SetMotorFault(&motorFaultData, OVER_CURRENT_MOT3_FAULT);
  } 
}

static void ErrorCheckMotorDriverFault(void)
{
//    if(Motor1_Fault == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR1_DRIVER_FAULT);
//    }
//    if(Motor2_Fault == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR2_DRIVER_FAULT);
//    }
//    if(BrushMotor_Fault == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR3_DRIVER_FAULT);
//    }
}

static void CheckMotorrunFault(void)
{
//    if(Motor1runFault == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR1_FAULT);
//    }
//    if(Motor2runFault == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR2_FAULT);
//    }
//    if(BrushMotorrunFault == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR3_FAULT);
//    }
}

static void ErrorCheckMotorCurrentRunTime(void)
{
//    if(Motor1CurrrentTimerError == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR1_CURRENT_RUNTIME_EXCEEDED_FAULT);
//    }
//    if(Motor2CurrrentTimerError == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR2_CURRENT_RUNTIME_EXCEEDED_FAULT);
//    }
//    if(Motor3CurrrentTimerError == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR3_CURRENT_RUNTIME_EXCEEDED_FAULT);
//    }
}

static void CheckBoardTempFault(void)
{
//    if(Motor1tempFault == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR1_TEMP_FAULT);
//    }
//    if(Motor2tempFault == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR1_TEMP_FAULT);
//    }
//    if(Motor3tempFault == true)
//    {
//        SetMotorFault(&motorFaultData, MOTOR1_TEMP_FAULT);
//    }
    
}


bool IsOnlyNonMotorError (void)
{
  bool status;
  uint32_t volatile tempMotorFaultData = motorFaultData;
  ClearMotorFault(&motorFaultData, ZIGBEE_FAULT);
  if(IsMotorFault (&motorFaultData, ALL_FAULT) == true)
  {
    status =  false; 
  }
  else
  {
    status = true;
  }
  motorFaultData = tempMotorFaultData;
  return status;
}


static bool CheckIfMotorToBeStopped (void)                                   /* Motor Need not be stopped if Only Comm Error or Only RTC Error with Manual Mode */
{
  bool status;
  uint32_t volatile tempMotorFaultData = motorFaultData;
  ClearMotorFault(&motorFaultData, COMMUNICATION_FAULT);
  ClearMotorFault(&motorFaultData, RTC_FAULT);
  ClearMotorFault(&motorFaultData, ZIGBEE_FAULT);  
  stReturn *setReturnValue = GetSetReturnLimits(); 
  if(IsMotorFault (&motorFaultData, ALL_FAULT) == true)
  {
    motorFaultData = tempMotorFaultData;
    eRobotState curRobState = GetRobotState();
    
    if((curRobState != ROBOT_COMPLETED) && (curRobState != ROBOT_IDLE))
    {
      if((Robot_Error_Count++ <= setReturnValue->Return1) && Robot_Error_Flag == 0)
      {
        SetMotionPauseState(PA_PAUSE);
        Robot_Error_Flag = 1;
        Robot_Error_Count1 = Robot_Error_Count;
      }
      else
      {
        if(Error_Cleared == 0)
        {
          SetMotionPauseState(PA_PAUSE);
          SetErrorState(ER_IDLE);
          Error_Cleared = 1;
        }
        status = true; 
      }
    }
  }
  else
  {
    motorFaultData = tempMotorFaultData;
    status = false;
  }
  return status;
}

void CheckCommunicationError (void)
{
  char Track_Changer_Error;
  stReturn *setReturnValue = GetSetReturnLimits();
  stcomdistance* setcomdistance = GetSetComDistanceLimits();
  stCountRange* setCountValues = GetSetCountRange();
  
  Track_Changer_Error = setReturnValue->Return1;
  if((GetRobotActualDirection() == POSITIVE))
  {  
    if(GetRotateSense1Count() <= setcomdistance->CDistance)
    {
        cleaner_in_comm = 1;
        if(IsCommTimeOver() == true)
        {
          ClearCommTimeOver();
          SetMotorFault(&motorFaultData, COMMUNICATION_FAULT);
          if(Robot_Error_Count <= setReturnValue->Return1 && only_once == 0)
          {
            SetMotionPauseState(PA_PAUSE);
            Robot_Error_Count++;
            Robot_Error_Count1 = Robot_Error_Count;
            only_once = 1;
          } 
          else
          {
            if(Error_Cleared == 0)
            {
              SetMotionPauseState(PA_PAUSE);
              Error_Cleared = 1;
            }
          }      
          Serial_Debug("\r\n~*~*~*~*~*Communication Error~*~*~*~*~\r\n"); 
        }       
      }
      else
      {
        cleaner_in_comm = 0;
      }
    }
  
  else if((GetRobotActualDirection() == NEGATIVE))
  { 
    int CountCheck=0;  
    if(setCountValues->maxNegCountForPVsetup > 42)
    {
      CountCheck = ((setCountValues->maxNegCountForPVsetup)-(setcomdistance->CDistance)); 
    }
    else
    {
      CountCheck = 0;
    } 
    if(GetRotateSense1Count() >= CountCheck)
    {
        cleaner_in_comm = 1;
        if(IsCommTimeOver() == true)
        {
          ClearCommTimeOver();
          SetMotorFault(&motorFaultData, COMMUNICATION_FAULT);
          if(Robot_Error_Count <= setReturnValue->Return1 && only_once == 0)
          {
            SetMotionPauseState(PA_PAUSE);
            Robot_Error_Count = ++Track_Changer_Error;    
            Robot_Error_Count1 = Robot_Error_Count;
            only_once = 1;
          } 
          else
          {
            if(Error_Cleared == 0)
            {
              SetMotionPauseState(PA_PAUSE);
              Error_Cleared = 1;
            }
          }      
          Serial_Debug("\r\n~*~*~*~*~*Communication Error~*~*~*~*~\r\n"); 
        }       
     }
      else
      {
        cleaner_in_comm = 0;
      }
   }
  
  SetPrevPauseState(GetMotionPauseState());
}

static bool IsMotorFault (uint32_t *faultData, eFaultFlags faultFlag)
{
  if(faultFlag != ALL_FAULT)
  {
    if(*faultData & (1 << faultFlag))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    if(*faultData)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

static void SetMotorFault (uint32_t *faultData, eFaultFlags faultFlag)
{
  char SD_Error_Log[10];
  if(faultFlag == ALL_FAULT)
  {
    *faultData = ALL_FAULT;
  }
  else if(faultFlag == NO_FAULT)
  {
    *faultData = NO_FAULT;
  }
  else
  {
    if((faultFlag < MAX_FAULT) && (faultFlag > NO_FAULT))
    {
      faultTimeData.errorCodeTime[faultFlag] = GetRTCDateTimeSeconds();
    }
    *faultData |= (1 << faultFlag);
    
    
    
    eRobotDirection robotState = GetRobotActualDirection();
    if(robotState == 1 || robotState == 0)
    {
        memset(SDerrorlog,'\0',sizeof(SDerrorlog));
        snprintf(SD_Error_Log,10,"%lX", GetMotorFaultCode());
        strcat(SDerrorlog,SD_Error_Log);
        ERROR_OCCURRED = 1; 
        ERROR_OCCURRED_FLASH = 1;
    }
  }
}

void SetFault(eFaultFlags faultFlag)
{
  SetMotorFault (&motorFaultData, faultFlag);
}

void ClearFault(eFaultFlags faultFlag)
{
  ClearMotorFault (&motorFaultData, faultFlag);
}

static void ClearMotorFault (uint32_t *faultData, eFaultFlags faultFlag)
{
  if(faultFlag != ALL_FAULT)
  {
    *faultData &= (~(1 << faultFlag));
  }
  else
  {
    *faultData = NO_FAULT;
  }
}

static void CheckClearMotorOverCurrentFaults (void)
{
  if(IsMotorFault(&motorFaultData, OVER_CURRENT_MOT1_FAULT) == true)
  {
    ClearMotor1OverCurrentFault();
    ClearMotorFault(&motorFaultData, OVER_CURRENT_MOT1_FAULT);
  }
  if(IsMotorFault(&motorFaultData, OVER_CURRENT_MOT2_FAULT) == true)
  {
    ClearMotor2OverCurrentFault();
    ClearMotorFault(&motorFaultData, OVER_CURRENT_MOT2_FAULT);
  }
  if(IsMotorFault(&motorFaultData, OVER_CURRENT_MOT3_FAULT) == true)
  {
    ClearMotor3OverCurrentFault();
    ClearMotorFault(&motorFaultData, OVER_CURRENT_MOT3_FAULT);
  }
}

static void CheckClearMotorDriverFaults (void)
{
  if(IsMotorFault(&motorFaultData, MOTOR1_DRIVER_FAULT) == true)
  {
//    ClearMotor1OverCurrentFault();
    Motor1_Fault = 0;
    ClearMotorFault(&motorFaultData, MOTOR1_DRIVER_FAULT);
  }
  if(IsMotorFault(&motorFaultData, MOTOR2_DRIVER_FAULT) == true)
  {
//    ClearMotor2OverCurrentFault();
    Motor2_Fault = 0;
    ClearMotorFault(&motorFaultData, MOTOR2_DRIVER_FAULT);
  }
  if(IsMotorFault(&motorFaultData, MOTOR3_DRIVER_FAULT) == true)
  {
//    ClearMotor3OverCurrentFault();
    BrushMotor_Fault = 0;
    ClearMotorFault(&motorFaultData, MOTOR3_DRIVER_FAULT);
  }
}


//
//static void CheckClearBatteryFault(void)
//{
//  Serial_Debug("\nCheckClearBattery-->");
//  Serial_Debug_Num(IsMotorFault(&motorFaultData, BATTERY_FAULT));
//  if(IsMotorFault(&motorFaultData, BATTERY_FAULT) == true)
//  {
//    ClearBatteryError();
//    ClearMotorFault(&motorFaultData, BATTERY_FAULT);
//  }
//}

void ClearAllFaults (void)
{
 if(IsMotorFault(&motorFaultData, ALL_FAULT) == true)
 {
   ClearFaultsOnRobotCommand();
//  CheckClearBatteryFault();
  ClearMotorFault(&motorFaultData, ALL_FAULT);
  ClearRoboterror();
 }
 errorState = ER_NO_ERROR; 
}

void ClearFaultsOnRobotCommand (void)
{
 if(IsMotorFault(&motorFaultData, ALL_FAULT) == true)
 {
    CheckClearMotorOverCurrentFaults();
    CheckClearMotorDriverFaults();
    ClearMotorFault(&motorFaultData, MIN_BAT_VOLTAGE_FAULT); 
 }
 errorState = ER_NO_ERROR; 
}

uint32_t GetMotorFaultCode (void)
{
  return motorFaultData;
}

bool IsRTCFault (void)
{
  return(IsMotorFault (&motorFaultData, RTC_FAULT));
}

bool IsAnyFault(void)
{
return(IsMotorFault (&motorFaultData,ALL_FAULT));
}

