#include "RobotOperation.h"
#include "RotateSenseCommon.h"
#include "Serial_Debug.h"
#include "NumStrConversion.h"
#include "ApiProcessing.h"
//#include "CycleTest.h"
#include "CycleMode.h"
#include "AutoSchedule.h"
#include "MotCurrentFaultCommon.h"
#include "FaultProcess.h"
#include "proximity.h"
//#include "FaultProcess.h"
//#include "EventLogger.h"
#include "LPTimer.h"
#include "ResponseAPIProcess.h"
#include "cleanerdetectcommon.h"
#include "rfid.h"
#include "RfidUart_Config.h"
#include "Relay.h"
#include "CommCommon.h"
#include "RobotControl.h"
#include "DefaultValuesConfig.h"
#include <stdio.h>
#include <string.h>
#include "rfid.h"

#define DEFAULT_OPERATION_MODE         MANUAL

typedef struct stTrackOperationData_def
{
  stTrackData trackInfo;
  uint32_t reverseDistance[30+1];
  uint64_t cleanerMacAddress;
  stRowLengthData rowLengthInfo;
  uint32_t sumOfPulseCount;
}stTrackOperationData;

static eMode operationMode              = IDLE;
static eRobotCommand robotCommand       = ROBOT_CMD_IDLE;
static stModeCounts     modeCounts;
static stRobotPwmParam manualModePwmParam;
static stRobotPwmParam emergencyModePwmParam;
static stRobotPwmParam jogModePwmParam;
static uint32_t gotoCount = 0;
static volatile bool pauseDelayTimeOver = false;
static int32_t pauseDelay_timecount     = 0;
static eManualState curManualState      = MAN_IDLE;
static eMotionPauseState pauseState     = PA_IDLE;

stTrackData trackData;
unCleanerMAC cleanerMACinfo;
stRowLengthData rowLengthData;
uint32_t encoderTolerance;
stTrackOperationData tempSetTrackOpData;
stRfidData storeRfidData;

eTrackChangerError trackChangerError = TC_NO_ERROR;
eTcControlMode TcControlMode = ENCODER_MODE;

uint8_t trackCount_temp = 5;
eTrackChangerState trackChangerState = TC_STOP;
eCleanerState cleanerFSMState = C_STOP;
eTrackChangerReportState reportState = TC_REPORT_UNKNOWN;
eJogState jogFSMState = JOG_STOP;
uint8_t TCNextIndex = 0xFF;
static bool isJogDone = false;

int trackchangercount=0;
int tc_count = 0;
int rowsActuallyCleaned = 0;
char newrowflag = 0;
extern uint8_t lastReadRfidData[12];
uint8_t Day1index=0;
int rowdaycount = 0;


static void AssignCommand (eRobotCommand command);

static void PauseDelayTimeIncrement_ms (void);
static void LoadManualMotionParameters (eRobotDirection direction);
static void LoadEmergencyMotionParameters (eRobotDirection direction);
static void PauseDelayTimerOn (uint32_t setPauseDelayTimeCount_ms);
static inline void PauseDelayTimerStop (void);
static inline bool IsPauseDelayTimeOver (void);
static inline void ClearPauseDelayTimeOver (void);
static void OperateMotionModes(void);
static void OperateManualMode(eManualState opManualState);
void OperateMotionPauseFSM (void);

uint8_t GetNumberofValidData(stTrackData tempData,uint32_t* sumofCounts);
uint8_t GetNumberofValidData2(stTrackData tempData,uint32_t* sumofCounts);


eRobotCommand prevRobotCommand;
eCycleModeState prevCycleModeState;

//uint8_t numberOfRows = 0;

uint32_t prevRotateSense1Count = 0;
uint32_t prevRotateSense2Count = 0;
extern char Error_Cleared;
extern char Robot_Error_Count,Robot_Error_Count1;
char SCHEDULE_LOG;

extern char Daycount;
extern bool cleaner_logtime;

static volatile bool rfid_sense_TimeOver          = false;
static int32_t rfid_sense_timecount = 0;
char read_rfid = 1,read_rfid_Fail = 0;

static volatile bool TC_Reverse_TimeOver          = false;
static int32_t TC_Reverse_timecount = 0;
//char read_rfid = 1,read_rfid_Fail = 0;
//extern char trans;

//int mode;
//uint8_t MODE1;
 
eRobotDirection prev_robotState = UNKNOWN;
eTrackChangerState prevTrackChangerState;
char prevTrackChange_Flag;
eTrackChangerError prev_track_error;
eTrackChangerError prev_track_error2;
eTrackChangerState prev_track_state;

char cleaner_op1;
extern uint16_t row_detected;
extern uint16_t C_ROW;

extern uint16_t cleaner_in_comm;

eTcControlMode GetTcControlMode(void)
{
  return TcControlMode;
}

void SetTcControlMode(eTcControlMode mode)
{
  TcControlMode = mode;
}

static void SetTrackChangerState(eTrackChangerState state)
{
  trackChangerState = state;
}

eTrackChangerState GetTrackChangerState(void)
{
  return trackChangerState;
}

static void SetCleanerFSMState(eCleanerState state)
{
  cleanerFSMState = state;
}

static eCleanerState GetCleanerFSMState(void)
{
  return cleanerFSMState;
}

uint64_t getCurrentCleanerMac(void)
{
  return tempSetTrackOpData.cleanerMacAddress;
}

eTrackChangerError GetTrackChangerError(void)
{
  return trackChangerError;
}

static void SetTrackChangerError(eTrackChangerError errorCode)
{
  trackChangerError = errorCode;
  SetFault((eFaultFlags)GetTrackChangerError());
}


void SetTrackChangerError2(eTrackChangerError errorCode)
{
  SetTrackChangerError(errorCode);
}

static void SetJogFSMState(eJogState state)
{
  jogFSMState = state;
}

static eJogState GetJogFSMState(void)
{
  return jogFSMState;
}

static void LoadMotionParameters_TC(eRobotDirection direction,uint32_t pulseCount,bool jogMode)
{
  stRobotParam robotParameter;
  stRobotPwmParam* autoModePwmParam = GetSetCycleAutoPwmParameters();
  if(GetOperationMode()==AUTO)
  {
    if(!jogMode)
    {
      robotParameter.RobotPwmParam = *autoModePwmParam;
    }
    else
    {
      jogModePwmParam = *autoModePwmParam;
      jogModePwmParam.steadyPwm1 = jogModePwmParam.approachPwm1;
      jogModePwmParam.steadyPwm2 = jogModePwmParam.approachPwm1;
      robotParameter.RobotPwmParam = jogModePwmParam;
    }
  }
  else
  {
    if(!jogMode)
    {
      robotParameter.RobotPwmParam = manualModePwmParam;
    }
    else
    {
      jogModePwmParam = manualModePwmParam;
      jogModePwmParam.steadyPwm1 = jogModePwmParam.approachPwm1;
      jogModePwmParam.steadyPwm2 = jogModePwmParam.approachPwm1;
      robotParameter.RobotPwmParam = jogModePwmParam;
    }
  }
  robotParameter.direction = direction;
  //stCountRange * maxCountRange = pulseCount;
  if(direction == POSITIVE)
  {
    robotParameter.count = pulseCount;
  }
  else
  {
    robotParameter.count = pulseCount;   
  }
  SetCurrentModeRobotParameter(&robotParameter);
}

