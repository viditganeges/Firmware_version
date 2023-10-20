#include "main.h"

static uint8_t txBuff[MAX_UART_TX_BUFFER_LENGTH];
static uint8_t txAckBuff[MAX_ACK_TX_BUFFER_LENGTH];

static eJsonStatus AssignJsonCommand (uint16_t  presentCommand_Hex); 
static eJsonStatus ProcessJsonCommand (const uint8_t * jsonString, uint16_t presentCommand_Hex);
static eJsonStatus ParseRxdata(stMessage* message);
static const stJsonCommand * presentJsonCommandParams = NULL; 

static eJsonStatus ProcessSetMode (const char * jsonString);
static eJsonStatus ProcessGetMode (const char * jsonString);
static eJsonStatus ProcessSetBrushPWMManual (const char * jsonString);
static eJsonStatus ProcessGetBrushMotorPWMManual (const char * jsonString);
static eJsonStatus ProcessSetBrushPWMAuto (const char * jsonString);
static eJsonStatus ProcessGetBrushMotorPWMAuto (const char * jsonString);
static eJsonStatus ProcessSetBrushDirection (const char * jsonString);
static eJsonStatus ProcessGetBrushMotorPolarity (const char * jsonString);
static eJsonStatus ProcessSetMotorPWMManual (const char * jsonString);
static eJsonStatus ProcessGetMotorPWMManual (const char * jsonString);
static eJsonStatus ProcessSetMotorPWMAuto (const char * jsonString);
static eJsonStatus ProcessGetMotorPWMAuto (const char * jsonString);
static eJsonStatus ProcessSetManualDirection (const char * jsonString);
static eJsonStatus ProcessGetMotorDirectionManual (const char * jsonString);
static eJsonStatus ProcessSetBrushMotorState (const char * jsonString);
static eJsonStatus ProcessGetBrushMotorEnabledState (const char * jsonString);
static eJsonStatus ProcessSetAutoScheduleTime (const char * jsonString);
static eJsonStatus ProcessGetAutoScheduleTime (const char * jsonString);
static eJsonStatus ProcessSetEdgeSensorState (const char * jsonString);
static eJsonStatus ProcessGetEdgeSenseEnabledState (const char * jsonString);
static eJsonStatus ProcessSetRTCValue (const char * jsonString);
static eJsonStatus ProcessGetRTCValue (const char * jsonString);

static eJsonStatus ProcessSetDeviceInfo (const char * jsonString);
static eJsonStatus ProcessGetDeviceInfo (const char * jsonString);

static eJsonStatus ProcessSetMotorCurrentLimits (const char * jsonString);
static eJsonStatus ProcessGetOverCurrentLimits (const char * jsonString);
static eJsonStatus ProcessSetLowBatSoC (const char * jsonString);
static eJsonStatus ProcessGetLowBatSoC (const char * jsonString);
static eJsonStatus ProcessSetPulseCount (const char * jsonString);
static eJsonStatus ProcessGetPulseCountValues (const char * jsonString);
static eJsonStatus ProcessSetMotorFaultConditions (const char * jsonString);
static eJsonStatus ProcessGetMotorFaultConditions (const char * jsonString);
static eJsonStatus ProcessSetHeartbeatConfig (const char * jsonString);
static eJsonStatus ProcessGetHeartbeatConfig (const char * jsonString);
static eJsonStatus ProcessGetErrorStatus (const char * jsonString);
static eJsonStatus ProcessGetCurrentStat (const char * jsonString);
static eJsonStatus ProcessGetLastOperationStat (const char * jsonString);
static eJsonStatus ProcessSetWheelDia (const char * jsonString);
static eJsonStatus ProcessGetWheelDia (const char * jsonString);
static eJsonStatus ProcessSetLogInterval (const char * jsonString);
static eJsonStatus ProcessGetLogInterval (const char * jsonString);
static eJsonStatus ProcessSetCycleFrequency (const char * jsonString);
static eJsonStatus ProcessGetCycleFrequency (const char * jsonString);
static eJsonStatus ProcessSetContinue (const char * jsonString);
static eJsonStatus ProcessGetContinueCount (const char * jsonString);
static eJsonStatus ProcessSetReturnState (const char * jsonString);
static eJsonStatus ProcessGetReturnState (const char * jsonString);
static eJsonStatus ProcessSetcomdistance (const char * jsonString);
static eJsonStatus ProcessGetcomdistance (const char * jsonString);
static eJsonStatus ProcessSetTrackchangerMAC (const char * jsonString);
static eJsonStatus ProcessGetTrackchangerMAC (const char * jsonString);
static eJsonStatus ProcessSetRelaytime (const char * jsonString);
static eJsonStatus ProcessGetRelaytime (const char * jsonString);
static eJsonStatus ProcessSetMotordriveState (const char * jsonString);
static eJsonStatus ProcessGetMotordriveState (const char * jsonString);
static eJsonStatus ProcessSetMotordriveEnabledState (const char * jsonString, bool* tempSetBool1, bool* tempSetBool2,bool* tempSetBool3);
static eJsonStatus ProcessClearErrorCodes (const char * jsonString);
static eJsonStatus ProcessGetBatteryAndPVvalues (const char * jsonString);
static eJsonStatus ProcessGetTemperaturevalues (const char * jsonString);
static eJsonStatus ProcessGetCurrentRobotStat (const char * jsonString);
static eJsonStatus RequestLog (const char * jsonString);
static eJsonStatus ProcessResetToDefaults (const char * jsonString);
static eJsonStatus ProcessSetPulseCountAndCycle (const char * jsonString);
static eJsonStatus ProcessGetMotorCurrentValues (const char * jsonString);
static eJsonStatus RequestActivityLog (const char * jsonString);
static eJsonStatus ProcessSetState (const char * jsonString, bool* tempSetBool);
static eJsonStatus ProcessSetMotorState (const char * jsonString, bool* tempSetBool1, bool* tempSetBool2);   
static eJsonStatus ProcessSetSensorsState (const char * jsonString, bool* tempSetBool1, bool* tempSetBool2);    
static eJsonStatus ProcessSetMotorPwm (const char * jsonString, stRobotPwmParam *tempSetPwmParam);
static eJsonStatus ProcessSetBrushPwm (const char * jsonString, stBrushPwmParam *tempSetPwmParam);
static bool ParseHeaderData(uint8_t* tempBuff, uint16_t* headerBufferLength, uint16_t command );

eJsonStatus motor_error_code;
eJsonStatus linear_error_code;
eJsonStatus drive1_error_code;
eJsonStatus drive2_error_code;
eJsonStatus drive3_error_code;
eJsonStatus edgesense_error_code;
eJsonStatus rotatesense_error_code;

extern eEepromStatus InitialiseAlltoDefault(stJsonCommand *jsonCmdParams, uint16_t commandParamsTotalCount);

typedef bool (*CheckValidity_Ptr_float)(float value);
typedef bool (*CheckValidity_Ptr_int)(uint32_t value);
typedef bool (*CheckValidity_Ptr_double)(double value);
typedef bool (*CheckValidity_Ptr_bool)(bool value);

static bool  SetParameterValue_bool(const char * inputString, bool * number, CheckValidity_Ptr_bool ExecuteCheckValidity);
static bool  SetParameterValue_int(const char * inputString, uint32_t * number, CheckValidity_Ptr_int ExecuteCheckValidity);
static bool  SetParameterValue_float(const char * inputString, float * number, CheckValidity_Ptr_float ExecuteCheckValidity);
//static bool  SetParameterValue_double(const char * inputString, double * number, CheckValidity_Ptr_double ExecuteCheckValidity);

stJsonCommand jsonCommandParams[] = {
  {CMD_NUM_SET_MODE,                            CMD_SET_MODE,                   ProcessSetMode,                 EepromSetMode,               SetDefaultOperationMode},
  {CMD_NUM_GET_MODE,                            CMD_GET_MODE,                   ProcessGetMode,                 EepromDataNil,               SetDefaultNil},
  {CMD_NUM_SET_MOTOR_DIR_MANUAL,                CMD_SET_MOTOR_DIR_MANUAL,       ProcessSetManualDirection,      EepromDataNil,               SetDefaultNil},
  {CMD_NUM_GET_MOTOR_DIR_MANUAL,                CMD_GET_MOTOR_DIR_MANUAL,       ProcessGetMotorDirectionManual, EepromDataNil,               SetDefaultNil},  
  {CMD_NUM_SET_MOTOR_PWM_MANUAL,                CMD_SET_MOTOR_PWM_MANUAL,       ProcessSetMotorPWMManual,       EepromSetMotorPwmManualMode,     SetDefaultPwmManulModeParameter},
  {CMD_NUM_GET_MOTOR_PWM_MANUAL,                CMD_GET_MOTOR_PWM_MANUAL,       ProcessGetMotorPWMManual,       EepromDataNil,                SetDefaultNil},  
  {CMD_NUM_SET_BRUSH_MOTOR_STATE,               CMD_SET_BRUSH_MOTOR_STATE,      ProcessSetBrushMotorState,      EepromSetBrushEnabledState,     SetDefaultBrushEnabledState},   
  {CMD_NUM_GET_BRUSH_MOTOR_STATE,               CMD_GET_BRUSH_MOTOR_STATE,      ProcessGetBrushMotorEnabledState,      EepromDataNil,           SetDefaultNil},
  {CMD_NUM_SET_BRUSH_MOTOR_PWM_MANUAL,          CMD_SET_BRUSH_MOTOR_PWM_MANUAL, ProcessSetBrushPWMManual,       EepromSetBrushPwmManualMode,     SetDefaultBrushPwmNormalModeParameter},
  {CMD_NUM_GET_BRUSH_MOTOR_PWM_MANUAL,          CMD_GET_BRUSH_MOTOR_PWM_MANUAL, ProcessGetBrushMotorPWMManual,  EepromDataNil,                   SetDefaultNil},  
  {CMD_NUM_SET_BRUSH_MOTOR_DIR,                 CMD_SET_BRUSH_MOTOR_DIR,        ProcessSetBrushDirection,       EepromSetBrushMotorPolarity,     SetDefaultBrushMotPolarity},
  {CMD_NUM_GET_BRUSH_MOTOR_DIR,                 CMD_GET_BRUSH_MOTOR_DIR,        ProcessGetBrushMotorPolarity,   EepromDataNil,                    SetDefaultNil},  
  {CMD_NUM_SET_MOTOR_PWM_AUTO,                  CMD_SET_MOTOR_PWM_AUTO,         ProcessSetMotorPWMAuto,         EepromSetMotorPwmAutoMode,      SetDefaultPwmCycleAutoModeParameter},
  {CMD_NUM_GET_MOTOR_PWM_AUTO,                  CMD_GET_MOTOR_PWM_AUTO,         ProcessGetMotorPWMAuto,         EepromDataNil,                  SetDefaultNil},
  {CMD_NUM_SET_BRUSH_MOTOR_PWM_AUTO,            CMD_SET_BRUSH_MOTOR_PWM_AUTO,   ProcessSetBrushPWMAuto,         EepromSetBrushPwmAutoMode,      SetDefaultBrushPwmAutoModeParameter},
  {CMD_NUM_GET_BRUSH_MOTOR_PWM_AUTO,            CMD_GET_BRUSH_MOTOR_PWM_AUTO,   ProcessGetBrushMotorPWMAuto,    EepromDataNil,                  SetDefaultNil},
  {CMD_NUM_SET_AUTO_SCHEDULE_TIME,              CMD_SET_AUTO_SCHEDULE_TIME,     ProcessSetAutoScheduleTime,     EepromSetAutoScheduleTime,      SetDefaultAutoScheduledTime},
  {CMD_NUM_GET_AUTO_SCHEDULE_TIME,              CMD_GET_AUTO_SCHEDULE_TIME,     ProcessGetAutoScheduleTime,     EepromDataNil,                  SetDefaultNil},
  {CMD_NUM_SET_EDGE_SENSE_STATE,                CMD_SET_EDGE_SENSE_STATE,       ProcessSetEdgeSensorState,      EepromSetEdgeSensorState,      SetDefaultEdgeSensorEnabledState},
  {CMD_NUM_GET_EDGE_SENSE_STATE,                CMD_GET_EDGE_SENSE_STATE,       ProcessGetEdgeSenseEnabledState,       EepromDataNil,           SetDefaultNil},
  {CMD_NUM_SET_RTC_VALUE,                       CMD_SET_RTC_VALUE,              ProcessSetRTCValue,             EepromDataNil,                 SetDefaultNil},
  {CMD_NUM_GET_RTC_VALUE,                       CMD_GET_RTC_VALUE,              ProcessGetRTCValue,             EepromDataNil,                   SetDefaultNil},
  {CMD_NUM_SET_OVERCURRENT_LIMIT,               CMD_SET_OVERCURRENT_LIMIT,      ProcessSetMotorCurrentLimits,   EepromSetCurrentLimits,        SetDefaultMaxMotCurrentLimits},
  {CMD_NUM_GET_OVERCURRENT_LIMIT,               CMD_GET_OVERCURRENT_LIMIT,      ProcessGetOverCurrentLimits,    EepromDataNil,                SetDefaultNil},  
  {CMD_NUM_SET_PULSE_COUNT_VALUES,              CMD_SET_PULSE_COUNT_VALUES,     ProcessSetPulseCount,           EepromSetPulseCount,          SetDefaultCountRange}, 
  {CMD_NUM_GET_PULSE_COUNT_VALUES,              CMD_GET_PULSE_COUNT_VALUES,     ProcessGetPulseCountValues,     EepromDataNil,               SetDefaultNil},
  {CMD_NUM_SET_LOW_BAT_SOC,                     CMD_SET_LOW_BAT_SOC,            ProcessSetLowBatSoC,            EepromSetLowBatSoC,           SetDefaultMinBatSoc},
  {CMD_NUM_GET_LOW_BAT_SOC,                     CMD_GET_LOW_BAT_SOC,            ProcessGetLowBatSoC,            EepromDataNil,                 SetDefaultNil},
  {CMD_NUM_SET_HEARTBEAT_CONFIG,                CMD_SET_HEARTBEAT_CONFIG,       ProcessSetHeartbeatConfig,      EepromHeartbeatConfiguration,SetDefaultHeartbeatConfig},  
  {CMD_NUM_GET_HEARTBEAT_CONFIG,                CMD_GET_HEARTBEAT_CONFIG,       ProcessGetHeartbeatConfig,      EepromDataNil,              SetDefaultNil}, 
  {CMD_NUM_SET_MOTOR_FAULT_CONDITION,           CMD_SET_MOTOR_FAULT_CONDITIONS, ProcessSetMotorFaultConditions, EepromSetMotorFaultConditions, SetDefaultIloadConfigValues},
  {CMD_NUM_GET_MOTOR_FAULT_CONDITION,           CMD_GET_MOTOR_FAULT_CONDITIONS, ProcessGetMotorFaultConditions, EepromDataNil,              SetDefaultNil},  
  {CMD_NUM_SET_LOG_INTERVAL,                    CMD_SET_LOG_INTERVAL,           ProcessSetLogInterval,          EepromSetLogInterval,       SetDefaultLogInterval},      
  {CMD_NUM_GET_LOG_INTERVAL,                    CMD_GET_LOG_INTERVAL,           ProcessGetLogInterval,          EepromDataNil,              SetDefaultNil}, 
  {CMD_NUM_SET_DIA_WHEEL,                       CMD_SET_DIA_WHEEL,              ProcessSetWheelDia,             EepromSetWheelDia,          SetDefaultWheelDia},        
  {CMD_NUM_GET_DIA_WHEEL,                       CMD_GET_DIA_WHEEL,              ProcessGetWheelDia,             EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_CYCLE_FREQUENCY,                 CMD_SET_CYCLE_FREQUENCY,        ProcessSetCycleFrequency,       EepromSetCycleFrequency,    SetDefaultCycleFrequency},   
  {CMD_NUM_GET_CYCLE_FREQUENCY,                 CMD_GET_CYCLE_FREQUENCY,        ProcessGetCycleFrequency,       EepromDataNil,              SetDefaultNil},   
  {CMD_NUM_SET_CONTINUE_COUNT,                  CMD_SET_RETURN_STATE,           ProcessSetContinue,             EepromSetContinue,          SetDefaultContinue},   
  {CMD_NUM_GET_CONTINUE_COUNT,                  CMD_GET_RETURN_STATE,           ProcessGetContinueCount,        EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_EMERGENCY_RETURN,                CMD_SET_CONTINUE_COUNT,         ProcessSetReturnState,          EepromSetReturnState,       SetDefaultReturnState},   
  {CMD_NUM_GET_RETURN_STATE,                    CMD_GET_CONTINUE_COUNT,         ProcessGetReturnState,          EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_COM_DISTANCE,                    CMD_SET_COM_DISTANCE,           ProcessSetcomdistance,          EepromSetcomdistance,       SetDefaultcomdistance},   
  {CMD_NUM_GET_COM_DISTANCE,                    CMD_GET_COM_DISTANCE,           ProcessGetcomdistance,          EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_DEVICE_INFO,                     CMD_SET_DEVICE_INFO,            ProcessSetDeviceInfo,           EepromDevIDInfo,               SetDefaultDevIDInfo},  
  {CMD_NUM_GET_DEVICE_INFO,                     CMD_GET_DEVICE_INFO,            ProcessGetDeviceInfo,           EepromDataNil,                SetDefaultNil},
  {CMD_NUM_SET_PULSE_COUNT_AND_CYCLE,           CMD_SET_PULSE_COUNT_AND_CYCLE,  ProcessSetPulseCountAndCycle,   EepromDataNil,                 SetDefaultNil},  
  {CMD_NUM_CLEAR_ERRORCODES,                    CMD_CLEAR_ERRORCODES,           ProcessClearErrorCodes,         EepromDataNil,               SetDefaultNil},  
  {CMD_NUM_RESET_TO_DEFAULTS,                   CMD_RESET_TO_DEFAULTS,          ProcessResetToDefaults,         EepromDataNil,              SetDefaultNil},  
  {CMD_NUM_GET_CURRENT_STAT,                    CMD_GET_CURRENT_STAT,           ProcessGetCurrentStat,          EepromDataNil,                SetDefaultNil},
  {CMD_NUM_GET_LAST_CYCLE_STAT,                 CMD_GET_LAST_CYCLE_STAT,        ProcessGetLastOperationStat,    EepromDataNil,                SetDefaultNil},
  {CMD_NUM_GET_MOT_CURRENT_VALUES,              CMD_GET_MOT_CURRENT_VALUES,     ProcessGetMotorCurrentValues,   EepromDataNil,                SetDefaultNil},
  {CMD_NUM_GET_CURRENT_ROBOT_STATUS,            CMD_GET_CURRENT_ROBOT_STATUS,   ProcessGetCurrentRobotStat,     EepromDataNil,                SetDefaultNil},   
  {CMD_NUM_GET_CONTROLLER_STATUS,               CMD_GET_CONTROLLER_STATUS,      ProcessGetErrorStatus,          EepromDataNil,                 SetDefaultNil},
  {CMD_NUM_GET_BAT_AND_PV_VAL,                  CMD_GET_BAT_AND_PV_VAL,         ProcessGetBatteryAndPVvalues,   EepromDataNil,              SetDefaultNil},
  {CMD_NUM_GET_TEMPERATURES,                    CMD_GET_TEMPERATURES,           ProcessGetTemperaturevalues,    EepromDataNil,              SetDefaultNil},   
  {CMD_NUM_REQ_ACTIVITY_LOG,                    CMD_REQ_ACTIVITY_LOG,           RequestActivityLog,             EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_DRIVE_STATE,                     CMD_SET_DRIVE_STATE,            ProcessSetMotordriveState,      EepromSetdriveEnabledState,              SetDefaultdriveEnabledState},
  {CMD_NUM_GET_DRIVE_STATE,                     CMD_GET_DRIVE_STATE,            ProcessGetMotordriveState,      EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_CHANGER_MAC,                     CMD_SET_CHANGER_MAC,            ProcessSetTrackchangerMAC,      EepromStoreTrackchangerMAC,              SetDefaultNil},
  {CMD_NUM_GET_CHANGER_MAC,                     CMD_GET_CHANGER_MAC,            ProcessGetTrackchangerMAC,      EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_RELAY_INTERVAL,                  CMD_SET_RELAY_INTERVAL,         ProcessSetRelaytime,            EepromSetrelaytime,       SetDefaultrelaytime},      
  {CMD_NUM_GET_RELAY_INTERVAL,                  CMD_GET_RELAY_INTERVAL,         ProcessGetRelaytime,            EepromDataNil,              SetDefaultNil},  
  {CMD_NUM_REQ_LOG,                             CMD_REQ_LOG,                    RequestLog,                     EepromDataNil,              SetDefaultNil}, 
};

