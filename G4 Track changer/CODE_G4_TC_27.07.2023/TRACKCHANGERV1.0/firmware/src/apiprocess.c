#include "main.h"

static uint8_t txBuff[MAX_UART_TX_BUFFER_LENGTH];
static uint8_t txAckBuff[MAX_ACK_TX_BUFFER_LENGTH];

static eJsonStatus AssignJsonCommand (uint16_t  presentCommand_Hex); 
static eJsonStatus ProcessJsonCommand (const uint8_t * jsonString, uint16_t presentCommand_Hex);
static eJsonStatus ParseRxdata(stMessage* message);
static const stJsonCommand * presentJsonCommandParams = NULL; 

static eJsonStatus ProcessSetMode (const char * jsonString);
static eJsonStatus ProcessGetMode (const char * jsonString);

static eJsonStatus ProcessSetMotorPWMManual (const char * jsonString);
static eJsonStatus ProcessGetMotorPWMManual (const char * jsonString);

static eJsonStatus ProcessSetMotorPWMAuto (const char * jsonString);
static eJsonStatus ProcessGetMotorPWMAuto (const char * jsonString);

static eJsonStatus ProcessSetManualDirection (const char * jsonString);
static eJsonStatus ProcessGetMotorDirectionManual (const char * jsonString);

static eJsonStatus ProcessSetAutoScheduleTime (const char * jsonString);
static eJsonStatus ProcessGetAutoScheduleTime (const char * jsonString);

static eJsonStatus ProcessSetEdgeSensorState (const char * jsonString);
static eJsonStatus ProcessGetEdgeSenseEnabledState (const char * jsonString);

static eJsonStatus ProcessSetRTCValue (const char * jsonString);
static eJsonStatus ProcessGetRTCValue (const char * jsonString);

static eJsonStatus ProcessSetDeviceInfo (const char * jsonString);
static eJsonStatus ProcessGetDeviceInfo (const char * jsonString);

static eJsonStatus ProcessSetLowBatSoC (const char * jsonString);
static eJsonStatus ProcessGetLowBatSoC (const char * jsonString);

static eJsonStatus ProcessSetPulseCount (const char * jsonString);
static eJsonStatus ProcessGetPulseCountValues (const char * jsonString);

static eJsonStatus ProcessSetHeartbeatConfig (const char * jsonString);
static eJsonStatus ProcessGetHeartbeatConfig (const char * jsonString);

static eJsonStatus ProcessGetChargerState (const char * jsonString);
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

static eJsonStatus ProcessSetTrackInfo (const char * jsonString);
static eJsonStatus ProcessGetTrackInfo (const char * jsonString);

static eJsonStatus ProcessSetTrackInfo2 (const char * jsonString);
static eJsonStatus ProcessGetTrackInfo2 (const char * jsonString);

static eJsonStatus ProcessSetCleanerMAC (const char * jsonString);
static eJsonStatus ProcessGetCleanerMAC (const char * jsonString);

static eJsonStatus ProcessSetEncoderPulseTolerance (const char * jsonString);
static eJsonStatus ProcessGetEncoderPulseTolerance (const char * jsonString);

static eJsonStatus ProcessSetTcControlMode (const char * jsonString);
static eJsonStatus ProcessGetTcControlMode (const char * jsonString);

static eJsonStatus ProcessSetRfidData (const char * jsonString);
static eJsonStatus ProcessGetRfidData (const char * jsonString);

static eJsonStatus ProcessSetRfidData2 (const char * jsonString);
static eJsonStatus ProcessGetRfidData2 (const char * jsonString);

static eJsonStatus ProcessSetRfidData3 (const char * jsonString);
static eJsonStatus ProcessGetRfidData3 (const char * jsonString);

static eJsonStatus ProcessSetRfidState (const char * jsonString);
static eJsonStatus ProcessGetRfidState (const char * jsonString);

static eJsonStatus ProcessSetNoofRowday (const char * jsonString);
static eJsonStatus ProcessGetNoofRowday (const char * jsonString);

static eJsonStatus ProcessSetRowLengthData (const char * jsonString);
static eJsonStatus ProcessGetRowLengthData (const char * jsonString);

static eJsonStatus ProcessSetRowLength2Data (const char * jsonString);
static eJsonStatus ProcessGetRowLength2Data (const char * jsonString);

static eJsonStatus ProcessSetRowLength3Data (const char * jsonString);
static eJsonStatus ProcessGetRowLength3Data (const char * jsonString);

static eJsonStatus ProcessSetRowLength4Data (const char * jsonString);
static eJsonStatus ProcessGetRowLength4Data (const char * jsonString);

static eJsonStatus ProcessSetRowLength5Data (const char * jsonString);
static eJsonStatus ProcessGetRowLength5Data (const char * jsonString);

static eJsonStatus ProcessSetRowLength6Data (const char * jsonString);
static eJsonStatus ProcessGetRowLength6Data (const char * jsonString);

static eJsonStatus ProcessSetRowLength7Data (const char * jsonString);
static eJsonStatus ProcessGetRowLength7Data (const char * jsonString);

static eJsonStatus ProcessSetRelaytime (const char * jsonString);
static eJsonStatus ProcessGetRelaytime (const char * jsonString);

static eJsonStatus ClearTrackInfo (const char * jsonString);
static eJsonStatus ProcessGetSensorStatus (const char * jsonString);
static eJsonStatus ProcessClearErrorCodes (const char * jsonString);
static eJsonStatus ProcessGetBatteryAndPVvalues (const char * jsonString);
static eJsonStatus RequestLog (const char * jsonString);
static eJsonStatus ProcessResetToDefaults (const char * jsonString);
static eJsonStatus RequestActivityLog (const char * jsonString);

//static eJsonStatus ProcessGetTemperaturevalues (const char * jsonString);
//static eJsonStatus ProcessGetCurrentRobotStat (const char * jsonString);
//static eJsonStatus ProcessGetMotorCurrentValues (const char * jsonString);
//static eJsonStatus ProcessSetZigbeeConfig (const char * jsonString);
//static eJsonStatus ProcessGetZigbeeConfig (const char * jsonString);
static eJsonStatus ProcessResetZigbeeNetwork (const char * jsonString);
//static eJsonStatus ProcessGetZigbeeNetworkParams (const char * jsonString);
//static eJsonStatus ClearActivityLog (const char * jsonString);

static eJsonStatus ProcessSetState (const char * jsonString, bool* tempSetBool);
static eJsonStatus ProcessSetMotorPwm (const char * jsonString, stRobotPwmParam *tempSetPwmParam);
static bool ParseHeaderData(uint8_t* tempBuff, uint16_t* headerBufferLength, uint16_t command );

eJsonStatus motor_error_code;
eJsonStatus linear_error_code;
eJsonStatus edgesense_error_code;
eJsonStatus rotatesense_error_code;

extern eEepromStatus InitialiseAlltoDefault(stJsonCommand *jsonCmdParams, uint16_t commandParamsTotalCount);

typedef bool (*CheckValidity_Ptr_float)(float value);
typedef bool (*CheckValidity_Ptr_int)(uint32_t value);
typedef bool (*CheckValidity_Ptr_double)(double value);
typedef bool (*CheckValidity_Ptr_bool)(bool value);

static bool  SetParameterValue_bool(const char * inputString, bool * number, CheckValidity_Ptr_bool ExecuteCheckValidity);
static bool  SetParameterValue_int(const char * inputString, uint32_t * number, CheckValidity_Ptr_int ExecuteCheckValidity);
//static bool  SetParameterValue_float(const char * inputString, float * number, CheckValidity_Ptr_float ExecuteCheckValidity);
//static bool  SetParameterValue_double(const char * inputString, double * number, CheckValidity_Ptr_double ExecuteCheckValidity);