static void LoadManualMotionParameters (eRobotDirection direction)
{
  stRobotParam robotParameter;
  robotParameter.RobotPwmParam = manualModePwmParam;
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

static void LoadEmergencyMotionParameters (eRobotDirection direction)
{
  stRobotParam robotParameter;
  robotParameter.RobotPwmParam = emergencyModePwmParam;
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

void LoadRobotParameters(uint32_t count, stRobotPwmParam * modePwmParam) 
{
  stRobotParam robotParameter;
  robotParameter.RobotPwmParam = *modePwmParam;
  robotParameter.count = count;
  stCountRange * maxCountRange = GetCountRange();
  if(robotParameter.count > maxCountRange->maxPosCountForPVsetup)
  {
    robotParameter.count = maxCountRange->maxPosCountForPVsetup;
  }
  else if(robotParameter.count < maxCountRange->maxNegCountForPVsetup)
  {
    robotParameter.count = maxCountRange->maxNegCountForPVsetup;
  }
  SetCurrentModeRobotParameter(&robotParameter);
}

void SetManualPwmParameters (stRobotPwmParam *motionPwmParam)
{
  manualModePwmParam =  *motionPwmParam;
}

void SetEmergencyPwmParameters (stRobotPwmParam *motionPwmParam)
{
  emergencyModePwmParam =  *motionPwmParam;
}

stRobotPwmParam * GetSetManualPwmParameters (void)
{
  return(&manualModePwmParam);
}

stRobotPwmParam * GetSetEmergencyPwmParameters (void)
{
  return(&emergencyModePwmParam);
}

void SetTrackData(stTrackData *trackDataPtr)
{
  trackData = *trackDataPtr;
}

stTrackData *GetSetTrackData(void)
{
  return(&trackData);
}

void SetRfidData(stRfidData *rfidData)
{
  storeRfidData = *rfidData;
}

stRfidData *GetSetRfidData(void)
{
  return(&storeRfidData);
}

void SetCleanerMAC(unCleanerMAC *macData)
{
  cleanerMACinfo = *macData;
}

unCleanerMAC *GetSetCleanerMAC(void)
{
  return(&cleanerMACinfo);
}

void SetRowLengthData(stRowLengthData *rowData)
{
  rowLengthData = *rowData;
}

stRowLengthData *GetSetRowLengthData(void)
{
  return(&rowLengthData);
}

void SetEncoderPulseTolerance(uint32_t* tolerance)
{
  encoderTolerance = *tolerance;
}

uint32_t *GetSetEncoderPulseTolerance(void)
{
  return (&encoderTolerance);
}

stModeCounts * GetSetModeCounts (void)
{
  return(&modeCounts);
}

static void AssignCommand (eRobotCommand command)
{
  switch(command)
  {
  case ROBOT_CMD_IDLE:
    SetOperationMode(DEFAULT_OPERATION_MODE, MAN_IDLE);                                   /* This is the Initial Operation Mode. */
    break;
  case ROBOT_CMD_NONE:
    break;
  case ROBOT_CMD_NORMAL_STOP:
    SetOperationMode(MANUAL, MAN_NORMAL_STOP); 
    EepromSetMode(EEPROM_WRITE);
    break;
  case ROBOT_CMD_STOP:
    SetOperationMode(MANUAL, MAN_STOP);  
    EepromSetMode(EEPROM_WRITE);
    break;
  case ROBOT_CMD_MANUAL:
    if(GetOperationMode()==AUTO)
    {
      SetOperationMode(MANUAL, MAN_NORMAL_STOP);
    }
    else
    {
      SetOperationMode(MANUAL, MAN_IDLE);
    }
    break;
  case ROBOT_CMD_CYCLE:
    SetOperationMode(MANUAL, MAN_CYCLE); 
    EepromSetMode(EEPROM_WRITE);
    break;
  case ROBOT_CMD_AUTO:
    if(GetOperationMode() != AUTO)
    {
      SetOperationMode(AUTO, MAN_IDLE);
    }
    break;
  case ROBOT_CMD_POSITIVE:
    SetOperationMode(MANUAL, MAN_POSITIVE ); 
    EepromSetMode(EEPROM_WRITE);
    break;
  case ROBOT_CMD_NEGATIVE:
    SetOperationMode(MANUAL, MAN_NEGATIVE); 
    EepromSetMode(EEPROM_WRITE);
    break;
  case ROBOT_CMD_EMERGENCY_STOW:
    SetOperationMode(EMERGENCY_STOW, MAN_IDLE);
    break;
  case ROBOT_CMD_GOTO_COUNT:
    SetOperationMode(MANUAL, MAN_GOTO_COUNT);
    break;
  case ROBOT_CMD_CYCLE_TEST:
    SetOperationMode(CYCLE_TEST, MAN_IDLE);
    break;
  case ROBOT_CMD_TRACK_CHANGER:
    //if(GetOperationMode()!=TRACK_CHANGER)
    //    {
    //      SetOperationMode(TRACK_CHANGER, MAN_NORMAL_STOP);
    //    }
    //    else
    //    {  
    SetOperationMode(MANUAL, MAN_TRACKCHANGER);
    eTrackChangerState curTrackChangerState = GetTrackChangerState();    
    if((curTrackChangerState == TC_STOP))   
    {
      SetTrackChangerState(TC_START);
      Daycount = Daycount + 1;
      if (Daycount > 7 || Daycount == 1 || trackchangercount == 0)
      {
        Daycount = 1;
      tc_count = trackchangercount;
        trackchangercount = 0; 
//        ClearRow_DetectCount();
      }
      EepromSetAutoScheduleTime(EEPROM_WRITE);
    }
//    break;
//    case ROBOT_CMD_CYCLE_RESUME :
////    SetOperationMode(getMode, MAN_RESUME);
//    SetOperationMode(MANUAL, MAN_RESUME);
//    //}
  default:
    break;
  }
  SetMotionCommand(ROBOT_CMD_NONE);
}

void RobotOperate (void)
{
  eRobotCommand command = GetMotionCommand();
  AssignCommand(command);
  if((command != ROBOT_CMD_NONE) && (command != ROBOT_CMD_IDLE))
  {
    //SetLogEvent (EV_LOG_MOT_CMD, (uint8_t)command);
    ClearFaultsOnRobotCommand();
  }
  //CheckCommunicationError();
  CheckError();
  OperateMotionPauseFSM();
  OperateMotionModes();
}

static void OperateMotionModes(void)
{ 
  if(GetMotionPauseState() == PA_IDLE)
  {
    bool nonMotorErstatus = IsOnlyNonMotorError();
    if((GetErrorState() == ER_IDLE || GetErrorState() == ER_NO_ERROR) || nonMotorErstatus)
    {
      if(GetOperationMode() == AUTO)
      {
        if(IsSchedueledTime())
        {
//          eTrackChangerState curTrackChangerState = GetTrackChangerState();
          if((GetTrackChangerState() == TC_STOP))   
          {
            SetTrackChangerState(TC_START);
          }
        } 
        else 
        {
         // Daycount = 0;
        }
        TrackChangerOperate();
      }
      if(GetOperationMode() == MANUAL)
      {
        if(curManualState == MAN_CYCLE)
        {
          CycleModeFSM();
        }
        else if(curManualState == MAN_TRACKCHANGER)
        {
          TrackChangerOperate();
        }
        else
        {
          /* Do Nothing for All other manual Modes */
        }
      }
      else if(GetOperationMode() == CYCLE_TEST)
      {
        //    CycleTestFSM();
      }
      //      else if(GetOperationMode() == TRACK_CHANGER)
      //      {
      //        TrackChangerOperate();
      //      }
      else
      {
        /* Do Nothing in All other Modes */
      }
    }
    else
    {
      eTrackChangerState curTrackChangerState = GetTrackChangerState();
      if(curTrackChangerState == TC_START)
      {
        SetTrackChangerState(TC_STOP);
      }
    }
  }
}

static void OperateManualMode(eManualState opManualState)
{
  char cc_temp;
   
  cc_temp = opManualState;
  
  switch(opManualState)
  {
  case MAN_IDLE:
    break;
  case MAN_NORMAL_STOP:
    SetRobotState(ROBOT_NORMAL_STOP);   
    break;
  case MAN_STOP:
    SetRobotState(ROBOT_IMMEDIATE_STOP);                                                 /* Need abrupt Stop of the Motor    */
    break;
  case MAN_POSITIVE:
    SetRobotState(ROBOT_START);
    LoadManualMotionParameters(POSITIVE);
    break;
  case MAN_NEGATIVE:
    SetRobotState(ROBOT_START);
    LoadManualMotionParameters(NEGATIVE);
    break;
  case MAN_CYCLE:
    {
      eCycleModeState curCycleModeState = GetCycleModeState();
      if((curCycleModeState == CYCLE_IDLE) || (curCycleModeState == CYCLE_COMPLETED))   
      {
        SetCycleModeState(CYCLE_START);
      }
    }
    break;
  case MAN_GOTO_COUNT:
    {
      uint32_t count = GetGotoCount();
      SetRobotState(ROBOT_START);
      LoadRobotParameters(count, &manualModePwmParam);
    }
//    case MAN_RESUME :
//    {
//      if((prevCycleModeState == CYCLE_WAIT_TILL_POS_END)||(prevCycleModeState == CYCLE_POS_START))
//      {
//  //      if(GetTargetCount())
//  //       {
//  //         SetRobotState(ROBOT_IMMEDIATE_STOP);
//  //         SetCycleModeState(CYCLE_NEG_START);
//  //         SetPrevPauseState(PA_IDLE);
//  //          break;
//  //       }
//  //      else {
//          SetCycleModeState(CYCLE_POS_START); 
//          CycleModeFSM();
//  //      }
//      }
//      else if((prevCycleModeState == CYCLE_WAIT_TILL_NEG_END)||(prevCycleModeState == CYCLE_NEG_START))
//      {
//  //       if(GetTargetCount())
//  //       {
//  //         SetRobotState(ROBOT_IMMEDIATE_STOP);  
//  //         SetPrevPauseState(PA_IDLE); 
//  //          break;
//  //       }
//  //       else
//  //       {
//            SetCycleModeState(CYCLE_NEG_START);
//            CycleModeFSM();
//  //       }    
//      }
//    }
//    break;
//    case MAN_TRACKCHANGER :
//        SetTrackChangerState(TC_START);
//        break;
   
  default:
    break;
  }
}

void CheckIfCleanerInPosition(void)
{
  if(!IsCleanerPresent())
  {
    SetTrackChangerError(TC_CLEANER_NOT_IN_POSITION);
    Serial_Debug("\r\n\r\n*****************Cleaner not in postion*******************\r\n\r\n");
//    SetTrackChangerState(TC_ERROR);  
     Robot_Error_Count1 = ++Robot_Error_Count;
    SetMotionPauseState(PA_PAUSE);
  }
}
void CheckIfTCInPosition(void)
{
    if(!IsValidProximitySensorState())
    {
      SetTrackChangerError(TC_NOT_IN_POSITION);
      if(cleaner_op1 == 1){
        SetCleanerFSMState(C_ERROR_STATE);
      }
      SetMotionPauseState(PA_PAUSE);
      Robot_Error_Count1 = ++Robot_Error_Count;
      Serial_Debug("\r\n/***************Track Changer Not in Position********/\r\n");
    }
}
uint8_t GetDetectedRfidIndex(uint32_t readRfid)
{
  uint8_t index = 0xFF;
  stRfidData *setRfidData = GetSetRfidData();
  for(int i=0;i<MAX_NO_OF_TRACKS;i++)
  {
    if((readRfid == setRfidData->rfidData[i])||(readRfid == setRfidData->rfidData2[i-10])||(readRfid == setRfidData->rfidData3[i-20]))
    {
      Serial_Debug("\r\n CARD -> ");
      Serial_Debug_Num(i+1);
      Serial_Debug(" DETECTED\r\n"); 
      index = i;
      break;
    }
    if((!setRfidData->rfidData[i])||(!setRfidData->rfidData2[i-10])||(!setRfidData->rfidData3[i-20]))
    {
      break;
    }
  }
  return index;
}

//uint8_t GetDetectedRfidIndex2(uint32_t readRfid)
//{
//  uint8_t index = 0xFF;
//  stRfidData *setRfidData = GetSetRfidData();
//  for(int i=0;i<MAX_NO_OF_TRACKS;i++)
//  {
//    if(readRfid == setRfidData->rfidData2[i])
//    {
//      Serial_Debug("\r\n CARD -> ");
//      Serial_Debug_Num(i+1);
//      Serial_Debug(" DETECTED\r\n"); 
//      index = i;
//      break;
//    }
//    if(!setRfidData->rfidData2[i])
//    {
//      break;
//    }
//  }
//  return index;
//}
//
//uint8_t GetDetectedRfidIndex3(uint32_t readRfid)
//{
//  uint8_t index = 0xFF;
//  stRfidData *setRfidData = GetSetRfidData();
//  for(int i=0;i<MAX_NO_OF_TRACKS;i++)
//  {
//    if(readRfid == setRfidData->rfidData3[i])
//    {
//      Serial_Debug("\r\n CARD -> ");
//      Serial_Debug_Num(i+1);
//      Serial_Debug(" DETECTED\r\n"); 
//      index = i;
//      break;
//    }
//    if(!setRfidData->rfidData3[i])
//    {
//      break;
//    }
//  }
//  return index;
//}

void TrackChangerOperate(void)
{
  eTrackChangerState currentTCState = GetTrackChangerState();
  strunrfid* getrunrfid = GetSetrunrfidLimits();
  strunrfid tempSetstrunrfid = *getrunrfid;
  
  static uint16_t rowsDone = 0;
  // static uint32_t waitTime = 0;
  
   static uint8_t numberOfRows = 0;
//  static uint8_t numberOfRows1 = 0; static uint8_t numberOfRows2 = 0;    //new
 // int numberOfRows=0,
  static uint8_t numberOfRows1=0;
  static uint8_t numberOfRows2=0;
  
  static uint8_t rownum = 0;
  
  static eRobotDirection currentDirection = UNKNOWN;
  eCleanerStatus cleanerStatus = C_UNKNOWN;
  static bool tcStart = 0;
  static uint16_t cleanerIndex = 0;
  static uint32_t prevStatetime = 0;
  static bool isTimeOver = 0;
  static uint32_t nextStateTimeout = 0;
  static  uint16_t row_to_clean;
 // static uint8_t rowsActuallyCleaned = 0;
  if((GetLPTimerMsTicks()-prevStatetime)>=nextStateTimeout)
  {
    isTimeOver = 1;
  }
  
  stnoofrows* getNorows = GetSetNoofrowsLimits();
  
  switch(currentTCState)
  {
  case TC_START : 
    cleaner_op1 = 0;
    SetNextIndex(0xFF);
    tcstarttime();
    isJogDone = false;
//    rowsActuallyCleaned = 0;
    rowsDone = 0;
    rowdaycount = rowsDone;
    row_to_clean = 0;
    //memcpy(tempSetTrackData,trackData,sizeof(trackData));
    tempSetTrackOpData.trackInfo = trackData;
    row_detected = trackchangercount;
    
   // numberOfRows = GetNumberofValidData(tempSetTrackOpData.trackInfo,&(tempSetTrackOpData.sumOfPulseCount));
    
    
    numberOfRows1 = GetNumberofValidData(tempSetTrackOpData.trackInfo,&(tempSetTrackOpData.sumOfPulseCount));
    numberOfRows2 = GetNumberofValidData2(tempSetTrackOpData.trackInfo,&(tempSetTrackOpData.sumOfPulseCount));
    
    numberOfRows = numberOfRows1 + numberOfRows2;

    tempSetTrackOpData.cleanerMacAddress = cleanerMACinfo.cleanerMAC_64;
    tempSetTrackOpData.rowLengthInfo = rowLengthData;
    SetTrackChangerError(TC_NO_ERROR);
    SetReportTrackChangerState(TC_REPORT_START);
    // ClearTrackChangerFaults();
    tcStart = 1;
    
//    for(int i = 0,j = numberOfRows-1; i<numberOfRows; i++,j--)
//    {     
//        tempSetTrackOpData.reverseDistance[j] = tempSetTrackOpData.trackInfo.trackDistance[i];
//    }
       
    
    for(int i = 0,j = numberOfRows-1; i<numberOfRows; i++,j--)
    {
      if(i<numberOfRows1)
      {
        tempSetTrackOpData.reverseDistance[j] = tempSetTrackOpData.trackInfo.trackDistance[i];
      }
      else if((i>=numberOfRows1)&&(i<=numberOfRows))
      {
        tempSetTrackOpData.reverseDistance[j] = tempSetTrackOpData.trackInfo.trackDistance2[i-numberOfRows1];
      }
    }
    
    
    stnoofrows* setNoofrows = GetSetNoofrowsLimits();              //Changes by RT
    stnoofrows getNoofrows = *setNoofrows;
   
    if(getrunrfid->runrfid != 0)
    {
      if(getrunrfid->runrfid <= tc_count)
      {
        SetTrackChangerError2(TC_INAPPROPRIATE_ROW_DATA);
        SetTrackChangerState(TC_STOP);
        break;
      }
    }
    
    if(numberOfRows > 0)
    {
      if(GetTrackChangerError()==TC_NO_ERROR)
      {
        SetTrackChangerState(TC_WAIT_FOR_RELAY_ON);        
        DoRelayOn();
        //StartProximityCount();
      }
      //break;
    }
    else
    {
      SetTrackChangerError(TC_INVALID_DATA);
      Serial_Debug("\r\n\r\n*****************Invalid Data*******************\r\n\r\n");
      SetTrackChangerState(TC_STOP);
    }
    
    if(IsEdgeReached(POSITIVE)&&IsEdgeReached(NEGATIVE))
    {
      currentDirection = UNKNOWN;
      SetTrackChangerError(TC_EDGE_UNKNOWN);
      Serial_Debug("\r\n\r\n*****************Edge Unknown*******************\r\n\r\n");
//      SetTrackChangerState(TC_ERROR);
        SetMotionPauseState(PA_PAUSE);
         Robot_Error_Count1 = ++Robot_Error_Count;
    }
    
    
#if (REVERSE_OPERATION_ENABLED)
    else if(IsEdgeReached(POSITIVE))
    {
      currentDirection = NEGATIVE;
    }
#endif
//    else if(IsEdgeReached(NEGATIVE))
//    {
//        currentDirection = POSITIVE;
//    }
//    else if((IsValidProximitySensorState()) && trackchangercount == 0)                //commented for home sensor stop
//    {
//      currentDirection = STOP;
//      SetTrackChangerState(TC_STOP);
//    }
     
    else if(trackchangercount >= 0)
    {
      currentDirection = POSITIVE;
    }
    else
    {
      if (GetRfidState()==RFID_ENABLE)
      {
        Serial_Debug("\r\n\r\n*****************Edge Unknown*******************\r\n\r\n");
        SetTrackChangerState(TC_JOG_AND_FIND_POSITION);
        SetJogFSMState(RESET_RFID_AND_WAIT);
      }
      else
      {
        currentDirection = UNKNOWN;
        SetTrackChangerError(TC_EDGE_UNKNOWN);
        Serial_Debug("\r\n\r\n*****************Edge Unknown*******************\r\n\r\n");
//        SetTrackChangerState(TC_ERROR);
        SetMotionPauseState(PA_PAUSE);
         Robot_Error_Count1 = ++Robot_Error_Count;
      }
      // break;
    }
    
    
    CheckIfCleanerInPosition();
    break;
  case TC_WAIT_FOR_RELAY_ON :
 #if (HARDWARE_VERSION == HVER_1_0)
    if(GetRelayStatus()==RELAY_ON)
    {
      SetTrackChangerState(TC_START_OPERATION);
    }
#else
    //MotorDriveOn();                                 //changed for cleaner card
    SetTrackChangerState(TC_START_OPERATION);
#endif    
    
    
    break;
    
  case TC_START_OPERATION :
    SCHEDULE_LOG = 1;
    SetRobotState(ROBOT_START); 
    SetTrackChangerState(TC_ASSIGN_VALUES);
    //break;
  case TC_ASSIGN_VALUES:
    read_rfid = 1;   
    ClearProximityCount();
    ClearLastReadRfidData();
    if(currentDirection == POSITIVE)
    { 
          
     //  LoadMotionParameters_TC(POSITIVE,tempSetTrackOpData.trackInfo.trackDistance[rowsDone],0);

      if(rowsDone<numberOfRows1)
      { 
       LoadMotionParameters_TC(POSITIVE,tempSetTrackOpData.trackInfo.trackDistance[rowsDone],0);
      }
      else if((rowsDone>=numberOfRows1)&&(rowsDone<numberOfRows))
      { 
       LoadMotionParameters_TC(POSITIVE,tempSetTrackOpData.trackInfo.trackDistance2[rowsDone-numberOfRows1],0);
      }
        
      ClearRotateSenseCount();
      SetNextIndex(rowsDone+1);
    }
    else if(currentDirection == NEGATIVE)
    {
//      LoadMotionParameters_TC(NEGATIVE,tempSetTrackOpData.reverseDistance[rowsDone],0);
      
      if(rowsDone<numberOfRows1)
      { 
       LoadMotionParameters_TC(NEGATIVE,tempSetTrackOpData.trackInfo.trackDistance[rowsDone],0);
      }
      else if((rowsDone>=numberOfRows1)&&(rowsDone<numberOfRows))
      { 
       LoadMotionParameters_TC(NEGATIVE,tempSetTrackOpData.trackInfo.trackDistance2[rowsDone-numberOfRows1],0);
      }
      
      ClearRotateSenseCount();
      SetNextIndex(rowsDone+1);
    }
#if (REVERSE_OPERATION_ENABLED)
    if(rowsDone >= (numberOfRows-1))
#else
      if(rowsDone >= (numberOfRows))
#endif
      {
        SetTrackChangerState(TC_WAIT_FOR_EDGE);
        prevStatetime = GetLPTimerMsTicks();
        nextStateTimeout = TC_WAIT_FOR_EDGE_TIME;  //seconds for next state
        isTimeOver = 0;
      }
      else
      {
        
               read_rfid = 1;
//        SetTrackChangerState(TC_WAIT_FOR_PROXIMITY);
        SetTrackChangerState(TC_NOT_IN_POS_ERROR);       
        prevStatetime = GetLPTimerMsTicks();
        if(GetTcControlMode() == ENCODER_MODE)
        {
        //  nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_ENC*tempSetTrackOpData.trackInfo.trackDistance[rowsDone];
          
          if(rowsDone<numberOfRows1)
          {
            nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_PROX*tempSetTrackOpData.trackInfo.trackDistance[rowsDone];        
          }
          else if((rowsDone>=numberOfRows1)&&(rowsDone<numberOfRows))
          {
            nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_PROX*tempSetTrackOpData.trackInfo.trackDistance2[rowsDone-numberOfRows1]; 
          } 
        }
        else
        {
        //  nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_PROX*tempSetTrackOpData.trackInfo.trackDistance[rowsDone];
          
          if(rowsDone<numberOfRows1)
          {
            nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_PROX*tempSetTrackOpData.trackInfo.trackDistance[rowsDone];        
          }
          else if((rowsDone>=numberOfRows1)&&(rowsDone<numberOfRows))
          {
            nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_PROX*tempSetTrackOpData.trackInfo.trackDistance2[rowsDone-numberOfRows1]; 
          }         
        }
        isTimeOver = 0;
      }

  case TC_NOT_IN_POS_ERROR:
    
      if(prev_track_error2 == TC_NOT_IN_POSITION)
      {
        if(TC_Reverse_TimeOver == true)
        {
          prev_track_error2 = TC_NO_ERROR;
          currentDirection = POSITIVE; //NEGATIVE
          SetTrackChangerState(TC_START_OPERATION);
        }
        break;
      }
      else
      {
        SetTrackChangerState(TC_WAIT_FOR_PROXIMITY); 
      }
//      break;
  case TC_WAIT_FOR_PROXIMITY :
    SetReportTrackChangerState(TC_REPORT_WAIT_FOR_PROXIMITY);
//    
    stRfidData* setRfidData = GetSetRfidData();
    stRfidData getRfidData = *setRfidData;
    GetLastReadRfidData(); 
    uint32_t RFID = atoi(lastReadRfidData);
    
    
    CheckIfCleanerInPosition();
    if(!isTimeOver)
    {
      
        if(IsProximityDetected())
        {
          uint32_t specificrow = getrunrfid->runrfid;
          uint16_t row_to_clean;
          
          if(specificrow == 0 && C_ROW == 0)
          {
             if(read_rfid == 1)
            {
               Rfid_Sense_TimerOn(20000);
               read_rfid = 0;
               rfid_sense_TimeOver = false;  
               read_rfid_Fail = 1;         
            }
            if((trackchangercount>=0 && trackchangercount <=9 && (getRfidData.rfidData[trackchangercount]==RFID))||(trackchangercount>=10 && trackchangercount < 20 && (getRfidData.rfidData2[trackchangercount-10]==RFID))||(trackchangercount>=20 && trackchangercount <=25 && (getRfidData.rfidData3[trackchangercount-20]==RFID)))
            {  
              read_rfid_Fail = 0;
              rfid_sense_TimerStop(); 
              EepromSetAutoScheduleTime(EEPROM_WRITE);
//              if(GetRobotState()==ROBOT_COMPLETED)
              if(GetRobotActualDirection() == STOP)
              {
           //     ClearProximityCount();
             //   if(abs(GetRotateSenseCount()-tempSetTrackOpData.trackInfo.trackDistance[rowsDone])<=*(GetSetEncoderPulseTolerance())) //modfied 
                
                if(rowsDone<numberOfRows1)
                {
                      if(abs(GetRotateSenseCount()-tempSetTrackOpData.trackInfo.trackDistance[rowsDone])<=*(GetSetEncoderPulseTolerance()))
                      {  
                        SetTrackChangerState(TC_CLEANER_OPERATION);
                        trackchangercount++; 
                        tc_count = trackchangercount;
                        ClearProximityCount();
                        if(currentDirection == POSITIVE)
                        {
                          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,rowsDone+1);        
                          cleanerIndex = rowsDone;
                        }
                        else if(currentDirection == NEGATIVE)
                        {
    //                      ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,numberOfRows - rowsDone);
    //                      cleanerIndex = ((numberOfRows-1) - rowsDone-1); //since last index is limit switch
                          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,rowsDone+1);        
                          cleanerIndex = rowsDone;
                          currentDirection = POSITIVE;
                        }           
                        SetCleanerFSMState(C_START);
                        isTimeOver = 0;
                        prevStatetime = GetLPTimerMsTicks();
                        nextStateTimeout = TC_TOTAL_CLEANER_OPERATION_TIME;
                      }      
                      else
                      {
                        SetTrackChangerState(TC_ERROR);
                        SetTrackChangerError(TC_INVALID_DISTANCE_TRAVELLED);
                        Serial_Debug("\r\n\r\n*****************Invalid Distance Travelled*******************\r\n\r\n");
                      }
                }
                
                else if((rowsDone>=numberOfRows1)&&(rowsDone<numberOfRows))
                {
                      if(abs(GetRotateSenseCount()-tempSetTrackOpData.trackInfo.trackDistance2[rowsDone])<=*(GetSetEncoderPulseTolerance()))
                      {
                        SetTrackChangerState(TC_CLEANER_OPERATION);
                        if(currentDirection == POSITIVE)
                        {
                          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,rowsDone+1);
                          cleanerIndex = rowsDone;
                        }
                        else if(currentDirection == NEGATIVE)
                        {
    //                      ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,numberOfRows - rowsDone);
    //                      cleanerIndex = ((numberOfRows-1) - rowsDone-1); //since last index is limit switch
                          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,rowsDone+1);        
                          cleanerIndex = rowsDone;
                          currentDirection = POSITIVE;
                        }           
                        SetCleanerFSMState(C_START);
                        isTimeOver = 0;
                        prevStatetime = GetLPTimerMsTicks();
                        nextStateTimeout = TC_TOTAL_CLEANER_OPERATION_TIME;
                      }      
                      else
                      {
                        SetTrackChangerState(TC_ERROR);
                        SetTrackChangerError(TC_INVALID_DISTANCE_TRAVELLED);
                        Serial_Debug("\r\n\r\n*****************Invalid Distance Travelled*******************\r\n\r\n");
                      }   
                }
                
              }
            }
            
            else if(rfid_sense_TimeOver == true && read_rfid_Fail == 1)
            {
    //          SetJogFSMState(RESET_RFID_AND_WAIT);               //changed
              if(IsValidProximitySensorState()) 
              {
                if(prev_track_error2 == TC_NOT_IN_POSITION)
                {
                  prev_track_error2 = TC_NO_ERROR;
                  currentDirection = POSITIVE; //NEGATIVE
                  SetTrackChangerState(TC_START_OPERATION);
                  EepromSetAutoScheduleTime(EEPROM_WRITE);
                  break;
                }
                else {
                 SetTrackChangerError(TC_INVALID_RFID); 
                 SetTrackChangerState(TC_ERROR);
                }
              }
              else
              {
                 SetTrackChangerError(TC_NOT_IN_POSITION); 
                 SetTrackChangerState(TC_ERROR);
                 read_rfid_Fail = 0;
              }
              read_rfid = 1; 
              ClearProximityCount();  
              SetMotionPauseState(PA_PAUSE);
              Robot_Error_Count1 = ++Robot_Error_Count;
            } 
          
          }
          else if(specificrow == 0 && C_ROW > 0)
          {  
            if(read_rfid == 1)
            {
               Rfid_Sense_TimerOn(10000);
               read_rfid = 0;
               rfid_sense_TimeOver = false;  
               read_rfid_Fail = 1;         
            }
            if((C_ROW>0 && C_ROW <=10 && (getRfidData.rfidData[C_ROW-1]==RFID))||(C_ROW>10 && C_ROW <=20 && (getRfidData.rfidData2[C_ROW-11]==RFID))||(C_ROW>20 && C_ROW <=25 && (getRfidData.rfidData3[C_ROW-21]==RFID)))
            { 
              read_rfid_Fail = 0;
              rfid_sense_TimerStop(); 
              
              if(GetRobotState()==ROBOT_COMPLETED)
              {
                if(Daycount == 1)
                {
                  row_to_clean = C_ROW;
                  rowsDone = row_to_clean - 1;
                  trackchangercount = C_ROW;
                  C_ROW = 0; 
                }
                else if(Daycount == 2)
                {
                  row_to_clean = C_ROW - getNorows->Row[0];
                  rowsDone = row_to_clean - 1;
                  trackchangercount = C_ROW;
                  C_ROW = 0; 
                }
                else if(Daycount == 3)
                {
                  row_to_clean = C_ROW - (getNorows->Row[1] +  getNorows->Row[0]);
                  rowsDone = row_to_clean - 1;
                  trackchangercount = C_ROW;
                  C_ROW = 0; 
                } 
                else if(Daycount == 4)
                {
                  row_to_clean = C_ROW - (getNorows->Row[2] + getNorows->Row[1] + getNorows->Row[0]);
                  rowsDone = row_to_clean - 1;
                  trackchangercount = C_ROW;
                  C_ROW = 0; 
                }
                else if(Daycount == 5)
                {
                  row_to_clean = C_ROW - (getNorows->Row[3] + getNorows->Row[2] + getNorows->Row[1] + getNorows->Row[0]);
                  rowsDone = row_to_clean - 1;
                  trackchangercount = C_ROW;
                  C_ROW = 0; 
                }
                else if(Daycount == 6)
                {
                  row_to_clean = C_ROW - (getNorows->Row[4] + getNorows->Row[3] + getNorows->Row[2] + getNorows->Row[1]+ getNorows->Row[0]);
                  rowsDone = row_to_clean - 1;
                  trackchangercount = C_ROW;
                  C_ROW = 0; 
                }
                else if(Daycount == 7)
                {
                  row_to_clean = C_ROW - (getNorows->Row[5] +getNorows->Row[4] + getNorows->Row[3] + getNorows->Row[2] + getNorows->Row[1] + getNorows->Row[0]);
                  rowsDone = row_to_clean - 1;
                  trackchangercount = C_ROW;
                  C_ROW = 0; 
                }
                tc_count = trackchangercount;
                EepromSetAutoScheduleTime(EEPROM_WRITE);
                if(rowsDone<numberOfRows1)
                {
                      if(abs(GetRotateSenseCount()-tempSetTrackOpData.trackInfo.trackDistance[rowsDone])<=*(GetSetEncoderPulseTolerance()))
                      {  
                        SetTrackChangerState(TC_CLEANER_OPERATION); 
                        ClearProximityCount();
                        if(currentDirection == POSITIVE)
                        {
                          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,row_to_clean-1);        
                          cleanerIndex = row_to_clean - 1;;
                        }
                        else if(currentDirection == NEGATIVE)
                        {
    //                      ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,numberOfRows - rowsDone);
    //                      cleanerIndex = ((numberOfRows-1) - rowsDone-1); //since last index is limit switch
                          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,row_to_clean-1);        
                          cleanerIndex = row_to_clean - 1;
                          currentDirection = POSITIVE;
                        }           
                        SetCleanerFSMState(C_START);
                        isTimeOver = 0;
                        prevStatetime = GetLPTimerMsTicks();
                        nextStateTimeout = TC_TOTAL_CLEANER_OPERATION_TIME;
                      }      
                      else
                      {
                        SetTrackChangerState(TC_ERROR);
                        SetTrackChangerError(TC_INVALID_DISTANCE_TRAVELLED);
                        Serial_Debug("\r\n\r\n*****************Invalid Distance Travelled*******************\r\n\r\n");
                      }
                }
                
                else if((rowsDone>=numberOfRows1)&&(rowsDone<numberOfRows))
                {
                      if(abs(GetRotateSenseCount()-tempSetTrackOpData.trackInfo.trackDistance2[rowsDone])<=*(GetSetEncoderPulseTolerance()))
                      {
                        SetTrackChangerState(TC_CLEANER_OPERATION);
                        if(currentDirection == POSITIVE)
                        {
                          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,row_to_clean-1);
                          cleanerIndex = row_to_clean - 1;;
                        }
                        else if(currentDirection == NEGATIVE)
                        {
    //                      ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,numberOfRows - rowsDone);
    //                      cleanerIndex = ((numberOfRows-1) - rowsDone-1); //since last index is limit switch
                          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,row_to_clean-1);        
                          cleanerIndex = row_to_clean - 1;;
                          currentDirection = POSITIVE;
                        }           
                        SetCleanerFSMState(C_START);
                        isTimeOver = 0;
                        prevStatetime = GetLPTimerMsTicks();
                        nextStateTimeout = TC_TOTAL_CLEANER_OPERATION_TIME;
                      }      
                      else
                      {
                        SetTrackChangerState(TC_ERROR);
                        SetTrackChangerError(TC_INVALID_DISTANCE_TRAVELLED);
                        Serial_Debug("\r\n\r\n*****************Invalid Distance Travelled*******************\r\n\r\n");
                      }   
                }
               
              }
            }
            else if(rfid_sense_TimeOver == true && read_rfid_Fail == 1)
            {
              if(row_detected < C_ROW)
              {
                SetTrackChangerState(TC_START_OPERATION); 
                read_rfid_Fail = 0;
                read_rfid = 1; 
              }
              else
              {
                 SetTrackChangerError(TC_INVALID_RFID); 
                 SetTrackChangerState(TC_ERROR);
                  read_rfid = 1; 
                  ClearProximityCount();  
                  SetMotionPauseState(PA_PAUSE);
                  Robot_Error_Count1 = ++Robot_Error_Count;
              }
            }
         }
        else if((GetOperationMode() == MANUAL)&&(specificrow > 0))
        {
            if(read_rfid == 1)
            {
               Rfid_Sense_TimerOn(20000);
               read_rfid = 0;
               rfid_sense_TimeOver = false;  
               read_rfid_Fail = 1;
            }  
            if((specificrow>=0 && specificrow <10 && (getRfidData.rfidData[specificrow-1]==RFID))||(specificrow>=10 && specificrow < 20 && (getRfidData.rfidData2[specificrow-11]==RFID))||(specificrow>=20 && specificrow <25 && (getRfidData.rfidData3[specificrow-21]==RFID)))
            { 
              read_rfid_Fail = 0;
              rfid_sense_TimerStop();
              if(GetRobotState()==ROBOT_COMPLETED)
//              if(GetRobotActualDirection() == STOP)
              {
                if(specificrow<=numberOfRows1)
                {  
                    if(abs(GetRotateSenseCount()-tempSetTrackOpData.trackInfo.trackDistance[specificrow-1])<=*(GetSetEncoderPulseTolerance()))
                    { 
                      trackchangercount = specificrow;
                      tc_count = trackchangercount;                     
                      SetTrackChangerState(TC_CLEANER_OPERATION); 
                      ClearProximityCount();
                      if(currentDirection == POSITIVE)
                      {
                        ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,specificrow);        
                        cleanerIndex = specificrow-1;
                      }
                      else if(currentDirection == NEGATIVE)
                      {
                        ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,specificrow);        
                        cleanerIndex = specificrow-1;
                        currentDirection = POSITIVE;
                      }           
                      SetCleanerFSMState(C_START);
                      isTimeOver = 0;
                      prevStatetime = GetLPTimerMsTicks();
                      nextStateTimeout = TC_TOTAL_CLEANER_OPERATION_TIME;
                      newrowflag = 1;
                    }      
                    else
                    {
                      SetTrackChangerState(TC_ERROR);
                      SetTrackChangerError(TC_INVALID_DISTANCE_TRAVELLED);
                      Serial_Debug("\r\n\r\n*****************Invalid Distance Travelled*******************\r\n\r\n");
                    }
                }
               else if((specificrow>=numberOfRows1)&&(specificrow<=numberOfRows))
                {
                      if(abs(GetRotateSenseCount()-tempSetTrackOpData.trackInfo.trackDistance2[specificrow-1])<=*(GetSetEncoderPulseTolerance()))
                      {
                        SetTrackChangerState(TC_CLEANER_OPERATION);
                        if(currentDirection == POSITIVE)
                        {
                          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,specificrow);
                          cleanerIndex = specificrow-1;
                        }
                        else if(currentDirection == NEGATIVE)
                        {
                          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,specificrow);        
                          cleanerIndex = specificrow-1;
                          currentDirection = POSITIVE;
                        }           
                        SetCleanerFSMState(C_START);
                        isTimeOver = 0;
                        prevStatetime = GetLPTimerMsTicks();
                        nextStateTimeout = TC_TOTAL_CLEANER_OPERATION_TIME;
                        newrowflag = 1;
                      }      
                      else
                      {
                        SetTrackChangerState(TC_ERROR);
                        SetTrackChangerError(TC_INVALID_DISTANCE_TRAVELLED);
                        Serial_Debug("\r\n\r\n*****************Invalid Distance Travelled*******************\r\n\r\n");
                      }   
                }
              }
            }
            else if(rfid_sense_TimeOver == true && read_rfid_Fail == 1)
            {
              if(row_detected < specificrow)
              {
                SetTrackChangerState(TC_START_OPERATION); 
                read_rfid_Fail = 0;
                read_rfid = 1; 
              }
              else
              {
                 SetTrackChangerError(TC_INVALID_RFID); 
                 SetTrackChangerState(TC_ERROR);
                  read_rfid = 1; 
                  ClearProximityCount();  
                  SetMotionPauseState(PA_PAUSE);
                  Robot_Error_Count1 = ++Robot_Error_Count;
              } 
            }
         }
          
        } 
          else if(IsEdgeReached(GetRobotActualDirection()))
          {
    //        SetTrackChangerState(TC_ERROR);
            SetTrackChangerError(TC_MISSED_ROWS);
            Serial_Debug("\r\n\r\n*****************Missed Rows*******************\r\n\r\n");
            SetMotionPauseState(PA_PAUSE);
             Robot_Error_Count1 = ++Robot_Error_Count;
    //          SetTrackChangerState(TC_REVERSE_AND_GOTO_START);
          }
          else if(IsFinalTargetCountReached())
          {
            SetTrackChangerState(TC_ERROR);
            SetTrackChangerError(TC_INVALID_DISTANCE_TRAVELLED);
            Serial_Debug("\r\n/***************Excess Distance Travelled********/\r\n");
          }      
    }
    else
    {
//      SetTrackChangerState(TC_ERROR);
      SetTrackChangerError(TC_TRACK_CHANGER_STALL);
      Serial_Debug("\r\n/***************Track Changer Stalled********/\r\n");
      SetMotionPauseState(PA_PAUSE);
       Robot_Error_Count1 = ++Robot_Error_Count;
      isTimeOver = 0;
    }
    break;
  case TC_CLEANER_OPERATION :
     prev_track_state = TC_CLEANER_OPERATION;
    SetReportTrackChangerState(TC_REPORT_WAIT_FOR_CLEANER);
    //    if(!isTimeOver)
    //    {   
    if(cleaner_in_comm == 1)
    {
      CheckIfTCInPosition();
    }
    cleanerStatus = OperateCleaner(cleanerIndex);
    if((cleanerStatus==C_DONE)&&(newrowflag == 0))
    {
      cleaner_logtime = true;
      if(currentDirection == POSITIVE)
      {
        ReportTrackChangerStatus(TC_REPORT_ROW_DONE,rowsDone+1);
      }
      else if(currentDirection == NEGATIVE)
      {
        ReportTrackChangerStatus(TC_REPORT_ROW_DONE,numberOfRows - rowsDone);
      }
      rowsActuallyCleaned++;
      rowsDone++;  
      rowdaycount = rowsDone;
      cleaner_op1 = 0;
        EepromSetAutoScheduleTime(EEPROM_WRITE);
      
      prev_track_state = TC_NIL;
      
      SetTrackChangerState(TC_CHECK_FOR_REMAINING_ROWS);
    }
    
    else if((newrowflag == 1)&&(cleanerStatus==C_DONE))
    {
      SetTrackChangerState(TC_REVERSE_AND_GOTO_START);
      newrowflag = 0;
      rowsActuallyCleaned = 0;
      tc_count = trackchangercount;
      trackchangercount = 0;
      Daycount = 0;   
      C_ROW = 0;
      EepromSetAutoScheduleTime(EEPROM_WRITE);
      
//      getrunrfid->runrfid = 0;
//      EepromSetCommDistance(EEPROM_WRITE);
    }
    

    else if (cleanerStatus==C_ERROR)
    {
//      SetTrackChangerState(TC_ERROR);
      SetMotionPauseState(PA_PAUSE);
      Robot_Error_Count1 = ++Robot_Error_Count;
    }
    //    }
    //    else
    //    {
    //      SetTrackChangerState(TC_ERROR);
    //      SetTrackChangerError(TC_TIMEOUT);
    //      isTimeOver = 0;
    //    }
    break;
  case TC_CHECK_FOR_REMAINING_ROWS : 
   
    
