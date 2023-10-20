#include "eepromstore.h"
#include "NumStrConversion.h"
#include "Serial_Debug.h"
#include "APIProcessing.h"
#include "TempVariables.h"
#include "RobotOperation.h"
#include "CycleMode.h"
#include "BrushControl.h"
#include "AutoSchedule.h"
#include "MotCurrentFaultCommon.h"
#include "EdgeSenseCommon.h"
#include "BrushMotorControl_hal.h"
#include "FaultProcess.h"
#include "DefaultValues.h"
#include "rfid.h"
#include <string.h>

#define LENGTH_OF_EACH_STORE            1                         /* One 32 bit variables    */
#define LENGTH_OF_EACH_STRING_STORE     1                         /* One 32 bit variables    */
#define EEPROM_VIRTUAL_START_ADDRESS    0x0000
#define EEPROM_DEV_INFO_START_ADDRESS   EEPROM_VIRTUAL_START_ADDRESS 
#define EEPROM_DEV_INFO_END_ADDRESS     (EEPROM_DEV_INFO_START_ADDRESS + ((JSON_NO_OF_SET_DEVICE_INFO-1) * MAX_DEV_INFO_FIELD_LEN * LENGTH_OF_EACH_STRING_STORE)-1)
#define EEPROM_VALIDITY_START_ADDRESS   EEPROM_DEV_INFO_END_ADDRESS +1
#define EEPROM_VALIDITY_END_ADDRESS     (EEPROM_VALIDITY_START_ADDRESS + LENGTH_OF_EACH_STORE -1)
#define EEPROM_VALIDITY_DATA            0x0357                    /*Ensemble Defined data. It can be anything apart from 0xffff */
#define STORE_MIN_START_ADDRESS         (EEPROM_VALIDITY_END_ADDRESS+1)