stJsonCommand jsonCommandParams[] = {
  {CMD_NUM_SET_MODE,                            CMD_SET_MODE,                   ProcessSetMode,                 EepromSetMode,               SetDefaultOperationMode},
  {CMD_NUM_GET_MODE,                            CMD_GET_MODE,                   ProcessGetMode,                 EepromDataNil,               SetDefaultNil},
  {CMD_NUM_SET_MOTOR_DIR_MANUAL,                CMD_SET_MOTOR_DIR_MANUAL,       ProcessSetManualDirection,      EepromDataNil,               SetDefaultNil},
  {CMD_NUM_GET_MOTOR_DIR_MANUAL,                CMD_GET_MOTOR_DIR_MANUAL,       ProcessGetMotorDirectionManual, EepromDataNil,               SetDefaultNil},  
  {CMD_NUM_SET_MOTOR_PWM_MANUAL,                CMD_SET_MOTOR_PWM_MANUAL,       ProcessSetMotorPWMManual,       EepromSetMotorPwmManualMode,     SetDefaultPwmManulModeParameter},
  {CMD_NUM_GET_MOTOR_PWM_MANUAL,                CMD_GET_MOTOR_PWM_MANUAL,       ProcessGetMotorPWMManual,       EepromDataNil,                SetDefaultNil},  
  {CMD_NUM_SET_MOTOR_PWM_AUTO,                  CMD_SET_MOTOR_PWM_AUTO,         ProcessSetMotorPWMAuto,         EepromSetMotorPwmAutoMode,      SetDefaultPwmCycleAutoModeParameter},
  {CMD_NUM_GET_MOTOR_PWM_AUTO,                  CMD_GET_MOTOR_PWM_AUTO,         ProcessGetMotorPWMAuto,         EepromDataNil,                  SetDefaultNil},
  {CMD_NUM_SET_AUTO_SCHEDULE_TIME,              CMD_SET_AUTO_SCHEDULE_TIME,     ProcessSetAutoScheduleTime,     EepromSetAutoScheduleTime,      SetDefaultAutoScheduledTime},
  {CMD_NUM_GET_AUTO_SCHEDULE_TIME,              CMD_GET_AUTO_SCHEDULE_TIME,     ProcessGetAutoScheduleTime,     EepromDataNil,                  SetDefaultNil},
  {CMD_NUM_SET_EDGE_SENSE_STATE,                CMD_SET_EDGE_SENSE_STATE,       ProcessSetEdgeSensorState,      EepromSetEdgeSensorState,      SetDefaultEdgeSensorEnabledState},
  {CMD_NUM_GET_EDGE_SENSE_STATE,                CMD_GET_EDGE_SENSE_STATE,       ProcessGetEdgeSenseEnabledState,       EepromDataNil,           SetDefaultNil},
  {CMD_NUM_SET_RTC_VALUE,                       CMD_SET_RTC_VALUE,              ProcessSetRTCValue,             EepromDataNil,                 SetDefaultNil},
  {CMD_NUM_GET_RTC_VALUE,                       CMD_GET_RTC_VALUE,              ProcessGetRTCValue,             EepromDataNil,                   SetDefaultNil},  
  {CMD_NUM_SET_PULSE_COUNT_VALUES,              CMD_SET_PULSE_COUNT_VALUES,     ProcessSetPulseCount,           EepromSetPulseCount,          SetDefaultCountRange}, 
  {CMD_NUM_GET_PULSE_COUNT_VALUES,              CMD_GET_PULSE_COUNT_VALUES,     ProcessGetPulseCountValues,     EepromDataNil,               SetDefaultNil},
  {CMD_NUM_SET_LOW_BAT_SOC,                     CMD_SET_LOW_BAT_SOC,            ProcessSetLowBatSoC,            EepromSetLowBatSoC,           SetDefaultMinBatSoc},
  {CMD_NUM_GET_LOW_BAT_SOC,                     CMD_GET_LOW_BAT_SOC,            ProcessGetLowBatSoC,            EepromDataNil,                 SetDefaultNil},
  {CMD_NUM_SET_HEARTBEAT_CONFIG,                CMD_SET_HEARTBEAT_CONFIG,       ProcessSetHeartbeatConfig,      EepromHeartbeatConfiguration,SetDefaultHeartbeatConfig},  
  {CMD_NUM_GET_HEARTBEAT_CONFIG,                CMD_GET_HEARTBEAT_CONFIG,       ProcessGetHeartbeatConfig,      EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_LOG_INTERVAL,                    CMD_SET_LOG_INTERVAL,           ProcessSetLogInterval,          EepromSetLogInterval,       SetDefaultLogInterval},      
  {CMD_NUM_GET_LOG_INTERVAL,                    CMD_GET_LOG_INTERVAL,           ProcessGetLogInterval,          EepromDataNil,              SetDefaultNil}, 
  {CMD_NUM_SET_DIA_WHEEL,                       CMD_SET_DIA_WHEEL,              ProcessSetWheelDia,             EepromSetWheelDia,          SetDefaultWheelDia},        
  {CMD_NUM_GET_DIA_WHEEL,                       CMD_GET_DIA_WHEEL,              ProcessGetWheelDia,             EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_CYCLE_FREQUENCY,                 CMD_SET_CYCLE_FREQUENCY,        ProcessSetCycleFrequency,       EepromSetCycleFrequency,    SetDefaultCycleFrequency},   
  {CMD_NUM_GET_CYCLE_FREQUENCY,                 CMD_GET_CYCLE_FREQUENCY,        ProcessGetCycleFrequency,       EepromDataNil,              SetDefaultNil},   
  {CMD_NUM_SET_CONTINUE_COUNT,                  CMD_SET_CONTINUE_COUNT,         ProcessSetContinue,             EepromSetContinue,          SetDefaultContinue},   
  {CMD_NUM_GET_CONTINUE_COUNT,                  CMD_GET_CONTINUE_COUNT,         ProcessGetContinueCount,        EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_EMERGENCY_RETURN,                CMD_SET_RETURN_STATE,           ProcessSetReturnState,          EepromSetReturnState,       SetDefaultReturnState},   
  {CMD_NUM_GET_RETURN_STATE,                    CMD_GET_RETURN_STATE,           ProcessGetReturnState,          EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_COMMDISTANCE,                    CMD_SET_RUNRFID,                ProcessSetcomdistance,          EepromSetcomdistance,       SetDefaultcomdistance},   
  {CMD_NUM_GET_COMMDISTANCE,                    CMD_GET_RUNRFID,                ProcessGetcomdistance,          EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_N0_ROW_DAY,                      CMD_SET_N0_ROW_DAY,             ProcessSetNoofRowday,           EepromSetNoofRowday,        SetDefaultNoofRowday},   
  {CMD_NUM_GET_N0_ROW_DAY,                      CMD_GET_N0_ROW_DAY,             ProcessGetNoofRowday,           EepromDataNil,              SetDefaultNil}, 
  {CMD_NUM_SET_CLEANER_MAC,                     CMD_SET_CLEANER_MAC,            ProcessSetCleanerMAC,           EepromStoreCleanerMAC,                  SetDefaultNil},
  {CMD_NUM_GET_CLEANER_MAC,                     CMD_GET_CLEANER_MAC,            ProcessGetCleanerMAC,           EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_ENC_PULSE_TOLERANCE,             CMD_SET_ENC_PULSE_TOLERANCE,    ProcessSetEncoderPulseTolerance,EepromStoreEncoderPulseTolerance,       SetDefaultEncoderPulseTolerance},
  {CMD_NUM_GET_ENC_PULSE_TOLERANCE,             CMD_GET_ENC_PULSE_TOLERANCE,    ProcessGetEncoderPulseTolerance,EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_TC_CONTROL_MODE,                 CMD_SET_TC_CONTROL_MODE,        ProcessSetTcControlMode,        EepromSetTcControlMode,                 SetDefaultTcControlMode},
  {CMD_NUM_SET_TC_CONTROL_MODE,                 CMD_GET_TC_CONTROL_MODE,        ProcessGetTcControlMode,        EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_TRACK_INFO,                       CMD_SET_TRACK_INFO,            ProcessSetTrackInfo,            EepromStoreTrackInfo,                   SetDefaultTrackInfo},
  {CMD_NUM_GET_TRACK_INFO,                       CMD_GET_TRACK_INFO,            ProcessGetTrackInfo,            EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_TRACK_INFO2,                      CMD_SET_TRACK_INFO2,           ProcessSetTrackInfo2,            EepromStoreTrackInfo2,                   SetDefaultTrackInfo2},
  {CMD_NUM_GET_TRACK_INFO2,                      CMD_GET_TRACK_INFO2,           ProcessGetTrackInfo2,            EepromDataNil,                          SetDefaultNil},  
  {CMD_NUM_CLEAR_TRACK_INFO,                    CMD_CLEAR_TRACK_INFO,           ClearTrackInfo,                 EepromStoreTrackInfo,                   SetDefaultNil},
  {CMD_NUM_SET_RFID_STATE,                      CMD_SET_RFID_STATE,             ProcessSetRfidState,            EepromStoreRfidState,                   SetDefaultRfidState},  
  {CMD_NUM_GET_RFID_STATE,                      CMD_GET_RFID_STATE,             ProcessGetRfidState,            EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_RFID_DATA,                       CMD_SET_RFID_DATA,              ProcessSetRfidData,             EepromStoreRfidData,                    SetDefaultRfidData},  
  {CMD_NUM_GET_RFID_DATA,                       CMD_GET_RFID_DATA,              ProcessGetRfidData,             EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_RFID_DATA2,                       CMD_SET_RFID_DATA2,            ProcessSetRfidData2,             EepromStoreRfidData2,                    SetDefaultRfidData2},  
  {CMD_NUM_GET_RFID_DATA2,                       CMD_GET_RFID_DATA2,            ProcessGetRfidData2,             EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_RFID_DATA3,                       CMD_SET_RFID_DATA3,            ProcessSetRfidData3,             EepromStoreRfidData3,                    SetDefaultRfidData3},  
  {CMD_NUM_GET_RFID_DATA3,                       CMD_GET_RFID_DATA3,            ProcessGetRfidData3,             EepromDataNil,                          SetDefaultNil},   
  {CMD_NUM_SET_ROW_LENGTH_DATA,                 CMD_SET_ROW_LENGTH_DATA,        ProcessSetRowLengthData,        EepromStoreRowLength,       SetDefaultRowLength},
  {CMD_NUM_GET_ROW_LENGTH_DATA,                 CMD_GET_ROW_LENGTH_DATA,        ProcessGetRowLengthData,        EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_ROW_LENGTH2_DATA,                CMD_SET_ROW_LENGTH2_DATA,       ProcessSetRowLength2Data,       EepromStoreRowLength2,      SetDefaultRowLength2},
  {CMD_NUM_GET_ROW_LENGTH2_DATA,                CMD_GET_ROW_LENGTH2_DATA,       ProcessGetRowLength2Data,       EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_ROW_LENGTH3_DATA,                CMD_SET_ROW_LENGTH3_DATA,       ProcessSetRowLength3Data,       EepromStoreRowLength3,      SetDefaultRowLength3},
  {CMD_NUM_GET_ROW_LENGTH3_DATA,                CMD_GET_ROW_LENGTH3_DATA,       ProcessGetRowLength3Data,       EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_ROW_LENGTH4_DATA,                CMD_SET_ROW_LENGTH4_DATA,       ProcessSetRowLength4Data,       EepromStoreRowLength4,      SetDefaultRowLength4},
  {CMD_NUM_GET_ROW_LENGTH4_DATA,                CMD_GET_ROW_LENGTH4_DATA,       ProcessGetRowLength4Data,       EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_ROW_LENGTH5_DATA,                CMD_SET_ROW_LENGTH5_DATA,       ProcessSetRowLength5Data,       EepromStoreRowLength5,      SetDefaultRowLength5},
  {CMD_NUM_GET_ROW_LENGTH5_DATA,                CMD_GET_ROW_LENGTH5_DATA,       ProcessGetRowLength5Data,       EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_ROW_LENGTH6_DATA,                CMD_SET_ROW_LENGTH6_DATA,       ProcessSetRowLength6Data,       EepromStoreRowLength6,      SetDefaultRowLength6},
  {CMD_NUM_GET_ROW_LENGTH6_DATA,                CMD_GET_ROW_LENGTH6_DATA,       ProcessGetRowLength6Data,       EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_ROW_LENGTH7_DATA,                CMD_SET_ROW_LENGTH7_DATA,       ProcessSetRowLength7Data,       EepromStoreRowLength7,      SetDefaultRowLength7},
  {CMD_NUM_GET_ROW_LENGTH7_DATA,                CMD_GET_ROW_LENGTH7_DATA,       ProcessGetRowLength7Data,       EepromDataNil,              SetDefaultNil}, 
  {CMD_NUM_SET_DEVICE_INFO,                     CMD_SET_DEVICE_INFO,            ProcessSetDeviceInfo,           EepromDevIDInfo,               SetDefaultDevIDInfo},
  {CMD_NUM_GET_DEVICE_INFO,                     CMD_GET_DEVICE_INFO,            ProcessGetDeviceInfo,           EepromDataNil,                SetDefaultNil},
  {CMD_NUM_GET_SENSOR_STATUS,                   CMD_GET_SENSOR_STATUS,          ProcessGetSensorStatus,         EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_CLEAR_ERRORCODES,                    CMD_CLEAR_ERRORCODES,           ProcessClearErrorCodes,         EepromDataNil,               SetDefaultNil},
  {CMD_NUM_RESET_TO_DEFAULTS,                   CMD_RESET_TO_DEFAULTS,          ProcessResetToDefaults,         EepromDataNil,              SetDefaultNil},  
  {CMD_NUM_GET_CURRENT_STAT,                    CMD_GET_CURRENT_STAT,           ProcessGetCurrentStat,          EepromDataNil,                SetDefaultNil},
  {CMD_NUM_GET_LAST_CYCLE_STAT,                 CMD_GET_LAST_CYCLE_STAT,        ProcessGetLastOperationStat,    EepromDataNil,                SetDefaultNil},
  {CMD_NUM_GET_CHARGER_STATE,                   CMD_GET_CHARGER_STATE,          ProcessGetChargerState,         EepromDataNil,                SetDefaultNil},
  {CMD_NUM_GET_CONTROLLER_STATUS,               CMD_GET_CONTROLLER_STATUS,      ProcessGetErrorStatus,          EepromDataNil,                 SetDefaultNil},
  {CMD_NUM_GET_BAT_AND_PV_VAL,                  CMD_GET_BAT_AND_PV_VAL,         ProcessGetBatteryAndPVvalues,   EepromDataNil,              SetDefaultNil},  
  {CMD_NUM_SET_RELAY_INTERVAL,                  CMD_SET_RELAY_INTERVAL,         ProcessSetRelaytime,          EepromSetrelaytime,       SetDefaultrelaytime},      
  {CMD_NUM_GET_RELAY_INTERVAL,                  CMD_GET_RELAY_INTERVAL,         ProcessGetRelaytime,          EepromDataNil,              SetDefaultNil},
  {CMD_NUM_REQ_LOG,                             CMD_REQ_LOG,                    RequestLog,                     EepromDataNil,              SetDefaultNil},
  {CMD_NUM_REQ_ACTIVITY_LOG,                    CMD_REQ_ACTIVITY_LOG,           RequestActivityLog,             EepromDataNil,              SetDefaultNil},
//  {CMD_NUM_SET_ZIGBEE_CONF,                     CMD_SET_ZIGBEE_CONF,            ProcessSetZigbeeConfig,         EepromZigbeeConfiguration,     SetDefaultZigbeeConfig}, 
//  {CMD_NUM_GET_ZIGBEE_CONF,                     CMD_GET_ZIGBEE_CONF,            ProcessGetZigbeeConfig,         EepromDataNil,              SetDefaultNil},  
//  {CMD_NUM_CLEAR_ACTIVITY_LOG,                  CMD_CLEAR_ACTIVITY_LOG,         ClearActivityLog,               EepromDataNil,              SetDefaultNil},  
  {CMD_NUM_RESET_ZIGBEE_NTW,                    CMD_RESET_ZIGBEE_NTW,           ProcessResetZigbeeNetwork,      EepromDataNil,              SetDefaultNil},  
//  {CMD_NUM_GET_ZIGBEE_NTW_PARAM,                CMD_GET_ZIGBEE_NTW_PARAM,       ProcessGetZigbeeNetworkParams,  EepromDataNil,              SetDefaultNil},
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
  ClearTxBuff();
}