//    if(GetOperationMode () == AUTO)
//    {
      if(trackchangercount >= getNorows->TRow)    
  //    if((rowsDone >= numberOfRows)&&( GetProximityCount() >= getNoofrows.TRow))
      {
  #if (REVERSE_OPERATION_ENABLED)
        SetTrackChangerState(TC_STOP);
  #else
        SetTrackChangerState(TC_REVERSE_AND_GOTO_START);
        rowsActuallyCleaned = 0;
        tc_count = trackchangercount;
         trackchangercount = 0;
         Daycount = 0;   
         C_ROW = 0;
//         ClearRow_DetectCount();
          tcendtime();
         EepromSetAutoScheduleTime(EEPROM_WRITE);
  #endif
      }
      else
      {
        if((rowsDone) >= getNorows->Row[Daycount - 1])             //getNoofrows.Row[rownum]
        {
          SetTrackChangerState(TC_STOP);
          tcendtime();
        }
        else
        {
          if(prev_track_error == TC_NOT_IN_POSITION)
          {  
            char data[30];
            prev_track_error  = TC_NO_ERROR;
            prev_track_error2 = TC_NOT_IN_POSITION;
//            if(trackchangercount > 0 && (read_rfid_Fail == 1 || IsCleanerPresent()))
//            {
            currentDirection = NEGATIVE; //POSITIVE
            TC_Reverse_TimerOn(20000);
//            trackchangercount = trackchangercount -1;
//            EepromSetAutoScheduleTime(EEPROM_WRITE);
////            }
//            tc_count = trackchangercount;
          }
          SetTrackChangerState(TC_START_OPERATION);
        }
      }
    
    break;
  case TC_REVERSE_AND_GOTO_START :
    ClearRotateSenseCount();
    SetRobotState(ROBOT_START); 
    LoadMotionParameters_TC(NEGATIVE,tempSetTrackOpData.sumOfPulseCount,0);
    SetTrackChangerState(TC_WAIT_FOR_EDGE);