enum
{
  STORE_MODE_STARTADD                    = STORE_MIN_START_ADDRESS,
  STORE_PWM_MOTOR_MANUAL_STARTADD        = STORE_MODE_STARTADD + ((JSON_NO_OF_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE),
  //STORE_BRUSH_ENABLED_STATE_STARTADD     = STORE_PWM_MOTOR_MANUAL_STARTADD + ((JSON_NO_OF_PWM_DATA_MOTOR-1) * LENGTH_OF_EACH_STORE),
 // STORE_PWM_BRUSH_MANUAL_STARTADD        = STORE_PWM_MOTOR_MANUAL_STARTADD + ((JSON_NO_OF_PWM_DATA_MOTOR-1) * LENGTH_OF_EACH_STORE),
  //STORE_BRUSH_MOTOR_POLARITY_STARTADD   = STORE_PWM_BRUSH_MANUAL_STARTADD + ((JSON_NO_OF_PWM_DATA_BRUSH-1) * LENGTH_OF_EACH_STORE),
  STORE_PWM_MOTOR_AUTO_STARTADD          = STORE_PWM_MOTOR_MANUAL_STARTADD + ((JSON_NO_OF_PWM_DATA_MOTOR-1) * LENGTH_OF_EACH_STORE),
  //STORE_PWM_BRUSH_AUTO_STARTADD          = STORE_PWM_MOTOR_AUTO_STARTADD + ((JSON_NO_OF_PWM_DATA_MOTOR-1) * LENGTH_OF_EACH_STORE),
  STORE_PULSE_COUNT_STARTADD             = STORE_PWM_MOTOR_AUTO_STARTADD + ((JSON_NO_OF_PWM_DATA_MOTOR-1) * LENGTH_OF_EACH_STORE),
  STORE_AUTO_SCHEDULE_STARTADD           = STORE_PULSE_COUNT_STARTADD + ((JSON_NO_OF_PULSE_COUNT_DATA-1) * LENGTH_OF_EACH_STORE),
  STORE_EDGE_SENSOR_STATE_STARTADD       = STORE_AUTO_SCHEDULE_STARTADD + ((JSON_NO_OF_AUTO_SCHED_DATA-1) * LENGTH_OF_EACH_STORE),
  //STORE_CURRENT_LIMIT_STARTADD           = STORE_EDGE_SENSOR_STATE_STARTADD + ((JSON_NO_OF_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE),
  STORE_LOW_BAT_SOC_STARTADD             = STORE_EDGE_SENSOR_STATE_STARTADD + ((JSON_NO_OF_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE),
  //STORE_MOT_FAULT_STARTADD               = STORE_LOW_BAT_SOC_STARTADD + ((JSON_NO_OF_SOC_DATA-1) * LENGTH_OF_EACH_STORE), 
  STORE_TRACK_INFO_STARTADD              = STORE_LOW_BAT_SOC_STARTADD + ((JSON_NO_OF_SOC_DATA-1) * LENGTH_OF_EACH_STORE),
  STORE_ROW_LENGTH_STARTADD              = STORE_TRACK_INFO_STARTADD + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_CLEANER_MAC_STARTADD             = STORE_ROW_LENGTH_STARTADD + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_ENC_PULSE_TOLERANCE_STARTADD     = STORE_CLEANER_MAC_STARTADD + ((JSON_NO_OF_CLEANER_MAC_DATA+JSON_NO_OF_CLEANER_MAC_DATA) * LENGTH_OF_EACH_STORE),
  STORE_TC_CONTROL_MODE_STARTADD         = STORE_ENC_PULSE_TOLERANCE_STARTADD + ((JSON_NO_OF_ENC_PULSE_TOLERANCE_DATA-1) * LENGTH_OF_EACH_STORE),
  STORE_RFID_DATA_STARTADD               = STORE_TC_CONTROL_MODE_STARTADD + ((JSON_NO_OF_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE),
  STORE_RFID_STATE_STARTADD              =  STORE_RFID_DATA_STARTADD + ((MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_HEARTBEAT_CONFIG_STARTADD        =  STORE_RFID_STATE_STARTADD + ((JSON_NO_OF_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE),
  
  STORE_LOG_INTERVAL_STARTADD            = STORE_HEARTBEAT_CONFIG_STARTADD + ((JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS-1) * LENGTH_OF_EACH_STORE),  //07D change 
  STORE_WHEEL_DIA_STARTADD               = STORE_LOG_INTERVAL_STARTADD + ((JSON_NO_OF_INTERVAL_DATA-1) * LENGTH_OF_EACH_STORE),
  STORE_CYCLE_FREQUENCY_STARTADD         = STORE_WHEEL_DIA_STARTADD + ((JSON_NO_OF_WHEELDIA_DATA-1) * LENGTH_OF_EACH_STORE),
  STORE_CONTINUE_STARTADD                = STORE_CYCLE_FREQUENCY_STARTADD + ((JSON_NO_OF_CYCLEFREQUENCY_DATA-1) * LENGTH_OF_EACH_STORE),
  STORE_BRUSH_ENABLED_STATE_STARTADD     = STORE_CONTINUE_STARTADD + ((JSON_NO_OF_CONTINUE-1) * LENGTH_OF_EACH_STORE),
  STORE_RETURN_STARTADD                 =  STORE_BRUSH_ENABLED_STATE_STARTADD + ((JSON_NO_OF_MOTOR_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_AUTO_SCHEDULE_STARTADD           = STORE_RETURN_STARTADD + ((JSON_NO_OF_RETURN-1) * LENGTH_OF_EACH_STORE), 
  STORE_ROWDAY_STARTADD                 = STORE_RETURN_STARTADD + ((JSON_NO_OF_RETURN-1) * LENGTH_OF_EACH_STORE),
  STORE_ROW_LENGTH2_STARTADD             = STORE_ROWDAY_STARTADD + ((JSON_NO_OF_ROWS-1) * LENGTH_OF_EACH_STORE), 
  STORE_ROW_LENGTH3_STARTADD                = STORE_ROW_LENGTH2_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_ROW_LENGTH4_STARTADD                = STORE_ROW_LENGTH3_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_ROW_LENGTH5_STARTADD                = STORE_ROW_LENGTH4_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_ROW_LENGTH6_STARTADD                = STORE_ROW_LENGTH5_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_ROW_LENGTH7_STARTADD                = STORE_ROW_LENGTH6_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_TRACK_INFO2_STARTADD                = STORE_ROW_LENGTH7_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_RFID_DATA2_STARTADD               = STORE_TRACK_INFO2_STARTADD + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_RFID_DATA3_STARTADD               = STORE_RFID_DATA2_STARTADD + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_RFID_RUN_ADDRESS                =  STORE_RFID_DATA3_STARTADD + ((MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_ROW_LENGTH12_STARTADD                =  STORE_RFID_RUN_ADDRESS + ((JSON_NO_OF_RUNRFID-1) * LENGTH_OF_EACH_STORE),
  STORE_ROW_LENGTH13_STARTADD                = STORE_ROW_LENGTH12_STARTADD  + ((MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_MAX_START_ADDRESS                = STORE_ROW_LENGTH13_STARTADD  + ((MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
  STORE_MAX_ADDRESS                      = STORE_MAX_START_ADDRESS - 1                                   
}eEepromEndAddress;


float circumference;
extern char Daycount;
extern int trackchangercount,rowsActuallyCleaned;
extern int distance;
extern int Row1value;
extern int Row1getvalue;

eEepromStatus InitialiseAlltoDefault(stJsonCommand *jsonCmdParams, uint16_t commandParamsTotalCount);

eEepromStatus EepromDataNil(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  return status;
}

uint16_t GetMaxVirtualAddress (void)
{
  return STORE_MAX_ADDRESS;  
}

eEepromStatus EepromSetMode(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_MODE_STARTADD;
  eMode setMode = GetOperationMode();
  float eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setMode;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_MODE_STATE_DATA-1);
    if(status == EEPROM_OK)
    {
      eMode getMode = (eMode)eepromValue[eepromAddIndex++];
      SetInitialOperationMode(getMode);
    }
  }
  return status; 
}
eEepromStatus EepromSetTcControlMode(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_TC_CONTROL_MODE_STARTADD;
  eTcControlMode setMode = GetTcControlMode();
  float eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setMode;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_MODE_STATE_DATA-1);
    if(status == EEPROM_OK)
    {
      eTcControlMode getMode = (eTcControlMode)eepromValue[eepromAddIndex++];
      SetTcControlMode(getMode);
    }
  }
  return status; 
}
eEepromStatus EepromStoreRfidState(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RFID_STATE_STARTADD;
  eRfidState setMode = GetRfidState();
  float eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setMode;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_MODE_STATE_DATA-1);
    if(status == EEPROM_OK)
    {
      eRfidState getMode = (eRfidState)eepromValue[eepromAddIndex++];
      SetRfidState(getMode);
    }
  }
  return status; 
}
eEepromStatus EepromSetMotorPwmManualMode(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_PWM_MOTOR_MANUAL_STARTADD;
  stRobotPwmParam *setPWMParams = GetSetManualPwmParameters();
  stRobotPwmParam *setPWMCycleParams = GetSetCycleManualPwmParameters();
  float eepromValue[JSON_NO_OF_PWM_DATA_MOTOR-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setPWMParams->accelTime;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm1;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm2;
    eepromValue[eepromAddIndex++] = setPWMParams->decelStartCountDiff;
    eepromValue[eepromAddIndex++] = setPWMParams->decelTime;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm1;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm2;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_PWM_DATA_MOTOR-1);
    if(status == EEPROM_OK)
    {
      setPWMCycleParams->accelTime               =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->accelTime                    =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->steadyPwm1              =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->steadyPwm1                   =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->steadyPwm2              =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->steadyPwm2                   =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->decelStartCountDiff     =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->decelStartCountDiff         =  (uint16_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->decelTime              =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->decelTime                   =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->approachPwm1           =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->approachPwm1                =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->approachPwm2           =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->approachPwm2                =  (uint32_t)eepromValue[eepromAddIndex];
    }
//    Serial_Debug("\nRead ");
//    Serial_Debug_Num(setPWMParams->accelTime );Serial_Debug(" ");
//    Serial_Debug_Num(setPWMParams->approachPWM );Serial_Debug(" ");
//    Serial_Debug_Num(setPWMParams->decelCountDiff );Serial_Debug(" ");
//    Serial_Debug_Num(setPWMParams->decelTime );Serial_Debug(" ");
//    Serial_Debug_Num(setPWMParams->runPWM );
  }
  return status;  
}

eEepromStatus EepromHeartbeatConfiguration(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_HEARTBEAT_CONFIG_STARTADD;
  stHeartbeatConfig* storeHeartbeatConfig;
  storeHeartbeatConfig = GetSetHeartbeatConfig();
  float eepromValue[JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
//    eepromValue[eepromAddIndex++] = storeHeartbeatConfig->enable;
    eepromValue[eepromAddIndex++] = storeHeartbeatConfig->interval_ms;
//    eepromValue[eepromAddIndex++] = storeHeartbeatConfig->noOfMessages;
    eepromValue[eepromAddIndex++] = storeHeartbeatConfig->maxReconnectTime;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS-1);
    if(status == EEPROM_OK)
    {
//      storeHeartbeatConfig->enable = eepromValue[eepromAddIndex++];
      storeHeartbeatConfig->interval_ms = (long int)eepromValue[eepromAddIndex++];   
//      storeHeartbeatConfig->noOfMessages =(long int)eepromValue[eepromAddIndex++];   
      storeHeartbeatConfig->maxReconnectTime =(uint32_t)eepromValue[eepromAddIndex]; 
    }
  }
  return status;
}
//eEepromStatus EepromSetBrushEnabledState(eEpromAccessType eepromOp)
//{
//  eEepromStatus status = EEPROM_OK;
//  uint16_t eepromAddress = STORE_BRUSH_ENABLED_STATE_STARTADD;
//  bool *setBrushEnabledState = GetSetBrushEnabledState();
//  float eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
//  uint16_t eepromAddIndex = 0;
//  if(eepromOp == EEPROM_WRITE)
//  {
//    eepromValue[eepromAddIndex++] = *setBrushEnabledState;
//    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
//  }
//  else
//  {
//    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_MODE_STATE_DATA-1);
//    if(status == EEPROM_OK)
//    {
//      *setBrushEnabledState             =  (bool)eepromValue[eepromAddIndex++];
//    }
//  }
//  return status; 
//}
//
//eEepromStatus EepromSetBrushPwmManualMode(eEpromAccessType eepromOp)
//{
//  eEepromStatus status = EEPROM_OK;
//  uint16_t eepromAddress = STORE_PWM_BRUSH_MANUAL_STARTADD;
//  stBrushPwmParam *setPWMParams = GetSetBrushNormalPwmParameters();
//  float eepromValue[JSON_NO_OF_PWM_DATA_BRUSH-1];
//  uint16_t eepromAddIndex = 0;
//  if(eepromOp == EEPROM_WRITE)
//  {
//    eepromValue[eepromAddIndex++] = setPWMParams->accelTime;
//    eepromValue[eepromAddIndex++] = setPWMParams->decelTime;
//    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm;
//    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
//  }
//  else
//  {
//    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_PWM_DATA_BRUSH-1);
//    if(status == EEPROM_OK)
//    {
//      setPWMParams->accelTime             =  (uint32_t)eepromValue[eepromAddIndex++];
//      setPWMParams->decelTime             =  (uint32_t)eepromValue[eepromAddIndex++];
//      setPWMParams->steadyPwm             =  (uint8_t)eepromValue[eepromAddIndex];
//    }
////    Serial_Debug("\nRead ");
////    Serial_Debug_Num(setPWMParams->accelTime );Serial_Debug(" ");
////    Serial_Debug_Num(setPWMParams->approachPWM );Serial_Debug(" ");
////    Serial_Debug_Num(setPWMParams->decelCountDiff );Serial_Debug(" ");
////    Serial_Debug_Num(setPWMParams->decelTime );Serial_Debug(" ");
////    Serial_Debug_Num(setPWMParams->runPWM );
//  }
//  return status;  
//}
//
//eEepromStatus EepromSetBrushMotorPolarity(eEpromAccessType eepromOp)
//{
//  eEepromStatus status = EEPROM_OK;
//  uint16_t eepromAddress = STORE_BRUSH_MOTOR_POLARITY_STARTADD;
//  stBrushMotPolarity *motPolarity = GetSetBrushMotPolarity();
//  float eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
//  uint16_t eepromAddIndex = 0;
//  if(eepromOp == EEPROM_WRITE)
//  {
//    eepromValue[eepromAddIndex++] = motPolarity->mot1Polarity;
//    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
//  }
//  else
//  {
//    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_MODE_STATE_DATA-1);
//    if(status == EEPROM_OK)
//    {
//      motPolarity->mot1Polarity  =  (bool)eepromValue[eepromAddIndex++];
//    }
//  }
//  return status; 
//}

eEepromStatus EepromSetMotorPwmAutoMode(eEpromAccessType eepromOp)
{

  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_PWM_MOTOR_AUTO_STARTADD;
  stRobotPwmParam *setPWMParams = GetSetCycleAutoPwmParameters();
  float eepromValue[JSON_NO_OF_PWM_DATA_MOTOR-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setPWMParams->accelTime;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm1;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm2;
    eepromValue[eepromAddIndex++] = setPWMParams->decelStartCountDiff;
    eepromValue[eepromAddIndex++] = setPWMParams->decelTime;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm1;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm2;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_PWM_DATA_MOTOR-1);
    if(status == EEPROM_OK)
    {
      setPWMParams->accelTime            =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->steadyPwm1           =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->steadyPwm2           =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->decelStartCountDiff  =  (uint16_t)eepromValue[eepromAddIndex++];
      setPWMParams->decelTime            =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->approachPwm1         =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->approachPwm2         =  (uint32_t)eepromValue[eepromAddIndex];
    }
//    Serial_Debug("\nRead ");
//    Serial_Debug_Num(setPWMParams->accelTime );Serial_Debug(" ");
//    Serial_Debug_Num(setPWMParams->approachPWM );Serial_Debug(" ");
//    Serial_Debug_Num(setPWMParams->decelCountDiff );Serial_Debug(" ");
//    Serial_Debug_Num(setPWMParams->decelTime );Serial_Debug(" ");
//    Serial_Debug_Num(setPWMParams->runPWM );
  }
  return status;  
}

//eEepromStatus EepromSetBrushPwmAutoMode(eEpromAccessType eepromOp)
//{
//  eEepromStatus status = EEPROM_OK;
//  uint16_t eepromAddress = STORE_PWM_BRUSH_AUTO_STARTADD;
//  stBrushPwmParam *setPWMParams = GetSetBrushAutoPwmParameters();
//  float eepromValue[JSON_NO_OF_PWM_DATA_BRUSH-1];
//  uint16_t eepromAddIndex = 0;
//  if(eepromOp == EEPROM_WRITE)
//  {
//    eepromValue[eepromAddIndex++] = setPWMParams->accelTime;
//    eepromValue[eepromAddIndex++] = setPWMParams->decelTime;
//    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm;
//    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
//  }
//  else
//  {
//    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_PWM_DATA_BRUSH-1);
//    if(status == EEPROM_OK)
//    {
//      setPWMParams->accelTime             =  (uint32_t)eepromValue[eepromAddIndex++];
//      setPWMParams->decelTime             =  (uint32_t)eepromValue[eepromAddIndex++];
//      setPWMParams->steadyPwm             =  (uint8_t)eepromValue[eepromAddIndex];
//    }
////    Serial_Debug("\nRead ");
////    Serial_Debug_Num(setPWMParams->accelTime );Serial_Debug(" ");
////    Serial_Debug_Num(setPWMParams->approachPWM );Serial_Debug(" ");
////    Serial_Debug_Num(setPWMParams->decelCountDiff );Serial_Debug(" ");
////    Serial_Debug_Num(setPWMParams->decelTime );Serial_Debug(" ");
////    Serial_Debug_Num(setPWMParams->runPWM );
//  }
//  return status;  
//}
eEepromStatus EepromSetPulseCount(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_PULSE_COUNT_STARTADD;
  stCountRange *setCountValues = GetSetCountRange();
  float eepromValue[JSON_NO_OF_PULSE_COUNT_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setCountValues->maxPosCountForPVsetup;
    eepromValue[eepromAddIndex++] = setCountValues->maxNegCountForPVsetup;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_PULSE_COUNT_DATA-1);
    if(status == EEPROM_OK)
    {
      setCountValues->maxPosCountForPVsetup             =  (uint32_t)eepromValue[eepromAddIndex++];
      setCountValues->maxNegCountForPVsetup             =  (uint32_t)eepromValue[eepromAddIndex++];
    }
  }
  return status;  
}

eEepromStatus EepromSetAutoScheduleTime(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_AUTO_SCHEDULE_STARTADD;
  stScheduleTime *setScheduleValues = GetSetScheduleTime();
  float eepromValue[JSON_NO_OF_AUTO_SCHED_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setScheduleValues->hour;
    eepromValue[eepromAddIndex++] = setScheduleValues->minute;
    eepromValue[eepromAddIndex++] = Daycount;
    eepromValue[eepromAddIndex++] = trackchangercount; 
    eepromValue[eepromAddIndex++] = rowsActuallyCleaned;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_AUTO_SCHED_DATA-1);
    if(status == EEPROM_OK)
    {
      setScheduleValues->hour             =  (uint8_t)eepromValue[eepromAddIndex++];
      setScheduleValues->minute           =  (uint8_t)eepromValue[eepromAddIndex++];
      Daycount                            =  (uint32_t)eepromValue[eepromAddIndex++];
      trackchangercount                   =   eepromValue[eepromAddIndex++];
      rowsActuallyCleaned                 =   eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromSetEdgeSensorState(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_EDGE_SENSOR_STATE_STARTADD;
  bool *setEdgeSensorState = GetSetEdgeSensorEnabledState();
  float eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = *setEdgeSensorState;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_MODE_STATE_DATA-1);
    if(status == EEPROM_OK)
    {
      *setEdgeSensorState             =  (bool)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

//eEepromStatus EepromSetCurrentLimits(eEpromAccessType eepromOp)
//{
//  eEepromStatus status = EEPROM_OK;
//  uint16_t eepromAddress = STORE_CURRENT_LIMIT_STARTADD;
//  stMaxMotorCurrent *setCurrentValues = GetSetMaxMotorCurrentLimits();
//  float eepromValue[JSON_NO_OF_CURRENT_LIMIT-1];
//  uint16_t eepromAddIndex = 0;
//  if(eepromOp == EEPROM_WRITE)
//  {
//    eepromValue[eepromAddIndex++] = setCurrentValues->Imot1;
//    eepromValue[eepromAddIndex++] = setCurrentValues->Imot2;
//    eepromValue[eepromAddIndex++] = setCurrentValues->Imot3;
//    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
//  }
//  else
//  {
//    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_CURRENT_LIMIT-1);
//    if(status == EEPROM_OK)
//    {
//      setCurrentValues->Imot1             =  eepromValue[eepromAddIndex++];
//      setCurrentValues->Imot2             =  eepromValue[eepromAddIndex++];
//      setCurrentValues->Imot3             =  eepromValue[eepromAddIndex++];
//    }
//  }
//  return status; 
//}

eEepromStatus EepromSetLowBatSoC(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_LOW_BAT_SOC_STARTADD;
  uint8_t *setLowBatSoC = GetSetMinBattSoc();
  float eepromValue[JSON_NO_OF_SOC_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = *setLowBatSoC;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_SOC_DATA-1);
    if(status == EEPROM_OK)
    {
      *setLowBatSoC  =  (uint8_t)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromZigbeeConfiguration(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  return status;
}

eEepromStatus EepromStoreTrackInfo(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_TRACK_INFO_STARTADD;
  stTrackData *setTrackData = GetSetTrackData();
  uint8_t eepromTrackInfoCount = MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    //eepromValue[eepromAddIndex++] = setTrackData->trackCount;
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = (bool)setTrackData->trackValid[i];
    }
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setTrackData->trackDistance[i];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromTrackInfoCount);
    if(status == EEPROM_OK)
    {
      //setTrackData->trackCount             =  (uint32_t)eepromValue[eepromAddIndex++];
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        setTrackData->trackValid[i]       =  (bool)eepromValue[eepromAddIndex++];
      }
      
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        setTrackData->trackDistance[i]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreTrackInfo2(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_TRACK_INFO2_STARTADD;
  stTrackData *setTrackData = GetSetTrackData();
  uint8_t eepromTrackInfoCount = MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    //eepromValue[eepromAddIndex++] = setTrackData->trackCount;
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = (bool)setTrackData->trackValid2[i];
    }
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setTrackData->trackDistance2[i];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromTrackInfoCount);
    if(status == EEPROM_OK)
    {
      //setTrackData->trackCount             =  (uint32_t)eepromValue[eepromAddIndex++];
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        setTrackData->trackValid2[i]       =  (bool)eepromValue[eepromAddIndex++];
      }
      
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        setTrackData->trackDistance2[i]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
 // Row1value = 1;
  if ((Row1value == 1)||(Row1getvalue == 0))
  {
    uint16_t eepromAddress = STORE_ROW_LENGTH_STARTADD;
    stRowLengthData *setRowLengthData = GetSetRowLengthData();
    uint8_t eepromRowLengthCount = MAX_NO_OF_TRACKS;//+MAX_NO_OF_TRACKS;
    float eepromValue[MAX_NO_OF_TRACKS];//+MAX_NO_OF_TRACKS];
    uint16_t eepromAddIndex = 0;
    if(eepromOp == EEPROM_WRITE)
    {
      for(int i=0;i<10;i++)
      {
        eepromValue[eepromAddIndex++] = setRowLengthData->rowLength[i][FORWARD_COUNT];
        eepromValue[eepromAddIndex++] = setRowLengthData->rowLength[i][REVERSE_COUNT];
      }
      status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
    }
    else
    {
      status = ReadEEPROM (eepromAddress, eepromValue, eepromRowLengthCount);      
      if(status == EEPROM_OK)
      {
        for(int i=0;i<10;i++)
        {
         setRowLengthData->rowLength[i][FORWARD_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
         setRowLengthData->rowLength[i][REVERSE_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
        }
       Row1value = 2; 
      }
    }    
//    return status; 
  }
  if (Row1value == 2)  //||(Row1getvalue == 2))
  {
    uint16_t eepromAddress = STORE_ROW_LENGTH12_STARTADD;
    stRowLengthData *setRowLengthData = GetSetRowLengthData();
    uint8_t eepromRowLengthCount = MAX_NO_OF_TRACKS;//+MAX_NO_OF_TRACKS;
    float eepromValue[MAX_NO_OF_TRACKS];//+MAX_NO_OF_TRACKS];
    uint16_t eepromAddIndex = 0;
    if(eepromOp == EEPROM_WRITE)
    {
      //eepromValue[eepromAddIndex++] = setTrackData->trackCount;
      for(int i=9;i<20;i++)
      {
        eepromValue[eepromAddIndex++] = setRowLengthData->rowLength[i][FORWARD_COUNT];
        eepromValue[eepromAddIndex++] = setRowLengthData->rowLength[i][REVERSE_COUNT];
      }
      status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
    }
    else
    {
      status = ReadEEPROM (eepromAddress, eepromValue, eepromRowLengthCount);
      if(status == EEPROM_OK)
      {
        //setTrackData->trackCount             =  (uint32_t)eepromValue[eepromAddIndex++];
        for(int i=9;i<20;i++)
        {
         setRowLengthData->rowLength[i][FORWARD_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
         setRowLengthData->rowLength[i][REVERSE_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
        }
        Row1value = 3;
      }
    }
    
    //return status; 
  }
  if (Row1value == 3)//||(Row1getvalue == 3))
  {
    uint16_t eepromAddress = STORE_ROW_LENGTH13_STARTADD;
    stRowLengthData *setRowLengthData = GetSetRowLengthData();
    uint8_t eepromRowLengthCount = MAX_NO_OF_TRACKS;//+MAX_NO_OF_TRACKS;
    float eepromValue[MAX_NO_OF_TRACKS];//+MAX_NO_OF_TRACKS];
    uint16_t eepromAddIndex = 0;
    if(eepromOp == EEPROM_WRITE)
    {
      //eepromValue[eepromAddIndex++] = setTrackData->trackCount;
      for(int i=18;i<30;i++)
      {
        eepromValue[eepromAddIndex++] = setRowLengthData->rowLength[i][FORWARD_COUNT];
        eepromValue[eepromAddIndex++] = setRowLengthData->rowLength[i][REVERSE_COUNT];
      }
      status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
    }
    else
    {
      status = ReadEEPROM (eepromAddress, eepromValue, eepromRowLengthCount);
      if(status == EEPROM_OK)
      {
        //setTrackData->trackCount             =  (uint32_t)eepromValue[eepromAddIndex++];
        for(int i=18;i<30;i++)
        {
         setRowLengthData->rowLength[i][FORWARD_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
         setRowLengthData->rowLength[i][REVERSE_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
        }
        Row1value = 0;
      }
    }
    
   // return status; 
  }
  return status; 
}

//eEepromStatus EepromSetMotorFaultConditions(eEpromAccessType eepromOp)
//{
//  eEepromStatus status = EEPROM_OK;
//  uint16_t eepromAddress = STORE_MOT_FAULT_STARTADD;
//  stMaxIloadConfigValues *setMaxIloadConfigValues = GetSetMaxIloadConfigValues();
//  float eepromValue[JSON_NO_OF_MOTOR_FAULT_CONDITIONS-1];
//  uint16_t eepromAddIndex = 0;
//  if(eepromOp == EEPROM_WRITE)
//  {
//    eepromValue[eepromAddIndex++] = setMaxIloadConfigValues->maxIloadNoiseTimeValue;
//    eepromValue[eepromAddIndex++] = setMaxIloadConfigValues->maxIloadFreqTimeValue;
//    eepromValue[eepromAddIndex++] = setMaxIloadConfigValues->maxIloadRepeatCountValue;
//    eepromValue[eepromAddIndex++] = setMaxIloadConfigValues->motorPauseDelay;
//    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
//  }
//  else
//  {
//    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_MOTOR_FAULT_CONDITIONS-1);
//    if(status == EEPROM_OK)
//    {
//      setMaxIloadConfigValues->maxIloadNoiseTimeValue      =  (uint32_t)eepromValue[eepromAddIndex++];
//      setMaxIloadConfigValues->maxIloadFreqTimeValue       =  (uint32_t)eepromValue[eepromAddIndex++];
//      setMaxIloadConfigValues->maxIloadRepeatCountValue    =  (uint32_t)eepromValue[eepromAddIndex++];
//      setMaxIloadConfigValues->motorPauseDelay             =  (uint32_t)eepromValue[eepromAddIndex++];
//    }
//  }
//  return status; 
//}

eEepromStatus EepromStoreCleanerMAC(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_CLEANER_MAC_STARTADD;
  unCleanerMAC* setCleanerMAC;
  setCleanerMAC = GetSetCleanerMAC();
  float eepromValue[JSON_NO_OF_CLEANER_MAC_DATA+JSON_NO_OF_CLEANER_MAC_DATA];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_32[0];
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_32[1];
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_32[2];
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_32[3];
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_CLEANER_MAC_DATA+JSON_NO_OF_CLEANER_MAC_DATA);
    if(status == EEPROM_OK)
    {
      setCleanerMAC->cleanerMAC_32[0]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setCleanerMAC->cleanerMAC_32[1]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setCleanerMAC->cleanerMAC_32[2]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setCleanerMAC->cleanerMAC_32[3]             =  (uint16_t)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromStoreEncoderPulseTolerance(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ENC_PULSE_TOLERANCE_STARTADD;
  uint32_t *setEncPulseTolerance = GetSetEncoderPulseTolerance();
  float eepromValue[JSON_NO_OF_ENC_PULSE_TOLERANCE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = *setEncPulseTolerance;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_ENC_PULSE_TOLERANCE_DATA-1);
    if(status == EEPROM_OK)
    {
      *setEncPulseTolerance  =  (uint32_t)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}
eEepromStatus EepromStoreRfidData(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RFID_DATA_STARTADD;
  stRfidData *setRfidData = GetSetRfidData();
  uint8_t eepromRfidDataCount = MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setRfidData->rfidData[i];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromRfidDataCount);
    if(status == EEPROM_OK)
    {
      
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        setRfidData->rfidData[i]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRfidData2(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RFID_DATA2_STARTADD;
  stRfidData *setRfidData = GetSetRfidData();
  uint8_t eepromRfidDataCount = MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setRfidData->rfidData2[i];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromRfidDataCount);
    if(status == EEPROM_OK)
    {
      
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        setRfidData->rfidData2[i]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRfidData3(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RFID_DATA3_STARTADD;
  stRfidData *setRfidData = GetSetRfidData();
  uint8_t eepromRfidDataCount = MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setRfidData->rfidData3[i];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromRfidDataCount);
    if(status == EEPROM_OK)
    {
      
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        setRfidData->rfidData3[i]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}


eEepromStatus ResetEepromVariables(void)
{
  eEepromStatus status = EEPROM_OK;
  stJsonCommand *jsonCmdParam;
  uint16_t commandParamsTotalCount = GetCommandProcessingParam(&jsonCmdParam);
  status = InitialiseAlltoDefault(jsonCmdParam, commandParamsTotalCount);
  return status;
}

eEepromStatus InitialiseEepromVariables(void)
{
  eEepromStatus status = EEPROM_OK;
  float eepromValue;
  stJsonCommand *jsonCmdParam;
  uint16_t commandParamsTotalCount = GetCommandProcessingParam(&jsonCmdParam);
  status = ReadEEPROM (EEPROM_VALIDITY_START_ADDRESS, &eepromValue, 1);
  if(status == EEPROM_OK)
  {
    if((uint32_t)eepromValue == EEPROM_VALIDITY_DATA)
    {
      for(uint16_t commandParamsCount =0; commandParamsCount < commandParamsTotalCount; commandParamsCount++)
      {
        status |= jsonCmdParam[commandParamsCount].executeStoreProcess(EEPROM_READ);
      }
    }
    else
    {
      status = InitialiseAlltoDefault(jsonCmdParam, commandParamsTotalCount);
    }
  }
  else
  {
    status = InitialiseAlltoDefault(jsonCmdParam, commandParamsTotalCount);     
  }
  status |= ReadEEPROM (EEPROM_VALIDITY_START_ADDRESS, &eepromValue, 1);
  
  return status;
}

eEepromStatus InitialiseAlltoDefault(stJsonCommand *jsonCmdParams, uint16_t commandParamsTotalCount)
{
  Serial_Debug("\n\n Initialising All to Default ");
  eEepromStatus status = EEPROM_OK;
  for(uint16_t commandParamsCount =0; commandParamsCount < commandParamsTotalCount; commandParamsCount++)
  {
    jsonCmdParams[commandParamsCount].executeAssignDefaultProcess();
  }
  for(uint16_t commandParamsCount =0; commandParamsCount < commandParamsTotalCount; commandParamsCount++)
  {
    status |= jsonCmdParams[commandParamsCount].executeStoreProcess(EEPROM_WRITE);
  }
  float eepromValue = EEPROM_VALIDITY_DATA;
  status |= WriteEEPROM (EEPROM_VALIDITY_START_ADDRESS, &eepromValue, 1);
  return status;
}

eEepromStatus EepromDevIDInfo(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = EEPROM_DEV_INFO_START_ADDRESS;
  stDevInfo *devIDInfoValues = GetSetDeviceIDInfo();
  uint16_t totalDataCount = (JSON_NO_OF_SET_DEVICE_INFO-1) * MAX_DEV_INFO_FIELD_LEN;
  char eepromValue[(JSON_NO_OF_SET_DEVICE_INFO-1) * MAX_DEV_INFO_FIELD_LEN];
  uint16_t datacount = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    snprintf(&eepromValue[datacount], MAX_DEV_INFO_FIELD_LEN, devIDInfoValues->devID);
    uint16_t storelen = strlen(&eepromValue[datacount]);
    datacount = datacount+storelen;
    eepromValue[datacount++] = ',';
    snprintf(&eepromValue[datacount], MAX_DEV_INFO_FIELD_LEN, devIDInfoValues->hwVersion);
    storelen = strlen(&eepromValue[datacount]);
    datacount = datacount+storelen;
    eepromValue[datacount++] = ',';
    snprintf(&eepromValue[datacount], MAX_DEV_INFO_FIELD_LEN, devIDInfoValues->serialNo);
    storelen = strlen(&eepromValue[datacount]);
    datacount = datacount+storelen;
    eepromValue[datacount++] = '\0';
    for(uint16_t dummyDataIndex = datacount; dummyDataIndex < totalDataCount; dummyDataIndex++)
    {
      eepromValue[dummyDataIndex] = '\0';
    }
    status = WriteEEPROM_Byte (eepromAddress, (uint8_t*)eepromValue, totalDataCount);
  }
  else
  {
    status = ReadEEPROM_Byte (eepromAddress, (uint8_t*)eepromValue, totalDataCount);
    if(status == EEPROM_OK)
    {
      char *positionInfoStr[JSON_NO_OF_SET_DEVICE_INFO-1];
      uint8_t maxNoStrings=(sizeof positionInfoStr)/4;
      uint8_t totalResulStringCount =  SplitString (eepromValue, positionInfoStr, ',', maxNoStrings );
      if(totalResulStringCount == (JSON_NO_OF_SET_DEVICE_INFO-1))
      {
        snprintf(devIDInfoValues->devID, MAX_DEV_INFO_FIELD_LEN, positionInfoStr[0]);
        snprintf(devIDInfoValues->hwVersion, MAX_DEV_INFO_FIELD_LEN, positionInfoStr[1]);
        snprintf(devIDInfoValues->serialNo, MAX_DEV_INFO_FIELD_LEN, positionInfoStr[2]);
      }
      else
      {
        status = EEPROM_READ_ERROR;
      }
    }
  }
  return status;
}

//07D

eEepromStatus EepromSetWheelDia(eEpromAccessType eepromOp)
{ 
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_WHEEL_DIA_STARTADD;
  stWheelDia *setWheelDia = GetSetwheelDiaLimits();
  float eepromValue[JSON_NO_OF_WHEELDIA_DATA-1];
  
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setWheelDia->Dia;
    eepromValue[eepromAddIndex++] = setWheelDia->Pulse;
    eepromValue[eepromAddIndex++] = distance;
   
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_WHEELDIA_DATA-1);
    if(status == EEPROM_OK)
    {
      setWheelDia->Dia = eepromValue[eepromAddIndex++];
      setWheelDia->Pulse = eepromValue[eepromAddIndex++];
      distance = eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromSetCycleFrequency(eEpromAccessType eepromOp)
{
 eEepromStatus status = EEPROM_OK;
 uint16_t eepromAddress = STORE_CYCLE_FREQUENCY_STARTADD;
  
  stCycleFrequency *setCycleFrequency = GetSetcycleFrequencyLimits();
  
  float eepromValue[JSON_NO_OF_CYCLEFREQUENCY_DATA-1];
  
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setCycleFrequency->Cycle;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_CYCLEFREQUENCY_DATA-1);
    if(status == EEPROM_OK)
    {
     setCycleFrequency->Cycle = eepromValue[eepromAddIndex++];
     
    }
  }
 
 return status;
}


eEepromStatus EepromSetLogInterval(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_LOG_INTERVAL_STARTADD;
 
  stInterval *setInterval = GetSetIntervalLimits();
  
  float eepromValue[JSON_NO_OF_INTERVAL_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setInterval->I1; eepromValue[eepromAddIndex++] = setInterval->P1;
    eepromValue[eepromAddIndex++] = setInterval->I2; eepromValue[eepromAddIndex++] = setInterval->P2;
    eepromValue[eepromAddIndex++] = setInterval->I3; eepromValue[eepromAddIndex++] = setInterval->P3;
    eepromValue[eepromAddIndex++] = setInterval->I4; eepromValue[eepromAddIndex++] = setInterval->P4;
    eepromValue[eepromAddIndex++] = setInterval->I5; eepromValue[eepromAddIndex++] = setInterval->P5;
    eepromValue[eepromAddIndex++] = setInterval->I6; eepromValue[eepromAddIndex++] = setInterval->P6;
    
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_INTERVAL_DATA-1);
    if(status == EEPROM_OK)
    {
      setInterval->I1 = eepromValue[eepromAddIndex++]; setInterval->P1 = eepromValue[eepromAddIndex++];
      setInterval->I2 = eepromValue[eepromAddIndex++]; setInterval->P2 = eepromValue[eepromAddIndex++];
      setInterval->I3 = eepromValue[eepromAddIndex++]; setInterval->P3 = eepromValue[eepromAddIndex++];
      setInterval->I4 = eepromValue[eepromAddIndex++]; setInterval->P4 = eepromValue[eepromAddIndex++];
      setInterval->I5 = eepromValue[eepromAddIndex++]; setInterval->P5 = eepromValue[eepromAddIndex++];
      setInterval->I6 = eepromValue[eepromAddIndex++]; setInterval->P6 = eepromValue[eepromAddIndex++];
     
    }
  }
  
  return status;
}


eEepromStatus EepromSetContinue(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_CONTINUE_STARTADD;
  stContinue *setContinueValue = GetSetContinueLimits();
  float eepromValue[JSON_NO_OF_CONTINUE-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setContinueValue->Continue1;
   // eepromValue[eepromAddIndex++] = setContinueValue->Count1;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_CONTINUE-1);
    if(status == EEPROM_OK)
    {
      setContinueValue->Continue1  =  eepromValue[eepromAddIndex++];
      //setContinueValue->Count1  =  eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromSetReturnState(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RETURN_STARTADD;
  stReturn *setReturnValue = GetSetReturnLimits();
  float eepromValue[JSON_NO_OF_RETURN-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setReturnValue->Return1;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_RETURN-1);
    if(status == EEPROM_OK)
    {
      setReturnValue->Return1  =  eepromValue[eepromAddIndex++];
      
    }
  }
  return status; 
}


eEepromStatus EepromSetCommDistance(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RFID_RUN_ADDRESS;
 // stReturn *setReturnValue = GetSetReturnLimits();
  strunrfid* setrunrfid = GetSetrunrfidLimits();
  float eepromValue[JSON_NO_OF_RUNRFID-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setrunrfid->runrfid;
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_RUNRFID-1);
    if(status == EEPROM_OK)
    {
      setrunrfid->runrfid  =  eepromValue[eepromAddIndex++];
      
    }
  }
  return status; 
}

eEepromStatus EepromSetNoofRowday(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROWDAY_STARTADD;
 
//  stInterval *setInterval = GetSetIntervalLimits();
  stnoofrows* setNoofrows = GetSetNoofrowsLimits();
  
  float eepromValue[JSON_NO_OF_ROWS-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<7;i++)
    {
      eepromValue[eepromAddIndex++] = setNoofrows->Row[i];
    }
     eepromValue[eepromAddIndex++] = setNoofrows->TRow;
//    eepromValue[eepromAddIndex++] = setNoofrows->R1; eepromValue[eepromAddIndex++] = setNoofrows->R2;
//    eepromValue[eepromAddIndex++] = setNoofrows->R3; eepromValue[eepromAddIndex++] = setNoofrows->R4;
//    eepromValue[eepromAddIndex++] = setNoofrows->R5; eepromValue[eepromAddIndex++] = setNoofrows->R6;
//    eepromValue[eepromAddIndex++] = setNoofrows->R7; 
    
//    eepromValue[eepromAddIndex++] = setInterval->P4;
//    eepromValue[eepromAddIndex++] = setInterval->I5; eepromValue[eepromAddIndex++] = setInterval->P5;
//    eepromValue[eepromAddIndex++] = setInterval->I6; eepromValue[eepromAddIndex++] = setInterval->P6;
    
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, JSON_NO_OF_ROWS-1);
    if(status == EEPROM_OK)
    {
      for(int i=0;i<7;i++)
      {
        setNoofrows->Row[i] = eepromValue[eepromAddIndex++];
      }
        setNoofrows->TRow = eepromValue[eepromAddIndex++];
      
//      setNoofrows->R1 = eepromValue[eepromAddIndex++]; setNoofrows->R2 = eepromValue[eepromAddIndex++];
//      setNoofrows->R3 = eepromValue[eepromAddIndex++]; setNoofrows->R4 = eepromValue[eepromAddIndex++];
//      setNoofrows->R5 = eepromValue[eepromAddIndex++]; setNoofrows->R6 = eepromValue[eepromAddIndex++];
//      setNoofrows->R7 = eepromValue[eepromAddIndex++]; 
////      
//      setInterval->P4 = eepromValue[eepromAddIndex++];
//      setInterval->I5 = eepromValue[eepromAddIndex++]; setInterval->P5 = eepromValue[eepromAddIndex++];
//      setInterval->I6 = eepromValue[eepromAddIndex++]; setInterval->P6 = eepromValue[eepromAddIndex++];
//     
    }
  }

}

eEepromStatus EepromStoreRowLength2(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH2_STARTADD;
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
  uint8_t eepromRowLengthCount = MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    //eepromValue[eepromAddIndex++] = setTrackData->trackCount;
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength2[i][FORWARD_COUNT];
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength2[i][REVERSE_COUNT];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromRowLengthCount);
    if(status == EEPROM_OK)
    {
      //setTrackData->trackCount             =  (uint32_t)eepromValue[eepromAddIndex++];
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
       setRowLengthData->rowLength2[i][FORWARD_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
       setRowLengthData->rowLength2[i][REVERSE_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength3(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH3_STARTADD;
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
  uint8_t eepromRowLengthCount = MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    //eepromValue[eepromAddIndex++] = setTrackData->trackCount;
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength3[i][FORWARD_COUNT];
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength3[i][REVERSE_COUNT];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromRowLengthCount);
    if(status == EEPROM_OK)
    {
      //setTrackData->trackCount             =  (uint32_t)eepromValue[eepromAddIndex++];
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
       setRowLengthData->rowLength3[i][FORWARD_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
       setRowLengthData->rowLength3[i][REVERSE_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength4(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH4_STARTADD;
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
  uint8_t eepromRowLengthCount = MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    //eepromValue[eepromAddIndex++] = setTrackData->trackCount;
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength4[i][FORWARD_COUNT];
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength4[i][REVERSE_COUNT];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromRowLengthCount);
    if(status == EEPROM_OK)
    {
      //setTrackData->trackCount             =  (uint32_t)eepromValue[eepromAddIndex++];
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
       setRowLengthData->rowLength4[i][FORWARD_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
       setRowLengthData->rowLength4[i][REVERSE_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength5(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH5_STARTADD;
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
  uint8_t eepromRowLengthCount = MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    //eepromValue[eepromAddIndex++] = setTrackData->trackCount;
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength5[i][FORWARD_COUNT];
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength5[i][REVERSE_COUNT];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromRowLengthCount);
    if(status == EEPROM_OK)
    {
      //setTrackData->trackCount             =  (uint32_t)eepromValue[eepromAddIndex++];
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
       setRowLengthData->rowLength5[i][FORWARD_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
       setRowLengthData->rowLength5[i][REVERSE_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength6(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH2_STARTADD;
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
  uint8_t eepromRowLengthCount = MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    //eepromValue[eepromAddIndex++] = setTrackData->trackCount;
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength6[i][FORWARD_COUNT];
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength6[i][REVERSE_COUNT];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromRowLengthCount);
    if(status == EEPROM_OK)
    {
      //setTrackData->trackCount             =  (uint32_t)eepromValue[eepromAddIndex++];
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
       setRowLengthData->rowLength6[i][FORWARD_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
       setRowLengthData->rowLength6[i][REVERSE_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength7(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH7_STARTADD;
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
  uint8_t eepromRowLengthCount = MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS;
  float eepromValue[MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    //eepromValue[eepromAddIndex++] = setTrackData->trackCount;
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength7[i][FORWARD_COUNT];
      eepromValue[eepromAddIndex++] = setRowLengthData->rowLength7[i][REVERSE_COUNT];
    }
    status = WriteEEPROM (eepromAddress, eepromValue, eepromAddIndex);
  }
  else
  {
    status = ReadEEPROM (eepromAddress, eepromValue, eepromRowLengthCount);
    if(status == EEPROM_OK)
    {
      //setTrackData->trackCount             =  (uint32_t)eepromValue[eepromAddIndex++];
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
       setRowLengthData->rowLength7[i][FORWARD_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
       setRowLengthData->rowLength7[i][REVERSE_COUNT]       =  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