eJsonStatus ProcessReceivedJsonData (uint8_t * jsonReceivedData,uint16_t size)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  eJsonStatus parseErrorCode = error_code;
  stMessage message;
  if(uartflag == 1)
  {
    error_code = ExtractData(jsonReceivedData,size,&message);
  }
  else if(uartflag == 2)
  {
    error_code = ExtractDatafromzigbee(jsonReceivedData,size,&message);
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
    error_code = ProcessJsonCommand(message.values, message.CMD);
    if(error_code != JSON_NO_ERROR)
    {
//      Error_Handler();
    }
  }
  else
  {
//    Error_Handler();
  }
  if((parseErrorCode != JSON_WILDCARD_DEV_ID) && (parseErrorCode != JSON_INVALID_DEV_ID))
  {
    GetAcknowledgementData(error_code); 
    Serial_Debug((char*)txAckBuff);
    UartTransmit(txAckBuff,strlen((char*)txAckBuff));
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
  return true; 
}

//static bool  SetParameterValue_float(const char * inputString, float * number, CheckValidity_Ptr_float ExecuteCheckValidity)
//{
//  if(inputString[0] != 0)
//  {
//    float convertedNumber = strtof(inputString, 0);
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
//    return true;                                                               /* return True - Null data but No error in data     */
//  }
//}

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
  uint32_t mode;
  char *setStateStr[JSON_NO_OF_MODE_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount <= JSON_NO_OF_MODE_STATE_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setStateStr[0],&mode, IsModeValid);
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Set Mode SetValue/s Out of Range");
    }
    else
    {
      switch(mode)
      {
      case 0 : SetMotionCommand(ROBOT_CMD_AUTO);
      SetOperationMode(AUTO,MAN_IDLE);
      break;
      case 1 : SetMotionCommand(ROBOT_CMD_MANUAL);
      SetOperationMode(MANUAL,MAN_IDLE);
      break;  
      default:
        break;
      }
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Set Mode SetValue : ");
      Serial_Debug_Num(mode);
      Serial_Debug("\r\n0->AUTO, 1->MANUAL");
    }            
  }
  else
  {
    Serial_Debug("\n Set Mode SetValue/s Invalid");
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
  eMode getMode =  GetOperationMode();
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
  tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",modeValue);
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

static eJsonStatus ProcessSetManualDirection (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetManualDirection->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  uint32_t mode;
  char *setStateStr[JSON_NO_OF_DIR_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
   int runvalue = atoi(setStateStr[1]);
   stnoofrows  *getNoofrows = GetSetNoofrowsLimits();
   int count = Daycount;
   int sumc = 0,sumc2 = 0;
   
   while(count >= 0)
   {
      sumc = sumc + getNoofrows->Row[count];
      count--;
      if(count >= 0)
      {
        sumc2 = sumc2 + getNoofrows->Row[count];
      }
   }
  
  if(totalResultStringCount <= JSON_NO_OF_DIR_STATE_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setStateStr[0],&mode, IsManualCommandValid);   
    
    if(mode == 6)
    {
//       if(runvalue > 0 ) 
      if(runvalue <= getNoofrows->TRow)     
      {
         status &= SetParameterValue_int(setStateStr[1],&C_ROW, Row_Num_Valid);
      }
      else
      {
        status = false;
      }
    }
    
    stContinue *setContinueValue = GetSetContinueLimits();
    strunrfid* getrunrfid = GetSetrunrfidLimits();
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Manual Mode SetValue/s Out of Range");
    }
    else if(setContinueValue->Continue1 != 2)
    {
      
      switch(mode)
      {
      case 1 : SetMotionCommand(ROBOT_CMD_POSITIVE);
                SetMotionPauseState(PA_IDLE);
      break;
      case 2 : SetMotionCommand(ROBOT_CMD_NEGATIVE);
                SetMotionPauseState(PA_IDLE);
      break;
      case 3 : SetMotionCommand(ROBOT_CMD_CYCLE);
                SetMotionPauseState(PA_IDLE);
      break;
      case 4 : SetMotionCommand(ROBOT_CMD_NORMAL_STOP);
               SetMotionPauseState(PA_IDLE);
      break;
      case 5 : SetMotionCommand(ROBOT_CMD_STOP);
                SetMotionPauseState(PA_IDLE);
      break;
      case 6 : if(getrunrfid->runrfid != 0 || (C_ROW <= sumc && C_ROW > sumc2))
                {
                  SetMotionCommand(ROBOT_CMD_TRACK_CHANGER);
                  SetMotionPauseState(PA_IDLE);
                }
                else
                {
                   SetTrackChangerError2(TC_INAPPROPRIATE_ROW_DATA);
                }
      break;
      default:
        break;
      }
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Manual Mode SetValue : ");
      Serial_Debug_Num(mode);
      Serial_Debug("\r\n0->POSITIVE, 1->NEGATIVE, 2->CYCLE, 3-> NORMAL_STOP, 4-> EM_STOP");
    }            
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
  tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",manualValue);
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
    snprintf(dispstring, sizeof dispstring, "\nPwm Manual Mode SetValues: %d, %d, %d, %d, %d, %d, %d",
             setPWMParams->accelTime, setPWMParams->decelTime, setPWMParams->steadyPwm1, setPWMParams->steadyPwm2,
             setPWMParams->approachPwm1, setPWMParams->approachPwm2,setPWMParams->decelStartCountDiff);
    Serial_Debug(dispstring);
  }
  return error_code;
}
static eJsonStatus ProcessSetMotorPwm (const char * jsonString, stRobotPwmParam *tempSetPwmParam)
{
    Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  
  char *pwmValuesStr[JSON_NO_OF_SETGET_PWM_DATA_MOTOR_tc];
  uint8_t maxNoStrings=(sizeof pwmValuesStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, pwmValuesStr, ',',maxNoStrings);
  Serial_Debug("\nPWM Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(pwmValuesStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_SETGET_PWM_DATA_MOTOR_tc)
  {
    bool status = true;
    status &= SetParameterValue_int(pwmValuesStr[0], (uint32_t *)&(tempSetPwmParam->accelTime), IsAccelTimeValid);
    status &= SetParameterValue_int(pwmValuesStr[1], (uint32_t *)&(tempSetPwmParam->decelTime) , IsDecelTimeValid);
    status &= SetParameterValue_int(pwmValuesStr[2], (uint32_t *)&(tempSetPwmParam->steadyPwm1), IsPWMvalueValid);
    status &= SetParameterValue_int(pwmValuesStr[2], (uint32_t *)&(tempSetPwmParam->steadyPwm2), IsPWMvalueValid);
    status &= SetParameterValue_int(pwmValuesStr[3], (uint32_t *)&(tempSetPwmParam->approachPwm1), IsPWMvalueValid);
    status &= SetParameterValue_int(pwmValuesStr[3], (uint32_t *)&(tempSetPwmParam->approachPwm2), IsPWMvalueValid);
    status &= SetParameterValue_int(pwmValuesStr[4], (uint32_t *)&(tempSetPwmParam->decelStartCountDiff), IsDecelPulseCountDiffValid);
    
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
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d",getPWMVal->accelTime,getPWMVal->decelTime,getPWMVal->steadyPwm1,getPWMVal->approachPwm1,getPWMVal->decelStartCountDiff);
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
    snprintf(dispstring, sizeof dispstring, "\nPwm Auto Mode SetValues: %d, %d, %d, %d, %d, %d, %d",
             setPWMParams->accelTime, setPWMParams->decelTime, setPWMParams->steadyPwm1, setPWMParams->steadyPwm2,
             setPWMParams->approachPwm1, setPWMParams->approachPwm2,setPWMParams->decelStartCountDiff);
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
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d",
                               getPWMVal->accelTime,getPWMVal->decelTime,getPWMVal->steadyPwm1,
                               getPWMVal->approachPwm1,getPWMVal->decelStartCountDiff);
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
static eJsonStatus ProcessSetAutoScheduleTime (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetAutoScheduleTime->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  char* autoScheduleStr[JSON_NO_OF_MODE_STATE_DATA];
  uint8_t maxNoStrings=(sizeof autoScheduleStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, autoScheduleStr, ',',maxNoStrings);
  Serial_Debug("\nAuto schedule Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(autoScheduleStr[dispcount]);
    Serial_Debug(",");
  }
  if((totalResultStringCount == JSON_NO_OF_MODE_STATE_DATA)&&(strlen(autoScheduleStr[0])==4))
  {
    uint8_t hour = 0;
    uint8_t minute = 0;
    hour = (autoScheduleStr[0][1] - 48)+((autoScheduleStr[0][0] - 48)*10);
    minute = (autoScheduleStr[0][3] - 48)+((autoScheduleStr[0][2] - 48)*10);
    Serial_Debug("\r\nHour-->");
    Serial_Debug_Num(hour);
    Serial_Debug("\r\nMinute-->");
    Serial_Debug_Num(minute);
    if((hour<24)&&(minute<60))
    {
      stScheduleTime setScheduleTime;
      setScheduleTime.hour = hour;
      setScheduleTime.minute = minute;
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
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%02d%02d,%d",getScheduleTime->hour,getScheduleTime->minute,Daycount);
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
  bool mode;
  error_code = ProcessSetState(jsonString, &mode);
  if(error_code == JSON_NO_ERROR)
  {
    Serial_Debug("\r\nEdge sensor state set as ");
    Serial_Debug_Num(mode);
    bool * edgeSensorState = GetSetEdgeSensorEnabledState();
    * edgeSensorState = mode;
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
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_MODE_STATE_DATA)
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


static eJsonStatus ProcessGetEdgeSenseEnabledState (const char * jsonString)
{
    eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_EDGE_SENSE_STATE);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",IsEdgeSensorEnabled());
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
    if((sys_time.tm_hour >= 0)&&(sys_time.tm_hour <= 23) && (sys_time.tm_min >= 0)&&(sys_time.tm_min <= 59) && (sys_time.tm_sec >= 0)&&(sys_time.tm_sec <= 59))
    {
        if( external_rtc_write(&sys_time))
        {
    //        ClearAutoDoneSeconds();
    //        ClearFault(RTC_FAULT);
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
        error_code = JSON_INVALID_DATA;
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

static eJsonStatus ProcessGetSensorStatus (const char * jsonString)
{
   eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_SENSOR_STATUS);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;

  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d", IsEdgeSensor1Detected(), IsEdgeSensor2Detected(), IsValidProximitySensorState(),GetCleanerDetect1PinState(), GetCleanerDetect2PinState());
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
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, clearErrorCodeStr, ',',maxNoStrings);
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
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, setLowBatSoCValueStr, ',',maxNoStrings);
  Serial_Debug("\nLow Battery Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setLowBatSoCValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_SOC_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setLowBatSoCValueStr[0], (uint32_t*)&(tempSetLowBatSoC), IsLowBatSoCValid);
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
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",*lowBatSoc);
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
    uint8_t totalResulStringCount =  SplitString ((char*)jsonString, heartbeatConfigStr, ',', maxNoStrings );
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
      status &= SetParameterValue_int(heartbeatConfigStr[0],(uint32_t*)&(tempSetHeartbeatConfig.interval_ms) , IsHeartBeatIntervalValid);
      status &= SetParameterValue_int(heartbeatConfigStr[1],(uint32_t*) &(tempSetHeartbeatConfig.maxReconnectTime) , IsMaxReconnectTimeValid);
      
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
        Serial_Debug_Num(setHeartbeatConfig->interval_ms);
        Serial_Debug(", ");
        Serial_Debug_Num(setHeartbeatConfig->maxReconnectTime);
        
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
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d",defaultHeartbeatConfig->interval_ms,defaultHeartbeatConfig->maxReconnectTime);
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
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, PulseCountStr, ',',maxNoStrings);
  Serial_Debug("\nPulse Count Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(PulseCountStr[dispcount]);
    Serial_Debug(",");
  }  
  if(totalResultStringCount == JSON_NO_OF_PULSE_COUNT_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(PulseCountStr[0],(uint32_t*)&(tempSetCountValues.maxPosCountForPVsetup), IsPulseCountValid);
    status &= SetParameterValue_int(PulseCountStr[1],(uint32_t*)&(tempSetCountValues.maxNegCountForPVsetup) , IsPulseCountValid);
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Pulse Count SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Pulse Count SetValue/s set : ");
      *setCountValues = tempSetCountValues;
      
      Serial_Debug_Num(setCountValues->maxPosCountForPVsetup);
      Serial_Debug(" ,");
      Serial_Debug_Num(setCountValues->maxNegCountForPVsetup);
    }            
  }
  else
  {
    Serial_Debug("\n Pulse Count SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

void PulseCalculation()
{
    P = 1;
    distance = (circumference * P);
    
    stWheelDia* setWheelDia = GetSetwheelDiaLimits();
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
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d",getCountValue->maxPosCountForPVsetup,getCountValue->maxNegCountForPVsetup);
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
  char man_day;
  switch(getMode)
  {
  case AUTO: 
    modeValue = 0;
    man_day = Daycount;
    break;
  case MANUAL:
    modeValue = 1;
    man_day = 1;
    break;
  default : 
    modeValue = 1;
    break;
  }
  stRfidData* getRfidData = GetSetRfidData(); 
  stWheelDia* setWheelDia = GetSetwheelDiaLimits(); 
  Distancevalue = (distance/setWheelDia->Pulse);
  uint8_t tempCount;
  stnoofrows* setNoofrows = GetSetNoofrowsLimits();
  stnoofrows tempsetNoofrows = *setNoofrows;
  
  if(lastReadRfidData[0] == 0)
  {
     strcpy((char*)lastReadRfidData,"0");
  }
  if(trackchangercount<=10) 
  {
    tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%lu,%d,%d,%d,%d,%s,%d", modeValue, robotState, GetRotateSense1Count(),GetRotateSense1Count()*Distancevalue,Cumulative_distance,man_day,trackchangercount,getRfidData->rfidData[trackchangercount-1],rowsActuallyCleaned,lastReadRfidData,tempsetNoofrows.TRow  - trackchangercount);
    dataCount = dataCount+tempCount; 
  }
  else if((trackchangercount>=11)&&(trackchangercount<=20)) 
  {
    tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%lu,%d,%d,%d,%d,%s,%d", modeValue, robotState, GetRotateSense1Count(),GetRotateSense1Count()*Distancevalue,Cumulative_distance,man_day,trackchangercount,getRfidData->rfidData2[trackchangercount-11],rowsActuallyCleaned,lastReadRfidData,tempsetNoofrows.TRow  - trackchangercount);
    dataCount = dataCount+tempCount; 
  }
  else if((trackchangercount>=21)&&(trackchangercount<=25)) 
  {
    tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%lu,%d,%d,%d,%d,%s,%d", modeValue, robotState, GetRotateSense1Count(),GetRotateSense1Count()*Distancevalue,Cumulative_distance,man_day,trackchangercount,getRfidData->rfidData3[trackchangercount-21],rowsActuallyCleaned,lastReadRfidData,tempsetNoofrows.TRow  - trackchangercount);
    dataCount = dataCount+tempCount; 
  }
  
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

static eJsonStatus ProcessGetLastOperationStat (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  int daycount;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_LAST_CYCLE_STAT);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stLastOperationStatus *lastOpStatus = GetLastOperationData();
  char startSoc_Str[10],endSoc_Str[10];
  ftoa(lastOpStatus->startSOC,startSoc_Str,2);
  ftoa(lastOpStatus->endSOC,endSoc_Str,2);
  eRobotDirection robotState = GetRobotActualDirection();
  eMode getMode =  GetOperationMode ();  
  int DAY = Daycount - 1;
  
    switch(getMode)
    {
    case AUTO: 
//      modeValue = 0;
      daycount = Daycount;
      break;
    case MANUAL:
//      modeValue = 1;
      daycount = 1;
      break;
    default : 
//      modeValue = 0;
      break;
    }
    uint8_t tempCount; 
    stnoofrows* setNoofrows = GetSetNoofrowsLimits();
    stnoofrows tempsetNoofrows = *setNoofrows;
    stWheelDia* setWheelDia = GetSetwheelDiaLimits();
    Distancevalue = (distance/setWheelDia->Pulse);
    if(trackchangercount<=10) 
    {
      tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%s,%s,%s,%s,%lX,%lu",robotState,daycount,trackchangercount,rowdaycount,tempsetNoofrows.Row[DAY]-rowdaycount,Cstarttime,Cstartsoc,Cendtime,Cendsoc,GetMotorFaultCode(),Cumulative_distance);  
      dataCount = dataCount+tempCount; 
    }
    else if((trackchangercount>=11)&&(trackchangercount<=20)) 
    {
      tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%s,%s,%s,%s,%lX,%lu",robotState,daycount,trackchangercount,rowdaycount,tempsetNoofrows.Row[DAY]-rowdaycount,Cstarttime,Cstartsoc,Cendtime,Cendsoc,GetMotorFaultCode(),Cumulative_distance);
      dataCount = dataCount+tempCount; 
    }
    else if((trackchangercount>=21)&&(trackchangercount<=25)) 
    {
      tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%s,%s,%s,%s,%lX,%lu",robotState,daycount,trackchangercount,rowdaycount,tempsetNoofrows.Row[DAY]-rowdaycount,Cstarttime,Cstartsoc,Cendtime,Cendsoc,GetMotorFaultCode(),Cumulative_distance); 
      dataCount = dataCount+tempCount; 
    }
    
  
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

static eJsonStatus ProcessSetWheelDia (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetWheelDia->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR; 
  stWheelDia* setWheelDia = GetSetwheelDiaLimits();
  stWheelDia tempsetWheelDia = *setWheelDia;
  char* setWheelDiaValueStr[JSON_NO_OF_WHEELDIA_DATA]; 
  uint8_t maxNoStrings=(sizeof setWheelDiaValueStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, setWheelDiaValueStr, ',',maxNoStrings);
  Serial_Debug("\nWheel Dia Set Received Value :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setWheelDiaValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_WHEELDIA_DATA)
  {
    bool status = true;
    
    status &= SetParameterValue_int(setWheelDiaValueStr[0], (uint32_t*)&(tempsetWheelDia.Dia), IswheelDiaLimitValid); 
    status &= SetParameterValue_int(setWheelDiaValueStr[1], (uint32_t*)&(tempsetWheelDia.Pulse), IsLowBatSoCValid);   
    
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
      
      Serial_Debug_Num(setWheelDia->Pulse);
      Serial_Debug(",");
   
  circumference = (float) ((float) 3.14 * (float) setWheelDia->Dia);
  distance =  (circumference * 1); 
  
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
  char WheelDia1_Str[10]; //char Pulse_str[10];
  ftoa(getWheelDia->Dia,WheelDia1_Str,0);
 
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%s,%d,%d",WheelDia1_Str,getWheelDia->Pulse,distance);
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

static eJsonStatus ProcessSetCycleFrequency (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetCycleFrequency->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;   
  stCycleFrequency* setCycleFrequency = GetSetcycleFrequencyLimits();
  stCycleFrequency tempsetCycleFrequency = *setCycleFrequency; 
  char* setcyclefrequencyValueStr[JSON_NO_OF_CYCLEFREQUENCY_DATA]; 
  uint8_t maxNoStrings=(sizeof setcyclefrequencyValueStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, setcyclefrequencyValueStr, ',',maxNoStrings);
  Serial_Debug("\nCycleFrequency Set Received Value :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setcyclefrequencyValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_CYCLEFREQUENCY_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setcyclefrequencyValueStr[0],(uint32_t*) &(tempsetCycleFrequency.Cycle), IscycleFrequencyLimitValid);
    
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
      Serial_Debug_Num(setCycleFrequency->Cycle);
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
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",getCycleFrequency->Cycle);
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
    status &= SetParameterValue_int(setIntervalValueStr[10], &(tempsetInterval.I6), IsparameterLimitValid);
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
  
  uint8_t maxNoStrings=(sizeof setContinueValueStr)/4;  
 
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, setContinueValueStr, ',',maxNoStrings);
  Serial_Debug("\nContinue Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setContinueValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_CONTINUE)
  {
    bool status = true;
    status &= SetParameterValue_int(setContinueValueStr[0], (uint32_t*)&(tempSetContinue.Continue1), IsContinue1Valid);
    
    int state = atoi(setContinueValueStr[0]);
    
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
    else if(state == 1)
    {   
      unCleanerMAC *getCleanerMAC = GetSetCleanerMAC();
      
      SendCommandToCleaner(getCleanerMAC->cleanerMAC_64,HOME_RETURN,(char *)"1,2");
      if(GetRobotActualDirection() == POSITIVE || GetRobotActualDirection() == STOP || GetRobotActualDirection() == UNKNOWN)
      {
          SetRobotState(ROBOT_HOME_RETURN);
      }
    }
    else if(state == 2)
    {
      
    }
    else if(state == 3)
    {
      Daycount = 0;
      tc_count = trackchangercount;
      trackchangercount = 0;
      rowsActuallyCleaned = trackchangercount;
      EepromSetAutoScheduleTime(EEPROM_WRITE);   
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
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CONTINUE_COUNT);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stContinue *getContinueValue = GetSetContinueLimits();
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",getContinueValue->Continue1);
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
  
  uint8_t maxNoStrings = (sizeof setContinueValueStr)/4;  
 
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, setContinueValueStr, ',',maxNoStrings);
  Serial_Debug("\nContinue Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setContinueValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_RETURN)
  {
    bool status = true;
    status &= SetParameterValue_int(setContinueValueStr[0],(uint32_t*) &(tempSetReturn.Return1), IsReturn1Valid);
    
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
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RETURN_STATE);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stReturn *getReturnValue = GetSetReturnLimits();
  
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",getReturnValue->Return1);
  
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

  strunrfid* setrunrfid = GetSetrunrfidLimits();
  strunrfid tempSetstrunrfid = *setrunrfid;
  
  char *ComdistanceStr[JSON_NO_OF_RUNRFID];
  uint8_t maxNoStrings=(sizeof ComdistanceStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, ComdistanceStr, ',',maxNoStrings);
  Serial_Debug("\nPulse Count Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(ComdistanceStr[dispcount]);
    Serial_Debug(",");
  } 
  if(totalResultStringCount == JSON_NO_OF_RUNRFID)
  {
    bool status = true; 
    Serial_Debug("\r\nIn Pulsecalc->");   
    status &= SetParameterValue_int(ComdistanceStr[0], (uint32_t*)&(tempSetstrunrfid.runrfid), IsRTCvalueValid);
  
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Pulse Count SetValue/s Out of Range");
    }
    else
    {
        error_code = JSON_NO_ERROR;
        Serial_Debug("\n Pulse Count SetValue/s set : ");
        *setrunrfid = tempSetstrunrfid;
        Serial_Debug_Num(setrunrfid->runrfid);
        Serial_Debug(" ,");
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
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RUNRFID);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  
  strunrfid* getrunrfid = GetSetrunrfidLimits();
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",getrunrfid->runrfid);
 
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

static eJsonStatus ProcessSetNoofRowday (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetLogInterval->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR; 
  stnoofrows* setNoofrows = GetSetNoofrowsLimits();
  stnoofrows tempsetNoofrows = *setNoofrows;
  char* setNoofrowsValueStr[JSON_NO_OF_ROWS]; 
  uint8_t maxNoStrings=(sizeof setNoofrowsValueStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char *)jsonString, setNoofrowsValueStr, ',',maxNoStrings);
  Serial_Debug("\nLogInterval Set Received Value :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setNoofrowsValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_ROWS-1)
  {
    bool status = true;
    tempsetNoofrows.TRow = 0;
 
    for(int i=0;i<7;i++)
    {
      status &= SetParameterValue_int(setNoofrowsValueStr[i],(uint32_t*)&(tempsetNoofrows.Row[i]), IsparameterLimitValid);     //Changed for zero
      tempsetNoofrows.TRow = tempsetNoofrows.TRow + tempsetNoofrows.Row[i];
    }
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n LogInterval SetValue/s Out of Range");
    }
    else
    {
      if (sum <= MAX_NO_OF_TRACKS)
      {    
        Daycount = 0;
        tc_count = trackchangercount;
        trackchangercount = 0;
        rowsActuallyCleaned = trackchangercount;
        EepromSetAutoScheduleTime(EEPROM_WRITE);
        error_code = JSON_NO_ERROR;
        Serial_Debug("\n LogInterval SetValue/s set : "); 
        *setNoofrows = tempsetNoofrows;
        for(int i=0;i<7;i++)
        {
          Serial_Debug_Num(setNoofrows->Row[i]);
        }
      }
      else 
      {
        error_code = JSON_INVALID_DATA;
        sum = 0;
        
      }
    }            
  }
  else
  {
    Serial_Debug("\n LogInterval SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
    sum = 0;
  }
  return error_code;
}

static eJsonStatus ProcessGetNoofRowday (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_N0_ROW_DAY);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount; 
  stnoofrows* getNoofrows = GetSetNoofrowsLimits();
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%d,%d,%d",getNoofrows->Row[0],getNoofrows->Row[1],getNoofrows->Row[2],getNoofrows->Row[3],
  getNoofrows->Row[4],getNoofrows->Row[5],getNoofrows->Row[6],getNoofrows->TRow);
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

static eJsonStatus ProcessGetChargerState (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CHARGER_STATE);
  dataCount = dataCount+bufferLength;   
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",GetChargingOrNot());
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
//zigbee
static eJsonStatus ProcessResetZigbeeNetwork (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessResetZigbeeNW->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char* zigbeeConfigStr[JSON_NO_OF_ZIGBEE_NW_RESET_DATA];
  int Parameterzigbee=0;
  uint8_t maxNoStrings=(sizeof zigbeeConfigStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char *)jsonString, zigbeeConfigStr, ',', maxNoStrings );
  Serial_Debug("\nZigbee Nw Reset Parameters Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(zigbeeConfigStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_ZIGBEE_NW_RESET_DATA)
  {
   if(zigbeeConfigStr[0] != NULL)
   { 
    Parameterzigbee = atoi(zigbeeConfigStr[0]); 
    if(Parameterzigbee == 1)
    {
      ZIGBEE_RESET_SUCCESS = true;
      
      ZigbeeNetworkReset(); 
      
      if(ZIGBEE_RESET_SUCCESS == false)
      {
        error_code = JSON_NO_ERROR;
      }
      else
      {
        error_code = JSON_PARSE_ERROR;
      }
    }
   }
   else
   {
      Serial_Debug("\nZigbee  Nw Reset SetValue/s: Out of Range");
      error_code = JSON_DATA_OUT_OF_RANGE;   
   }
  } 
  else
  {
    Serial_Debug("\nZigbee  Nw Reset SetValue/s : Invalid Data");
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
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqLogparaStr, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  memset(txBuff,0,strlen((char *)txBuff));
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqLogparaStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_REQUEST_LOG)
  {
    if(reqLogparaStr[0] != 0)
    {  
      Parameter = atoi(reqLogparaStr[0]);  
      uint16_t dataCount=0;    
      txBuff[dataCount]=0;
      uint16_t bufferLength = sizeof(txBuff)-dataCount; 
      ParseHeaderData(&txBuff[dataCount], &bufferLength, CMD_REQ_LOG);
      dataCount = dataCount+bufferLength; 
      bufferLength = sizeof(txBuff)-dataCount;    
      memset(data_read_buffer2,'\0',sizeof(data_read_buffer2));   
       
      if(Parameter == 2)
      {         
        GETLog_SOC();
        error_code = JSON_NO_ERROR; 
      }
      else if(Parameter == 3)
      { 
        GETLog_Cleaner();
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


static eJsonStatus ProcessSetTrackInfo (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetTrackInfo->");
  Serial_Debug(jsonString);
  int startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *trackDataStr[JSON_NO_OF_SET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof trackDataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, trackDataStr, ',', maxNoStrings );
  
  Serial_Debug("\nTrack Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(trackDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_SET_TRACK_INFO)
  {
    if((trackDataStr[0][0] != 0) && (trackDataStr[1][0] != 0))
    {
      startIndex = strtol(trackDataStr[0], 0, 10);
      endIndex = strtol(trackDataStr[1], 0, 10);
      Trackinfoend1 = strtol(trackDataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if(difference == noOfdata)
        {
          stTrackData* setTrackInfo = GetSetTrackData();
          stTrackData tempSetTrackInfo = *setTrackInfo;
          bool status = true;
          for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
          {
            status &= SetParameterValue_int(trackDataStr[i+2],(uint32_t*) &(tempSetTrackInfo.trackDistance[i]), IsTrackDistanceValid);
            if(status)
            {
              tempSetTrackInfo.trackValid[i] = TRACK_INFO_VALID;
            }
          }
          for(int i;i<15;i++)
          {
             tempSetTrackInfo.trackDistance[i] = 0;
             tempSetTrackInfo.trackValid[i] = 0;
          }
          if(status==true)
          {
            Serial_Debug("\n Track Distance SetValue/s set : "); 
            *setTrackInfo = tempSetTrackInfo;
            for(int j = (startIndex-1);j<(difference+startIndex-1);j++)
            {
              Serial_Debug_Num(setTrackInfo->trackDistance[j]);
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;  
            Serial_Debug("\n Track Distance SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nTrack Info Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nTrack Info Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nTrack Info Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  
  return error_code;
}

static eJsonStatus ProcessGetTrackInfo (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetTrackInfo->");
  Serial_Debug(jsonString);
  int startIndex = 0, endIndex = 0;
  char *reqTrackInfo[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqTrackInfo)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqTrackInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqTrackInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqTrackInfo[0][0] != 0) && (reqTrackInfo[1][0] != 0))
    {
      startIndex = strtol(reqTrackInfo[0], 0, 10);
      endIndex = strtol(reqTrackInfo[1], 0, 10);
      Trackinfoend1read = strtol(reqTrackInfo[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 &&((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest Track Info Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest Track Info Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Track Info Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_TRACK_INFO);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    stTrackData* setTrackData = GetSetTrackData();
    for(int i=startIndex;i<=endIndex;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setTrackData->trackDistance[i-1]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}

static eJsonStatus ProcessSetTrackInfo2 (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetTrackInfo->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *trackDataStr[JSON_NO_OF_SET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof trackDataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, trackDataStr, ',', maxNoStrings );
  
  Serial_Debug("\nTrack Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(trackDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_SET_TRACK_INFO)
  {
    if((trackDataStr[0][0] != 0) && (trackDataStr[1][0] != 0))
    {
      startIndex = strtol(trackDataStr[0], 0, 10);
      endIndex = strtol(trackDataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if(difference == noOfdata)
        {
          stTrackData* setTrackInfo = GetSetTrackData();
          stTrackData tempSetTrackInfo = *setTrackInfo;
          bool status = true;
          for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
          {
            status &= SetParameterValue_int(trackDataStr[i+2], (uint32_t*)&(tempSetTrackInfo.trackDistance2[i]), IsTrackDistanceValid);
            if(status)
            {
              tempSetTrackInfo.trackValid2[i] = TRACK_INFO_VALID;
            }
          }
          for(int i;i<15;i++)
          {
             tempSetTrackInfo.trackDistance[i] = 0;
             tempSetTrackInfo.trackValid[i] = 0;
          }
          if(status==true)
          {
            Serial_Debug("\n Track Distance SetValue/s set : "); 
            *setTrackInfo = tempSetTrackInfo;
            for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
            {
              Serial_Debug_Num(setTrackInfo->trackDistance2[i]);
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;  
            Serial_Debug("\n Track Distance SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nTrack Info Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nTrack Info Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nTrack Info Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  
  return error_code;
}

static eJsonStatus ProcessGetTrackInfo2 (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetTrackInfo->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  char *reqTrackInfo[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqTrackInfo)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqTrackInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqTrackInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqTrackInfo[0][0] != 0) && (reqTrackInfo[1][0] != 0))
    {
      startIndex = strtol(reqTrackInfo[0], 0, 10);
      endIndex = strtol(reqTrackInfo[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 &&((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest Track Info Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest Track Info Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Track Info Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_TRACK_INFO2);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    stTrackData* setTrackData = GetSetTrackData();
    for(int i=startIndex;i<=endIndex;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setTrackData->trackDistance2[i-1]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}

static eJsonStatus ClearTrackInfo (const char * jsonString)
{
  Serial_Debug("\r\nClearTrackInfo");
  Serial_Debug(jsonString);
  stTrackData* setTrackInfo = GetSetTrackData();
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
  
  for(int i = 0;i<MAX_NO_OF_TRACKS;i++)
  {
    setTrackInfo->trackDistance[i] = 0;
    setTrackInfo->trackValid[i] = TRACK_INFO_INVALID;
    
    setRowLengthData->rowLength[i][FORWARD_COUNT] = 0;
    setRowLengthData->rowLength[i][REVERSE_COUNT] = 0;
    setRowLengthData->rowLength2[i][FORWARD_COUNT] = 0;
    setRowLengthData->rowLength2[i][REVERSE_COUNT] = 0;
    setRowLengthData->rowLength3[i][FORWARD_COUNT] = 0;
    setRowLengthData->rowLength3[i][REVERSE_COUNT] = 0;
    setRowLengthData->rowLength4[i][FORWARD_COUNT] = 0;
    setRowLengthData->rowLength4[i][REVERSE_COUNT] = 0;
    setRowLengthData->rowLength5[i][FORWARD_COUNT] = 0;
    setRowLengthData->rowLength5[i][REVERSE_COUNT] = 0;
    setRowLengthData->rowLength6[i][FORWARD_COUNT] = 0;
    setRowLengthData->rowLength6[i][REVERSE_COUNT] = 0;
    setRowLengthData->rowLength7[i][FORWARD_COUNT] = 0;
    setRowLengthData->rowLength7[i][REVERSE_COUNT] = 0;
  }
  return JSON_NO_ERROR;
}


static eJsonStatus ProcessSetRfidData (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRfidData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  
  char *rfidDataStr[JSON_NO_OF_SET_TRACK_INFO];
 
  uint8_t maxNoStrings=(sizeof rfidDataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, rfidDataStr, ',', maxNoStrings );
  Serial_Debug("\nRFID Data Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rfidDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_SET_TRACK_INFO)
  {
    if((rfidDataStr[0][0] != 0) && (rfidDataStr[1][0] != 0))
    {
      startIndex = strtol(rfidDataStr[0], 0, 10);
      endIndex = strtol(rfidDataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if(difference) //== noOfdata)
        {
          stRfidData* setRfidData = GetSetRfidData();
          stRfidData tempSetRfidData = *setRfidData;
          bool status = true;
           memset(tempSetRfidData.rfidData,0,sizeof(tempSetRfidData.rfidData));
          for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
          {
            status &= SetParameterValue_int(rfidDataStr[i+2],(uint32_t*) &(tempSetRfidData.rfidData[i]), IsTrackDistanceValid);
          }
          if(status==true)
          {
            Serial_Debug("\n RFID Data SetValue/s set : "); 
            *setRfidData = tempSetRfidData;
            for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
            {
              Serial_Debug_Num(setRfidData->rfidData[i]);
              Serial_Debug(" ,");
              
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;
            Serial_Debug("\n RFID Data SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nRFID Data Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRFID Data Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRFID Data Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  
  return error_code;
}

static eJsonStatus ProcessGetRfidData (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetRfidData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  char *reqRfidData[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRfidData)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqRfidData, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRfidData[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRfidData[0][0] != 0) && (reqRfidData[1][0] != 0))
    {
      startIndex = strtol(reqRfidData[0], 0, 10);
      endIndex = strtol(reqRfidData[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 &&((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest RFID Data Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest RFID Data Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest RFID Data Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RFID_DATA);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    stRfidData* setRfidData = GetSetRfidData();
    for(int i=startIndex;i<=endIndex;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRfidData->rfidData[i-1]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}

static eJsonStatus ProcessSetRfidData2 (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRfidData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *rfidDataStr[JSON_NO_OF_SET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof rfidDataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, rfidDataStr, ',', maxNoStrings );
  Serial_Debug("\nRFID Data Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rfidDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_SET_TRACK_INFO)
  {
    if((rfidDataStr[0][0] != 0) && (rfidDataStr[1][0] != 0))
    {
      startIndex = strtol(rfidDataStr[0], 0, 10);
      endIndex = strtol(rfidDataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if(difference) //== noOfdata)
        {
          stRfidData* setRfidData = GetSetRfidData();
          stRfidData tempSetRfidData = *setRfidData;
          bool status = true;
          for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
          {
            status &= SetParameterValue_int(rfidDataStr[i+2], (uint32_t*)&(tempSetRfidData.rfidData2[i]), IsTrackDistanceValid);
          }
          if(status==true)
          {
            Serial_Debug("\n RFID Data SetValue/s set : "); 
            *setRfidData = tempSetRfidData;
            for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
            {
              Serial_Debug_Num(setRfidData->rfidData2[i]);
              Serial_Debug(" ,");
              
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;
            Serial_Debug("\n RFID Data SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nRFID Data Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRFID Data Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRFID Data Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  
  return error_code;
}

static eJsonStatus ProcessGetRfidData2 (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetRfidData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  char *reqRfidData[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRfidData)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqRfidData, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRfidData[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRfidData[0][0] != 0) && (reqRfidData[1][0] != 0))
    {
      startIndex = strtol(reqRfidData[0], 0, 10);
      endIndex = strtol(reqRfidData[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 &&((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest RFID Data Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest RFID Data Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest RFID Data Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RFID_DATA2);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    stRfidData* setRfidData = GetSetRfidData();
    for(int i=startIndex;i<=endIndex;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRfidData->rfidData2[i-1]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}

static eJsonStatus ProcessSetRfidData3 (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRfidData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *rfidDataStr[JSON_NO_OF_SET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof rfidDataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, rfidDataStr, ',', maxNoStrings );
  Serial_Debug("\nRFID Data Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rfidDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_SET_TRACK_INFO)
  {
    if((rfidDataStr[0][0] != 0) && (rfidDataStr[1][0] != 0))
    {
      startIndex = strtol(rfidDataStr[0], 0, 10);
      endIndex = strtol(rfidDataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if(difference) //== noOfdata)
        {
          stRfidData* setRfidData = GetSetRfidData();
          stRfidData tempSetRfidData = *setRfidData;
          bool status = true;
          for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
          {
            status &= SetParameterValue_int(rfidDataStr[i+2], (uint32_t*)&(tempSetRfidData.rfidData3[i]), IsTrackDistanceValid);
          }
          if(status==true)
          {
            Serial_Debug("\n RFID Data SetValue/s set : "); 
            *setRfidData = tempSetRfidData;
            for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
            {
              Serial_Debug_Num(setRfidData->rfidData3[i]);
              Serial_Debug(" ,");
              
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;
            Serial_Debug("\n RFID Data SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nRFID Data Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRFID Data Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRFID Data Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  
  return error_code;
}

static eJsonStatus ProcessGetRfidData3 (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetRfidData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  char *reqRfidData[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRfidData)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqRfidData, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRfidData[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRfidData[0][0] != 0) && (reqRfidData[1][0] != 0))
    {
      startIndex = strtol(reqRfidData[0], 0, 10);
      endIndex = strtol(reqRfidData[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 &&((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest RFID Data Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest RFID Data Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest RFID Data Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RFID_DATA3);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    //stMaxIloadConfigValues* getMaxIloadConfigValues = GetSetMaxIloadConfigValues();
    stRfidData* setRfidData = GetSetRfidData();
    for(int i=startIndex;i<=endIndex;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRfidData->rfidData3[i-1]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}

static eJsonStatus ProcessSetRfidState (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetRfidState->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  uint8_t mode;
  char *setStateStr[JSON_NO_OF_MODE_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_MODE_STATE_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setStateStr[0], (uint32_t*)&mode, IsModeValid);
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Set Mode SetValue/s Out of Range");
    }
    else
    {
      SetRfidState((eRfidState)mode);
          
    }
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Set RFID state mode SetValue : ");
      Serial_Debug_Num(mode);
      Serial_Debug("\r\n0->RFID DISABLE, 1->RFID ENABLE");
    }            
  else
  {
    Serial_Debug("\n Set RFID state SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code; 
}

static eJsonStatus ProcessGetRfidState (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RFID_STATE);
  dataCount = dataCount+bufferLength;   
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = 0;
  eRfidState getMode =  GetRfidState ();
  tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",getMode);
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

static eJsonStatus ProcessSetCleanerMAC (const char * jsonString)
{
  Serial_Debug("\r\nIn Process Set Cleaner MAC->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  unCleanerMAC* setCleanerMAC;
  setCleanerMAC = GetSetCleanerMAC();
  unCleanerMAC tempSetCleanerMAC = *setCleanerMAC;
  char *CleanerMACdataStr[JSON_NO_OF_CLEANER_MAC_DATA];
  uint8_t maxNoStrings=(sizeof CleanerMACdataStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, CleanerMACdataStr, ',',maxNoStrings);
  Serial_Debug("\nCleaner MAC Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(CleanerMACdataStr[dispcount]);
    Serial_Debug(",");
  }  
  if((totalResultStringCount == JSON_NO_OF_CLEANER_MAC_DATA)&&(strlen(CleanerMACdataStr[0])!=0))
  {
    bool status = true;
    if(strlen(CleanerMACdataStr[0])>=17)  
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
      tempSetCleanerMAC.cleanerMAC_64 = HexStringtoInt((uint8_t*)CleanerMACdataStr[0]);
      *setCleanerMAC = tempSetCleanerMAC;
      char dispstring[20];
      uint16_t length = sizeof dispstring;
      Int64ToHexString(setCleanerMAC->cleanerMAC_64,dispstring,&length);
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

static eJsonStatus ProcessGetCleanerMAC (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CLEANER_MAC);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  unCleanerMAC *getSetCleanerMAC = GetSetCleanerMAC();
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%X%X%X%X",getSetCleanerMAC->cleanerMAC_32[3],getSetCleanerMAC->cleanerMAC_32[2],getSetCleanerMAC->cleanerMAC_32[1],getSetCleanerMAC->cleanerMAC_32[0]);
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

static eJsonStatus ProcessSetEncoderPulseTolerance (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetEncoderPulseTolerance->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  char* setEncPulToleranceValueStr[JSON_NO_OF_ENC_PULSE_TOLERANCE_DATA];
  uint32_t maxNoStrings=(sizeof setEncPulToleranceValueStr)/4;  
  uint32_t *setEncPulTolerance = GetSetEncoderPulseTolerance();
  uint32_t tempSetEncPulTolerance = *setEncPulTolerance;
  uint32_t totalResultStringCount =  SplitString ((char*)jsonString, setEncPulToleranceValueStr, ',',maxNoStrings);
  Serial_Debug("\nEnc Pulse Tolerance Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setEncPulToleranceValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_ENC_PULSE_TOLERANCE_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setEncPulToleranceValueStr[0], &(tempSetEncPulTolerance), IsPulseCountValid);
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Enc Pulse Tolerance SetValue/s Out of Range");
    }
    else
    {
      error_code = JSON_NO_ERROR;
      *setEncPulTolerance = tempSetEncPulTolerance;
      Serial_Debug("\n Enc Pulse Tolerance SetValue/s Set");
      Serial_Debug_Num(*setEncPulTolerance);
    } 
  }
  else
  {
    Serial_Debug("\n Enc Pulse Tolerance SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;   
  }
  return error_code;
}
static eJsonStatus ProcessGetEncoderPulseTolerance (const char * jsonString)
{
   eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_ENC_PULSE_TOLERANCE);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  uint32_t *pulseTolerance = GetSetEncoderPulseTolerance();
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%lu", *pulseTolerance);
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
static eJsonStatus ProcessSetTcControlMode (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetTcControlMode->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  uint8_t mode;
  char *setStateStr[JSON_NO_OF_MODE_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_MODE_STATE_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setStateStr[0],(uint32_t*)&mode, IsModeValid);
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Set Mode SetValue/s Out of Range");
    }
    else
    {
      switch(mode)
      {
//      case ENCODER_MODE : 
//        DisableRotateSense();
//        SetTcControlMode(ENCODER_MODE);
//        RotateSenseInit();
//        break;
      case PROXIMITY_MODE : 
        DisableRotateSense();
        SetTcControlMode(PROXIMITY_MODE);
        RotateSenseInit();
        break; 
      default:
        break;
      }
      error_code = JSON_NO_ERROR;
      Serial_Debug("\n Set TC control mode SetValue : ");
      Serial_Debug_Num(mode);
      Serial_Debug("\r\n0->ENCODER MODE, 1->PROXIMITY MODE");
    }            
  }
  else
  {
    Serial_Debug("\n Set TC control mode SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetTcControlMode (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_TC_CONTROL_MODE);
  dataCount = dataCount+bufferLength;   
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = 0;
  eTcControlMode getMode =  GetTcControlMode ();
  tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d",getMode);
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

static eJsonStatus ProcessSetRowLengthData (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *rowLengthDataStr[JSON_NO_OF_ROW_LENGTH_DATA];
  uint8_t maxNoStrings=(sizeof rowLengthDataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, rowLengthDataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLengthDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLengthDataStr[0][0] != 0) && (rowLengthDataStr[1][0] != 0))
    {
      startIndex = strtol(rowLengthDataStr[0], 0, 10);
      endIndex = strtol(rowLengthDataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 4;           //changed 3
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          Row1value = atoi(rowLengthDataStr[2]);
          
          if(Row1value==1) 
          {
            for(int i = (startIndex-1),j=3;(i<(difference+startIndex-1)&&j<=noOfdata+1);i++,j+=2)
            {
              status &= SetParameterValue_int(rowLengthDataStr[j],(uint32_t*)&(tempSetRowLengthData.rowLength[i][FORWARD_COUNT]), IsPulseCountValid);
              status &= SetParameterValue_int(rowLengthDataStr[j+1],(uint32_t*)&(tempSetRowLengthData.rowLength[i][REVERSE_COUNT]), IsPulseCountValid);
            }
            if(status==true)
            {
              Serial_Debug("\n Row Length SetValue/s set : "); 
              *setRowLengthData = tempSetRowLengthData;
              for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
              {
                Serial_Debug_Num(setRowLengthData->rowLength[i][FORWARD_COUNT]);
                Serial_Debug(",");
                Serial_Debug_Num(setRowLengthData->rowLength[i][REVERSE_COUNT]);
                Serial_Debug("  ");
              }
              error_code = JSON_NO_ERROR;
            }        
          }
         else if(Row1value==2) 
          {
            for(int i = (startIndex-1),j=3;(i<(difference+startIndex-1)&&j<=noOfdata+1);i++,j+=2)
            {
              status &= SetParameterValue_int(rowLengthDataStr[j], (uint32_t*)&(tempSetRowLengthData.rowLength[i+9][FORWARD_COUNT]), IsPulseCountValid);
              status &= SetParameterValue_int(rowLengthDataStr[j+1], (uint32_t*)&(tempSetRowLengthData.rowLength[i+9][REVERSE_COUNT]), IsPulseCountValid);
            }
            if(status==true)
            {
              Serial_Debug("\n Row Length SetValue/s set : "); 
              *setRowLengthData = tempSetRowLengthData;
              for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
              {
                Serial_Debug_Num(setRowLengthData->rowLength[i+9][FORWARD_COUNT]);
                Serial_Debug(",");
                Serial_Debug_Num(setRowLengthData->rowLength[i+9][REVERSE_COUNT]);
                Serial_Debug("  ");
              }
              error_code = JSON_NO_ERROR;
            }                  
          }
          else if(Row1value==3) 
          {
            for(int i = (startIndex-1),j=3;(i<(difference+startIndex-1)&&j<=noOfdata+1);i++,j+=2)
            {
              status &= SetParameterValue_int(rowLengthDataStr[j], (uint32_t*)&(tempSetRowLengthData.rowLength[i+18][FORWARD_COUNT]), IsPulseCountValid);
              status &= SetParameterValue_int(rowLengthDataStr[j+1], (uint32_t*)&(tempSetRowLengthData.rowLength[i+18][REVERSE_COUNT]), IsPulseCountValid);
            }
            if(status==true)
            {
              Serial_Debug("\n Row Length SetValue/s set : "); 
              *setRowLengthData = tempSetRowLengthData;
              for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
              {
                Serial_Debug_Num(setRowLengthData->rowLength[i+18][FORWARD_COUNT]);
                Serial_Debug(",");
                Serial_Debug_Num(setRowLengthData->rowLength[i+18][REVERSE_COUNT]);
                Serial_Debug("  ");
              }
              error_code = JSON_NO_ERROR;
            }             
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;
            Serial_Debug("\n Row Length SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nRow Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRow Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRow Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  
  return error_code;
}

static eJsonStatus ProcessGetRowLengthData (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  
  char *reqRowLengthInfo[JSON_NO_OF_GET_TRACK_INFO+1];
  uint8_t maxNoStrings=(sizeof reqRowLengthInfo)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO+1)
  {
    if((reqRowLengthInfo[0][0] != 0) && (reqRowLengthInfo[1][0] != 0))
    {
      startIndex = strtol(reqRowLengthInfo[0], 0, 10);
      endIndex = strtol(reqRowLengthInfo[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA+MAX_NO_OF_SET_DATA))  //changed
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        
        Row1getvalue = atoi(reqRowLengthInfo[2]);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest Row Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_ROW_LENGTH_DATA);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    stRowLengthData *setRowlengthData = GetSetRowLengthData();
    
    if(Row1getvalue == 1)
    {  
      for(int i=startIndex-1;i<=endIndex-1;i++)
      {
        uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength[i][FORWARD_COUNT]);
        dataCount = dataCount+tempCount; 
        tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength[i][REVERSE_COUNT]);
        dataCount = dataCount+tempCount; 
      }
    }
   else  if(Row1getvalue == 2)
    {  
      for(int i=startIndex-1;i<=endIndex-1;i++)
      {
        uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength[i+9][FORWARD_COUNT]);
        dataCount = dataCount+tempCount; 
        tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength[i+9][REVERSE_COUNT]);
        dataCount = dataCount+tempCount; 
      }
    }
   else if(Row1getvalue == 3)
    {  
      for(int i=startIndex-1;i<=endIndex-1;i++)
      {
        uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength[i+18][FORWARD_COUNT]);
        dataCount = dataCount+tempCount; 
        tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength[i+18][REVERSE_COUNT]);
        dataCount = dataCount+tempCount; 
      }
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}

static eJsonStatus ProcessSetRowLength2Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *rowLength2DataStr[JSON_NO_OF_ROW_LENGTH_DATA];
  uint8_t maxNoStrings=(sizeof rowLength2DataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, rowLength2DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength2DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength2DataStr[0][0] != 0) && (rowLength2DataStr[1][0] != 0))
    {
      startIndex = strtol(rowLength2DataStr[0], 0, 10);
      endIndex = strtol(rowLength2DataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength2DataStr[j], (uint32_t*)&(tempSetRowLengthData.rowLength2[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength2DataStr[j+1], (uint32_t*)&(tempSetRowLengthData.rowLength2[i][REVERSE_COUNT]), IsPulseCountValid);
          }
          if(status==true)
          {
            Serial_Debug("\n Row Length SetValue/s set : "); 
            *setRowLengthData = tempSetRowLengthData;
            for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
            {
              Serial_Debug_Num(setRowLengthData->rowLength2[i][FORWARD_COUNT]);
              Serial_Debug(",");
              Serial_Debug_Num(setRowLengthData->rowLength2[i][REVERSE_COUNT]);
              Serial_Debug("  ");
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;
            Serial_Debug("\n Row Length SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nRow Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRow Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRow Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetRowLength2Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  char *reqRowLengthInfo[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRowLengthInfo)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != 0) && (reqRowLengthInfo[1][0] != 0))
    {
      startIndex = strtol(reqRowLengthInfo[0], 0, 10);
      endIndex = strtol(reqRowLengthInfo[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest Row Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_ROW_LENGTH2_DATA);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    for(int i=startIndex-1;i<=endIndex-1;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",rowLengthData.rowLength2[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",rowLengthData.rowLength2[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}

static eJsonStatus ProcessSetRowLength3Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *rowLength3DataStr[JSON_NO_OF_ROW_LENGTH_DATA];
  uint8_t maxNoStrings=(sizeof rowLength3DataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, rowLength3DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength3DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength3DataStr[0][0] != 0) && (rowLength3DataStr[1][0] != 0))
    {
      startIndex = strtol(rowLength3DataStr[0], 0, 10);
      endIndex = strtol(rowLength3DataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength3DataStr[j], (uint32_t*)&(tempSetRowLengthData.rowLength3[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength3DataStr[j+1], (uint32_t*)&(tempSetRowLengthData.rowLength3[i][REVERSE_COUNT]), IsPulseCountValid);
          }
          if(status==true)
          {
            Serial_Debug("\n Row Length SetValue/s set : "); 
            *setRowLengthData = tempSetRowLengthData;
            for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
            {
              Serial_Debug_Num(setRowLengthData->rowLength3[i][FORWARD_COUNT]);
              Serial_Debug(",");
              Serial_Debug_Num(setRowLengthData->rowLength3[i][REVERSE_COUNT]);
              Serial_Debug("  ");
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;
            Serial_Debug("\n Row Length SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nRow Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRow Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRow Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetRowLength3Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  char *reqRowLengthInfo[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRowLengthInfo)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != 0) && (reqRowLengthInfo[1][0] != 0))
    {
      startIndex = strtol(reqRowLengthInfo[0], 0, 10);
      endIndex = strtol(reqRowLengthInfo[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest Row Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_ROW_LENGTH3_DATA);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    stRowLengthData *setRowlengthData = GetSetRowLengthData();
    for(int i=startIndex-1;i<=endIndex-1;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength3[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength3[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}
static eJsonStatus ProcessSetRowLength4Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *rowLength4DataStr[JSON_NO_OF_ROW_LENGTH_DATA];
  uint8_t maxNoStrings=(sizeof rowLength4DataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, rowLength4DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength4DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength4DataStr[0][0] != 0) && (rowLength4DataStr[1][0] != 0))
    {
      startIndex = strtol(rowLength4DataStr[0], 0, 10);
      endIndex = strtol(rowLength4DataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength4DataStr[j], (uint32_t*)&(tempSetRowLengthData.rowLength4[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength4DataStr[j+1], (uint32_t*)&(tempSetRowLengthData.rowLength4[i][REVERSE_COUNT]), IsPulseCountValid);
          }
          if(status==true)
          {
            Serial_Debug("\n Row Length SetValue/s set : "); 
            *setRowLengthData = tempSetRowLengthData;
            for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
            {
              Serial_Debug_Num(setRowLengthData->rowLength4[i][FORWARD_COUNT]);
              Serial_Debug(",");
              Serial_Debug_Num(setRowLengthData->rowLength4[i][REVERSE_COUNT]);
              Serial_Debug("  ");
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;
            Serial_Debug("\n Row Length SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nRow Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRow Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRow Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetRowLength4Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  char *reqRowLengthInfo[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRowLengthInfo)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != 0) && (reqRowLengthInfo[1][0] != 0))
    {
      startIndex = strtol(reqRowLengthInfo[0], 0, 10);
      endIndex = strtol(reqRowLengthInfo[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest Row Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_ROW_LENGTH4_DATA);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    stRowLengthData *setRowlengthData = GetSetRowLengthData();
    for(int i=startIndex-1;i<=endIndex-1;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength4[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength4[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}
static eJsonStatus ProcessSetRowLength5Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *rowLength5DataStr[JSON_NO_OF_ROW_LENGTH_DATA];
  uint8_t maxNoStrings=(sizeof rowLength5DataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, rowLength5DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength5DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength5DataStr[0][0] != 0) && (rowLength5DataStr[1][0] != 0))
    {
      startIndex = strtol(rowLength5DataStr[0], 0, 10);
      endIndex = strtol(rowLength5DataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength5DataStr[j], (uint32_t*)&(tempSetRowLengthData.rowLength5[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength5DataStr[j+1], (uint32_t*)&(tempSetRowLengthData.rowLength5[i][REVERSE_COUNT]), IsPulseCountValid);
          }
          if(status==true)
          {
            Serial_Debug("\n Row Length SetValue/s set : "); 
            *setRowLengthData = tempSetRowLengthData;
            for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
            {
              Serial_Debug_Num(setRowLengthData->rowLength5[i][FORWARD_COUNT]);
              Serial_Debug(",");
              Serial_Debug_Num(setRowLengthData->rowLength5[i][REVERSE_COUNT]);
              Serial_Debug("  ");
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;
            Serial_Debug("\n Row Length SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nRow Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRow Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRow Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetRowLength5Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  char *reqRowLengthInfo[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRowLengthInfo)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != 0) && (reqRowLengthInfo[1][0] != 0))
    {
      startIndex = strtol(reqRowLengthInfo[0], 0, 10);
      endIndex = strtol(reqRowLengthInfo[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest Row Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_ROW_LENGTH5_DATA);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    stRowLengthData *setRowlengthData = GetSetRowLengthData();
    for(int i=startIndex-1;i<=endIndex-1;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength5[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength5[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}
static eJsonStatus ProcessSetRowLength6Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *rowLength6DataStr[JSON_NO_OF_ROW_LENGTH_DATA];
  uint8_t maxNoStrings=(sizeof rowLength6DataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, rowLength6DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength6DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength6DataStr[0][0] != 0) && (rowLength6DataStr[1][0] != 0))
    {
      startIndex = strtol(rowLength6DataStr[0], 0, 10);
      endIndex = strtol(rowLength6DataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength6DataStr[j], (uint32_t*)&(tempSetRowLengthData.rowLength6[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength6DataStr[j+1], (uint32_t*)&(tempSetRowLengthData.rowLength6[i][REVERSE_COUNT]), IsPulseCountValid);
          }
          if(status==true)
          {
            Serial_Debug("\n Row Length SetValue/s set : "); 
            *setRowLengthData = tempSetRowLengthData;
            for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
            {
              Serial_Debug_Num(setRowLengthData->rowLength6[i][FORWARD_COUNT]);
              Serial_Debug(",");
              Serial_Debug_Num(setRowLengthData->rowLength6[i][REVERSE_COUNT]);
              Serial_Debug("  ");
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;
            Serial_Debug("\n Row Length SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nRow Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRow Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRow Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetRowLength6Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  char *reqRowLengthInfo[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRowLengthInfo)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != 0) && (reqRowLengthInfo[1][0] != 0))
    {
      startIndex = strtol(reqRowLengthInfo[0], 0, 10);
      endIndex = strtol(reqRowLengthInfo[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest Row Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_ROW_LENGTH6_DATA);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    stRowLengthData *setRowlengthData = GetSetRowLengthData();
    for(int i=startIndex-1;i<=endIndex-1;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength6[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength6[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}
static eJsonStatus ProcessSetRowLength7Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  char *rowLength7DataStr[JSON_NO_OF_ROW_LENGTH_DATA];
  uint8_t maxNoStrings=(sizeof rowLength7DataStr)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, rowLength7DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength7DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength7DataStr[0][0] != 0) && (rowLength7DataStr[1][0] != 0))
    {
      startIndex = strtol(rowLength7DataStr[0], 0, 10);
      endIndex = strtol(rowLength7DataStr[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        difference = (endIndex - startIndex)+1;
        noOfdata = totalResulStringCount - 3;
        Serial_Debug("\r\nNo of data as per Index->");
        Serial_Debug_Num(difference);
        Serial_Debug("\r\nActual no of data->");
        Serial_Debug_Num(noOfdata);
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength7DataStr[j], (uint32_t*)&(tempSetRowLengthData.rowLength7[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength7DataStr[j+1], (uint32_t*)&(tempSetRowLengthData.rowLength7[i][REVERSE_COUNT]), IsPulseCountValid);
          }
          if(status==true)
          {
            Serial_Debug("\n Row Length SetValue/s set : "); 
            *setRowLengthData = tempSetRowLengthData;
            for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
            {
              Serial_Debug_Num(setRowLengthData->rowLength7[i][FORWARD_COUNT]);
              Serial_Debug(",");
              Serial_Debug_Num(setRowLengthData->rowLength7[i][REVERSE_COUNT]);
              Serial_Debug("  ");
            }
            error_code = JSON_NO_ERROR;
          }
          else
          {
            error_code = JSON_DATA_OUT_OF_RANGE;
            Serial_Debug("\n Row Length SetValue/s Out of Range");
          }
        }
        else
        {
          error_code = JSON_INVALID_DATA;
        }
      }
      else
      {
        Serial_Debug("\nRow Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRow Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRow Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetRowLength7Data (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessGetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0;
  char *reqRowLengthInfo[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRowLengthInfo)/4;
  uint8_t totalResulStringCount =  SplitString ((char*)jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != 0) && (reqRowLengthInfo[1][0] != 0))
    {
      startIndex = strtol(reqRowLengthInfo[0], 0, 10);
      endIndex = strtol(reqRowLengthInfo[1], 0, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %lu, EndIndex = %lu", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest Row Length Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
    }
    else
    {
      Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
      error_code = JSON_INVALID_DATA;
    }
  }
  else
  {
    Serial_Debug("\nRequest Row Length Value/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_ROW_LENGTH7_DATA);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    stRowLengthData *setRowlengthData = GetSetRowLengthData();
    for(int i=startIndex-1;i<=endIndex-1;i++)
    {
      uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength7[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,",setRowlengthData->rowLength7[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}

void tcstarttime()
{
//  char getTime_str1[40];
//  memset(cyclelog,'\0',sizeof(cyclelog));
//  strcat(cyclelog,getTime_str1);
//  strcat(cyclelog,",");
  
  char getTime_str1[40];
  memset(cyclelog,'\0',sizeof(cyclelog));
  GetRTCDateTimeString((char*)&getTime_str1, sizeof getTime_str1);
  strcat(cyclelog,getTime_str1);
  strcat(cyclelog,",");
  strcpy(Cstarttime,getTime_str1);
  
  float startSOC = Batt_SOC;
  char startSoc_Str[10];
  ftoa(startSOC,startSoc_Str,2);
  strcat(cyclelog,startSoc_Str);
  strcat(cyclelog,",");
  strcpy(Cstartsoc,startSoc_Str);
}
void tcendtime()
{
//  char getTime_str1[40]; 
//  strcat(cyclelog,getTime_str1);
  
    char getTime_str1[40]; //,Error_Cnt[2];
    GetRTCDateTimeString((char*)&getTime_str1, sizeof getTime_str1);
    strcat(cyclelog,getTime_str1);  
    strcat(cyclelog,",");
    strcpy(Cendtime,getTime_str1);
    
    float endSOC = Batt_SOC;
    char startSoc_Str[10];
    ftoa(endSOC,startSoc_Str,2);
    strcpy(Cendsoc,startSoc_Str);
  
}

eJsonStatus ReportTrackChangerStatus(uint8_t reportType,uint32_t reportValue)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_REPORT_MSG);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%d,%lu",reportType,reportValue);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  ClearTxBuff();
  return error_code;
}

ErrorStatus SendCommandToCleaner(uint64_t macAddress,uint16_t command,char* values)
{
  commandtocleaner = 1;
  bool status;
  status = 0;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,command);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf((char *)&txBuff[dataCount],bufferLength,"%s",values);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes((char*)txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  status = ZigbeeTxPacket_Zigbee(macAddress,txBuff,dataCount);
  if (status == true)
  {
      return SUCCESS;
  }
  else 
  {
      return ERROR;
  }
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