//    SetTrackChangerState(TC_START_OPERATION);
    prevStatetime = GetLPTimerMsTicks();
//    currentDirection = NEGATIVE ;
//    if(rowsDone <= numberOfRows)
//    {
//        SetTrackChangerState(TC_START_OPERATION);
//    }
//    else
//    {
//        SetTrackChangerState(TC_WAIT_FOR_EDGE);
//    }
    if(GetTcControlMode() == ENCODER_MODE)
    {
      nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_ENC*tempSetTrackOpData.sumOfPulseCount;  //seconds for next state
    }
    else
    {
      nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_PROX*tempSetTrackOpData.sumOfPulseCount;  //seconds for next state
    }
    isTimeOver = 0;
    break;
  case TC_WAIT_FOR_EDGE:
    SetReportTrackChangerState(TC_REPORT_WAIT_FOR_EDGE);
    if(!isTimeOver)
    {
      if(IsEdgeReached(GetRobotActualDirection()))
      {
        SetTrackChangerState(TC_STOP);
        if(currentDirection == POSITIVE)
        {
          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,rowsDone+1);
        }
        else if(currentDirection == NEGATIVE)
        {
          ReportTrackChangerStatus(TC_REPORT_INDEX_REACHED,numberOfRows - rowsDone);
        }
      }
      else if(IsFinalTargetCountReached())
      {
        SetTrackChangerState(TC_ERROR);
        SetTrackChangerError(TC_INVALID_DISTANCE_TRAVELLED);
        Serial_Debug("\r\n/***************Excess Distance Travelled********/\r\n");
      }
    }
    else
    {
//      SetTrackChangerState(TC_ERROR);
      SetTrackChangerError(TC_TRACK_CHANGER_STALL);
      Serial_Debug("\r\n/***************Track Changer Stalled********/\r\n");
      SetMotionPauseState(PA_PAUSE);
      Robot_Error_Count1 = ++Robot_Error_Count;
      isTimeOver = 0;
    }
    CheckIfCleanerInPosition();
    break;
  case TC_STOP : 
