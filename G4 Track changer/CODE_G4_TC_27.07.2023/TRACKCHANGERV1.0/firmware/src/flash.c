#include "main.h"

#define LENGTH_OF_EACH_STORE            1                         /* One 32 bit variables    */
//#define LENGTH_OF_EACH_STRING_STORE     1                         /* One 32 bit variables    */
//#define EEPROM_VIRTUAL_START_ADDRESS    APP_MEM_ADDR1

#define STORE_MIN_START_ADDRESS      APP_MEM_ADDR1
//#define EEPROM_DEV_INFO_START_ADDRESS   EEPROM_VIRTUAL_START_ADDRESS 
//#define EEPROM_DEV_INFO_END_ADDRESS     (EEPROM_DEV_INFO_START_ADDRESS + ((JSON_NO_OF_SET_DEVICE_INFO-1) * MAX_DEV_INFO_FIELD_LEN * LENGTH_OF_EACH_STRING_STORE)-1)
//#define EEPROM_VALIDITY_START_ADDRESS   EEPROM_DEV_INFO_END_ADDRESS +1
//#define EEPROM_VALIDITY_END_ADDRESS     (EEPROM_VALIDITY_START_ADDRESS + LENGTH_OF_EACH_STORE -1)
////#define EEPROM_VALIDITY_DATA            0x0357                    /*Ensemble Defined data. It can be anything apart from 0xffff */
//#define STORE_MIN_START_ADDRESS         (EEPROM_VALIDITY_END_ADDRESS+1)

#define EEPROM_VALIDITY_DATA            1234


//extern int Autoschcountval;

//enum
//{
//  STORE_MODE_STARTADD                    = STORE_MIN_START_ADDRESS,
//  STORE_PWM_MOTOR_MANUAL_STARTADD        = STORE_MODE_STARTADD + ((JSON_NO_OF_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_PWM_MOTOR_AUTO_STARTADD          = STORE_PWM_MOTOR_MANUAL_STARTADD + ((JSON_NO_OF_PWM_DATA_MOTOR-1) * LENGTH_OF_EACH_STORE),
//  STORE_PULSE_COUNT_STARTADD             = STORE_PWM_MOTOR_AUTO_STARTADD + ((JSON_NO_OF_PWM_DATA_MOTOR-1) * LENGTH_OF_EACH_STORE),
//  STORE_AUTO_SCHEDULE_STARTADD           = STORE_PULSE_COUNT_STARTADD + ((JSON_NO_OF_PULSE_COUNT_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_EDGE_SENSOR_STATE_STARTADD       = STORE_AUTO_SCHEDULE_STARTADD + ((JSON_NO_OF_AUTO_SCHED_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_LOW_BAT_SOC_STARTADD             = STORE_EDGE_SENSOR_STATE_STARTADD + ((JSON_NO_OF_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_TRACK_INFO_STARTADD              = STORE_LOW_BAT_SOC_STARTADD + ((JSON_NO_OF_SOC_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_ROW_LENGTH_STARTADD              = STORE_TRACK_INFO_STARTADD + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_CLEANER_MAC_STARTADD             = STORE_ROW_LENGTH_STARTADD + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_ENC_PULSE_TOLERANCE_STARTADD     = STORE_CLEANER_MAC_STARTADD + ((JSON_NO_OF_CLEANER_MAC_DATA+JSON_NO_OF_CLEANER_MAC_DATA) * LENGTH_OF_EACH_STORE),
//  STORE_TC_CONTROL_MODE_STARTADD         = STORE_ENC_PULSE_TOLERANCE_STARTADD + ((JSON_NO_OF_ENC_PULSE_TOLERANCE_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_RFID_DATA_STARTADD               = STORE_TC_CONTROL_MODE_STARTADD + ((JSON_NO_OF_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_RFID_STATE_STARTADD              =  STORE_RFID_DATA_STARTADD + ((MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_HEARTBEAT_CONFIG_STARTADD        =  STORE_RFID_STATE_STARTADD + ((JSON_NO_OF_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE), 
//  STORE_LOG_INTERVAL_STARTADD            = STORE_HEARTBEAT_CONFIG_STARTADD + ((JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS-1) * LENGTH_OF_EACH_STORE),  //07D change 
//  STORE_WHEEL_DIA_STARTADD               = STORE_LOG_INTERVAL_STARTADD + ((JSON_NO_OF_INTERVAL_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_CYCLE_FREQUENCY_STARTADD         = STORE_WHEEL_DIA_STARTADD + ((JSON_NO_OF_WHEELDIA_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_CONTINUE_STARTADD                = STORE_CYCLE_FREQUENCY_STARTADD + ((JSON_NO_OF_CYCLEFREQUENCY_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_BRUSH_ENABLED_STATE_STARTADD     = STORE_CONTINUE_STARTADD + ((JSON_NO_OF_CONTINUE-1) * LENGTH_OF_EACH_STORE),
//  STORE_RETURN_STARTADD                 =  STORE_BRUSH_ENABLED_STATE_STARTADD + ((JSON_NO_OF_MOTOR_MODE_STATE_DATA-1) * LENGTH_OF_EACH_STORE),
//  STORE_ROWDAY_STARTADD                 = STORE_RETURN_STARTADD + ((JSON_NO_OF_RETURN-1) * LENGTH_OF_EACH_STORE),
//  STORE_ROW_LENGTH2_STARTADD             = STORE_ROWDAY_STARTADD + ((JSON_NO_OF_ROWS-1) * LENGTH_OF_EACH_STORE), 
//  STORE_ROW_LENGTH3_STARTADD                = STORE_ROW_LENGTH2_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_ROW_LENGTH4_STARTADD                = STORE_ROW_LENGTH3_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_ROW_LENGTH5_STARTADD                = STORE_ROW_LENGTH4_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_ROW_LENGTH6_STARTADD                = STORE_ROW_LENGTH5_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_ROW_LENGTH7_STARTADD                = STORE_ROW_LENGTH6_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_TRACK_INFO2_STARTADD                = STORE_ROW_LENGTH7_STARTADD  + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_RFID_DATA2_STARTADD               = STORE_TRACK_INFO2_STARTADD + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_RFID_DATA3_STARTADD               = STORE_RFID_DATA2_STARTADD + ((MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_RFID_RUN_ADDRESS                =  STORE_RFID_DATA3_STARTADD + ((MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_ROW_LENGTH12_STARTADD                =  STORE_RFID_RUN_ADDRESS + ((JSON_NO_OF_RUNRFID-1) * LENGTH_OF_EACH_STORE),
//  STORE_ROW_LENGTH13_STARTADD                = STORE_ROW_LENGTH12_STARTADD  + ((MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_MAX_START_ADDRESS                = STORE_ROW_LENGTH13_STARTADD  + ((MAX_NO_OF_TRACKS) * LENGTH_OF_EACH_STORE),
//  STORE_MAX_ADDRESS                      = STORE_MAX_START_ADDRESS - 1                                   
//}eEepromEndAddress;