char Cstarttime[40]={'0'};
char Cstartsoc[5]={'0'};
char Cendtime[40]={'0'};
char Cendsoc[5]={'0'};
char Cerrorcount[5]={'0'};


void ClearTxBuff(void)
{
  for(int i =0;i<MAX_UART_TX_BUFFER_LENGTH;i++)
  {
    txBuff[i] = 0;
  }
}

char* GetAcknowledgementData (eJsonStatus errorCode)
{
  uartflag = 3; 
  uint16_t dataCount=0;
  txAckBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txAckBuff)-dataCount;
  ParseHeaderData((uint8_t *)&txAckBuff[dataCount],&bufferLength,CMD_ACK_DATA);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txAckBuff)-dataCount;
  uint8_t tempCount = snprintf((char *)&txAckBuff[dataCount],bufferLength,"%X",errorCode);
  dataCount = dataCount+tempCount; 
  txAckBuff[dataCount] = STOP_CHAR;
  dataCount++;
  txAckBuff[dataCount] = 0;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txAckBuff,dataCount);
  Serial_Debug("\r\nEnd");
  //ClearTxAckBuff();
  return (char *)txAckBuff;
}

void TransmitWelcomeMessage (void)
{
  uartflag = 3;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_DEVCIE_WELCOME_MSG);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%s",DEFAULT_DEVICE_WELCOME_MSG);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
//  ZigbeeTxPacket_Gateway(0,txBuff,dataCount);
//  UartTransmitType(uarttType,(uint8_t*)txBuff, dataCount);
  ClearTxBuff();
}


eJsonStatus ProcessReceivedJsonData (uint8_t * jsonReceivedData,uint16_t size)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  
  ////  const char * Json_String;
  ////  JsonFormatKeys JsonKeys;
  ////  JsonFormatValues JsonValues;
  ////  char JsonValues_Multi[MAX_JSON_VALUES_ONE_TIME][MAX_JSON_VALUES_ONE_TIME_LENGTH+1];
  ////  JsonValues.Value = &JsonValues_Multi;
  ////  JsonValues.Max_Value_Length = MAX_JSON_VALUES_ONE_TIME_LENGTH;
  ////  Json_String = (const char *)jsonReceivedData;
  //error_code = ParseRxdata(jsonReceivedData);
  eJsonStatus parseErrorCode = error_code;
  //uint16_t command = 0;
  stMessage message;
  
  if(uartflag == 1)
  {
    error_code = ExtractData(jsonReceivedData,size,&message);
//    uartflag = 0;
  }
  else if(uartflag == 2)
  {
    error_code = ExtractDatafromzigbee(jsonReceivedData,size,&message);
//    uartflag = 0;
  }
  else 
  {
      uartflag = 0;
  }
  
  if(error_code == JSON_NO_ERROR)
  {
    error_code = ParseRxdata(&message);
  }
  parseErrorCode = error_code;
  if((error_code == JSON_NO_ERROR) || (error_code == JSON_WILDCARD_DEV_ID))
  {
    Serial_Debug((char const *)"\nCMD Value: ");
    Serial_Debug_Num(message.CMD); 
    error_code = ProcessJsonCommand(message.values, message.CMD);//(*JsonValues.Value)[SNO_KEY_TYPE_CMD]);
    if(error_code != JSON_NO_ERROR)
    {
     // Error_Handler();
    }
  }
  else
  {
   // Error_Handler();
  }
  if((parseErrorCode != JSON_WILDCARD_DEV_ID) && (parseErrorCode != JSON_INVALID_DEV_ID))
  {
    char * AckBuffer = GetAcknowledgementData(error_code);
    Serial_Debug(AckBuffer);
    UartTransmit((uint8_t *)AckBuffer,strlen(AckBuffer));
  }
  return error_code;
}

eJsonStatus ExtractData(uint8_t* data,uint8_t size,stMessage* finalMessage)
{
  uint16_t startAddress = 0;
  uint16_t destAddress = 0;

  Serial_Debug("\nExtract Data");
  Serial_Debug("");
  for(startAddress = 0; ((data[startAddress]!=START_CHAR)&&(startAddress<size));startAddress++);
  if(data[startAddress]!=START_CHAR)
  {
    Serial_Debug("START_CHAR not found");
    return JSON_PARSE_ERROR;
  }
  startAddress++;
  uint8_t tempBuffer[100];
  for(destAddress = 0; (data[startAddress]!=DELIMITER_1)&&(startAddress<size);startAddress++,destAddress++)
  {
    
    tempBuffer[destAddress] = data[startAddress];
  }
  if(data[startAddress]!=DELIMITER_1)
  {
    Serial_Debug("DELIMITER1 not found");
    return JSON_PARSE_ERROR;
  }
  tempBuffer[destAddress] = 0;
  Serial_Debug("\nDevice ID-->");
  Serial_Debug((char*)tempBuffer);
  finalMessage->deviceID = strtoull((const char *)tempBuffer,NULL,16);
  startAddress++;
  tempBuffer[0] = 0;
  for(destAddress = 0; (data[startAddress]!=DELIMITER_2)&&(startAddress<size);startAddress++,destAddress++)
  {
    
    tempBuffer[destAddress] = data[startAddress];
  }
  if(data[startAddress]!=DELIMITER_2)
  {
    Serial_Debug("DELIMITER2 not found");
    return JSON_PARSE_ERROR;
  }
  tempBuffer[destAddress] = 0;
  Serial_Debug("\nCommand-->");Serial_Debug((char*)tempBuffer);
  finalMessage->CMD = strtol((const char *)tempBuffer,NULL,16);
  Serial_Debug_Num(finalMessage->CMD);
  startAddress++;
  tempBuffer[0] = 0;
  for(destAddress = 0; (data[startAddress]!=STOP_CHAR)&&(startAddress<size);startAddress++,destAddress++)
  {
    
    tempBuffer[destAddress] = data[startAddress];
  }
  if(data[startAddress]!=STOP_CHAR)
  {
     Serial_Debug("STOP_CHAR not found");
    return JSON_PARSE_ERROR;
  }
  tempBuffer[destAddress] = 0;
   Serial_Debug("\nValues-->");Serial_Debug((char*)tempBuffer);
  snprintf((char *)finalMessage->values,100,"%s",tempBuffer);
  char* tempValues[MAX_NO_OF_VALUES];
  uint8_t totalNoOfStr = SplitString((char *)tempBuffer,tempValues,',',MAX_NO_OF_VALUES);
   Serial_Debug("\r\ntotalNoOfStr-->");
   Serial_Debug_Num(totalNoOfStr);
  if(totalNoOfStr == 0)
  {
    return JSON_INVALID_DATA;
  }
  
  for(int i=0; i<totalNoOfStr;i++)
  {
        Serial_Debug("\r\nValue_");
        Serial_Debug_Num(i+1);
        Serial_Debug("-->");
        Serial_Debug(tempValues[i]);
  }
  finalMessage->CRC = strtol(tempValues[totalNoOfStr-1],NULL,16);
  Serial_Debug("\r\nCRC-->");
  Serial_Debug_Num(finalMessage->CRC);
  return JSON_NO_ERROR;
}