//    SetReportTrackChangerState(TC_REPORT_STOP);
    if(tcStart)
    {
      //MotorDriveOff();
      if(GetTrackChangerError()==TC_CLEANER_ERROR)  
      {
        ReportTrackChangerStatus(TC_REPORT_CLEANER_ERROR,IsErrorFromCleaner());
      }
      //      else
      //      {
      ReportTrackChangerStatus(TC_REPORT_STATUS,GetMotorFaultCode());
      //      }
      tcStart = 0;
    }
    //SetTrackChangerState(TC_CHECK_FOR_REMAINING_ROWS);
    break;
  case TC_ERROR :
    SetReportTrackChangerState(TC_REPORT_ERROR);
    Serial_Debug("\r\n\r\n*****************Track Changer FSM ERROR*******************\r\n\r\n");
    Serial_Debug("\r\n\r\n*************************ERROR ->");
    Serial_Debug_Num(GetTrackChangerError());
    Serial_Debug("**********************\r\n\r\n");
    SetTrackChangerState(TC_WAIT_FOR_ROBOT_STOP);
    SetRobotState(ROBOT_END);
    break;
  case TC_WAIT_FOR_ROBOT_STOP :
    if(GetRobotState()==ROBOT_COMPLETED)
    {
      DoRelayOff();      
      SetTrackChangerState(TC_WAIT_FOR_RELAY_OFF);
    }
    break;
  case TC_WAIT_FOR_RELAY_OFF:
#if (HARDWARE_VERSION == HVER_1_0)
    if(GetRelayStatus()==RELAY_OFF)
    {
      SetTrackChangerState(TC_STOP);
    }
#else
   // MotorDriveOff();                      //changed for cleaner card
    SetTrackChangerState(TC_STOP); 
#endif
    break;
  case TC_JOG_AND_FIND_POSITION :
    {
      eJogStatus jogStatus = JogAndFindPosition(&rowsDone);
      if((jogStatus == J_DONE)||(jogStatus == J_DONE_EDGE))
      {
        if(jogStatus == J_DONE)
        {
          currentDirection = POSITIVE;
          SetTrackChangerState(TC_CLEANER_OPERATION);
          cleanerIndex = rowsDone;
          SetNextIndex(rowsDone+1);
          SetCleanerFSMState(C_START);
        }
        else
        {
          currentDirection = POSITIVE;
          SetTrackChangerState(TC_CHECK_FOR_REMAINING_ROWS);
          SetNextIndex(rowsDone+1);
        }
      }
      else if(jogStatus== J_ERROR)
      {
//        SetTrackChangerState(TC_ERROR);
//        currentDirection = UNKNOWN;
        currentDirection = GetRobotActualDirection();
        SetTrackChangerError(TC_EDGE_UNKNOWN);  
        SetMotionPauseState(PA_PAUSE);
         Robot_Error_Count1 = ++Robot_Error_Count;
        Serial_Debug("\r\n\r\n*****************Position Unknown*******************\r\n\r\n");
      }
    }
    CheckIfCleanerInPosition();
    break;
    
  }
}

eJogStatus JogAndFindPosition(uint8_t* index)
{
  static eJogStatus currentStatus = J_STARTED; 
  static eJogState  previousState = JOG_START;
  static eJogState currentJogState = JOG_START;
  static bool ifFromEdge = false;
  uint32_t readData = 0;
  static uint32_t prevStatetime = 0;
  static bool isTimeOver = 0;
  static uint32_t nextStateTimeout = 0;
  if((GetLPTimerMsTicks()-prevStatetime)>=nextStateTimeout)
  {
    isTimeOver = 1;
  }
  currentJogState = GetJogFSMState();
  
  switch(currentJogState)
  {
  case JOG_START:
    currentStatus = J_STARTED; 
    ClearRotateSenseCount();
    previousState = JOG_START;
    isJogDone = true;
#if (HARDWARE_VERSION == HVER_1_0)
    DoRelayOn(); 
    SetJogFSMState(JOG_WAIT_FOR_RELAY_ON);
    break;
  case JOG_WAIT_FOR_RELAY_ON :
    if(GetRelayStatus()==RELAY_ON)
    {
      LoadMotionParameters_TC(POSITIVE,tempSetTrackOpData.sumOfPulseCount,1);
     
      SetRobotState(ROBOT_START); 
      SetJogFSMState(CHECK_FOR_PROX_OR_EDGE);
      prevStatetime = GetLPTimerMsTicks();
      if(GetTcControlMode() == ENCODER_MODE)
      {
        nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_ENC*tempSetTrackOpData.sumOfPulseCount;  //seconds for next state
      }
      else
      {
        nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_PROX*tempSetTrackOpData.sumOfPulseCount;  //seconds for next state
      }
      isTimeOver = 0;
    }
    break;
#else
    LoadMotionParameters_TC(POSITIVE,tempSetTrackOpData.sumOfPulseCount,1);
    SetJogFSMState(CHECK_FOR_PROX_OR_EDGE);
    SetRobotState(ROBOT_START); 
    //MotorDriveOn();
    prevStatetime = GetLPTimerMsTicks();
    if(GetTcControlMode() == ENCODER_MODE)								  
    {
      nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_ENC*tempSetTrackOpData.sumOfPulseCount;  //seconds for next state
    }
    else
    {
      nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_PROX*tempSetTrackOpData.sumOfPulseCount;  //seconds for next state				 
    }
    isTimeOver = 0;
    break;
#endif    
  case CHECK_FOR_PROX_OR_EDGE:
    if(!isTimeOver)
    {
      previousState = CHECK_FOR_PROX_OR_EDGE;
      currentStatus = J_IN_PROGRESS; 
      if(IsProximityDetected())
      {
        if(GetRobotState()==ROBOT_COMPLETED)
        {
          Serial_Debug("\r\nDetected Proximity\r\n");
//          ClearProximityCount();
          SetJogFSMState(RESET_RFID_AND_WAIT);
//          SetJogFSMState(JOG_STOP);
        }
      }
      if(IsEdgeReached(GetRobotActualDirection()))
      {
        if(GetRobotActualDirection()==POSITIVE)
        {
          Serial_Debug("\r\nDetected Edge\r\n");
          LoadMotionParameters_TC(NEGATIVE,tempSetTrackOpData.sumOfPulseCount,1);
          SetRobotState(ROBOT_START); 
//          SetJogFSMState(CHECK_FOR_PROX_OR_EDGE);
//          SetJogFSMState(JOG_STOP);
          ifFromEdge = true;
          prevStatetime = GetLPTimerMsTicks();
          if(GetTcControlMode() == ENCODER_MODE)
          {
            nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_ENC*tempSetTrackOpData.sumOfPulseCount;  //seconds for next state
          }
          else
          {
            nextStateTimeout = MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_PROX*tempSetTrackOpData.sumOfPulseCount;  //seconds for next state
          }
          isTimeOver = 0;
        }
        else
        {
          Serial_Debug("\r\n\r\nDetected NEGATIVE Edge\r\n\r\n");
          SetJogFSMState(JOG_ERROR);        
        }
      }
    }
    else
    {
      SetJogFSMState(JOG_ERROR);
      SetTrackChangerError(TC_TRACK_CHANGER_STALL);
      isTimeOver = 0;
    }
    break;
  case RESET_RFID_AND_WAIT:
    {
      previousState = RESET_RFID_AND_WAIT;
      currentStatus = J_IN_PROGRESS; 
      eRfidResetStatus resetStatus = ResetRfidFSM();
      if(resetStatus == R_RESET_DONE)
      {
        SetRfidResetState(R_RESET_START);  
        SetJogFSMState(CHECK_RFID_AND_ASSIGN_INDEX);
      }
    }
    break;
  case CHECK_RFID_AND_ASSIGN_INDEX:
    previousState = CHECK_RFID_AND_ASSIGN_INDEX;
    currentStatus = J_IN_PROGRESS; 
    
    readData = atoi(lastReadRfidData);
    //readData = GetLastReadRfidData();                  //changed
    if(readData)
    {
      ClearLastReadRfidData();
 //     uint8_t rfidIndex = GetDetectedRfidIndex(readData);
      
      uint8_t rfidIndex = GetDetectedRfidIndex(readData);
//      uint8_t rfidIndex2 = GetDetectedRfidIndex2(readData);
//      uint8_t rfidIndex3 = GetDetectedRfidIndex3(readData);
      
//      uint8_t rfidIndex = rfidIndex1+rfidIndex2+rfidIndex3;
     
 //     uint8_t numberOfRows = GetNumberofValidData(tempSetTrackOpData.trackInfo,&(tempSetTrackOpData.sumOfPulseCount));
      
      uint8_t numberOfRows1 = GetNumberofValidData(tempSetTrackOpData.trackInfo,&(tempSetTrackOpData.sumOfPulseCount));
      uint8_t numberOfRows2 = GetNumberofValidData2(tempSetTrackOpData.trackInfo,&(tempSetTrackOpData.sumOfPulseCount));
      
      uint8_t numberOfRows = numberOfRows1+numberOfRows2;
      
      if((rfidIndex!=0xFF)&&(rfidIndex<numberOfRows))
      {
        if(!ifFromEdge)
        {
          *index = rfidIndex;
          SetJogFSMState(JOG_STOP);
        }
        else
        {
          *index = rfidIndex+1;
          SetJogFSMState(JOG_STOP);
        }
      }
      else
      {
        Serial_Debug("\r\n\r\n*****************Unknown RFID*******************\r\n\r\n");
        SetJogFSMState(JOG_ERROR);
      }
    }
    else if(isJogDone == false)
    {
      SetJogFSMState(JOG_START); 
    }
    else
    {
      SetJogFSMState(JOG_ERROR);
    }
    break;
  case JOG_STOP:
    //previousState = JOG_STOP;
    if(previousState!=JOG_ERROR)
    {
      if(ifFromEdge)
      {
        currentStatus = J_DONE_EDGE;
      }
      else
      {
        currentStatus = J_DONE;
      }
    }
    else
    {
      currentStatus = J_ERROR; 
    }
//    SetRfidResetState(R_RESET_START);
    isJogDone = false;
    ifFromEdge = false;
    break;
  case JOG_ERROR:
    previousState = JOG_ERROR;
    //currentStatus = J_ERROR; 
    SetJogFSMState(JOG_STOP); 
    Serial_Debug("\r\n\r\n*****************Jog Error*******************\r\n\r\n");
    break;
  }
  return currentStatus;
}