enum
{
  STORE_MODE_STARTADD                    = STORE_MIN_START_ADDRESS,
  STORE_PWM_MOTOR_MANUAL_STARTADD        = STORE_MODE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_PWM_MOTOR_AUTO_STARTADD          = STORE_PWM_MOTOR_MANUAL_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_PULSE_COUNT_STARTADD             = STORE_PWM_MOTOR_AUTO_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_AUTO_SCHEDULE_STARTADD           = STORE_PULSE_COUNT_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_EDGE_SENSOR_STATE_STARTADD       = STORE_AUTO_SCHEDULE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_LOW_BAT_SOC_STARTADD             = STORE_EDGE_SENSOR_STATE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_TRACK_INFO_STARTADD              = STORE_LOW_BAT_SOC_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_ROW_LENGTH_STARTADD              = STORE_TRACK_INFO_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_CLEANER_MAC_STARTADD             = STORE_ROW_LENGTH_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_ENC_PULSE_TOLERANCE_STARTADD     = STORE_CLEANER_MAC_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_TC_CONTROL_MODE_STARTADD         = STORE_ENC_PULSE_TOLERANCE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_RFID_DATA_STARTADD               = STORE_TC_CONTROL_MODE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_RFID_STATE_STARTADD              =  STORE_RFID_DATA_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_HEARTBEAT_CONFIG_STARTADD        =  STORE_RFID_STATE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_LOG_INTERVAL_STARTADD            = STORE_HEARTBEAT_CONFIG_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_WHEEL_DIA_STARTADD               = STORE_LOG_INTERVAL_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_CYCLE_FREQUENCY_STARTADD         = STORE_WHEEL_DIA_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_CONTINUE_STARTADD                = STORE_CYCLE_FREQUENCY_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_BRUSH_ENABLED_STATE_STARTADD     = STORE_CONTINUE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_RETURN_STARTADD                  =  STORE_BRUSH_ENABLED_STATE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_ROWDAY_STARTADD                  = STORE_RETURN_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_ROW_LENGTH2_STARTADD             = STORE_ROWDAY_STARTADD +LENGTH_OF_EACH_STORE, 
  STORE_ROW_LENGTH3_STARTADD             = STORE_ROW_LENGTH2_STARTADD  +LENGTH_OF_EACH_STORE,
  STORE_ROW_LENGTH4_STARTADD             = STORE_ROW_LENGTH3_STARTADD  +LENGTH_OF_EACH_STORE,
  STORE_ROW_LENGTH5_STARTADD             = STORE_ROW_LENGTH4_STARTADD  +LENGTH_OF_EACH_STORE,
  STORE_ROW_LENGTH6_STARTADD             = STORE_ROW_LENGTH5_STARTADD  +LENGTH_OF_EACH_STORE,
  STORE_ROW_LENGTH7_STARTADD             = STORE_ROW_LENGTH6_STARTADD  +LENGTH_OF_EACH_STORE,
  STORE_TRACK_INFO2_STARTADD             = STORE_ROW_LENGTH7_STARTADD  +LENGTH_OF_EACH_STORE,
  STORE_RFID_DATA2_STARTADD              = STORE_TRACK_INFO2_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_RFID_DATA3_STARTADD              = STORE_RFID_DATA2_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_RFID_RUN_ADDRESS                 =  STORE_RFID_DATA3_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_ROW_LENGTH12_STARTADD            =  STORE_RFID_RUN_ADDRESS +LENGTH_OF_EACH_STORE,
  STORE_ROW_LENGTH13_STARTADD            = STORE_ROW_LENGTH12_STARTADD  +LENGTH_OF_EACH_STORE,
  STORE_RELAY_INTERVAL_STARTADD           = STORE_ROW_LENGTH13_STARTADD  +LENGTH_OF_EACH_STORE,
  STORE_MAX_START_ADDRESS                = STORE_RELAY_INTERVAL_STARTADD  +LENGTH_OF_EACH_STORE,
  EEPROM_DEV_INFO_START_ADDRESS          = STORE_MAX_START_ADDRESS +LENGTH_OF_EACH_STORE,
  STORE_MAX_ADDRESS                      = STORE_MAX_START_ADDRESS - 1                                   
}eEepromEndAddress;