static eJsonStatus ParseRxdata(stMessage* message)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  ////  JsonKeys->Key[SNO_KEY_TYPE_TIME_STAMP] = (char*)KEY_TYPE_TIME_STAMP;              
  ////  JsonKeys->Key[SNO_KEY_TYPE_ID_NO]      = (char*)KEY_TYPE_ID_NO;                  
  ////  JsonKeys->Key[SNO_KEY_TYPE_CMD]        = (char*)KEY_TYPE_CMD;   
  ////  JsonKeys->No_of_Keys = SNO_KEY_TYPE_CMD+1;                                  // Assuming this as the MAXimum enum value used in this assignment
  ////  JsonValues->No_of_Values = JsonKeys->No_of_Keys; 
  ////  if(Parse_Json_Multi(Json_String, JsonKeys, JsonValues) == 0)
  ////  {
  ////    stDevInfo * devInfo = GetSetDeviceIDInfo();
  Serial_Debug("\r\nDevice ID-->");
  Serial_Debug_Num(message->deviceID);
  if(message->deviceID == WILDCARD_DEVICE_ID)
  {
    Serial_Debug("\n Wild Card Device ID");
    error_code = JSON_WILDCARD_DEV_ID;
  }
  else if(message->deviceID == DEVICE_ID)
  {
    error_code = JSON_NO_ERROR;
  }
  else 
  {
    Serial_Debug("\n Invalid Device ID");
     error_code = JSON_INVALID_DEV_ID; 
    error_code = JSON_NO_ERROR;
  } 
  return error_code; 
}
static eJsonStatus ProcessJsonCommand (const uint8_t * jsonString, uint16_t presentCommand_Hex) 
{
  eJsonStatus jsonStatus;
  jsonStatus = AssignJsonCommand (presentCommand_Hex); 
  if(presentJsonCommandParams != NULL)
  {
    jsonStatus = presentJsonCommandParams->executeCommand((const char *)jsonString);
    if(jsonStatus == JSON_NO_ERROR)
    {
      eEepromStatus eepromStoreStatus = presentJsonCommandParams->executeStoreProcess(EEPROM_WRITE);
      if(eepromStoreStatus != EEPROM_OK)
      {
        jsonStatus = JSON_EEPROM_ERROR;
        Serial_Debug("\nEeprom Error");
      }
      else
      {
        Serial_Debug("\nEeprom Stored");
      }
    }
  }
  
  return jsonStatus;
}

static eJsonStatus AssignJsonCommand (uint16_t presentCommand_Hex) 
{
  uint16_t commandParamsCount = 0;
  uint16_t commandParamsTotalCount = sizeof(jsonCommandParams)/sizeof(jsonCommandParams[0]);
  for(commandParamsCount =0; commandParamsCount < commandParamsTotalCount; commandParamsCount++)
  {
    if(jsonCommandParams[commandParamsCount].command_Hex == presentCommand_Hex)
    {
      presentJsonCommandParams = &jsonCommandParams[commandParamsCount];
      break;
    }
  }
  if(commandParamsCount >= commandParamsTotalCount)
  {
    presentJsonCommandParams = NULL;
    return JSON_INVALID_CMD_VALUE;
  }
  else
  {
    return JSON_NO_ERROR;
  }
}


uint16_t GetCommandProcessingParam (stJsonCommand **jsonCmdParam)
{
  *jsonCmdParam = &jsonCommandParams[0];
  uint16_t commandParamsTotalCount = sizeof(jsonCommandParams)/sizeof(jsonCommandParams[0]);
  return(commandParamsTotalCount);
}

static bool ParseHeaderData(uint8_t* tempBuff, uint16_t* headerBufferLength, uint16_t command )
{
  uint16_t buffCount = 0;
  tempBuff[0] = START_CHAR;
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  uint64_t macAddr = GetZigbeeMacAddress();                                    
  uint16_t maxStrLength = MAC_ID_STR_LEN;
  char macStr[MAC_ID_STR_LEN];
  Int64ToHexString (macAddr, macStr, &maxStrLength);
  uint16_t snData = snprintf((char *)&tempBuff[buffCount],*headerBufferLength,"%02X",DEVICE_ID);
  buffCount = buffCount+snData;
  *headerBufferLength = *headerBufferLength - buffCount;
  tempBuff[buffCount] = DELIMITER_1;
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  snData = snprintf((char *)&tempBuff[buffCount],*headerBufferLength,"%04X",command);
  buffCount = buffCount+snData;
  *headerBufferLength = *headerBufferLength - buffCount;
  tempBuff[buffCount] = DELIMITER_2;
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  *headerBufferLength = buffCount;
  Serial_Debug_Bytes((char*)tempBuff,buffCount);
  Serial_Debug((char*)tempBuff); 
  return true; 
}

static bool  SetParameterValue_float(const char * inputString, float * number, CheckValidity_Ptr_float ExecuteCheckValidity)
{
  if(inputString[0] != 0)
  {
    float convertedNumber = strtof(inputString, 0);
    if(ExecuteCheckValidity(convertedNumber))
    {
      *number = convertedNumber;
      return true;
    }
    else
    {
      return false;
    }
  }   
  else
  {
    return true;                                                               /* return True - Null data but No error in data     */
  }
}

static bool  SetParameterValue_int(const char * inputString, uint32_t * number, CheckValidity_Ptr_int ExecuteCheckValidity)
{
  if(inputString[0] != 0)
  {
    long int convertedNumber = strtol(inputString, 0, 10);
    if(ExecuteCheckValidity(convertedNumber))
    {
      *number = convertedNumber;
      return true;
    }
    else
    {
      return false;
    }
  }   
  else
  {
    return true;                                                               // return True - Null data but No error in data
  }
}

static bool  SetParameterValue_bool(const char * inputString, bool * number, CheckValidity_Ptr_bool ExecuteCheckValidity)
{
  if(inputString[0] != 0)
  {
    bool convertedNumber = strtol(inputString, 0, 10);
    if(ExecuteCheckValidity(convertedNumber))
    {
      *number = convertedNumber;
      return true;
    }
    else
    {
      return false;
    }
  }   
  else
  {
    return true;                                                               // return True - Null data but No error in data
  }
}

//static bool  SetParameterValue_double(const char * inputString, double * number, CheckValidity_Ptr_double ExecuteCheckValidity)
//{
//  if(inputString[0] != 0)
//  {
//    double convertedNumber = strtod(inputString, 0);
//    if(ExecuteCheckValidity(convertedNumber))
//    {
//      *number = convertedNumber;
//      return true;
//    }
//    else
//    {
//      return false;
//    }
//  }   
//  else
//  {
//    return true;                                                               // return True - Null data but No error in data
//  }
//}

static eJsonStatus ProcessSetMode (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetMode->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  bool mode ;
  error_code = ProcessSetState(jsonString, &mode);
  if(error_code == JSON_NO_ERROR)
  {
    if(mode == 0)
    {
      SetMotionCommand(ROBOT_CMD_AUTO);
      SetOperationMode(AUTO,MAN_IDLE);
    }
    else if(mode == 1)
    {
      SetMotionCommand(ROBOT_CMD_MANUAL);
      SetOperationMode(MANUAL,MAN_IDLE);
    }
    Serial_Debug("\r\nMode set as ");
    Serial_Debug_Num(mode);
    Serial_Debug("\r\n0->AUTO, 1->MANUAL ");
  }
  return error_code;
}