eCleanerStatus OperateCleaner(uint16_t index)
{
  static eCleanerStatus operateCleanerStatus = C_STARTED;
  eCleanerState currentCleanerState = GetCleanerFSMState();
  char data[30];
  //static uint32_t current_time = 0;
  static uint32_t prevStatetime = 0;
  static bool isTimeOver = 0;
  static uint32_t nextStateTimeout = 0;
  static uint8_t commRetryCount = 0;
  static bool cStart = 0;
  static uint32_t commTimeout = 0;
  static uint32_t prevCommTime = 0;
  bool isCommTimeOver = 0;
  stHeartbeatConfig* hbtConfig = GetSetHeartbeatConfig();  
  commTimeout = hbtConfig->interval_ms;
  static uint32_t reconnectStartTime = 0;
  static uint32_t cleanerOpRemainingTime = 0;
  
//  SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,GET_CLEANER_MODE,"2");
//  
//   mode = 2;
//  stMessage message;
//  mode = message.values[0];
  
//  if(mode == 0)
//  {
//    SetCleanerFSMState(C_SEND_CYCLE_CMD);
//  }
//  else if(mode == 1)
//  {
//    SetCleanerFSMState(C_SEND_CYCLE_CMD);
//  }
//  else
//  {
//    SetCleanerFSMState(C_ERROR_STATE);
//  }

//  rowdaycount =  index+1;
  if((GetLPTimerMsTicks()-prevStatetime)>=nextStateTimeout)
  {
    isTimeOver = 1;
  }
  if((GetLPTimerMsTicks()-prevCommTime)>=commTimeout)
  {
    isCommTimeOver = 1;
  }
  switch(currentCleanerState)
  {
  case C_START: 
    operateCleanerStatus = C_STARTED;
    
    SetCleanerFSMState(C_CHECK_PROXOMITY_1); 
    prevStatetime = GetLPTimerMsTicks();
    nextStateTimeout = TC_CLEANER_DETECT_TIME_START;
    isTimeOver = 0;
    commRetryCount = 0;
    cStart = 1;
    ClearIsCleanerDone();
    ClearIsAckReceived();
    ClearErrorFromCleaner();
    break;
  case C_CHECK_PROXOMITY_1: 
    if(IsCleanerPresent())
    { 
      SetCleanerFSMState(C_SEND_CYCLE_CMD);
    }  
    else if(isTimeOver)
    {
      Serial_Debug("\r\n\r\n*****************Cleaner not in position*******************\r\n\r\n");
      SetTrackChangerError(TC_CLEANER_NOT_IN_POSITION);
      SetCleanerFSMState(C_STOP); 
      isTimeOver = 0;
      prevStatetime = GetLPTimerMsTicks();
    }
    break;
  case C_SEND_CYCLE_CMD: 

      if(Daycount == 1)
      {        
        snprintf(data,30,"%d,%d,%d,%d,2",tempSetTrackOpData.rowLengthInfo.rowLength[index][FORWARD_COUNT],tempSetTrackOpData.rowLengthInfo.rowLength[index][REVERSE_COUNT],trackchangercount,Daycount);
      }
      else if(Daycount == 2)
      {
        snprintf(data,30,"%d,%d,%d,%d,2",tempSetTrackOpData.rowLengthInfo.rowLength2[index][FORWARD_COUNT],tempSetTrackOpData.rowLengthInfo.rowLength2[index][REVERSE_COUNT],trackchangercount,Daycount);
      }
      else if(Daycount == 3)
      {
        snprintf(data,30,"%d,%d,%d,%d,2",tempSetTrackOpData.rowLengthInfo.rowLength3[index][FORWARD_COUNT],tempSetTrackOpData.rowLengthInfo.rowLength3[index][REVERSE_COUNT],trackchangercount,Daycount);
      }
      else if(Daycount == 4)
      {
        snprintf(data,30,"%d,%d,%d,%d,2",tempSetTrackOpData.rowLengthInfo.rowLength4[index][FORWARD_COUNT],tempSetTrackOpData.rowLengthInfo.rowLength4[index][REVERSE_COUNT],trackchangercount,Daycount);
      }
      else if(Daycount == 5)
      {
        snprintf(data,30,"%d,%d,%d,%d,2",tempSetTrackOpData.rowLengthInfo.rowLength5[index][FORWARD_COUNT],tempSetTrackOpData.rowLengthInfo.rowLength5[index][REVERSE_COUNT],trackchangercount,Daycount);
      }
      else if(Daycount == 6)
      {
        snprintf(data,30,"%d,%d,%d,%d,2",tempSetTrackOpData.rowLengthInfo.rowLength6[index][FORWARD_COUNT],tempSetTrackOpData.rowLengthInfo.rowLength6[index][REVERSE_COUNT],trackchangercount,Daycount);
      }
      else if(Daycount == 7)
      {
        snprintf(data,30,"%d,%d,%d,%d,2",tempSetTrackOpData.rowLengthInfo.rowLength7[index][FORWARD_COUNT],tempSetTrackOpData.rowLengthInfo.rowLength7[index][REVERSE_COUNT],trackchangercount,Daycount);
      } 
      
    if(isCommTimeOver)
    {
      if(SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,SET_AND_START_CYCLE,data)==SUCCESS)
      {  
        cleaner_op1 = 1;
        SetCleanerFSMState(C_WAIT_FOR_ACK);
        ClearIsAckReceived();
        nextStateTimeout = TC_WAIT_FOR_ACK_TIME;
        isCommTimeOver = 0;
        isTimeOver = 0;
        prevStatetime = GetLPTimerMsTicks();
      }
      else
      {
        if(commRetryCount>=3)
        {
          Serial_Debug("\r\n\r\n*****************Communication Error*******************\r\n\r\n");
          SetTrackChangerError(TC_COMMUNICATION_ERROR);
          //      SetCleanerFSMState(C_ERROR_STATE);
          SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,STOP_CLEANER,"7,2");
          operateCleanerStatus = C_ERROR;
        }
        commRetryCount++;
      }
      isCommTimeOver = 0;
      prevCommTime = GetLPTimerMsTicks();
    }
    break;
  case C_WAIT_FOR_ACK: 
    if(!isTimeOver)
    {
      if(IsAckReceived())
      {
        ClearIsAckReceived();
        ClearIsCleanerDone();  
        SetCleanerFSMState(C_DELAY_FOR_CLEANER_START); 
        operateCleanerStatus = C_IN_PROGRESS;
        prevStatetime = GetLPTimerMsTicks();
        nextStateTimeout = TC_WAIT_FOR_CLEANER_START;
        isTimeOver = 0;
      }
    }
    else 
    {
        prevStatetime = GetLPTimerMsTicks();
        isTimeOver = 0;
        Serial_Debug("\r\n/***************Cleaner Unresponsive********/\r\n");
        SetTrackChangerError(TC_COMMUNICATION_ERROR);
        //      SetCleanerFSMState(C_ERROR_STATE);
//        SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,STOP_CLEANER,"7,2");
        operateCleanerStatus = C_ERROR;
    }
    break;
  case C_DELAY_FOR_CLEANER_START:
    if(isTimeOver)
    {
      SetCleanerFSMState(C_GET_STATUS); 
      prevStatetime = GetLPTimerMsTicks();

        if (Daycount == 1)
        {
//          Day1index = index+1;
          nextStateTimeout = (uint32_t)(MAX_TIME_FOR_ONE_PULSE_CLEANER*((float)(tempSetTrackOpData.rowLengthInfo.rowLength[index][FORWARD_COUNT]+tempSetTrackOpData.rowLengthInfo.rowLength[index][REVERSE_COUNT])*2.5));
//          nextStateTimeout = (uint32_t)(MAX_TIME_FOR_ONE_PULSE_CLEANER*((float)(tempSetTrackOpData.rowLengthInfo.rowLength[Day1index][FORWARD_COUNT]+tempSetTrackOpData.rowLengthInfo.rowLength[Day1index][REVERSE_COUNT])*2.5));       
        }
        else if(Daycount == 2)
        {
          nextStateTimeout = (uint32_t)(MAX_TIME_FOR_ONE_PULSE_CLEANER*((float)(tempSetTrackOpData.rowLengthInfo.rowLength2[index][FORWARD_COUNT]+tempSetTrackOpData.rowLengthInfo.rowLength2[index][REVERSE_COUNT])*2.5));
        }
        else if(Daycount == 3)
        {
          nextStateTimeout = (uint32_t)(MAX_TIME_FOR_ONE_PULSE_CLEANER*((float)(tempSetTrackOpData.rowLengthInfo.rowLength3[index][FORWARD_COUNT]+tempSetTrackOpData.rowLengthInfo.rowLength3[index][REVERSE_COUNT])*2.5));
        }
        else if(Daycount == 4)
        {
          nextStateTimeout = (uint32_t)(MAX_TIME_FOR_ONE_PULSE_CLEANER*((float)(tempSetTrackOpData.rowLengthInfo.rowLength4[index][FORWARD_COUNT]+tempSetTrackOpData.rowLengthInfo.rowLength4[index][REVERSE_COUNT])*2.5));
        }
        else if(Daycount == 5)
        {
          nextStateTimeout = (uint32_t)(MAX_TIME_FOR_ONE_PULSE_CLEANER*((float)(tempSetTrackOpData.rowLengthInfo.rowLength5[index][FORWARD_COUNT]+tempSetTrackOpData.rowLengthInfo.rowLength5[index][REVERSE_COUNT])*2.5));
        }
        else if(Daycount == 6)
        {
          nextStateTimeout = (uint32_t)(MAX_TIME_FOR_ONE_PULSE_CLEANER*((float)(tempSetTrackOpData.rowLengthInfo.rowLength6[index][FORWARD_COUNT]+tempSetTrackOpData.rowLengthInfo.rowLength6[index][REVERSE_COUNT])*2.5));
        }
        else if(Daycount == 7)
        {
          nextStateTimeout = (uint32_t)(MAX_TIME_FOR_ONE_PULSE_CLEANER*((float)(tempSetTrackOpData.rowLengthInfo.rowLength7[index][FORWARD_COUNT]+tempSetTrackOpData.rowLengthInfo.rowLength7[index][REVERSE_COUNT])*2.5));
        }
//      }
      
      isTimeOver = 0;
    }
    break;
  case C_GET_STATUS: 
    if(!isTimeOver)
    {
      if(!IsCleanerDone())
      {
        if(isCommTimeOver)
        {
          if((SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,GET_CLEANER_STATUS,"2")==SUCCESS))
          {
            ClearIsCleanerDone();
          }
          else
          {
//            Serial_Debug("\r\n\r\n*****************Communication Error*******************\r\n\r\n");
//            SetTrackChangerError(TC_COMMUNICATION_ERROR);
//            SetCleanerFSMState(C_ERROR_STATE);
            reconnectStartTime = GetLPTimerMsTicks(); //////////////
            cleanerOpRemainingTime = nextStateTimeout - (GetLPTimerMsTicks()-prevStatetime);
            Serial_Debug("\r\n\r\n*****************Communication Error - Trying Reconnect*******************\r\n\r\n");
//            SetTrackChangerError(TC_COMMUNICATION_ERROR);
            SetCleanerFSMState(C_COMM_RECONNECT);
            ClearIsAckReceived();
            ClearIsCleanerDone(); 
            ClearErrorFromCleaner();
          }
          isCommTimeOver = 0;
          prevCommTime = GetLPTimerMsTicks();
        }
        if(IsErrorFromCleaner())
        {
          Serial_Debug("\r\n\r\n*****************Error from Cleaner*******************\r\n\r\n");
          SetTrackChangerError(TC_CLEANER_ERROR);
//          SetCleanerFSMState(C_ERROR_STATE);
          operateCleanerStatus = C_ERROR;
        }
      }
      else
      {
        ClearIsCleanerDone();
        SetCleanerFSMState(C_CHECK_PROXIMITY_2);
        isTimeOver = 0;
        prevStatetime = GetLPTimerMsTicks();
        nextStateTimeout = TC_CLEANER_DETECT_TIME_STOP; 
        if(IsErrorFromCleaner())
        {
          Serial_Debug("\r\n\r\n*****************Error from Cleaner*******************\r\n\r\n");
          SetTrackChangerError(TC_CLEANER_ERROR);
//          SetCleanerFSMState(C_ERROR_STATE);
          operateCleanerStatus = C_ERROR;
        }
      }
    }
    else
    {
      Serial_Debug("\r\n\r\n*****************Cleaner Stalled/Timeout*******************\r\n\r\n");
      isTimeOver = 0;
      prevStatetime = GetLPTimerMsTicks();
      SetTrackChangerError(TC_INVALID_DATA);
//      SetCleanerFSMState(C_ERROR_STATE);
      operateCleanerStatus = C_ERROR;
      SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,STOP_CLEANER,"7,2");
    }
    break;
  case C_CHECK_PROXIMITY_2: 
    if(!isTimeOver)
    {
      if(IsCleanerPresent())
      {
        SetCleanerFSMState(C_STOP);
      }
      else
      {
         SetCleanerFSMState(C_GET_STATUS);
      }
    }
    else
    {
      Serial_Debug("\r\n\r\n**************Cleaner Not in Position***************\r\n\r\n");
      SetTrackChangerError(TC_CLEANER_NOT_IN_POSITION);
      SetCleanerFSMState(C_STOP);
//      SetCleanerFSMState(C_GET_STATUS);
      isTimeOver = 0;
      prevStatetime = GetLPTimerMsTicks();
    }
    break;
  case C_STOP: 
    operateCleanerStatus = C_DONE;
    //SetCleanerFSMState(C_ERROR_STATE); 
    if(cStart)
    {
      SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,STOP_CLEANER,"5,2");
      cStart = 0;
    }
    break;
  case C_ERROR_STATE: 
    operateCleanerStatus = C_ERROR;