eEepromStatus InitialiseAlltoDefault(stJsonCommand *jsonCmdParams, uint16_t commandParamsTotalCount);

eEepromStatus InitialiseEepromVariables(void)
{ 
  eEepromStatus status = EEPROM_OK;
  stJsonCommand *jsonCmdParam;
  uint16_t commandParamsTotalCount = GetCommandProcessingParam(&jsonCmdParam);
  memset(Flash_buffer.Temp_Data,Null,sizeof(Flash_buffer.Temp_Data));
  Read_SST_26_VF_016B_Data( APP_MEM_ADDR,4,(uint8_t *)Flash_buffer.Temp_Data);
  char read[4];
  memset(read,Null,sizeof(read));
  memcpy(read,Flash_buffer.Temp_Data,4);
  int read1 = atoi(read);
  
  if(status == EEPROM_OK)
  {
    if(read1 == EEPROM_VALIDITY_DATA)
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
  memset(Flash_buffer.Temp_Data,Null,sizeof(Flash_buffer.Temp_Data));
  Read_SST_26_VF_016B_Data( APP_MEM_ADDR,4,(uint8_t *)Flash_buffer.Temp_Data);
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
  status |= Write_SST_26_VF_016B_Data(APP_MEM_ADDR,(uint8_t *)"1234",4);
  return status;
}


eEepromStatus EepromDataNil(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK; 
  return status;
}

eEepromStatus EepromSetMode(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_MODE_STARTADD;
  eMode setMode = GetOperationMode();
  char eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setMode;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_MODE_STATE_DATA-1,(uint8_t *)eepromValue);
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
  char eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setMode;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_MODE_STATE_DATA-1,(uint8_t *)eepromValue);
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
  char eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setMode;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_MODE_STATE_DATA-1,(uint8_t *)eepromValue);      
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
  char eepromValue[JSON_NO_OF_PWM_DATA_MOTOR_flash-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->accelTime;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm1)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm1)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm1)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm1;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm2)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm2)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm2)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm2;
    eepromValue[eepromAddIndex++] = setPWMParams->decelStartCountDiff;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->decelTime;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm1)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm1)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm1)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm1;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm2)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm2)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm2)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm2;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_PWM_DATA_MOTOR_flash-1,(uint8_t *)eepromValue);     
    if(status == EEPROM_OK)
    {
      setPWMCycleParams->accelTime              =  (uint32_t)eepromValue[eepromAddIndex]<<24;
      setPWMParams->accelTime                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMCycleParams->accelTime              +=  (uint32_t)eepromValue[eepromAddIndex]<<16;
      setPWMParams->accelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMCycleParams->accelTime              +=  (uint32_t)eepromValue[eepromAddIndex]<<8;
      setPWMParams->accelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMCycleParams->accelTime              +=  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->accelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++];

      setPWMCycleParams->steadyPwm1              =  (uint32_t)eepromValue[eepromAddIndex]<<24;
      setPWMParams->steadyPwm1                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMCycleParams->steadyPwm1              +=  (uint32_t)eepromValue[eepromAddIndex]<<16;
      setPWMParams->steadyPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMCycleParams->steadyPwm1              +=  (uint32_t)eepromValue[eepromAddIndex]<<8;
      setPWMParams->steadyPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMCycleParams->steadyPwm1              +=  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->steadyPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++];
      
      setPWMCycleParams->steadyPwm2              =  (uint32_t)eepromValue[eepromAddIndex]<<24;
      setPWMParams->steadyPwm2                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMCycleParams->steadyPwm2              +=  (uint32_t)eepromValue[eepromAddIndex]<<16;
      setPWMParams->steadyPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMCycleParams->steadyPwm2              +=  (uint32_t)eepromValue[eepromAddIndex]<<8;
      setPWMParams->steadyPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMCycleParams->steadyPwm2              +=  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->steadyPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++];

      setPWMCycleParams->decelStartCountDiff     =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->decelStartCountDiff         =  (uint16_t)eepromValue[eepromAddIndex++];
      
      setPWMCycleParams->decelTime              =  (uint32_t)eepromValue[eepromAddIndex]<<24;
      setPWMParams->decelTime                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMCycleParams->decelTime              +=  (uint32_t)eepromValue[eepromAddIndex]<<16;
      setPWMParams->decelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMCycleParams->decelTime              +=  (uint32_t)eepromValue[eepromAddIndex]<<8;
      setPWMParams->decelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMCycleParams->decelTime              +=  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->decelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++];
      
      setPWMCycleParams->approachPwm1              =  (uint32_t)eepromValue[eepromAddIndex]<<24;
      setPWMParams->approachPwm1                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMCycleParams->approachPwm1              +=  (uint32_t)eepromValue[eepromAddIndex]<<16;
      setPWMParams->approachPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMCycleParams->approachPwm1              +=  (uint32_t)eepromValue[eepromAddIndex]<<8;
      setPWMParams->approachPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMCycleParams->approachPwm1              +=  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->approachPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++];
      
      setPWMCycleParams->approachPwm2              =  (uint32_t)eepromValue[eepromAddIndex]<<24;
      setPWMParams->approachPwm2                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMCycleParams->approachPwm2              +=  (uint32_t)eepromValue[eepromAddIndex]<<16;
      setPWMParams->approachPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMCycleParams->approachPwm2              +=  (uint32_t)eepromValue[eepromAddIndex]<<8;
      setPWMParams->approachPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMCycleParams->approachPwm2              +=  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->approachPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++];
    }
  }
  return status;  
}