static eJsonStatus ProcessSetState (const char * jsonString, bool* tempSetBool)
{
  Serial_Debug("\r\nIn ProcessSetState->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char *setStateStr[JSON_NO_OF_MODE_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_MODE_STATE_DATA) // for zigbee
  {
    bool status = true;
    status &= SetParameterValue_bool(setStateStr[0], tempSetBool, IsStateValid);
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Mode SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Mode SetValue");
    }            
  }
  else
  {
    Serial_Debug("\n Mode SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetMode (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_MODE);
  dataCount = dataCount+bufferLength;   
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = 0; 
  eMode getMode =  GetOperationMode ();
  uint8_t modeValue = 0;
  switch(getMode)
  {
  case AUTO: 
    modeValue = 0;
    break;
  case MANUAL:
    modeValue = 1;
    break;
  default : 
    modeValue = 1;
    break;
  }
  tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d",modeValue);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetManualDirection (const char * jsonString)
{
  char Trackchanger_Error;
  Serial_Debug("\r\nIn ProcessSetManualDirection->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  uint32_t mode;
  char *setStateStr[JSON_NO_OF_MODE_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_MODE_STATE_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setStateStr[0], &mode, IsManualCommandValid);
    
    stContinue *setContinueValue = GetSetContinueLimits();
    stReturn *setReturnValue = GetSetReturnLimits();
    Trackchanger_Error = setReturnValue->Return1;
    
    EepromHeartbeatConfiguration(EEPROM_READ);
      
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Manual Mode SetValue/s Out of Range");
    }
    else if(setContinueValue->Continue1 != 2)
    {
      SetResumeBit(0); 
      switch(mode)
      {
      case 1 : 
      SetCycleModeState(CYCLE_COMPLETED);
      SetMotionPauseState(PA_IDLE);
      ClearRoboterror();
      Cumulative_distance = 0;
      SetMotionCommand(ROBOT_CMD_POSITIVE);
      break;
      case 2 : 
      SetCycleModeState(CYCLE_COMPLETED);
      SetMotionPauseState(PA_IDLE);
      ClearRoboterror();
      Cumulative_distance = 0;
      SetMotionCommand(ROBOT_CMD_NEGATIVE); 
      break;
      case 3 : 
      SetCycleModeState(CYCLE_COMPLETED);
      SetMotionPauseState(PA_IDLE);
      ClearRoboterror();
      ClearRotateSenseCount();      //change
      Cumulative_distance = 0;
      SetMotionCommand(ROBOT_CMD_CYCLE);
      SetCycleModeState(CYCLE_START);   
      SCHEDULE_LOG = 1;
      break;
      case 4 : SetMotionCommand(ROBOT_CMD_NORMAL_STOP);
      break;
      case 5 : 
            Motor1Stop();
            SetMotionCommand(ROBOT_CMD_STOP);
      break;
      case 6 : //SetResumeBit(1);
       ClearAllFaults();
       ClearRoboterror();
      break;
      case 7:
       Robot_Error_Count = ++Trackchanger_Error;
       SetMotionPauseState(PA_PAUSE);
      break;
      default:
        break;
      }
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Manual Mode SetValue : ");
      Serial_Debug_Num(mode);
      Serial_Debug("\r\n0->POSITIVE, 1->NEGATIVE, 2->CYCLE, 3-> NORMAL_STOP, 4-> EM_STOP");
    }  
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Manual Mode SetValue : ");
      Serial_Debug_Num(mode);
      Serial_Debug("\r\n0->POSITIVE, 1->NEGATIVE, 2->CYCLE, 3-> NORMAL_STOP, 4-> EM_STOP");          
  }
  else
  {
    Serial_Debug("\n Manual Mode SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetMotorDirectionManual (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_MOTOR_DIR_MANUAL);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount =0;
  eManualState manualState = GetManualState ();
  uint8_t manualValue = 0;
  switch(manualState)
  {
  case MAN_POSITIVE : 
    manualValue = 1;
    break;
  case MAN_NEGATIVE :
    manualValue = 2;
    break;
  case MAN_CYCLE : 
    manualValue = 3;
    break;
  case MAN_NORMAL_STOP : 
    manualValue = 4;
    break;
  case MAN_STOP : 
    manualValue = 5;
    break;
  default : 
    manualValue = 5;
    break;
  }
  tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d",manualValue);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();

  return error_code;
}

static eJsonStatus ProcessSetMotorPWMManual (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetMotorPWMManual->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  stRobotPwmParam* setPWMParams;
  stRobotPwmParam* setPWMCycleParams;
  setPWMParams = GetSetManualPwmParameters();
  setPWMCycleParams = GetSetCycleManualPwmParameters();
  stRobotPwmParam tempSetPWMParams = *setPWMParams;
  error_code = ProcessSetMotorPwm(jsonString,&tempSetPWMParams);
  if(error_code == JSON_NO_ERROR)
  {
    *setPWMParams = tempSetPWMParams;
    *setPWMCycleParams = tempSetPWMParams;
    char dispstring [100];
    snprintf(dispstring, sizeof dispstring, "\nPwm Manual Mode SetValues: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
             setPWMParams->accelTime, setPWMParams->decelTime,setPWMParams->acelStartCountDiff,setPWMParams->accelapproachPwm1, setPWMParams->accelapproachPwm2, setPWMParams->steadyPwm1, setPWMParams->steadyPwm2,
             setPWMParams->approachPwm1, setPWMParams->approachPwm2,setPWMParams->decelStartCountDiff);
    Serial_Debug(dispstring);
  }
  return error_code;
}
static eJsonStatus ProcessSetMotorPwm (const char * jsonString, stRobotPwmParam *tempSetPwmParam)
{
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  
  char *pwmValuesStr[JSON_NO_OF_SETGET_PWM_DATA_MOTOR];
  uint8_t maxNoStrings=(sizeof pwmValuesStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, pwmValuesStr, ',',maxNoStrings);
  Serial_Debug("\nPWM Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(pwmValuesStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_SETGET_PWM_DATA_MOTOR)
  {
    bool status = true;  
    pulse_flag = 2;  
    Accel_speed = atoi(pwmValuesStr[2]);
    Decel_speed = atoi(pwmValuesStr[6]); 
    PulseCalculation();   
    status &= SetParameterValue_int(pwmValuesStr[0], (uint32_t *)&(tempSetPwmParam->accelTime), IsAccelTimeValid);
    status &= SetParameterValue_int(pwmValuesStr[1], (uint32_t *)&(tempSetPwmParam->decelTime) , IsDecelTimeValid); 
    status &= SetParameterValue_int(pwmValuesStr[2], (uint32_t *)&(tempSetPwmParam->Accel), IsPulseCountValid);   
    status &= SetParameterValue_int(Accel_pulse_count, (uint32_t *)&(tempSetPwmParam->acelStartCountDiff), IsDecelPulseCountDiffValid);   
    status &= SetParameterValue_int(pwmValuesStr[3], (uint32_t *)&(tempSetPwmParam->accelapproachPwm1), IsPWMvalueValid);
    status &= SetParameterValue_int(pwmValuesStr[3], (uint32_t *)&(tempSetPwmParam->accelapproachPwm2), IsPWMvalueValid);    
    status &= SetParameterValue_int(pwmValuesStr[4], (uint32_t *)&(tempSetPwmParam->steadyPwm1), IsPWMvalueValid);
    status &= SetParameterValue_int(pwmValuesStr[4], (uint32_t *)&(tempSetPwmParam->steadyPwm2), IsPWMvalueValid); 
    status &= SetParameterValue_int(pwmValuesStr[5], (uint32_t *)&(tempSetPwmParam->approachPwm1), IsPWMvalueValid);
    status &= SetParameterValue_int(pwmValuesStr[5], (uint32_t *)&(tempSetPwmParam->approachPwm2), IsPWMvalueValid);   
    status &= SetParameterValue_int(pwmValuesStr[6], (uint32_t *)&(tempSetPwmParam->Decel), IsPulseCountValid);    
    status &= SetParameterValue_int(Decel_pulse_count, (uint32_t *)&(tempSetPwmParam->decelStartCountDiff), IsDecelPulseCountDiffValid);
       
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Pwm SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
    }            
  }
  else
  {
    Serial_Debug("\n Pwm SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;  
}
static eJsonStatus ProcessGetMotorPWMManual (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_MOTOR_PWM_MANUAL);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stRobotPwmParam* getPWMVal = GetSetManualPwmParameters();
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%d,%d,%d,%d",getPWMVal->accelTime,getPWMVal->decelTime,getPWMVal->Accel,getPWMVal->accelapproachPwm1,getPWMVal->steadyPwm1,
                               getPWMVal->approachPwm1,getPWMVal->Decel,getPWMVal->acelStartCountDiff,getPWMVal->decelStartCountDiff);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetBrushMotorState (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetBrushMotorState->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  bool mode1,mode2;
  ProcessSetMotorState(jsonString, &mode1, &mode2);
  if(motor_error_code == JSON_NO_ERROR)
  {
    Serial_Debug("\r\nBrush motor state set as ");
    Serial_Debug_Num(mode1);
    bool *brushEnState = GetSetBrushEnabledState();
    *brushEnState = mode1;
  }
  else
    error_code = JSON_DATA_OUT_OF_RANGE;
  
  if(linear_error_code == JSON_NO_ERROR)
  {
    Serial_Debug("\r\nLinear motor state set as ");
    Serial_Debug_Num(mode2);
    bool *LinearEnState = GetSetLinearEnabledState();
    *LinearEnState = mode2;
  }
  else
    error_code = JSON_DATA_OUT_OF_RANGE;
  
  return error_code;
}

static eJsonStatus ProcessSetMotorState (const char * jsonString, bool* tempSetBool1, bool* tempSetBool2)              // SH
{
  Serial_Debug("\r\nIn ProcessSetState->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char *setStateStr[JSON_NO_OF_MOTOR_MODE_STATE_DATA - 1];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_MOTOR_MODE_STATE_DATA - 1)
  {
    bool status1 = true, status2 = true;
    status1 &= SetParameterValue_bool(setStateStr[0], tempSetBool1, IsStateValid);
    status2 &= SetParameterValue_bool(setStateStr[1], tempSetBool2, IsStateValid);
    if(status1 == false)
    {
      motor_error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Mode SetValue/s Out of Range");
    }
    else
    {
      motor_error_code = JSON_NO_ERROR;
      Serial_Debug("\n Mode SetValue");
    }    
    if(status2 == false)
    {
      linear_error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Mode SetValue/s Out of Range");
    }
    else
    {
      linear_error_code = JSON_NO_ERROR;
      Serial_Debug("\n Mode SetValue");
    }
  }
  else
  {
    Serial_Debug("\n Mode SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code; 
}

static eJsonStatus ProcessGetBrushMotorEnabledState (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_BRUSH_MOTOR_STATE);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  bool *brushEnabledState = GetSetBrushEnabledState();
  bool *LinearEnabledState = GetSetLinearEnabledState();
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d,%d", *brushEnabledState,*LinearEnabledState);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetMotordriveState (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetBrushMotorState->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  bool mode1,mode2,mode3;
  ProcessSetMotordriveEnabledState(jsonString, &mode1, &mode2, &mode3);
  if(drive1_error_code == JSON_NO_ERROR)
  {
    Serial_Debug("\r\nBrush motor state set as ");
    Serial_Debug_Num(mode1);
    bool *drive1State = GetSetdrive1EnabledState();
    *drive1State = mode1;
  }
  else
    error_code = JSON_DATA_OUT_OF_RANGE;
  
  if(drive2_error_code == JSON_NO_ERROR)
  {
    Serial_Debug("\r\nLinear motor state set as ");
    Serial_Debug_Num(mode2);
    bool *drive2State = GetSetdrive2EnabledState();
    *drive2State = mode2;
  }
  else
    error_code = JSON_DATA_OUT_OF_RANGE;
  
  if(drive3_error_code == JSON_NO_ERROR)
  {
    Serial_Debug("\r\nLinear motor state set as ");
    Serial_Debug_Num(mode2);
    bool *drive3State = GetSetdrive3EnabledState();
    *drive3State = mode3;
  }
  else
    error_code = JSON_DATA_OUT_OF_RANGE;
  
  return error_code;
}

static eJsonStatus ProcessSetMotordriveEnabledState (const char * jsonString, bool* tempSetBool1, bool* tempSetBool2, bool* tempSetBool3)              
{
  Serial_Debug("\r\nIn ProcessSetState->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char *setStateStr[JSON_NO_OF_MOTOR_MODE_STATE_DATA]; //-1
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_MOTOR_MODE_STATE_DATA)
  {
    bool status1 = true, status2 = true,status3 = true;
    status1 &= SetParameterValue_bool(setStateStr[0], tempSetBool1, IsStateValid);
    status2 &= SetParameterValue_bool(setStateStr[1], tempSetBool2, IsStateValid);
    status3 &= SetParameterValue_bool(setStateStr[2], tempSetBool3, IsStateValid);
    if(status1 == false)
    {
      drive1_error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Mode SetValue/s Out of Range");
    }
    else
    {
      drive1_error_code = JSON_NO_ERROR;
      Serial_Debug("\n Mode SetValue");
    }    
    if(status2 == false)
    {
      drive2_error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Mode SetValue/s Out of Range");
    }
    else
    {
      drive2_error_code = JSON_NO_ERROR;
      Serial_Debug("\n Mode SetValue");
    }
    if(status3 == false)
    {
      drive3_error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Mode SetValue/s Out of Range");
    }
    else
    {
      drive3_error_code = JSON_NO_ERROR;
      Serial_Debug("\n Mode SetValue");
    }
  }
  else
  {
    Serial_Debug("\n Mode SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code; 
}

static eJsonStatus ProcessGetMotordriveState (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_DRIVE_STATE);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  bool *drive1State = GetSetdrive1EnabledState();
  bool *drive2State = GetSetdrive2EnabledState();
  bool *drive3State = GetSetdrive3EnabledState();
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d,%d,%d", *drive1State,*drive2State,*drive3State);  
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetBrushPWMManual (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetMotorPWMManual->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  stBrushPwmParam* setBrushParams;
  setBrushParams = GetSetBrushNormalPwmParameters();
  stBrushPwmParam tempSetBrushParams = *setBrushParams;
  error_code = ProcessSetBrushPwm(jsonString,&tempSetBrushParams);
  if(error_code == JSON_NO_ERROR)
  {
    *setBrushParams = tempSetBrushParams;
    char dispstring [100];
    snprintf(dispstring, sizeof dispstring, "\nBrush Pwm Manual Mode SetValues: %d, %d, %d, %d",
    setBrushParams->accelTime, setBrushParams->decelTime, setBrushParams->steadyPwm, setBrushParams->brushTime);
    Serial_Debug(dispstring);
  }
  return error_code; 
}

static eJsonStatus ProcessSetBrushPwm (const char * jsonString, stBrushPwmParam *tempSetPwmParam)
{
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  
  char *pwmValuesStr[JSON_NO_OF_PWM_DATA_BRUSH];
  uint8_t maxNoStrings=(sizeof pwmValuesStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, pwmValuesStr, ',',maxNoStrings);
  Serial_Debug("\nPWM Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(pwmValuesStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_PWM_DATA_BRUSH)
  {
    bool status = true;
    status &= SetParameterValue_int(pwmValuesStr[0], (uint32_t *)&(tempSetPwmParam->accelTime), IsAccelTimeValid);
    status &= SetParameterValue_int(pwmValuesStr[1], (uint32_t *)&(tempSetPwmParam->decelTime) , IsDecelTimeValid);
    status &= SetParameterValue_int(pwmValuesStr[2], (uint32_t *)&(tempSetPwmParam->steadyPwm), IsPWMvalueValid);
    status &= SetParameterValue_int(pwmValuesStr[3], (uint32_t *)&(tempSetPwmParam->brushTime), IsBrushtimeValid);

    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Pwm SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
    }            
  }
  else
  {
    Serial_Debug("\n Pwm SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;  
}

static eJsonStatus ProcessGetBrushMotorPWMManual (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_BRUSH_MOTOR_PWM_MANUAL);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stBrushPwmParam* getBrushPWMVal = GetSetBrushNormalPwmParameters();
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d",getBrushPWMVal->accelTime,
  getBrushPWMVal->decelTime,getBrushPWMVal->steadyPwm,getBrushPWMVal->brushTime);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetBrushDirection (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetBrushDirection->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  bool mode;
  error_code = ProcessSetState(jsonString, &mode);
  if(error_code == JSON_NO_ERROR)
  {
    Serial_Debug("\r\nBrush motor direction set as ");
    Serial_Debug_Num(mode);
    stBrushMotPolarity *motPolarity = GetSetBrushMotPolarity();
    motPolarity->mot1Polarity = mode;
  }
  return error_code;
}
static eJsonStatus ProcessGetBrushMotorPolarity (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_BRUSH_MOTOR_DIR);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stBrushMotPolarity *setMotPolarity = GetSetBrushMotPolarity();
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d",setMotPolarity->mot1Polarity);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}


static eJsonStatus ProcessSetMotorPWMAuto (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetMotorPWMAuto->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  stRobotPwmParam* setPWMParams;
  setPWMParams = GetSetCycleAutoPwmParameters();
  stRobotPwmParam tempSetPWMParams = *setPWMParams;
  error_code = ProcessSetMotorPwm(jsonString,&tempSetPWMParams);
  if(error_code == JSON_NO_ERROR)
  {
    *setPWMParams = tempSetPWMParams;
    char dispstring [100];
   snprintf(dispstring, sizeof dispstring, "\nPwm Manual Mode SetValues: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ,%d ,%d",
             setPWMParams->accelTime, setPWMParams->decelTime,setPWMParams->acelStartCountDiff,setPWMParams->accelapproachPwm1, setPWMParams->accelapproachPwm2, setPWMParams->steadyPwm1, setPWMParams->steadyPwm2,
             setPWMParams->approachPwm1, setPWMParams->approachPwm2,setPWMParams->decelStartCountDiff,setPWMParams->timePwm,CYCLEROTATIONTIME);
    Serial_Debug(dispstring);
  }
  return error_code;
}

static eJsonStatus ProcessGetMotorPWMAuto (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_MOTOR_PWM_AUTO);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stRobotPwmParam* getPWMVal = GetSetCycleAutoPwmParameters(); 
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%d,%d,%d,%d",getPWMVal->accelTime,getPWMVal->decelTime,getPWMVal->Accel,getPWMVal->accelapproachPwm1,
                               getPWMVal->steadyPwm1,getPWMVal->approachPwm1,getPWMVal->Decel,getPWMVal->acelStartCountDiff,getPWMVal->decelStartCountDiff);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetBrushPWMAuto (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetBrushPWMAuto->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  stBrushPwmParam* setBrushParams;
  setBrushParams = GetSetBrushAutoPwmParameters();
  stBrushPwmParam tempSetBrushParams = *setBrushParams;
  error_code = ProcessSetBrushPwm(jsonString,&tempSetBrushParams);
  if(error_code == JSON_NO_ERROR)
  {
    *setBrushParams = tempSetBrushParams;
    char dispstring [100];
    snprintf(dispstring, sizeof dispstring, "\nBrush Pwm Auto Mode SetValues: %d, %d, %d, %d",
    setBrushParams->accelTime, setBrushParams->decelTime, setBrushParams->steadyPwm, setBrushParams->brushTime);
    Serial_Debug(dispstring);
  }
  return error_code;
}

static eJsonStatus ProcessGetBrushMotorPWMAuto (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_BRUSH_MOTOR_PWM_AUTO);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stBrushPwmParam* getBrushPWMVal = GetSetBrushAutoPwmParameters();
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d",getBrushPWMVal->accelTime,
  getBrushPWMVal->decelTime,getBrushPWMVal->steadyPwm,getBrushPWMVal->brushTime);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetAutoScheduleTime (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetAutoScheduleTime->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  Autoschcountval = Autoschcountval + 1;
  char* autoScheduleStr[JSON_NO_AUTOSCH];
  uint8_t maxNoStrings=(sizeof autoScheduleStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, autoScheduleStr, ',',maxNoStrings);
  Serial_Debug("\nAuto schedule Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(autoScheduleStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_AUTOSCH)
  {
     ClearAutoDoneSeconds();
     for (int i=0;i<JSON_NO_AUTOSCH-1;i++)
     {
      uint8_t hour = 0;
      uint8_t minute = 0;
      hour = (autoScheduleStr[i][1] - 48)+((autoScheduleStr[i][0] - 48)*10);
      minute = (autoScheduleStr[i][3] - 48)+((autoScheduleStr[i][2] - 48)*10);
      
      Serial_Debug("\r\nHour-->");
      Serial_Debug_Num(hour);
      Serial_Debug("\r\nMinute-->");
      Serial_Debug_Num(minute);
      if((hour<24)&&(minute<60))
      {
        stScheduleTime setScheduleTime;
        setScheduleTime.HOUR[i] = hour;
        setScheduleTime.MINUTE[i] = minute;
        ClearAutoDoneSeconds();
        SetScheduleTime(&setScheduleTime);
        error_code = JSON_NO_ERROR;
        Serial_Debug("\n Auto time SetValue/s Set"); 
      }
      else
      {
        error_code = JSON_DATA_OUT_OF_RANGE;
        Serial_Debug("\n Auto time SetValue/s Out of Range");    
      } 
    }  
  }
    else
    {
      Serial_Debug("\n Auto time SetValue/s Invalid");
      error_code = JSON_INVALID_DATA;
    } 
  return error_code;
}

static eJsonStatus ProcessGetAutoScheduleTime (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_AUTO_SCHEDULE_TIME);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stScheduleTime* getScheduleTime = GetSetScheduleTime();   
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d,%02d%02d",
  getScheduleTime->HOUR[0],getScheduleTime->MINUTE[0],getScheduleTime->HOUR[1],getScheduleTime->MINUTE[1],
  getScheduleTime->HOUR[2],getScheduleTime->MINUTE[2],getScheduleTime->HOUR[3],getScheduleTime->MINUTE[3],
  getScheduleTime->HOUR[4],getScheduleTime->MINUTE[4],getScheduleTime->HOUR[5],getScheduleTime->MINUTE[5],  
  getScheduleTime->HOUR[6],getScheduleTime->MINUTE[6],getScheduleTime->HOUR[7],getScheduleTime->MINUTE[7],
  getScheduleTime->HOUR[8],getScheduleTime->MINUTE[8],getScheduleTime->HOUR[9],getScheduleTime->MINUTE[9], 
  getScheduleTime->HOUR[10],getScheduleTime->MINUTE[10],getScheduleTime->HOUR[11],getScheduleTime->MINUTE[11],
  getScheduleTime->HOUR[12],getScheduleTime->MINUTE[12],getScheduleTime->HOUR[13],getScheduleTime->MINUTE[13],  
  getScheduleTime->HOUR[14],getScheduleTime->MINUTE[14],getScheduleTime->HOUR[15],getScheduleTime->MINUTE[15],
  getScheduleTime->HOUR[16],getScheduleTime->MINUTE[16],getScheduleTime->HOUR[17],getScheduleTime->MINUTE[17],
  getScheduleTime->HOUR[18],getScheduleTime->MINUTE[18]);   
  dataCount = dataCount+tempCount;
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetEdgeSensorState (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetEdgeSensorState->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  bool mode1,mode2;
  ProcessSetSensorsState(jsonString, &mode1, &mode2);
  if(edgesense_error_code == JSON_NO_ERROR)
  {
    Serial_Debug("\r\nEdge sensor state set as ");
    Serial_Debug_Num(mode1);
    bool * edgeSensorState = GetSetEdgeSensorEnabledState();
    * edgeSensorState = mode1;
  }
  else
  {
    error_code = JSON_INVALID_DATA;
  }
  if(rotatesense_error_code == JSON_NO_ERROR)
  {
    Serial_Debug("\r\nRotate sensor state set as ");
    Serial_Debug_Num(mode2);
    bool * rotateSensorState = GetSetRotateSensorEnabledState();
    * rotateSensorState = mode2;
    rotatesensorflag = mode2;
  }
  else
  {
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}
static eJsonStatus ProcessSetSensorsState (const char * jsonString, bool* tempSetBool1, bool* tempSetBool2)  //SH
{
  Serial_Debug("\r\nIn ProcessSetState->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char *setStateStr[JSON_NO_OF_SENSOR_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_SENSOR_STATE_DATA)
  {
    bool status1 = true,status2 = true;
    status1 &= SetParameterValue_bool(setStateStr[0], tempSetBool1, IsStateValid);
    status2 &= SetParameterValue_bool(setStateStr[1], tempSetBool2, IsStateValid);
    if(status1 == false)
    {
      edgesense_error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Mode SetValue/s Out of Range");
    }
    else
    {
      edgesense_error_code = JSON_NO_ERROR;
      Serial_Debug("\n Mode SetValue");
    }   
    if(status2 == false)
    {
      rotatesense_error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Mode SetValue/s Out of Range");
    }
    else
    {
      rotatesense_error_code = JSON_NO_ERROR;
      Serial_Debug("\n Mode SetValue");
    }   
  }
  else
  {
    Serial_Debug("\n Mode SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetEdgeSenseEnabledState (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_EDGE_SENSE_STATE);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d,%d",IsEdgeSensorEnabled(),IsRotateSensorEnabled());
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetRTCValue (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetRTCValue->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  char* setRTCValueStr[JSON_NO_OF_RTC_DATA];
  uint8_t maxNoStrings=(sizeof setRTCValueStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setRTCValueStr, ',',maxNoStrings);
  Serial_Debug("\nAuto schedule Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setRTCValueStr[dispcount]);
    Serial_Debug(",");
  }
    sys_time.tm_hour = atoi(setRTCValueStr[3]);
    sys_time.tm_sec = atoi(setRTCValueStr[5]);
    sys_time.tm_min = atoi(setRTCValueStr[4]);
    sys_time.tm_mon = atoi(setRTCValueStr[1]);
    sys_time.tm_year = atoi(setRTCValueStr[2]);
    sys_time.tm_mday = atoi(setRTCValueStr[0]);
 
  if(totalResultStringCount == JSON_NO_OF_RTC_DATA)
  {
    if( external_rtc_write(&sys_time))
    {
        error_code = JSON_NO_ERROR;
    }
    else
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n RTC SetValue/s Out of Range");    
    }
  }
  else
  {
    Serial_Debug("\n RTC  SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;  
  }
  return error_code;
}

static eJsonStatus ProcessGetRTCValue (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RTC_VALUE);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount; 
  external_rtc_read(&sys_time);
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d-%d-%d %d:%d:%d",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec); 
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  return error_code;
}

static eJsonStatus ProcessSetDeviceInfo (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetDeviceInfo->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  char* devIDInfoStr[JSON_NO_OF_SET_DEVICE_INFO];
  uint8_t maxNoStrings=(sizeof devIDInfoStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, devIDInfoStr, ',',maxNoStrings);
  Serial_Debug("\nDevice Info Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(devIDInfoStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_SET_DEVICE_INFO)
  {
    stDevInfo *devIDInfoValues = GetSetDeviceIDInfo();
    if(strlen(devIDInfoStr[0]) != 0)
    {
      snprintf(devIDInfoValues->devID, MAX_DEV_INFO_FIELD_LEN,"%s",devIDInfoStr[0]);
    }
    if(strlen(devIDInfoStr[1]) != 0)
    {
      snprintf(devIDInfoValues->hwVersion, MAX_DEV_INFO_FIELD_LEN,"%s",devIDInfoStr[1]);
    }
    if(strlen(devIDInfoStr[2]) != 0)
    {
      snprintf(devIDInfoValues->serialNo, MAX_DEV_INFO_FIELD_LEN,"%s",devIDInfoStr[2]);
    }
    Serial_Debug("\n Device Info SetValue/s Set");
    error_code = JSON_NO_ERROR;
  }
  else
  {
    Serial_Debug("\n Device info SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;   
  }
  return error_code;
}
static eJsonStatus ProcessGetDeviceInfo (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_DEVICE_INFO);
  dataCount = dataCount+bufferLength;   
  bufferLength = sizeof(txBuff)-dataCount;
  stDevInfo * devInfo = GetSetDeviceIDInfo();
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%s,%s,%s,%s",
                               devInfo->devID, devInfo->hwVersion, devInfo->serialNo, DEFAULT_DEV_SW_REV);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}
//static eJsonStatus ProcessGetDeviceInfo (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_DEVICE_INFO);
//  dataCount = dataCount+bufferLength;   
//  bufferLength = sizeof(txBuff)-dataCount;
//  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%s",DEFAULT_DEVICE_WELCOME_MSG);
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes((char*)txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}

static eJsonStatus ProcessSetMotorCurrentLimits (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetMotorCurrentLimits->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  stMaxMotorCurrent* setMotorcurrent = GetSetMaxMotorCurrentLimits();
  stMaxMotorCurrent tempSetMotorCurrent = *setMotorcurrent;
  char *MotorCurrentLimitStr[JSON_NO_OF_CURRENT_LIMIT];
  uint8_t maxNoStrings=(sizeof MotorCurrentLimitStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, MotorCurrentLimitStr, ',',maxNoStrings);
  Serial_Debug("\nPWM Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(MotorCurrentLimitStr[dispcount]);
    Serial_Debug(",");
  }  
  if(totalResultStringCount == JSON_NO_OF_CURRENT_LIMIT)
  {
    bool status = true;
    status &= SetParameterValue_float(MotorCurrentLimitStr[0], &(tempSetMotorCurrent.Imot1), IsMotorCurrentLimitValid);
    status &= SetParameterValue_float(MotorCurrentLimitStr[1], &(tempSetMotorCurrent.Imot2) , IsMotorCurrentLimitValid);
    status &= SetParameterValue_float(MotorCurrentLimitStr[2], &(tempSetMotorCurrent.Imot3), IsMotorCurrentLimitValid);
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n IMOT current limit SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n IMOT current limit SetValue/s set : ");
      *setMotorcurrent = tempSetMotorCurrent;  
      Serial_Debug_Float(setMotorcurrent->Imot1,3);
      Serial_Debug(" ,");
      Serial_Debug_Float(setMotorcurrent->Imot2,3);
      Serial_Debug(" ,");
      Serial_Debug_Float(setMotorcurrent->Imot3,3);
    }            
  }
  else
  {
    Serial_Debug("\n IMOT current limit SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetOverCurrentLimits (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_OVERCURRENT_LIMIT);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stMaxMotorCurrent* getMotorCurrent = GetSetMaxMotorCurrentLimits();
  ftoa(getMotorCurrent->Imot1,Imot1_Str,2);
  ftoa(getMotorCurrent->Imot2,Imot2_Str,2);
  ftoa(getMotorCurrent->Imot3,Imot3_Str,2);
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%s,%s,%s",Imot1_Str,Imot2_Str,Imot3_Str);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}
static eJsonStatus ProcessClearErrorCodes (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessClearErrorCodes->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  char* clearErrorCodeStr[JSON_NO_OF_ERROR_CODE_DATA];
  uint8_t maxNoStrings=(sizeof clearErrorCodeStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, clearErrorCodeStr, ',',maxNoStrings);
  Serial_Debug("\nClear Error code Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(clearErrorCodeStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_ERROR_CODE_DATA)
  {
    error_code = JSON_NO_ERROR;
    Serial_Debug("\n Clear Error code SetValue/s Set");
    Serial_Debug_Num(strtol(clearErrorCodeStr[0],NULL,16));
    bool isRTCfault = IsRTCFault();
    
    if(GetMotorFaultCode() == 2048)
    {
      stHeartbeatConfig *defaultHeartbeatConfig = GetSetHeartbeatConfig();
      defaultHeartbeatConfig->enable = 0;
    }
    
    ClearAllFaults();
    ClearRoboterror();
    if(isRTCfault)
    {
      SetFault(RTC_FAULT);
    }
  }
  else
  {
    Serial_Debug("\n Clear Error code SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;   
  }
  return error_code;
}
static eJsonStatus ProcessSetLowBatSoC (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetLowBatSoC->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  char* setLowBatSoCValueStr[JSON_NO_OF_SOC_DATA];
  uint8_t maxNoStrings=(sizeof setLowBatSoCValueStr)/4;  
  uint8_t* setLowBatSoC = GetSetMinBattSoc();
  uint8_t tempSetLowBatSoC = *setLowBatSoC;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setLowBatSoCValueStr, ',',maxNoStrings);
  Serial_Debug("\nLow Battery Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setLowBatSoCValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_SOC_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setLowBatSoCValueStr[0],(uint32_t *)&(tempSetLowBatSoC), IsLowBatSoCValid);
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Low Bat limit SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      *setLowBatSoC = tempSetLowBatSoC;
      Serial_Debug("\n Low Bat SoC SetValue/s Set");
      Serial_Debug_Num(*setLowBatSoC);
    } 
  }
  else
  {
    Serial_Debug("\n Low Bat SoC SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;   
  }
  return error_code;
}

static eJsonStatus ProcessGetLowBatSoC (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_LOW_BAT_SOC);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t *lowBatSoc = GetSetMinBattSoc();
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d",*lowBatSoc);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetHeartbeatConfig (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetHeartbeatConfig->");
  Serial_Debug(jsonString); 
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
    char *heartbeatConfigStr[JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS];
    uint8_t maxNoStrings=(sizeof heartbeatConfigStr)/4;
    uint8_t totalResulStringCount =  SplitString ((char *)jsonString, heartbeatConfigStr, ',', maxNoStrings );
    Serial_Debug("\nHeartbeat Config Parameters Received Values :");
    for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
    {
      Serial_Debug(heartbeatConfigStr[dispcount]);
      Serial_Debug(",");
    }
    if(totalResulStringCount == JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS)
    {
      stHeartbeatConfig tempSetHeartbeatConfig;
      stHeartbeatConfig* setHeartbeatConfig;
      setHeartbeatConfig = GetSetHeartbeatConfig();
      tempSetHeartbeatConfig = *setHeartbeatConfig;
      bool status = true;
      status &= SetParameterValue_bool(heartbeatConfigStr[0], &(tempSetHeartbeatConfig.enable), IsHeartBeatStateValid);
      status &= SetParameterValue_int(heartbeatConfigStr[1], (uint32_t*)&(tempSetHeartbeatConfig.interval_ms) , IsHeartBeatIntervalValid);
      status &= SetParameterValue_int(heartbeatConfigStr[2], (uint32_t*)&(tempSetHeartbeatConfig.noOfMessages) , IsNoOfHeartBeatMsgValid);
      status &= SetParameterValue_int(heartbeatConfigStr[3], (uint32_t*)&(tempSetHeartbeatConfig.maxReconnectTime) , IsMaxReconnectTimeValid);
      
      if(status == false)
      {
        Serial_Debug("\nHeartbeat Config Parameters SetValue/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
      else
      {
        Serial_Debug("\nHeartbeat Config Parameters Set");
        *setHeartbeatConfig = tempSetHeartbeatConfig;
        error_code = JSON_NO_ERROR;
        Serial_Debug("\nHear Beat Parameters SetValues: ");
        Serial_Debug_Num(setHeartbeatConfig->enable);
        Serial_Debug(", ");
        Serial_Debug_Num(setHeartbeatConfig->interval_ms);
        Serial_Debug(", ");
        Serial_Debug_Num(setHeartbeatConfig->noOfMessages);
        Serial_Debug(", ");
        Serial_Debug_Num(setHeartbeatConfig->maxReconnectTime);
        if(setHeartbeatConfig->enable)
        {
            RestartCommTimer();
        }
      } 
    }
    else
    {
      Serial_Debug("\nHeartbeat Config Parameters SetValue/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  
  return error_code;   
}
static eJsonStatus ProcessGetHeartbeatConfig (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_HEARTBEAT_CONFIG);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stHeartbeatConfig *defaultHeartbeatConfig = GetSetHeartbeatConfig();
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d",defaultHeartbeatConfig->enable,defaultHeartbeatConfig->interval_ms,defaultHeartbeatConfig->noOfMessages,defaultHeartbeatConfig->maxReconnectTime);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}


static eJsonStatus ProcessSetMotorFaultConditions (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetMotor fault conditions->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  stMaxIloadConfigValues* setMaxIloadConfigValues;
  setMaxIloadConfigValues = GetSetMaxIloadConfigValues();
  stMaxIloadConfigValues tempSetMaxIloadConfigValues = *setMaxIloadConfigValues;
  char *MotorFaultConditionsStr[JSON_NO_OF_MOTOR_FAULT_CONDITIONS];
  uint8_t maxNoStrings=(sizeof MotorFaultConditionsStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, MotorFaultConditionsStr, ',',maxNoStrings);
  Serial_Debug("\nMotor fault conditions Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(MotorFaultConditionsStr[dispcount]);
    Serial_Debug(",");
  }  
  if(totalResultStringCount == JSON_NO_OF_MOTOR_FAULT_CONDITIONS)
  {
    bool status = true;
    status &= SetParameterValue_int(MotorFaultConditionsStr[0],(uint32_t *) &(tempSetMaxIloadConfigValues.maxIloadNoiseTimeValue), IsIloadMaxConfigTimeValid);
    status &= SetParameterValue_int(MotorFaultConditionsStr[1],(uint32_t *) &(tempSetMaxIloadConfigValues.maxIloadFreqTimeValue) , IsIloadMaxConfigTimeValid);
    status &= SetParameterValue_int(MotorFaultConditionsStr[2],(uint32_t *) &(tempSetMaxIloadConfigValues.maxIloadRepeatCountValue), IsIloadMaxConfigTimeValid);
    status &= SetParameterValue_int(MotorFaultConditionsStr[3],(uint32_t *) &(tempSetMaxIloadConfigValues.motorPauseDelay) , IsIloadMaxConfigTimeValid);
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Motor fault conditions SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Motor fault conditions SetValue/s set : ");
      *setMaxIloadConfigValues = tempSetMaxIloadConfigValues;
      Serial_Debug_Num(setMaxIloadConfigValues->maxIloadNoiseTimeValue);
      Serial_Debug(",");
      Serial_Debug_Num(setMaxIloadConfigValues->maxIloadFreqTimeValue);
      Serial_Debug(",");
      Serial_Debug_Num(setMaxIloadConfigValues->maxIloadRepeatCountValue);
      Serial_Debug(",");
      Serial_Debug_Num(setMaxIloadConfigValues->motorPauseDelay);
    }            
  }
  else
  {
    Serial_Debug("\n Motor fault conditions SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}


static eJsonStatus ProcessGetMotorFaultConditions (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_MOTOR_FAULT_CONDITIONS);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stMaxIloadConfigValues* getMaxIloadConfigValues = GetSetMaxIloadConfigValues();
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d",getMaxIloadConfigValues->maxIloadNoiseTimeValue,getMaxIloadConfigValues->maxIloadFreqTimeValue,getMaxIloadConfigValues->maxIloadRepeatCountValue,getMaxIloadConfigValues->motorPauseDelay);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}
static eJsonStatus ProcessSetPulseCount (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetPulseCount->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  stCountRange* setCountValues;
  setCountValues = GetSetCountRange();
  stCountRange tempSetCountValues = *setCountValues;
  char *PulseCountStr[JSON_NO_OF_PULSE_COUNT_DATA];
  uint8_t maxNoStrings=(sizeof PulseCountStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, PulseCountStr, ',',maxNoStrings);
  Serial_Debug("\nPulse Count Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(PulseCountStr[dispcount]);
    Serial_Debug(",");
  }  
  if(totalResultStringCount == JSON_NO_OF_PULSE_COUNT_DATA)
  {
    bool status = true;  
    pulse_flag = 1; 
    posdistance = atof(PulseCountStr[0]);
    negdistance = atof(PulseCountStr[1]);  
    posdistance_int = atoi(PulseCountStr[0]);                  
    negdistance_int = atoi(PulseCountStr[1]);                    
    PulseCalculation();
    status &= SetParameterValue_int(POS, (uint32_t *)&(tempSetCountValues.maxPosCountForPVsetup), IsPulseCountValid);
    status &= SetParameterValue_int(NEG, (uint32_t *)&(tempSetCountValues.maxNegCountForPVsetup) , IsPulseCountValid);  
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
    }
    else
    {
      error_code = JSON_NO_ERROR;
      *setCountValues = tempSetCountValues;
    }            
  }
  else
  {
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

void PulseCalculation()
{
    P=1;
    stWheelDia* setWheelDia = GetSetwheelDiaLimits();
    distance =  ((float) ((float) 3.14 * (float) setWheelDia->Dia) * P);
    distance = distance / (setWheelDia->Pulse);
    Rotation_per_second = distance / CYCLEROTATIONTIME;
    
    if (pulse_flag == 1)
    {
      pos = (float) ((float) posdistance /(float) distance);
      neg = (float) ((float) negdistance /(float) distance);      
      forward_remaining_distance = (posdistance_int % distance);         
      backward_remaining_distance = (negdistance_int % distance);       
      ftoa(pos, POS, 0);  
      ftoa(neg, NEG, 0);      
      Time_seconds_forward = forward_remaining_distance / Rotation_per_second;
      Time_seconds_backward = backward_remaining_distance / Rotation_per_second;      
      pulse_flag = 0;
    }
    
    else if(pulse_flag == 2)
    {  
      Accel_speed_pulse = (float) ((float) Accel_speed /(float) distance);
      Decel_speed_pulse = (float) ((float) Decel_speed /(float) distance);     
      Accel_remaining_distance = (Accel_speed % distance);
      Decel_remaining_distance = (Decel_speed % distance);
      ftoa(Accel_speed_pulse,Accel_pulse_count,0); 
      ftoa(Decel_speed_pulse,Decel_pulse_count,0);     
      Time_seconds_accel = Accel_remaining_distance / Rotation_per_second;
      Time_seconds_decel = Decel_remaining_distance / Rotation_per_second;       
      pulse_flag = 0;
    }
    
    else if(pulse_flag == 3)
    {
      Compulse = (float) ((float) ComDistance / (float)distance);
      ftoa(Compulse,ComPulse,0);
      pulse_flag = 0;
    }
    
    else if(pulse_flag == 4)
    {
     
      Cpositive = (float) ((float) CpositiveDistance /(float) distance);
      Cnegative = (float) ((float) CnegativeDistance /(float) distance);     
      Cpositivedistance = (CpositiveDistance_int % distance);
      Cnegativedistance = (CnegativeDistance_int % distance);
      ftoa(Cpositive,Cpospulse,0); 
      ftoa(Cnegative,Cnegpulse,0);
      Time_seconds_forward = Cpositivedistance / Rotation_per_second;
      Time_seconds_backward = Cnegativedistance / Rotation_per_second;        
      pulse_flag = 0;
    }
}

static eJsonStatus ProcessGetPulseCountValues (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_PULSE_COUNT_VALUES);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stCountRange* getCountValue = GetSetCountRange(); 
  stWheelDia* setWheelDia = GetSetwheelDiaLimits();
  Getpos = (float) ((float) 3.14 * (float) setWheelDia->Dia) * getCountValue->maxPosCountForPVsetup;
  Getpos = Getpos + forward_remaining_distance; 
  Getneg = (float) ((float) 3.14 * (float) setWheelDia->Dia) * getCountValue->maxNegCountForPVsetup;
  Getneg = Getneg + backward_remaining_distance; 
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d",posdistance_int,negdistance_int,getCountValue->maxPosCountForPVsetup,getCountValue->maxNegCountForPVsetup);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessResetToDefaults (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  eEepromStatus status = EEPROM_OK;
  stJsonCommand *jsonCmdParam;
  uint16_t commandParamsTotalCount = GetCommandProcessingParam(&jsonCmdParam);
  status = InitialiseAlltoDefault(jsonCmdParam, commandParamsTotalCount);
  if(status == EEPROM_OK)
  {
    error_code = JSON_NO_ERROR;
  }
  else
  {
    error_code = JSON_EEPROM_ERROR;
  }
  return error_code;
}

static eJsonStatus ProcessGetCurrentStat (const char * jsonString)
{
   eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CURRENT_STAT);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  eRobotDirection robotState = GetRobotActualDirection();
  eMode getMode =  GetOperationMode ();
  uint8_t modeValue = 0;  
  stCycleFrequency* getCycleFrequency = GetSetcycleFrequencyLimits(); 
  char Cycle_Str[2]; 
  ftoa(getCycleFrequency->Cycle,Cycle_Str,0);
  snprintf((char *)&txBuff[dataCount],bufferLength,"%s",Cycle_Str); 
  char total_Schedule = atoi(Cycle_Str);
  int Currentdistance; 
  switch(getMode)
  {
  case AUTO: 
    modeValue = 0;
    break;
  case MANUAL:
    modeValue = 1;
    break;
  default : 
    modeValue = 1;
    break;
  }  
  PulseCalculation();  
  Currentdistance = GetRotateSense1Count() * distance;  
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
          modeValue, robotState, GetRotateSense1Count(),Currentdistance,GetRotateSense2Count(), Get_Current_Brush_Count(), GetRotateSense1State(), GetRotateSense2State(),GetEdgeSense1State(), GetEdgeSense2State(),Get_Completed_Schedule(),total_Schedule - Get_Completed_Schedule(),Cumulative_distance);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessGetLastOperationStat (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  int mode_now;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_LAST_CYCLE_STAT);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stLastOperationStatus *lastOpStatus = GetLastOperationData();
  char startSoc_Str[10],endSoc_Str[10];
  ftoa(lastOpStatus->startSOC,startSoc_Str,2);
  ftoa(lastOpStatus->endSOC,endSoc_Str,2);
  int Currentdistance;
  PulseCalculation();
  Currentdistance = GetRotateSense1Count() * distance;
  eRobotDirection robotState = GetRobotActualDirection();
  eMode getMode =  GetOperationMode ();   
  uint8_t modeValue = 0;
  switch(getMode)
  {
    case AUTO: 
      modeValue = 0;
      mode_now =  Get_Completed_Schedule();
      break;
    case MANUAL:
      modeValue = 1;
      mode_now = 0;
      break;
    default : 
      modeValue = 0;
      mode_now =  Get_Completed_Schedule();
      break;
  }
  
  //    tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%000X,%d,%d,%d,%d,%s",modeValue,robotState,GetRotateSense1Count()*Distancevalue,Cumulative_distance,GetMotorFaultCode(),daycount,trackchangercount,rowdaycount,tempsetNoofrows.Row[daycount]-rowdaycount,cyclelog);
//    tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%s,%s,%s,%s,%000X,%d",robotState,daycount,trackchangercount,rowdaycount,tempsetNoofrows.Row[daycount]-rowdaycount,Cstarttime,Cstartsoc,Cendtime,Cendsoc,GetMotorFaultCode(),Cumulative_distance);  
//    dataCount = dataCount+tempCount;
  
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%lX,%d,%d,%s,%s,%s,%s,%s,%d",modeValue,mode_now,GetMotorFaultCode(),Currentdistance,robotState,Cstarttime,Cstartsoc,Cendtime,Cendsoc,Cerrorcount,Cumulative_distance);
    
//  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%lX,%d,%d,%s,%d",modeValue,mode_now,GetMotorFaultCode(),Currentdistance,robotState,cyclelog,Cumulative_distance); 
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessGetMotorCurrentValues (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_MOT_CURRENT_VALUES);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  ftoa(GetImot1Value(),Imot1_Str,3);
  ftoa(GetImot2Value(),Imot2_Str,3);
  ftoa(GetImot3Value(),Imot3_Str,3);
  uint8_t tempCount;
  if(GetRobotState() != ROBOT_COMPLETED && GetRobotState() != ROBOT_IDLE)
  {  
      tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%s,%s,%s",Imot1_Str,Imot2_Str,Imot3_Str);
  }
  else
  {
      memset(Imot1_Str,'\0',sizeof(Imot1_Str)); 
      memset(Imot2_Str,'\0',sizeof(Imot2_Str)); 
      memset(Imot3_Str,'\0',sizeof(Imot3_Str)); 
      strcpy(Imot1_Str,"0.00");  strcpy(Imot2_Str,"0.00"); strcpy(Imot3_Str,"0.00");
      
      tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%s,%s,%s",Imot1_Str,Imot2_Str,Imot3_Str);  
  }
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessGetCurrentRobotStat (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CURRENT_ROBOT_STATUS);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  eRobotDirection robotState = GetRobotActualDirection();
  eMode getMode =  GetOperationMode ();
  uint8_t modeValue = 0;
  switch(getMode)
  {
    case AUTO: 
      modeValue = 0;
      break;
    case MANUAL:
      modeValue = 1;
      break;
    default : 
      modeValue = 1;
      break;
  }
  uint32_t tc_error_code;
  
  if(GetMotionPauseState() == PA_WAIT_FOR_ERROR_CLEAR)
  {
     tc_error_code = GetMotorFaultCode();
  }
  else
  {
     tc_error_code = 0; 
  }
  zigbeetcflag = 1;
  uartflag = 3;
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%ld,%d,%d", modeValue, robotState, /*GetRobotState()*/ GetCycleModeState(), GetRotateSense1Count(), GetRotateSense2Count(),tc_error_code,Cumulative_distance,cleaner_in_comm);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetWheelDia (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetWheelDia->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR; 
  stWheelDia* setWheelDia = GetSetwheelDiaLimits();
  stWheelDia tempsetWheelDia = *setWheelDia;
  char* setWheelDiaValueStr[JSON_NO_OF_WHEELDIA_DATA]; 
  uint8_t maxNoStrings=(sizeof setWheelDiaValueStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setWheelDiaValueStr, ',',maxNoStrings);
  Serial_Debug("\nWheel Dia Set Received Value :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setWheelDiaValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_WHEELDIA_DATA)
  {
    bool status = true;
    status &= SetParameterValue_float(setWheelDiaValueStr[0], &(tempsetWheelDia.Dia), IswheelDiaLimitValid); 
    status &= SetParameterValue_float(setWheelDiaValueStr[1], &(tempsetWheelDia.Pulse), IspulseLimitValid);  //change
    status &= SetParameterValue_int(setWheelDiaValueStr[2], (uint32_t *)&(tempsetWheelDia.Speed), IsPWMvalueValid);  
    int Cycle_rotation = atoi(setWheelDiaValueStr[2]);  
    if ((Cycle_rotation > 54) && (Cycle_rotation < 65))
    { 
     CYCLEROTATIONTIME = 15; 
    }  
    else if ((Cycle_rotation > 64) && (Cycle_rotation < 71))
    { 
      CYCLEROTATIONTIME = 5;
    }
    else if ((Cycle_rotation > 70) && (Cycle_rotation < 76))
    { 
      CYCLEROTATIONTIME = 2;
    }  
     else if ((Cycle_rotation > 75) && (Cycle_rotation < 86))
    { 
      CYCLEROTATIONTIME = 1;
    }  
    else
    {
      CYCLEROTATIONTIME = 0;     
    }
        
     if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n wheelDia SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n wheelDia SetValue/s set : ");
      *setWheelDia = tempsetWheelDia;      
      Serial_Debug_Float(setWheelDia->Dia,3);
      Serial_Debug_Float(setWheelDia->Pulse,3);
      Serial_Debug_Float(setWheelDia->Speed,3);
      Serial_Debug(",");    
    }            
  }
  else
  {
    Serial_Debug("\n wheelDia SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetWheelDia (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_DIA_WHEEL);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stWheelDia* getWheelDia = GetSetwheelDiaLimits();
  char WheelDia1_Str[10];char Pulse_str[10];
  ftoa(getWheelDia->Dia,WheelDia1_Str,0);
  ftoa(getWheelDia->Pulse,Pulse_str,0);
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%s,%s,%d,%d",WheelDia1_Str,Pulse_str,getWheelDia->Speed,CYCLEROTATIONTIME);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetCycleFrequency (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetCycleFrequency->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;   
  stCycleFrequency* setCycleFrequency = GetSetcycleFrequencyLimits();
  stCycleFrequency tempsetCycleFrequency = *setCycleFrequency; 
  char* setcyclefrequencyValueStr[JSON_NO_OF_CYCLEFREQUENCY_DATA]; 
  uint8_t maxNoStrings=(sizeof setcyclefrequencyValueStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setcyclefrequencyValueStr, ',',maxNoStrings);
  Serial_Debug("\nCycleFrequency Set Received Value :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setcyclefrequencyValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_CYCLEFREQUENCY_DATA)
  {
    bool status = true;
    status &= SetParameterValue_float(setcyclefrequencyValueStr[0], &(tempsetCycleFrequency.Cycle), IscycleFrequencyLimitValid);
    
     if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n CycleFrequency SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n CycleFrequency SetValue/s set : ");    
      *setCycleFrequency = tempsetCycleFrequency;     
      Serial_Debug_Float(setCycleFrequency->Cycle,0);
      Autoschcountval=setCycleFrequency->Cycle;
      Serial_Debug(",");
    }            
  }
  else
  {
    Serial_Debug("\n CycleFrequency SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetCycleFrequency (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CYCLE_FREQUENCY);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stCycleFrequency* getCycleFrequency = GetSetcycleFrequencyLimits(); 
  char Cycle_Str[10]; 
  ftoa(getCycleFrequency->Cycle,Cycle_Str,0);
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%s",Cycle_Str);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetLogInterval (const char * jsonString)
{ 
  Serial_Debug("\r\nIn ProcessSetLogInterval->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR; 
  stInterval* setInterval = GetSetIntervalLimits();
  stInterval tempsetInterval = *setInterval;  
  char* setIntervalValueStr[JSON_NO_OF_INTERVAL_DATA];   
  uint8_t maxNoStrings=(sizeof setIntervalValueStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setIntervalValueStr, ',',maxNoStrings);
  Serial_Debug("\nLogInterval Set Received Value :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setIntervalValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_INTERVAL_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setIntervalValueStr[0], &(tempsetInterval.I1), IsintervalLimitValid);
    status &= SetParameterValue_int(setIntervalValueStr[1], &(tempsetInterval.P1), IsparameterLimitValid);     
    status &= SetParameterValue_int(setIntervalValueStr[2], &(tempsetInterval.I2), IsintervalLimitValid);
    status &= SetParameterValue_int(setIntervalValueStr[3], &(tempsetInterval.P2), IsparameterLimitValid);
    status &= SetParameterValue_int(setIntervalValueStr[4], &(tempsetInterval.I3), IsintervalLimitValid);
    status &= SetParameterValue_int(setIntervalValueStr[5], &(tempsetInterval.P3), IsparameterLimitValid);     
    status &= SetParameterValue_int(setIntervalValueStr[6], &(tempsetInterval.I4), IsintervalLimitValid);
    status &= SetParameterValue_int(setIntervalValueStr[7], &(tempsetInterval.P4), IsparameterLimitValid);
    status &= SetParameterValue_int(setIntervalValueStr[8], &(tempsetInterval.I5), IsintervalLimitValid);
    status &= SetParameterValue_int(setIntervalValueStr[9], &(tempsetInterval.P5), IsparameterLimitValid);     
    status &= SetParameterValue_int(setIntervalValueStr[10], &(tempsetInterval.I6), IsintervalLimitValid);
    status &= SetParameterValue_int(setIntervalValueStr[11], &(tempsetInterval.P6), IsparameterLimitValid);
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n LogInterval SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n LogInterval SetValue/s set : "); 
      *setInterval = tempsetInterval;
      Serial_Debug_Float(setInterval->I1,2);
      Serial_Debug(",");
      Serial_Debug_Float(setInterval->P1,1);
      Serial_Debug(",");
      Serial_Debug_Float(setInterval->I2,2);Serial_Debug_Float(setInterval->P2,1);
      Serial_Debug_Float(setInterval->I3,2);Serial_Debug_Float(setInterval->P3,1);
      Serial_Debug_Float(setInterval->I4,2);Serial_Debug_Float(setInterval->P4,1);
      Serial_Debug_Float(setInterval->I5,2);Serial_Debug_Float(setInterval->P5,1);
      Serial_Debug_Float(setInterval->I6,2);Serial_Debug_Float(setInterval->P6,1);
    }            
  }
  else
  {
    Serial_Debug("\n LogInterval SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetLogInterval (const char * jsonString)
{ 
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_LOG_INTERVAL);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;  
  stInterval* getInterval = GetSetIntervalLimits(); 
  char I1_Str[10],I2_Str[10],I3_Str[10],I4_Str[10],I5_Str[10],I6_Str[10];
  char P1_Str[10],P2_Str[10],P3_Str[10],P4_Str[10],P5_Str[10],P6_Str[10];
  ftoa(getInterval->I1,I1_Str,0); ftoa(getInterval->P1,P1_Str,0);  
  ftoa(getInterval->I2,I2_Str,0); ftoa(getInterval->P2,P2_Str,0);
  ftoa(getInterval->I3,I3_Str,0); ftoa(getInterval->P3,P3_Str,0);
  ftoa(getInterval->I4,I4_Str,0); ftoa(getInterval->P4,P4_Str,0);
  ftoa(getInterval->I5,I5_Str,0); ftoa(getInterval->P5,P5_Str,0);
  ftoa(getInterval->I6,I6_Str,0); ftoa(getInterval->P6,P6_Str,0); 
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",I1_Str,P1_Str,I2_Str,P2_Str,I3_Str,P3_Str,I4_Str,P4_Str,I5_Str,P5_Str,I6_Str,P6_Str);  
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetContinue (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetContinue->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR; 
  stContinue* setContinueValue;
  setContinueValue = GetSetContinueLimits();
  stContinue tempSetContinue = *setContinueValue;  
  char *setContinueValueStr[JSON_NO_OF_CONTINUE];
  char state; 
  uint8_t maxNoStrings=(sizeof setContinueValueStr)/4;   
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setContinueValueStr, ',',maxNoStrings);
  Serial_Debug("\nContinue Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setContinueValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_CONTINUE)
  {
    bool status = true;
    status &= SetParameterValue_int(setContinueValueStr[0],(uint32_t *)&(tempSetContinue.Continue1), IsContinue1Valid);    
    state = atoi(setContinueValueStr[0]);

    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Continue SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      *setContinueValue = tempSetContinue;
      Serial_Debug("\n Continue SetValue/s Set");
      Serial_Debug_Num(setContinueValue->Continue1);
    }     
    if(state == 0)
    {
      
    }
    if(state == 1)
    {
      if(GetRobotActualDirection() == POSITIVE)
      {
        SetRobotState(ROBOT_HOME_RETURN);
      }
    }
    if(state == 2)
    {
      
    }
  }
  else
  {
    Serial_Debug("\n Continue SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;   
  }
  return error_code;
}

static eJsonStatus ProcessGetContinueCount (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RETURN_STATE);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stContinue *setContinueValue = GetSetContinueLimits();
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d",setContinueValue->Continue1);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetReturnState (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetContinue->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR; 
  stReturn* setReturnValue;
  setReturnValue = GetSetReturnLimits();
  stReturn tempSetReturn = *setReturnValue;  
  char *setContinueValueStr[JSON_NO_OF_RETURN];
  char state1;
  uint8_t maxNoStrings = (sizeof setContinueValueStr)/4;  
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setContinueValueStr, ',',maxNoStrings);
  Serial_Debug("\nContinue Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setContinueValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_RETURN)
  {
    bool status = true;
    status &= SetParameterValue_int(setContinueValueStr[0],(uint32_t*)&(tempSetReturn.Return1), IsReturn1Valid);
    
    state1 = atoi(setContinueValueStr[1]);
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Continue SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      *setReturnValue = tempSetReturn;
      Serial_Debug("\n Continue SetValue/s Set");
      Serial_Debug_Num(setReturnValue->Return1);
     
    }     
    if(state1 == 0)
    {
        ClearRoboterror();
    }
        
  }
  else
  {
    Serial_Debug("\n Continue SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;   
  }
  return error_code;
}

static eJsonStatus ProcessGetReturnState (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CONTINUE_COUNT);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stReturn *setReturnValue = GetSetReturnLimits(); 
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%d",setReturnValue->Return1); 
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetcomdistance (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetPulseCount->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;   
  stcomdistance* setcomdistance = GetSetComDistanceLimits();
  stcomdistance tempSetComDistanceValues = *setcomdistance;  
  char *ComdistanceStr[JSON_NO_OF_COMDISTANCE];
  uint8_t maxNoStrings=(sizeof ComdistanceStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, ComdistanceStr, ',',maxNoStrings);
  Serial_Debug("\nPulse Count Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(ComdistanceStr[dispcount]);
    Serial_Debug(",");
  }  
  if(totalResultStringCount == JSON_NO_OF_COMDISTANCE)
  {
    bool status = true;
    
    pulse_flag = 3;
    
    ComDistance = atof(ComdistanceStr[0]);
    ComDistance_int = atoi(ComdistanceStr[0]); 
    
    if(ComDistance_int >= DEFAULT_COMDISTANCE)
    {
      PulseCalculation();
      Serial_Debug("\r\nIn Pulsecalc->");
      status &= SetParameterValue_int(ComPulse,(uint32_t*)&(tempSetComDistanceValues.CDistance), IsRTCvalueValid);
      
      if(status == false)
      {
        error_code = JSON_DATA_OUT_OF_RANGE;
        Serial_Debug("\n Pulse Count SetValue/s Out of Range");
      }
      else
      {
        error_code = JSON_NO_ERROR;
        Serial_Debug("\n Pulse Count SetValue/s set : ");
        *setcomdistance = tempSetComDistanceValues;
        Serial_Debug_Num(setcomdistance->CDistance);
        Serial_Debug(" ,");
      } 
    }
    else
    {
      Serial_Debug("\n Pulse Count SetValue/s Invalid");
      
       EepromSetcomdistance(EEPROM_READ);
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\n Pulse Count SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetcomdistance (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_COM_DISTANCE);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;  
  stcomdistance* getcomdistance = GetSetComDistanceLimits();
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d",ComDistance_int,getcomdistance->CDistance); 
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetPulseCountAndCycle (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetPulseCountAndCycle->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  stCountRange* setCountValues;
  setCountValues = GetSetCountRange();
  stCountRange tempSetCountValues = *setCountValues;
  char *PulseCountStr[JSON_NO_OF_PULSE_CYCLE_COUNT_DATA];
  uint8_t maxNoStrings=(sizeof PulseCountStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, PulseCountStr, ',',maxNoStrings);
  Serial_Debug("\nPulse Count Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(PulseCountStr[dispcount]);
    Serial_Debug(",");
  }  
  if(totalResultStringCount == JSON_NO_OF_PULSE_CYCLE_COUNT_DATA)
  {
    bool status = true;
    pulse_flag = 4;
    track_no = atoi(PulseCountStr[2]);
    day_count= atoi(PulseCountStr[3]);
    cleaner_in_comm = 1;
    stHeartbeatConfig* storeHeartbeatConfig = GetSetHeartbeatConfig();
    storeHeartbeatConfig->enable = 1;
    Cumulative_distance = 0;  
    CpositiveDistance = atof(PulseCountStr[0]);
    CpositiveDistance = CpositiveDistance*1000; 
    CnegativeDistance = atof(PulseCountStr[1]);
    CnegativeDistance = CnegativeDistance*1000;
    CnegativeDistance = CnegativeDistance+300; 
    CpositiveDistance_int = atoi(PulseCountStr[0]); 
    CpositiveDistance_int = CpositiveDistance_int*1000;
    CnegativeDistance_int = atoi(PulseCountStr[1]); 
    CnegativeDistance_int = CnegativeDistance_int*1000;
    CnegativeDistance_int = CnegativeDistance_int+300;
    PulseCalculation();
    status &= SetParameterValue_int(Cpospulse, (uint32_t*)&(tempSetCountValues.maxPosCountForPVsetup), IsPulseCountValid);
    status &= SetParameterValue_int(Cnegpulse, (uint32_t*)&(tempSetCountValues.maxNegCountForPVsetup) , IsPulseCountValid);  
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Pulse Count And Cycle SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Pulse Count And Cycle SetValue/s set : ");
      *setCountValues = tempSetCountValues;
      if((GetManualState()==MAN_STOP)||(GetManualState()==MAN_NORMAL_STOP))
      {
        SetCycleModeState(CYCLE_COMPLETED);
      } 
      SetMotionPauseState(PA_IDLE);
      ClearRoboterror();
      SetCycleModeState(CYCLE_COMPLETED);
      SetMotionCommand(ROBOT_CMD_CYCLE);
      uartflag = 4; 
      zigbeetcflag = 1;
      Serial_Debug_Num(setCountValues->maxPosCountForPVsetup);
      Serial_Debug(" ,");
      Serial_Debug_Num(setCountValues->maxNegCountForPVsetup);
    }            
  }
  else
  {
    Serial_Debug("\n Pulse Count And Cycle SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetErrorStatus (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CONTROLLER_STATUS);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;  
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%lX", GetMotorFaultCode());  
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessGetBatteryAndPVvalues (const char * jsonString)
{
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_BAT_AND_PV_VAL);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;  
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%0.2f,%0.2f,%0.2f,%0.2f,%d,%0.2f,%0.2f,%d,%d,%d,%d,%d",pv_vol,pv_curr,pv_power,battery_volt,batt_temp,batt_charg_curr,batt_discharg_curr,pv_work_status,Batt_SOC,charge_cont_temp,generated_energy,controller_status); 
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char *)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return JSON_NO_ERROR;
}

static eJsonStatus ProcessGetTemperaturevalues (const char * jsonString)
{
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  float Battemp_f,Drivetemp_f,Dietemp_f;
  Battemp_f = temp_read_A();
  ftoa(Battemp_f,Battemp_str,2);
  Drivetemp_f = temp_read_B();
  ftoa(Drivetemp_f,Drivetemp_str,2);
  Dietemp_f = temp_read_C();
  ftoa(Dietemp_f,Dietemp_str,2);
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount], &bufferLength, CMD_GET_TEMPERATURES);
  dataCount = dataCount+bufferLength; 
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%s,%s,%s",Battemp_str,Drivetemp_str,Dietemp_str);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return JSON_NO_ERROR;
}
static eJsonStatus RequestActivityLog (const char * jsonString)
{
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char *reqLogparaStr[JSON_NO_OF_REQUEST_LOG];
  int Parameterflash=0;
  uint8_t maxNoStrings=(sizeof reqLogparaStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char *)jsonString, reqLogparaStr, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");

  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqLogparaStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_REQUEST_LOG)
  {
    if(reqLogparaStr[0] != NULL)
    {  
      Parameterflash = atoi(reqLogparaStr[0]);  
      uint16_t dataCount=0;        
      txBuff[dataCount]=0;
      uint16_t bufferLength = sizeof(txBuff)-dataCount;
      ParseHeaderData(&txBuff[dataCount], &bufferLength, CMD_REQ_LOG);
      dataCount = dataCount+bufferLength; 
      bufferLength = sizeof(txBuff)-dataCount;   
      GetRTCDateTimeString((char*)&getTime_str, sizeof(getTime_str));
      memset(data_read_buffer2,'\0',sizeof(data_read_buffer2));
      memset(data_read_buffer,'\0',sizeof(data_read_buffer));
      
       if(Parameterflash == 1)
      {       
        Flash_readall_log(CYLCE_LOG);
        error_code = JSON_NO_ERROR;
      }
      else if(Parameterflash == 2)
      {         
        Flash_readall_log(ERROR_LOG);
        error_code = JSON_NO_ERROR; 
      }
      else if(Parameterflash == 3)
      { 
        Flash_clearlog(CYLCE_LOG);
        Flash_clearlog(ERROR_LOG);
        error_code = JSON_NO_ERROR;
      } 
      ClearTxBuff();
    }
    else
    {
      Serial_Debug("\nRequest Logs Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Logs Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }  
  return error_code;
}


static eJsonStatus RequestLog (const char * jsonString)
{
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char *reqLogparaStr[JSON_NO_OF_REQUEST_LOG];
  int Parameter=0;
  uint8_t maxNoStrings=(sizeof reqLogparaStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char *)jsonString, reqLogparaStr, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");

  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqLogparaStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_REQUEST_LOG)
  {
    if(reqLogparaStr[0] != NULL)
    {  
      Parameter = atoi(reqLogparaStr[0]);  
      uint16_t dataCount=0;          
      txBuff[dataCount]=0;
      uint16_t bufferLength = sizeof(txBuff)-dataCount; 
      ParseHeaderData(&txBuff[dataCount], &bufferLength, CMD_REQ_LOG);
      dataCount = dataCount+bufferLength; 
      bufferLength = sizeof(txBuff)-dataCount;   
      GetRTCDateTimeString((char*)&getTime_str, sizeof(getTime_str));
      
      if(Parameter == 1)
      {       
        GETLog_Temperature();
        error_code = JSON_NO_ERROR;
      }
      else if(Parameter == 2)
      {         
        GETLog_SOC();
        error_code = JSON_NO_ERROR; 
      }
      else if(Parameter == 3)
      { 
        GETLog_MOTORCRNT();
        error_code = JSON_NO_ERROR;
      }
      
      else if(Parameter == 4)
      {  
        GETLog_AUTOCYCLE();
        error_code = JSON_NO_ERROR;
      }
      
      else if(Parameter == 5)
      {     
        GETLog_ERROR();
        error_code = JSON_NO_ERROR;  
      }
      else if(Parameter == 6)
      {          
           Delete_SDdata();  
           SD_MOUNT_STATUS = 1;   
      }
      ClearTxBuff();
    }
    else
    {
      Serial_Debug("\nRequest Logs Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Logs Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }  
  return error_code;
}


eJsonStatus ExtractDatafromzigbee(uint8_t* data,uint8_t size,stMessage* finalMessage)
{
  uint16_t startAddress = 0;
  uint16_t destAddress = 0;
  Serial_Debug("\nExtract Data");
  Serial_Debug("");
  for(startAddress = 0;((data[startAddress]!=STARTID)&&(startAddress<size));startAddress++);
  if(data[startAddress]!=STARTID)
  {
    Serial_Debug("START_CHAR not found");
    return JSON_PARSE_ERROR;
  }
  startAddress++;
  uint8_t tempBuffer[100];
  memset(tempBuffer,0,sizeof(tempBuffer));
  for(destAddress = 0; (data[startAddress]!=DELIMITER_1)&&(destAddress<8);startAddress++,destAddress++)
  {   
    tempBuffer[destAddress] = data[startAddress];
  }
  Serial_Debug("\nDevice ID-->");
  Serial_Debug((char*)tempBuffer);
  finalMessage->deviceID = 0;
  for(int i=0;i<8;i++)
  {
     finalMessage->deviceID |= tempBuffer[i];
     if(i<7)
     {
        finalMessage->deviceID <<= 8;
     }
  }
  startAddress++;
  for(startAddress = 0;((data[startAddress]!=DELIMITER_1)&&(startAddress<size));startAddress++);
  memset(tempBuffer,0,sizeof(tempBuffer));
  startAddress++;
  for(destAddress = 0; (data[startAddress]!=DELIMITER_2)&&(destAddress<4);startAddress++,destAddress++)
  {  
    tempBuffer[destAddress] = data[startAddress];
  }
  if(data[startAddress]!=DELIMITER_2)
  {
    Serial_Debug("DELIMITER2 not found");
    return JSON_PARSE_ERROR;
  }
  Serial_Debug("\nCommand-->");Serial_Debug((char*)tempBuffer);
  finalMessage->CMD = strtol((const char *)tempBuffer,NULL,16);
  Serial_Debug_Num(finalMessage->CMD);
  startAddress++;
  memset(tempBuffer,0,sizeof(tempBuffer));
  for(destAddress = 0; (data[startAddress]!=STOP_CHAR)&&(startAddress<size);startAddress++,destAddress++)
  {
    tempBuffer[destAddress] = data[startAddress];
  }
  if(data[startAddress]!=STOP_CHAR)
  {
    Serial_Debug("STOP_CHAR not found");
    return JSON_PARSE_ERROR;
  }
  Serial_Debug("\nValues-->");Serial_Debug((char*)tempBuffer);
  snprintf((char *)finalMessage->values,100,"%s",tempBuffer);
  char* tempValues[MAX_NO_OF_VALUES];
  uint8_t totalNoOfStr = SplitString((char *)tempBuffer,tempValues,',',MAX_NO_OF_VALUES);
  Serial_Debug("\r\ntotalNoOfStr-->");
  Serial_Debug_Num(totalNoOfStr);
  if(totalNoOfStr == 0)
  {
    return JSON_INVALID_DATA;
  } 
  for(int i=0; i<totalNoOfStr;i++)
  {
    Serial_Debug("\r\nValue_");
    Serial_Debug_Num(i+1);
    Serial_Debug("-->");
    Serial_Debug(tempValues[i]);
  }
  finalMessage->CRC = strtol(tempValues[totalNoOfStr-1],NULL,16);
  Serial_Debug("\r\nCRC-->");
  Serial_Debug_Num(finalMessage->CRC);
  return JSON_NO_ERROR;
}

void SD_SOC_Start(void)
{
    char getTime_str1[40];
    eRobotDirection robotState = GetRobotActualDirection();
    memset(schedule_log,'\0',sizeof(schedule_log));
    snprintf(schedule_log,3,"%d,",robotState);
    GetRTCDateTimeString(getTime_str1, sizeof(getTime_str1));        
    strcat(schedule_log,getTime_str1);
    strcat(schedule_log,",");
    float startSOC = 0;
    char startSoc_Str[10];
    ftoa(startSOC,startSoc_Str,2);
    strcat(schedule_log,startSoc_Str);
    strcat(schedule_log,",");
}

void SD_SOC_End(void)
{
    char getTime_str1[40],Error_Cnt[2];
    memset(Error_Cnt,'\0',2);
    Error_Cnt[0] =  (Robot_Error_Count1) + 0x30; 
    Robot_Error_Count1 = 0; 
    GetRTCDateTimeString(getTime_str1, sizeof(getTime_str1));
    strcat(schedule_log,getTime_str1);
    strcat(schedule_log,",");
    float endSOC = 0;
    char startSoc_Str[10];
    ftoa(endSOC,startSoc_Str,2);
    strcat(schedule_log,startSoc_Str);
    strcat(schedule_log,",");
    strcat(schedule_log,Error_Cnt);
}

void SD_SOC_Start_21(void)
{
    char getTime_str1[40];
    memset(schedule_log_21,'\0',sizeof(schedule_log_21));
    memset(cyclelog,'\0',sizeof(cyclelog));     
    strcat(schedule_log_21,"Cycle Start Time: ");
    GetRTCDateTimeString(getTime_str1, sizeof(getTime_str1));       
    strcat(schedule_log_21,getTime_str1);
    strcat(schedule_log_21,",");
    strcat(cyclelog,getTime_str1);
    strcat(cyclelog,",");
    strcpy(Cstarttime,getTime_str1);

    float startSOC = 0;
    char startSoc_Str[10];
    ftoa(startSOC,startSoc_Str,2);
    strcat(cyclelog,startSoc_Str);
    strcat(cyclelog,",");
    strcpy(Cstartsoc,startSoc_Str);
}

void SD_SOC_End_21(void)
{
    char getTime_str1[40],Error_Cnt[2];
    memset(Error_Cnt,'\0',2);
    Error_Cnt[0] =  (Robot_Error_Count1) + 0x30; 
    Robot_Error_Count1 = 0; 
    GetRTCDateTimeString(getTime_str1, sizeof(getTime_str1));
    strcat(schedule_log_21," End Time: ");
    strcat(schedule_log_21,getTime_str1);
    strcat(cyclelog,getTime_str1);
    strcat(cyclelog,",");
    strcpy(Cendtime,getTime_str1);
    
    float endSOC = 0;
    char startSoc_Str[10];
    ftoa(endSOC,startSoc_Str,2);
    strcat(cyclelog,startSoc_Str);
    strcat(cyclelog,",");
    strcpy(Cendsoc,startSoc_Str);
    
    strcat(cyclelog,Error_Cnt);
    strcpy(Cerrorcount,Error_Cnt);
    schedule_BYTE = 1;
    schedule_BYTE_flashlog = 1;
}

static eJsonStatus ProcessSetTrackchangerMAC (const char * jsonString)
{
  Serial_Debug("\r\nIn Process Set Cleaner MAC->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  unChangerMAC* setChangerMAC;
  setChangerMAC = GetSetChangerMAC();
  unChangerMAC tempSetChangerMAC = *setChangerMAC;
  char *ChangerMACdataStr[JSON_NO_OF_CHANGER_MAC_DATA];
  uint8_t maxNoStrings=(sizeof ChangerMACdataStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, ChangerMACdataStr, ',',maxNoStrings);
  Serial_Debug("\nCleaner MAC Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(ChangerMACdataStr[dispcount]);
    Serial_Debug(",");
  }  
  if((totalResultStringCount == JSON_NO_OF_CHANGER_MAC_DATA)&&(strlen(ChangerMACdataStr[0])!=0))
  {
    bool status = true;
    if(strlen(ChangerMACdataStr[0])>=MAC_ID_STR_LEN)     
    {
      status = false;
    }
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Cleaner MAC SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Cleaner MAC SetValue/s set : ");
      tempSetChangerMAC.changerMAC_64 = HexStringtoInt((uint8_t*)ChangerMACdataStr[0]);
      *setChangerMAC = tempSetChangerMAC;
      char dispstring[20];
      uint16_t length = sizeof dispstring;
      Int64ToHexString(setChangerMAC->changerMAC_64,dispstring,&length);
      Serial_Debug(dispstring);
    }            
  }
  else
  {
    Serial_Debug("\n Cleaner MAC SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetTrackchangerMAC (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CHANGER_MAC);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  unChangerMAC *getSetChangerMAC = GetSetChangerMAC();
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%X%X%X%X",getSetChangerMAC->changerMAC_32[3],getSetChangerMAC->changerMAC_32[2],getSetChangerMAC->changerMAC_32[1],getSetChangerMAC->changerMAC_32[0]);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetRelaytime (const char * jsonString)
{ 
  Serial_Debug("\r\nIn ProcessSetLogInterval->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR; 
  strelay* setrelay = GetSetrelayLimits();
  strelay tempsetrelay = *setrelay; 
  char* setRelayValueStr[JSON_NO_OF_RELAY_DATA ];  
  uint8_t maxNoStrings=(sizeof setRelayValueStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setRelayValueStr, ',',maxNoStrings);
  Serial_Debug("\nLogInterval Set Received Value :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setRelayValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_RELAY_DATA )
  {
    bool status = true;
    status &= SetParameterValue_int(setRelayValueStr[0], &(tempsetrelay.T1), IsintervalLimitValid);
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n LogInterval SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n LogInterval SetValue/s set : "); 
      *setrelay = tempsetrelay;
      Serial_Debug_Float(setrelay->T1,2);
    }            
  }
  else
  {
    Serial_Debug("\n LogInterval SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetRelaytime (const char * jsonString)
{ 
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RELAY_INTERVAL);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;  
  strelay* getrelay = GetSetrelayLimits(); 
  char T1_Str[10];
  ftoa(getrelay->T1,T1_Str,0); 
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%s",T1_Str);  
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}