//    SetCleanerFSMState(C_STOP); 
    Serial_Debug("\r\n\r\n*****************Cleaner Error*******************\r\n\r\n");
    if(cStart)
    {
      SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,STOP_CLEANER,"7,2");
      cStart = 0;
    }
    break;
  case C_COMM_RECONNECT:
    if((GetLPTimerMsTicks()-reconnectStartTime)<= hbtConfig->maxReconnectTime)
    {
      if(isCommTimeOver)
      {
        ClearIsAckReceived();
        ClearIsCleanerDone(); 
        ClearErrorFromCleaner();
        Serial_Debug("\r\nRECONNECT --> GET STATUS");
        if((SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,GET_CLEANER_STATUS,"2")==SUCCESS))
        {        
        }
        else
        {
        }
        isCommTimeOver = 0;
        prevCommTime = GetLPTimerMsTicks();
      }
      if(IsErrorFromCleaner()&1<<COMMUNICATION_FAULT)
      {
        SetCleanerFSMState(C_SEND_RESUME); 
        commRetryCount = 0;
        isCommTimeOver = 0;
        prevCommTime = GetLPTimerMsTicks();
      }
      else if(IsAckReceived())
      {
        SetCleanerFSMState(C_GET_STATUS);         
      }
    }
     else
    {
        Serial_Debug("\r\n\r\n*****************Communication Error*******************\r\n\r\n");
        SetTrackChangerError(TC_COMMUNICATION_ERROR);
  //      SetCleanerFSMState(C_ERROR_STATE);
        operateCleanerStatus = C_ERROR;
    }
    break;
  case C_SEND_RESUME:    
    snprintf(data,30,"6,2"); //resume command,crc
    if(isCommTimeOver)
    {
//      if(SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,CYCLE_RESUME,data)==SUCCESS)
//      {
        SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,CYCLE_RESUME,data);
         cStart = 1;
        SetCleanerFSMState(C_WAIT_FOR_ACK);
//        SetCleanerFSMState(C_WAIT_FOR_RESUME_ACK);
        ClearIsAckReceived();            //
        SetIsAckReceived(); //
            ClearIsCleanerDone(); 
            ClearErrorFromCleaner();
        nextStateTimeout = TC_WAIT_FOR_ACK_TIME;
        isCommTimeOver = 0;
        isTimeOver = 0;
        prevStatetime = GetLPTimerMsTicks();
      prevCommTime = GetLPTimerMsTicks();
      }
//      else
//      {
//        if(commRetryCount>=3 )
//        {
//          Serial_Debug("\r\n\r\n*****************Communication Error*******************\r\n\r\n");
//          SetTrackChangerError(TC_COMMUNICATION_ERROR);
//  //      SetCleanerFSMState(C_ERROR_STATE);
//          SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,STOP_CLEANER,"7,2");
//          operateCleanerStatus = C_ERROR;
//        }
//        commRetryCount++;
//      }
//      isCommTimeOver = 0;
//      prevCommTime = GetLPTimerMsTicks();
//    }
    break;
  case C_WAIT_FOR_RESUME_ACK:
    if(!isTimeOver)
    {
      if(IsAckReceived())
      {
        ClearIsAckReceived();  
        SetCleanerFSMState(C_DELAY_FOR_CLEANER_RESUME); 
        operateCleanerStatus = C_IN_PROGRESS;
        prevStatetime = GetLPTimerMsTicks();
        nextStateTimeout = TC_WAIT_FOR_CLEANER_START;
        isTimeOver = 0;
      }
    }
    else 
    {
        prevStatetime = GetLPTimerMsTicks();
        isTimeOver = 0;
        Serial_Debug("\r\n/***************Cleaner Unresponsive********/\r\n");
        SetTrackChangerError(TC_COMMUNICATION_ERROR);
        //      SetCleanerFSMState(C_ERROR_STATE);
        SendCommandToCleaner(tempSetTrackOpData.cleanerMacAddress,STOP_CLEANER,"7,2");
        operateCleanerStatus = C_ERROR;
    }
    break;
  case C_DELAY_FOR_CLEANER_RESUME:
     if(isTimeOver)
    {
      SetCleanerFSMState(C_GET_STATUS); 
      prevStatetime = GetLPTimerMsTicks();
      prevCommTime = GetLPTimerMsTicks();
      nextStateTimeout = cleanerOpRemainingTime;
      isTimeOver = 0;
    }
    break;
  }
  return operateCleanerStatus;
}

uint8_t GetNumberofValidData(stTrackData tempData,uint32_t* sumofCounts)
{
  uint32_t tempSumOfCounts = 0;
  uint8_t validDataCount = 0;
  for(int i = 0; i<MAX_NO_OF_TRACKS ;i++)
  {
    if(tempData.trackValid[i] == TRACK_INFO_VALID)
    {
      validDataCount++;
      tempSumOfCounts+=tempData.trackDistance[i];
    }
    else
    {
      break;
    }
  }
  *sumofCounts = tempSumOfCounts;
  return validDataCount;
}

uint8_t GetNumberofValidData2(stTrackData tempData,uint32_t* sumofCounts)
{
  uint32_t tempSumOfCounts = 0;
  uint8_t validDataCount = 0;
  for(int i = 0; i<MAX_NO_OF_TRACKS ;i++)
  {
    if(tempData.trackValid2[i] == TRACK_INFO_VALID)
    {
      validDataCount++;
      tempSumOfCounts+=tempData.trackDistance2[i];
    }
    else
    {
      break;
    }
  }
  *sumofCounts += tempSumOfCounts;
  return validDataCount;
}

eMode GetOperationMode (void)
{
  return (operationMode);
}

void SetInitialOperationMode (eMode eepromReadMode)
{
  if(eepromReadMode == AUTO)
  {
    SetMotionCommand(ROBOT_CMD_AUTO);
  }
//  else if(eepromReadMode == MANUAL)
//  {
//    SetMotionCommand(ROBOT_CMD_MANUAL);
//  }
  else
  {
    SetMotionCommand(ROBOT_CMD_MANUAL);
  }    
}

eManualState GetManualState (void)
{
  return (curManualState);
}

void SetOperationMode (eMode setMode, eManualState opManualState)
{
  // SetLogEvent (EV_LOG_OP_MODE, (uint8_t)setMode);
  if((setMode == AUTO) && (IsRTCFault() == true))
  {
    setMode = MANUAL;
    opManualState = MAN_STOP;
  }
  operationMode = setMode; 
  curManualState = opManualState;
//  if(GetMotionPauseState() == PA_WAIT_FOR_TIMEOVER_RESUME)
//  {
//    return;
//  }
  switch(setMode)
  {
  case IDLE:
    break;
  case MANUAL:
    SetTrackChangerState(TC_STOP);
    OperateManualMode(opManualState);
    break;
  case EMERGENCY_STOW:
    SetCycleModeState(CYCLE_IDLE);
    SetRobotState(ROBOT_START);
    LoadEmergencyMotionParameters(NEGATIVE);
    break;
  case COMM_ERROR_EMERGENCY_STOW:
    SetCycleModeState(CYCLE_IDLE);
    SetRobotState(ROBOT_START);
    LoadEmergencyMotionParameters(NEGATIVE);
    break;
  case CYCLE_TEST:
    SetCycleModeState(CYCLE_IDLE);
    //  SetCycleTestState(CT_START);
    break;
  case AUTO:
    {
      SetCycleModeState(CYCLE_IDLE);
      SetRobotState(ROBOT_NORMAL_STOP); 
    }
    break;
    //  case TRACK_CHANGER:
    //    SetCycleModeState(CYCLE_IDLE);
    //    SetRobotState(ROBOT_NORMAL_STOP);
    //    SetTrackChangerState(TC_START);
  default:
    break;
  }
}

void SetMotionPauseState (eMotionPauseState pauseSt)
{
  pauseState = pauseSt;
}

eMotionPauseState GetMotionPauseState (void)
{
  return pauseState;
}
   
eMotionPauseState prevPauseState;

eMotionPauseState GetPrevPauseState(void)
{
 return prevPauseState;
}

void SetPrevPauseState(eMotionPauseState state)
{
prevPauseState = state;
}