eEepromStatus EepromHeartbeatConfiguration(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_HEARTBEAT_CONFIG_STARTADD;
  stHeartbeatConfig* storeHeartbeatConfig;
  storeHeartbeatConfig = GetSetHeartbeatConfig();
  char eepromValue[JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->interval_ms)>>24;
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->interval_ms)>>16;
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->interval_ms)>>8;
    eepromValue[eepromAddIndex++] = storeHeartbeatConfig->interval_ms;
    
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->maxReconnectTime)>>24;
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->maxReconnectTime)>>16;
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->maxReconnectTime)>>8;
    eepromValue[eepromAddIndex++] = storeHeartbeatConfig->maxReconnectTime;
    
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS_FLASH-1,(uint8_t *)eepromValue); 
    if(status == EEPROM_OK)
    {
      storeHeartbeatConfig->interval_ms              =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      storeHeartbeatConfig->interval_ms             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      storeHeartbeatConfig->interval_ms             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      storeHeartbeatConfig->interval_ms             +=  (uint32_t)eepromValue[eepromAddIndex++];
      
      storeHeartbeatConfig->maxReconnectTime              =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      storeHeartbeatConfig->maxReconnectTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      storeHeartbeatConfig->maxReconnectTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      storeHeartbeatConfig->maxReconnectTime            +=  (uint32_t)eepromValue[eepromAddIndex++];
    }
  }
  return status;
}
eEepromStatus EepromSetMotorPwmAutoMode(eEpromAccessType eepromOp)
{

  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_PWM_MOTOR_AUTO_STARTADD;
  stRobotPwmParam *setPWMParams = GetSetCycleAutoPwmParameters();
  char eepromValue[JSON_NO_OF_PWM_DATA_MOTOR_flash-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->accelTime;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm1)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm1)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm1)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm1;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm2)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm2)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->steadyPwm2)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm2;
    eepromValue[eepromAddIndex++] = setPWMParams->decelStartCountDiff;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->decelTime;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm1)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm1)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm1)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm1;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm2)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm2)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->approachPwm2)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm2;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_PWM_DATA_MOTOR_flash-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      setPWMParams->accelTime                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->accelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->accelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->accelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->steadyPwm1                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->steadyPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->steadyPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->steadyPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->steadyPwm2                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->steadyPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->steadyPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->steadyPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->decelStartCountDiff         =  (uint16_t)eepromValue[eepromAddIndex++];
      setPWMParams->decelTime                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->decelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->decelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->decelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++];     
      setPWMParams->approachPwm1                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->approachPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->approachPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->approachPwm1                   +=  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->approachPwm2                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->approachPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->approachPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->approachPwm2                   +=  (uint32_t)eepromValue[eepromAddIndex++];
    }
  }
  return status;  
}
eEepromStatus EepromSetPulseCount(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_PULSE_COUNT_STARTADD;
  stCountRange *setCountValues = GetSetCountRange();
  char eepromValue[JSON_NO_OF_PULSE_COUNT_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setCountValues->maxPosCountForPVsetup;
    eepromValue[eepromAddIndex++] = setCountValues->maxNegCountForPVsetup;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_PULSE_COUNT_DATA-1,(uint8_t *)eepromValue);
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
  char eepromValue[JSON_NO_OF_AUTO_SCHED_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setScheduleValues->hour;
    eepromValue[eepromAddIndex++] = setScheduleValues->minute;
    eepromValue[eepromAddIndex++] = Daycount;
    eepromValue[eepromAddIndex++] = trackchangercount; 
    eepromValue[eepromAddIndex++] = rowsActuallyCleaned;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_AUTO_SCHED_DATA-1,(uint8_t *)eepromValue);      
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
  char eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = *setEdgeSensorState;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_MODE_STATE_DATA-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      *setEdgeSensorState             =  (bool)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromSetLowBatSoC(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_LOW_BAT_SOC_STARTADD;
  uint8_t *setLowBatSoC = GetSetMinBattSoc();
  char eepromValue[JSON_NO_OF_SOC_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = *setLowBatSoC;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_SOC_DATA-1,(uint8_t *)eepromValue);
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
  char eepromValue[JSON_NO_OF_TRACKINFO_FLASH_tc-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = (bool)trackData.trackValid[i];
    } 
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = (trackData.trackDistance[i])>>24;
      eepromValue[eepromAddIndex++] = (trackData.trackDistance[i])>>16;
      eepromValue[eepromAddIndex++] = (trackData.trackDistance[i])>>8;
      eepromValue[eepromAddIndex++] = trackData.trackDistance[i];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);    
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_TRACKINFO_FLASH_tc-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        trackData.trackValid[i]       =  (bool)eepromValue[eepromAddIndex];
        eepromAddIndex++;
      }
      
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        trackData.trackDistance[i]                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        trackData.trackDistance[i]                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        trackData.trackDistance[i]                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        trackData.trackDistance[i]                   +=  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreTrackInfo2(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_TRACK_INFO2_STARTADD;
  char eepromValue[JSON_NO_OF_TRACKINFO_FLASH_tc-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = (bool)trackData.trackValid2[i];
    }
    for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      eepromValue[eepromAddIndex++] = (trackData.trackDistance2[i])>>24;
      eepromValue[eepromAddIndex++] = (trackData.trackDistance2[i])>>16;
      eepromValue[eepromAddIndex++] = (trackData.trackDistance2[i])>>8;
      eepromValue[eepromAddIndex++] = trackData.trackDistance2[i];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_TRACKINFO_FLASH_tc-1,(uint8_t *)eepromValue);      
    if(status == EEPROM_OK)
    {
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        trackData.trackValid2[i]       =  (bool)eepromValue[eepromAddIndex++];
      }
      for(int i=0;i<MAX_NO_OF_TRACKS;i++)
      {
        trackData.trackDistance2[i]                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        trackData.trackDistance2[i]                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        trackData.trackDistance2[i]                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        trackData.trackDistance2[i]                   +=  (uint32_t)eepromValue[eepromAddIndex++];
      } 
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  if ((Row1value == 1)||(Row1getvalue == 0))
  {
    uint16_t eepromAddress = STORE_ROW_LENGTH_STARTADD;
    char eepromValue[JSON_NO_OF_ROWLEN_FLASH-1];
    uint16_t eepromAddIndex = 0;
    if(eepromOp == EEPROM_WRITE)
    {
      for(int i=0;i<9;i++)
      {
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][FORWARD_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][FORWARD_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][FORWARD_COUNT])>>8;
        eepromValue[eepromAddIndex++] =  rowLengthData.rowLength[i][FORWARD_COUNT];
        
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][REVERSE_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][REVERSE_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][REVERSE_COUNT])>>8;
        eepromValue[eepromAddIndex++] = rowLengthData.rowLength[i][REVERSE_COUNT];
        
      }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
    }
    else
    {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ROWLEN_FLASH-1,(uint8_t *)eepromValue);   
      if(status == EEPROM_OK)
      {
        for(int i=0;i<9;i++)
        {  
        rowLengthData.rowLength[i][FORWARD_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        
        rowLengthData.rowLength[i][REVERSE_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        }
       Row1value = 2; 
      }
    }     
  }
  if (Row1value == 2)
  {
    uint16_t eepromAddress = STORE_ROW_LENGTH12_STARTADD;
    char eepromValue[JSON_NO_OF_ROWLEN_FLASH-1];
    uint16_t eepromAddIndex = 0;
    if(eepromOp == EEPROM_WRITE)
    {
      for(int i=9;i<18;i++)
      {
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][FORWARD_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][FORWARD_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][FORWARD_COUNT])>>8;
        eepromValue[eepromAddIndex++] =  rowLengthData.rowLength[i][FORWARD_COUNT];
        
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][REVERSE_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][REVERSE_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][REVERSE_COUNT])>>8;
        eepromValue[eepromAddIndex++] =  rowLengthData.rowLength[i][REVERSE_COUNT];
      }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
    }
    else
    {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ROWLEN_FLASH-1,(uint8_t *)eepromValue);        
      if(status == EEPROM_OK)
      {
        for(int i=9;i<18;i++)
        {
        rowLengthData.rowLength[i][FORWARD_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        
        rowLengthData.rowLength[i][REVERSE_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        }
        Row1value = 3;
      }
    }
  }
  if (Row1value == 3)
  {
    uint16_t eepromAddress = STORE_ROW_LENGTH13_STARTADD;
    char eepromValue[JSON_NO_OF_ROWLEN_FLASH-1];
    uint16_t eepromAddIndex = 0;
    if(eepromOp == EEPROM_WRITE)
    {
      for(int i=18;i<25;i++)
      {
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][FORWARD_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][FORWARD_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][FORWARD_COUNT])>>8;
        eepromValue[eepromAddIndex++] = rowLengthData.rowLength[i][FORWARD_COUNT];
        
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][REVERSE_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][REVERSE_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength[i][REVERSE_COUNT])>>8;
        eepromValue[eepromAddIndex++] = rowLengthData.rowLength[i][REVERSE_COUNT];
      }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
    }
    else
    {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ROWLEN_FLASH-1,(uint8_t *)eepromValue);        
      if(status == EEPROM_OK)
      {
        for(int i=18;i<25;i++)
        {
            rowLengthData.rowLength[i][FORWARD_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
            rowLengthData.rowLength[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
            rowLengthData.rowLength[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
            rowLengthData.rowLength[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];

            rowLengthData.rowLength[i][REVERSE_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
            rowLengthData.rowLength[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
            rowLengthData.rowLength[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
            rowLengthData.rowLength[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        }
        Row1value = 0;
      }
    }
  }
  return status; 
}
eEepromStatus EepromStoreCleanerMAC(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_CLEANER_MAC_STARTADD;
  unCleanerMAC* setCleanerMAC;
  setCleanerMAC = GetSetCleanerMAC();
  char eepromValue[JSON_NO_OF_CLEANER_MAC_DATA_flash-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_16[0];
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_16[1];
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_16[2];
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_16[3];
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_16[4];
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_16[5];
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_16[6];
    eepromValue[eepromAddIndex++] = setCleanerMAC->cleanerMAC_16[7];
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_CLEANER_MAC_DATA_flash-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      setCleanerMAC->cleanerMAC_16[0]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setCleanerMAC->cleanerMAC_16[1]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setCleanerMAC->cleanerMAC_16[2]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setCleanerMAC->cleanerMAC_16[3]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setCleanerMAC->cleanerMAC_16[4]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setCleanerMAC->cleanerMAC_16[5]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setCleanerMAC->cleanerMAC_16[6]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setCleanerMAC->cleanerMAC_16[7]             =  (uint16_t)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromStoreEncoderPulseTolerance(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ENC_PULSE_TOLERANCE_STARTADD;
  uint32_t *setEncPulseTolerance = GetSetEncoderPulseTolerance();
  char eepromValue[JSON_NO_OF_ENC_PULSE_TOLERANCE_DATA_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = (*setEncPulseTolerance)>>24;
    eepromValue[eepromAddIndex++] = (*setEncPulseTolerance)>>16;
    eepromValue[eepromAddIndex++] = (*setEncPulseTolerance)>>8;
    eepromValue[eepromAddIndex++] = *setEncPulseTolerance;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ENC_PULSE_TOLERANCE_DATA_FLASH-1,(uint8_t *)eepromValue);      
    if(status == EEPROM_OK)
    {
        *setEncPulseTolerance                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        *setEncPulseTolerance                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        *setEncPulseTolerance                 +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        *setEncPulseTolerance                 +=  (uint32_t)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}
eEepromStatus EepromStoreRfidData(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RFID_DATA_STARTADD;
//  stRfidData *setRfidData = GetSetRfidData();
  char eepromValue[JSON_NO_OF_RFIDDATA_FLASH_tc-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<MAX_NO_OF_RLTRACKS;i++)
    {
        eepromValue[eepromAddIndex++] = (storeRfidData.rfidData[i])>>24;
        eepromValue[eepromAddIndex++] = (storeRfidData.rfidData[i])>>16;
        eepromValue[eepromAddIndex++] = (storeRfidData.rfidData[i])>>8;
        eepromValue[eepromAddIndex++] = storeRfidData.rfidData[i];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_RFIDDATA_FLASH_tc-1,(uint8_t *)eepromValue);      
    if(status == EEPROM_OK)
    {
      for(int i=0;i<MAX_NO_OF_RLTRACKS;i++)
      {
        storeRfidData.rfidData[i]                 =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        storeRfidData.rfidData[i]                 +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        storeRfidData.rfidData[i]                 +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        storeRfidData.rfidData[i]                 +=  (uint32_t)eepromValue[eepromAddIndex++];
      }
    }
  }
  return status; 
}

eEepromStatus EepromStoreRfidData2(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RFID_DATA2_STARTADD;
  char eepromValue[JSON_NO_OF_RFIDDATA_FLASH_tc-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    
    for(int i=0;i<MAX_NO_OF_RLTRACKS;i++)
    {
        eepromValue[eepromAddIndex++] = (storeRfidData.rfidData2[i])>>24;
        eepromValue[eepromAddIndex++] = (storeRfidData.rfidData2[i])>>16;
        eepromValue[eepromAddIndex++] = (storeRfidData.rfidData2[i])>>8;
        eepromValue[eepromAddIndex++] = storeRfidData.rfidData2[i];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
    
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_RFIDDATA_FLASH_tc-1,(uint8_t *)eepromValue);      
    if(status == EEPROM_OK)
    {  
      for(int i=0;i<MAX_NO_OF_RLTRACKS;i++)
      {
        storeRfidData.rfidData2[i]                 =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        storeRfidData.rfidData2[i]                 +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        storeRfidData.rfidData2[i]                 +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        storeRfidData.rfidData2[i]                 +=  (uint32_t)eepromValue[eepromAddIndex++];
      }
    }
  }
  return status; 
}

eEepromStatus EepromStoreRfidData3(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RFID_DATA3_STARTADD;
  char eepromValue[JSON_NO_OF_RFIDDATA_FLASH_tc-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    
    for(int i=0;i<5;i++)
    {
        eepromValue[eepromAddIndex++] = (storeRfidData.rfidData3[i])>>24;
        eepromValue[eepromAddIndex++] = (storeRfidData.rfidData3[i])>>16;
        eepromValue[eepromAddIndex++] = (storeRfidData.rfidData3[i])>>8;
        eepromValue[eepromAddIndex++] = storeRfidData.rfidData3[i];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_RFIDDATA_FLASH_tc-1,(uint8_t *)eepromValue);      
    if(status == EEPROM_OK)
    {
      for(int i=0;i<5;i++)
      {
        storeRfidData.rfidData3[i]                 =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        storeRfidData.rfidData3[i]                 +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        storeRfidData.rfidData3[i]                 +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        storeRfidData.rfidData3[i]                 +=  (uint32_t)eepromValue[eepromAddIndex++];
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
    snprintf(&eepromValue[datacount], MAX_DEV_INFO_FIELD_LEN,"%s", devIDInfoValues->devID);
    uint16_t storelen = strlen(&eepromValue[datacount]);
    datacount = datacount+storelen;
    eepromValue[datacount++] = ',';
    snprintf(&eepromValue[datacount], MAX_DEV_INFO_FIELD_LEN,"%s", devIDInfoValues->hwVersion);
    storelen = strlen(&eepromValue[datacount]);
    datacount = datacount+storelen;
    eepromValue[datacount++] = ',';
    snprintf(&eepromValue[datacount], MAX_DEV_INFO_FIELD_LEN,"%s", devIDInfoValues->serialNo);
    storelen = strlen(&eepromValue[datacount]);
    datacount = datacount+storelen;
    eepromValue[datacount++] = '\0';
    for(uint16_t dummyDataIndex = datacount; dummyDataIndex < totalDataCount; dummyDataIndex++)
    {
      eepromValue[dummyDataIndex] = '\0';
    }
     Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,totalDataCount);
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,totalDataCount,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      char *positionInfoStr[JSON_NO_OF_SET_DEVICE_INFO-1];
      uint8_t maxNoStrings=(sizeof positionInfoStr)/4;
      uint8_t totalResulStringCount =  SplitString (eepromValue, positionInfoStr, ',', maxNoStrings );
      if(totalResulStringCount == (JSON_NO_OF_SET_DEVICE_INFO-1))
      {
        snprintf(devIDInfoValues->devID, MAX_DEV_INFO_FIELD_LEN,"%s", positionInfoStr[0]);
        snprintf(devIDInfoValues->hwVersion, MAX_DEV_INFO_FIELD_LEN,"%s", positionInfoStr[1]);
        snprintf(devIDInfoValues->serialNo, MAX_DEV_INFO_FIELD_LEN,"%s", positionInfoStr[2]);
      }
      else
      {
        status = EEPROM_READ_ERROR;
      }
    }
  }
  return status;
}

eEepromStatus EepromSetWheelDia(eEpromAccessType eepromOp)
{ 
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_WHEEL_DIA_STARTADD;
  stWheelDia *setWheelDia = GetSetwheelDiaLimits();
  char eepromValue[JSON_NO_OF_WHEELDIA_DATA-1];
  
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setWheelDia->Dia;
    eepromValue[eepromAddIndex++] = setWheelDia->Pulse;
    eepromValue[eepromAddIndex++] = distance;
   
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_WHEELDIA_DATA-1,(uint8_t *)eepromValue);      
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
  char eepromValue[JSON_NO_OF_CYCLEFREQUENCY_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setCycleFrequency->Cycle;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_CYCLEFREQUENCY_DATA-1,(uint8_t *)eepromValue);      
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
  
  char eepromValue[JSON_NO_OF_INTERVAL_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setInterval->I1; eepromValue[eepromAddIndex++] = setInterval->P1;
    eepromValue[eepromAddIndex++] = setInterval->I2; eepromValue[eepromAddIndex++] = setInterval->P2;
    eepromValue[eepromAddIndex++] = setInterval->I3; eepromValue[eepromAddIndex++] = setInterval->P3;
    eepromValue[eepromAddIndex++] = setInterval->I4; eepromValue[eepromAddIndex++] = setInterval->P4;
    eepromValue[eepromAddIndex++] = setInterval->I5; eepromValue[eepromAddIndex++] = setInterval->P5;
    eepromValue[eepromAddIndex++] = setInterval->I6; eepromValue[eepromAddIndex++] = setInterval->P6;
    
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_INTERVAL_DATA-1,(uint8_t *)eepromValue);      
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
  char eepromValue[JSON_NO_OF_CONTINUE-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setContinueValue->Continue1;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_CONTINUE-1,(uint8_t *)eepromValue);      
    if(status == EEPROM_OK)
    {
      setContinueValue->Continue1  =  eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromSetReturnState(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RETURN_STARTADD;
  stReturn *setReturnValue = GetSetReturnLimits();
  char eepromValue[JSON_NO_OF_RETURN-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setReturnValue->Return1;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_RETURN-1,(uint8_t *)eepromValue);      
    if(status == EEPROM_OK)
    {
      setReturnValue->Return1  =  eepromValue[eepromAddIndex++];  
    }
  }
  return status; 
}


eEepromStatus EepromSetcomdistance(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_RFID_RUN_ADDRESS;
  strunrfid* setrunrfid = GetSetrunrfidLimits();
  char eepromValue[JSON_NO_OF_RUNRFID-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setrunrfid->runrfid;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_RUNRFID-1,(uint8_t *)eepromValue);      
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
  char eepromValue[JSON_NO_OF_ROWS-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<7;i++)
    {
      eepromValue[eepromAddIndex++] = maxNoofrowsLimits.Row[i];
    }
     eepromValue[eepromAddIndex++] = maxNoofrowsLimits.TRow;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ROWS-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      for(int i=0;i<7;i++)
      {
        maxNoofrowsLimits.Row[i] = eepromValue[eepromAddIndex++];
      }
        maxNoofrowsLimits.TRow = eepromValue[eepromAddIndex++];     
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength2(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH2_STARTADD;
  char eepromValue[JSON_NO_OF_ROWLEN_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<9;i++)
    {
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength2[i][FORWARD_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength2[i][FORWARD_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength2[i][FORWARD_COUNT])>>8;
        eepromValue[eepromAddIndex++] =  rowLengthData.rowLength2[i][FORWARD_COUNT];
        
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength2[i][REVERSE_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength2[i][REVERSE_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength2[i][REVERSE_COUNT])>>8;
        eepromValue[eepromAddIndex++] = rowLengthData.rowLength2[i][REVERSE_COUNT];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ROWLEN_FLASH-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      for(int i=0;i<9;i++)
      {
        rowLengthData.rowLength2[i][FORWARD_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength2[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength2[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength2[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        
        rowLengthData.rowLength2[i][REVERSE_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength2[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength2[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength2[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength3(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH3_STARTADD;
  char eepromValue[JSON_NO_OF_ROWLEN_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<9;i++)
    {
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength3[i][FORWARD_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength3[i][FORWARD_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength3[i][FORWARD_COUNT])>>8;
        eepromValue[eepromAddIndex++] =  rowLengthData.rowLength3[i][FORWARD_COUNT];
        
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength3[i][REVERSE_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength3[i][REVERSE_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength3[i][REVERSE_COUNT])>>8;
        eepromValue[eepromAddIndex++] = rowLengthData.rowLength3[i][REVERSE_COUNT];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ROWLEN_FLASH-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      for(int i=0;i<9;i++)
      {
        rowLengthData.rowLength3[i][FORWARD_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength3[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength3[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength3[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        
        rowLengthData.rowLength3[i][REVERSE_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength3[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength3[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength3[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength4(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH4_STARTADD;
  char eepromValue[JSON_NO_OF_ROWLEN_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<9;i++)
    {
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength4[i][FORWARD_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength4[i][FORWARD_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength4[i][FORWARD_COUNT])>>8;
        eepromValue[eepromAddIndex++] =  rowLengthData.rowLength4[i][FORWARD_COUNT];
        
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength4[i][REVERSE_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength4[i][REVERSE_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength4[i][REVERSE_COUNT])>>8;
        eepromValue[eepromAddIndex++] = rowLengthData.rowLength4[i][REVERSE_COUNT];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ROWLEN_FLASH-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      for(int i=0;i<9;i++)
      {
        rowLengthData.rowLength4[i][FORWARD_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength4[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength4[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength4[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        
        rowLengthData.rowLength4[i][REVERSE_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength4[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength4[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength4[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
      }
      
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength5(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH5_STARTADD;
  char eepromValue[JSON_NO_OF_ROWLEN_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<9;i++)
    {
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength5[i][FORWARD_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength5[i][FORWARD_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength5[i][FORWARD_COUNT])>>8;
        eepromValue[eepromAddIndex++] =  rowLengthData.rowLength5[i][FORWARD_COUNT];
        
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength5[i][REVERSE_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength5[i][REVERSE_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength5[i][REVERSE_COUNT])>>8;
        eepromValue[eepromAddIndex++] = rowLengthData.rowLength5[i][REVERSE_COUNT];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ROWLEN_FLASH-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      for(int i=0;i<9;i++)
      {
        rowLengthData.rowLength5[i][FORWARD_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength5[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength5[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength5[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        
        rowLengthData.rowLength5[i][REVERSE_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength5[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength5[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength5[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
      } 
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength6(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH2_STARTADD;
  char eepromValue[JSON_NO_OF_ROWLEN_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<9;i++)
    {
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength6[i][FORWARD_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength6[i][FORWARD_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength6[i][FORWARD_COUNT])>>8;
        eepromValue[eepromAddIndex++] =  rowLengthData.rowLength6[i][FORWARD_COUNT];
        
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength6[i][REVERSE_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength6[i][REVERSE_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength6[i][REVERSE_COUNT])>>8;
        eepromValue[eepromAddIndex++] = rowLengthData.rowLength6[i][REVERSE_COUNT];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ROWLEN_FLASH-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      for(int i=0;i<9;i++)
      {
        rowLengthData.rowLength6[i][FORWARD_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength6[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength6[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength6[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        
        rowLengthData.rowLength6[i][REVERSE_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength6[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength6[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength6[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
      } 
    }
  }
  return status; 
}

eEepromStatus EepromStoreRowLength7(eEpromAccessType eepromOp)
{
    eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_ROW_LENGTH7_STARTADD;
  char eepromValue[JSON_NO_OF_ROWLEN_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    for(int i=0;i<9;i++)
    {
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength7[i][FORWARD_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength7[i][FORWARD_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength7[i][FORWARD_COUNT])>>8;
        eepromValue[eepromAddIndex++] =  rowLengthData.rowLength7[i][FORWARD_COUNT];
        
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength7[i][REVERSE_COUNT])>>24;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength7[i][REVERSE_COUNT])>>16;
        eepromValue[eepromAddIndex++] = (rowLengthData.rowLength7[i][REVERSE_COUNT])>>8;
        eepromValue[eepromAddIndex++] = rowLengthData.rowLength7[i][REVERSE_COUNT];
    }
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_ROWLEN_FLASH-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      for(int i=0;i<9;i++)
      {
        rowLengthData.rowLength7[i][FORWARD_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength7[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength7[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength7[i][FORWARD_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
        
        rowLengthData.rowLength7[i][REVERSE_COUNT]                  =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        rowLengthData.rowLength7[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        rowLengthData.rowLength7[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        rowLengthData.rowLength7[i][REVERSE_COUNT]                  +=  (uint32_t)eepromValue[eepromAddIndex++];
      }
    }
  }
  return status; 
}

eEepromStatus EepromSetrelaytime(eEpromAccessType eepromOp)
  {
    eEepromStatus status = EEPROM_OK;
    uint16_t eepromAddress = STORE_RELAY_INTERVAL_STARTADD; 
    strelay *setrelay = GetSetrelayLimits();
    char eepromValue[JSON_NO_OF_RELAY_DATA-1];
    uint16_t eepromAddIndex = 0;
    if(eepromOp == EEPROM_WRITE)
    {
      eepromValue[eepromAddIndex++] = setrelay->T1;
      
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);
    }
    else
    {
      memset(eepromValue,Null,sizeof(eepromValue));
      Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_RELAY_DATA-1,(uint8_t *)eepromValue);
      if(status == EEPROM_OK)
      {
        setrelay->T1 = eepromValue[eepromAddIndex++]; 
      }
    }
    
    return status;
  }