//void OperateMotionPauseFSM (void)
//{
//  eMotionPauseState pauseSt = GetMotionPauseState();
//  switch (pauseSt)
//  {
//   case PA_IDLE:
//    break;
//   case PA_PAUSE:
//    {
//    //  SetLogEvent (EV_LOG_PAUSE_STATE, (uint8_t)PA_PAUSE);
//      SetRobotState(ROBOT_IMMEDIATE_STOP);
//      stMaxIloadConfigValues *maxIloadconfigValues = GetSetMaxIloadConfigValues();
//      PauseDelayTimerOn(maxIloadconfigValues->motorPauseDelay);
//      SetMotionPauseState(PA_WAIT_FOR_TIMEOVER);
//    }
//    break;
//   case PA_WAIT_FOR_TIMEOVER:
//    if(IsPauseDelayTimeOver() == true)
//    {
//      SetMotionPauseState(PA_IDLE);
//      if(GetOperationMode() != AUTO)
//      {
//        SetOperationMode(GetOperationMode(), curManualState);
//      }
//    }
//    break;
//   default:
//    break;
//  }
//}


void OperateMotionPauseFSM (void)
{
  eMotionPauseState pauseSt = GetMotionPauseState();
  eMode getMode =  GetOperationMode();    
  stReturn *setReturnValue = GetSetReturnLimits();
  
  switch (pauseSt)
  {
   case PA_IDLE:
    break;
    
    
   case PA_PAUSE:
    {
      //MotorDriveOff();
      prevRobotCommand = GetMotionCommand();
      prevCycleModeState = GetCycleModeState();
      prev_robotState = GetRobotActualDirection();
      prevTrackChangerState = GetTrackChangerState();
      
      if(getMode == MANUAL){
//        SetMotionCommand(ROBOT_CMD_STOP);
        SetRobotState(ROBOT_IMMEDIATE_STOP);
      }
      else if(getMode == AUTO){
        SetRobotState(ROBOT_IMMEDIATE_STOP); 
      }
//      SetOperationMode(getMode, MAN_IDLE);
      SetMotionPauseState(PA_WAIT_FOR_STOP);
    }
    break;
  case PA_WAIT_FOR_STOP:
    {
      if(GetRobotState()==ROBOT_COMPLETED)
      {
        prevRotateSense1Count = GetRotateSense1Count();
        prevRotateSense2Count = GetRotateSense2Count();
//        stHeartbeatConfig* hbtConfig = GetSetHeartbeatConfig();
//        PauseDelayTimerOn(hbtConfig->maxReconnectTime);
        PauseDelayTimerOn(1000);
        
        if(Error_Count() <= setReturnValue->Return1)
        {
        SetMotionPauseState(PA_WAIT_FOR_TIMEOVER_RESUME);
        }
        
        if(Error_Count() > setReturnValue->Return1 || GetTrackChangerError() == TC_CLEANER_NOT_IN_POSITION || GetTrackChangerError() == TC_CLEANER_ERROR || GetTrackChangerError() == TC_NOT_IN_POSITION
           || GetTrackChangerError() == TC_CLEANER_STALL || GetTrackChangerError() == TC_COMMUNICATION_ERROR || GetTrackChangerError() == TC_EDGE_UNKNOWN || GetTrackChangerError() == TC_MISSED_ROWS
             || GetTrackChangerError() == TC_INVALID_RFID)
        {
            SetMotionPauseState(PA_WAIT_FOR_ERROR_CLEAR);
        }
      }
    }
      break;
     
  case PA_WAIT_FOR_ERROR_CLEAR:
       if(Error_Count() == 0)
       {
          SetMotionPauseState(PA_WAIT_FOR_TIMEOVER_RESUME);
          Error_Cleared = 0;
       }
    
        break;
   case PA_WAIT_FOR_TIMEOVER_RESUME:
    if(IsPauseDelayTimeOver() == true)
    {
//      SetCycleModeState(CYCLE_COMPLETED);
    //MotorDriveOn();
      
      if(getMode == AUTO || curManualState == MAN_TRACKCHANGER)
      {
           if(GetTrackChangerError() == TC_CLEANER_NOT_IN_POSITION || GetTrackChangerError() == TC_TRACK_CHANGER_STALL )
           {
             SetTrackChangerState(TC_CHECK_FOR_REMAINING_ROWS);
           }
           else if(GetTrackChangerError() == TC_CLEANER_ERROR || /*GetTrackChangerError() == TC_CLEANER_STALL ||*/ GetTrackChangerError() == TC_COMMUNICATION_ERROR)
           {
//              if(IsValidProximitySensorState())
//              {  
                SetTrackChangerState(TC_CLEANER_OPERATION);
                SetCleanerFSMState(C_SEND_RESUME);
//              }
//              else
//              { 
//                prev_track_error = GetTrackChangerError();
//                SetTrackChangerState(TC_CHECK_FOR_REMAINING_ROWS);
//              }
           }
           else if(GetTrackChangerError() == TC_INVALID_DATA)
           {
              SetTrackChangerState(TC_CLEANER_OPERATION);
              SetCleanerFSMState(C_START);
              Daycount--;
           }
           else if(GetTrackChangerError() == TC_NOT_IN_POSITION)
           {
             row_detected = trackchangercount;
              if(IsValidProximitySensorState())
              {
//                stRfidData* setRfidData = GetSetRfidData();
//                stRfidData getRfidData = *setRfidData;
//                uint32_t RFID = atoi(lastReadRfidData);  
//                if((trackchangercount>0 && trackchangercount <=10 && (getRfidData.rfidData[trackchangercount-1]==RFID))||(trackchangercount>10 && trackchangercount <= 20 && (getRfidData.rfidData2[trackchangercount-11]==RFID))||(trackchangercount>20 && trackchangercount <=25 && (getRfidData.rfidData3[trackchangercount-21]==RFID)))
//                { 
                  SetTrackChangerState(TC_CLEANER_OPERATION);
                  SetCleanerFSMState(C_SEND_RESUME);
//                }
//                else
//                {
//                  prev_track_error = GetTrackChangerError();
//                  SetTrackChangerError(TC_INVALID_RFID); 
////                  SetTrackChangerState(TC_ERROR);
//                }
              } 
              else if(IsCleanerPresent())
              { 
                prev_track_error = GetTrackChangerError();
                SetTrackChangerState(TC_CHECK_FOR_REMAINING_ROWS);
                trackchangercount = trackchangercount -1;
                EepromSetAutoScheduleTime(EEPROM_WRITE);
                tc_count = trackchangercount;
              }
//              else if(prev_track_state == TC_CLEANER_OPERATION)
//              {
//                 prev_track_state = TC_NIL; 
//                 CheckIfTCInPosition();
//                 SetTrackChangerState(TC_CLEANER_OPERATION);
//                 SetCleanerFSMState(C_SEND_RESUME);
//              }
           }
           else if (GetTrackChangerError() == TC_EDGE_UNKNOWN)
           {
              //SetTrackChangerState(TC_STOP);
              SetRobotState(ROBOT_HOME_RETURN);
              break;
           }
           else if (GetTrackChangerError() == TC_MISSED_ROWS)
           {
              SetTrackChangerState(TC_CHECK_FOR_REMAINING_ROWS); 
           }
           else if(GetTrackChangerError() == TC_INVALID_RFID)
           {
//             row_detected = trackchangercount;
             if(prev_track_error == TC_NOT_IN_POSITION)
             {
                 prev_track_error = TC_INVALID_RFID;
                 SetTrackChangerState(TC_CHECK_FOR_REMAINING_ROWS); 
             }
             else
             {
                SetTrackChangerState(TC_WAIT_FOR_PROXIMITY);
                SetProximityCount(1);
             }
           }
//           else
//           {
//             SetTrackChangerState(TC_START); 
//           }
           TrackChangerOperate();
      }
      else if(getMode == MANUAL){
        if(GetMotionCommand() == ROBOT_CMD_POSITIVE)
        {
            SetMotionCommand(ROBOT_CMD_POSITIVE);
        }
        else if(GetMotionCommand() == ROBOT_CMD_NEGATIVE)
        {
            SetMotionCommand(ROBOT_CMD_NEGATIVE);
        }
        else
        {
            SetMotionCommand(ROBOT_CMD_CYCLE_RESUME);
        }
      }
      SetPrevPauseState(GetMotionPauseState());
      SetMotionPauseState(PA_IDLE);
    }
    break;
   default:
    break;
  }
}


void SetMotionCommand (eRobotCommand command)
{
  robotCommand = command;  
}

eRobotCommand GetMotionCommand (void)
{
  return(robotCommand);  
}

uint32_t GetGotoCount(void)
{
  return gotoCount;
}

bool SetGotoCount(uint32_t count)
{
  if(IsCountValid(count) == true)
  {
    gotoCount = count;
    return true;
  }
  else
  {
    return false;
  }
}

bool IsCountValid(uint32_t count)
{
  stCountRange *countRange = GetCountRange();
  if((count >=  countRange->maxNegCountForPVsetup) && (count <= countRange->maxPosCountForPVsetup))
  {
    return true;
  }
  else
  {
    return false;
  }
}
//
static void PauseDelayTimerOn (uint32_t setPauseDelayTimeCount_ms)
{
  if(setPauseDelayTimeCount_ms == 0)
  {
    pauseDelayTimeOver = true;
    PauseDelayTimerStop();
  }
  else
  {
    pauseDelay_timecount = setPauseDelayTimeCount_ms;
    pauseDelayTimeOver = false;
  }
}

static inline void PauseDelayTimerStop (void)
{
  pauseDelay_timecount = 0;
}

static inline bool IsPauseDelayTimeOver (void)
{
  return (pauseDelayTimeOver);
}

static inline void ClearPauseDelayTimeOver (void)
{
  pauseDelayTimeOver = false; 
}

static void PauseDelayTimeIncrement_ms (void)
{
  if(pauseDelay_timecount)
  {
    if(--pauseDelay_timecount <= 0)
    {
      pauseDelayTimeOver = true;
      PauseDelayTimerStop();
    }
    else
    {
      ClearPauseDelayTimeOver();
    }
  }
}

void RobotOperationTimeIncrement_ms (void)
{
  IloadMotMaxTimeIncrement_ms();
  PauseDelayTimeIncrement_ms();  
}

void CheckAndStopRobotOnProximity(void)
{
  if(((curManualState==MAN_TRACKCHANGER)||(GetOperationMode()==AUTO))&&GetTrackChangerState()!=TC_WAIT_FOR_EDGE)
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
      if(IsProximityDetected() && GetRobotActualDirection() != NEGATIVE)
      {
        Serial_Debug("\n Proximity Sensor Detected"); 
        SetRobotState(ROBOT_END);

      }                                                 /* Stop the motor if it reaches the end */
      break;
    } 
  }
}
void SetReportTrackChangerState(eTrackChangerReportState state)
{
  reportState = state;
}
eTrackChangerReportState GetReportTrackChangerState(void)
{
  return reportState;
}
void ClearReportTrackChangerState(void)
{
  reportState = TC_REPORT_UNKNOWN;
}
void SetNextIndex(uint8_t index)
{
  TCNextIndex = index;
}
uint8_t GetNextIndex(void)
{
  return TCNextIndex;
}

uint32_t GetPrevRotateSense1Count(void)
{
  return prevRotateSense1Count;
}


/********** RFID Timer***********/

static inline void rfid_sense_TimerStop (void)
{
  rfid_sense_timecount = 0;
}

void RFID_Sense_Time_Increment()
{
   if(rfid_sense_timecount)
  {
    if(--rfid_sense_timecount <= 0)
    {
      rfid_sense_TimeOver = true;
      rfid_sense_TimerStop();
    }
    else
    {
      rfid_sense_TimeOver = false;
    }
  }
}

static void Rfid_Sense_TimerOn (uint32_t rfid_sense_TimeCount_ms)
{
  if(rfid_sense_TimeCount_ms == 0)
  {
    rfid_sense_TimeOver = true;
    rfid_sense_TimerStop();
  }
  else
  {
    rfid_sense_timecount = rfid_sense_TimeCount_ms;
    rfid_sense_TimeOver = false;
  }
}

/*******Track changer error reverse timer*********/

static inline void TC_Reverse_TimerStop (void)
{
  TC_Reverse_timecount = 0;
}

void TC_Reverse_Time_Increment()
{
   if(TC_Reverse_timecount)
  {
    if(--TC_Reverse_timecount <= 0)
    {
      TC_Reverse_TimeOver = true;
      TC_Reverse_TimerStop();
    }
    else
    {
      TC_Reverse_TimeOver = false;
    }
  }
}

static void TC_Reverse_TimerOn (uint32_t TC_Reverse_TimeCount_ms)
{
  if(TC_Reverse_TimeCount_ms == 0)
  {
    TC_Reverse_TimeOver = true;
    TC_Reverse_TimerStop();
  }
  else
  {
    TC_Reverse_timecount = TC_Reverse_TimeCount_ms;
    TC_Reverse_TimeOver = false;
  }
}
