#include "main.h"

#define LENGTH_OF_EACH_STORE            1                         /* One 32 bit variables    */
#define LENGTH_OF_EACH_STRING_STORE     1                         /* One 32 bit variables    */

#define STORE_MIN_START_ADDRESS      APP_MEM_ADDR1
#define EEPROM_VALIDITY_DATA         1234

enum
{
  STORE_MODE_STARTADD                    = STORE_MIN_START_ADDRESS,
  STORE_PWM_MOTOR_MANUAL_STARTADD        = STORE_MODE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_PWM_BRUSH_MANUAL_STARTADD        = STORE_PWM_MOTOR_MANUAL_STARTADD +LENGTH_OF_EACH_STORE,  
  STORE_BRUSH_MOTOR_POLARITY_STARTADD    = STORE_PWM_BRUSH_MANUAL_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_PWM_MOTOR_AUTO_STARTADD          = STORE_BRUSH_MOTOR_POLARITY_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_PWM_BRUSH_AUTO_STARTADD          = STORE_PWM_MOTOR_AUTO_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_PULSE_COUNT_STARTADD             = STORE_PWM_BRUSH_AUTO_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_EDGE_SENSOR_STATE_STARTADD       = STORE_PULSE_COUNT_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_CURRENT_LIMIT_STARTADD           = STORE_EDGE_SENSOR_STATE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_LOW_BAT_SOC_STARTADD             = STORE_CURRENT_LIMIT_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_MOT_FAULT_STARTADD               = STORE_LOW_BAT_SOC_STARTADD +LENGTH_OF_EACH_STORE, 
  STORE_HEARTBEAT_CONFIG_STARTADD        = STORE_MOT_FAULT_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_LOG_INTERVAL_STARTADD            = STORE_HEARTBEAT_CONFIG_STARTADD +LENGTH_OF_EACH_STORE, 
  STORE_WHEEL_DIA_STARTADD               = STORE_LOG_INTERVAL_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_CYCLE_FREQUENCY_STARTADD         = STORE_WHEEL_DIA_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_CONTINUE_STARTADD                = STORE_CYCLE_FREQUENCY_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_BRUSH_ENABLED_STATE_STARTADD     = STORE_CONTINUE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_RETURN_STARTADD                  = STORE_BRUSH_ENABLED_STATE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_AUTO_SCHEDULE_STARTADD           = STORE_RETURN_STARTADD +LENGTH_OF_EACH_STORE, 
  STORE_COMDISTANCE_STARTADD             = STORE_AUTO_SCHEDULE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_DRIVE_ENABLED_STATE_STARTADD     = STORE_COMDISTANCE_STARTADD +LENGTH_OF_EACH_STORE, 
  STORE_CHANGER_MAC_STARTADD             = STORE_DRIVE_ENABLED_STATE_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_RELAY_INTERVAL_STARTADD          = STORE_CHANGER_MAC_STARTADD +LENGTH_OF_EACH_STORE,
  STORE_MAX_START_ADDRESS                = STORE_RELAY_INTERVAL_STARTADD +LENGTH_OF_EACH_STORE,  
  EEPROM_DEV_INFO_START_ADDRESS          = STORE_MAX_START_ADDRESS +LENGTH_OF_EACH_STORE,  
  STORE_MAX_ADDRESS                      = EEPROM_DEV_INFO_START_ADDRESS - 1                                   
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
    status = EEPROM_OK;
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

eEepromStatus EepromSetMotorPwmManualMode(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_PWM_MOTOR_MANUAL_STARTADD;
  stRobotPwmParam *setPWMParams = GetSetManualPwmParameters();
  stRobotPwmParam *setPWMCycleParams = GetSetCycleManualPwmParameters();
  char eepromValue[JSON_NO_OF_PWM_DATA_MOTOR_FLASH_WRITE-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>8;  
    eepromValue[eepromAddIndex++] = setPWMParams->accelTime;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->decelTime;
    eepromValue[eepromAddIndex++] = (setPWMParams->Accel)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->Accel)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->Accel)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->Accel;
    eepromValue[eepromAddIndex++] = setPWMParams->accelapproachPwm1;
    eepromValue[eepromAddIndex++] = setPWMParams->accelapproachPwm2;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm1;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm2;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm1;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm2;
    eepromValue[eepromAddIndex++] = (setPWMParams->Decel)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->Decel)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->Decel)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->Decel;
    eepromValue[eepromAddIndex++] = setPWMParams->acelStartCountDiff;
    eepromValue[eepromAddIndex++] = setPWMParams->decelStartCountDiff;
    
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);   
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_PWM_DATA_MOTOR_FLASH_WRITE-1,(uint8_t *)eepromValue);
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

      setPWMCycleParams->decelTime              =  (uint32_t)eepromValue[eepromAddIndex]<<24;
      setPWMParams->decelTime                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMCycleParams->decelTime             +=  (uint32_t)eepromValue[eepromAddIndex]<<16;
      setPWMParams->decelTime                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMCycleParams->decelTime             +=  (uint32_t)eepromValue[eepromAddIndex]<<8;
      setPWMParams->decelTime                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMCycleParams->decelTime             +=  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->decelTime                  +=  (uint32_t)eepromValue[eepromAddIndex++];
      
      setPWMParams->Accel                       =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->Accel                      +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->Accel                      +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->Accel                      +=  (uint32_t)eepromValue[eepromAddIndex++];

      setPWMCycleParams->accelapproachPwm1       =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->accelapproachPwm1            =  (uint8_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->accelapproachPwm2       =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->accelapproachPwm2            =  (uint8_t)eepromValue[eepromAddIndex++]; 
      setPWMCycleParams->steadyPwm1              =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->steadyPwm1                   =  (uint8_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->steadyPwm2              =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->steadyPwm2                   =  (uint8_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->approachPwm1            =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->approachPwm1                 =  (uint8_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->approachPwm2            =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->approachPwm2                 =  (uint8_t)eepromValue[eepromAddIndex++];
      
      setPWMParams->Decel              =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->Decel             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->Decel             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->Decel             +=  (uint32_t)eepromValue[eepromAddIndex++];

      setPWMCycleParams->acelStartCountDiff      =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->acelStartCountDiff           =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMCycleParams->decelStartCountDiff     =  (uint32_t)eepromValue[eepromAddIndex];
      setPWMParams->decelStartCountDiff          =  (uint32_t)eepromValue[eepromAddIndex++];
    }
  }
  return status;  
}


eEepromStatus EepromSetBrushEnabledState(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_BRUSH_ENABLED_STATE_STARTADD;
  bool *setBrushEnabledState = GetSetBrushEnabledState();
  bool *setRobotEnabledState = GetSetLinearEnabledState(); 
  char eepromValue[JSON_NO_OF_MOTOR_MODE_STATE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = *setBrushEnabledState;
    eepromValue[eepromAddIndex++] = *setRobotEnabledState;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_MOTOR_MODE_STATE_DATA-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      *setBrushEnabledState             =  (bool)eepromValue[eepromAddIndex++];
      *setRobotEnabledState             =  (bool)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromSetdriveEnabledState(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_DRIVE_ENABLED_STATE_STARTADD;
  bool *setdrive1EnabledState = GetSetdrive1EnabledState();
  bool *setdrive2EnabledState = GetSetdrive2EnabledState();
  bool *setdrive3EnabledState = GetSetdrive3EnabledState();
  char eepromValue[JSON_NO_OF_MOTOR_MODE_STATE_DATA];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = *setdrive1EnabledState;
    eepromValue[eepromAddIndex++] = *setdrive2EnabledState;
    eepromValue[eepromAddIndex++] = *setdrive3EnabledState;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_MOTOR_MODE_STATE_DATA,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      *setdrive1EnabledState             =  (bool)eepromValue[eepromAddIndex++];
      *setdrive2EnabledState             =  (bool)eepromValue[eepromAddIndex++];
      *setdrive3EnabledState             =  (bool)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromSetBrushPwmManualMode(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_PWM_BRUSH_MANUAL_STARTADD;
  stBrushPwmParam *setPWMParams = GetSetBrushNormalPwmParameters();
  char eepromValue[JSON_NO_OF_PWM_DATA_BRUSH_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->accelTime;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->decelTime;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm;
    eepromValue[eepromAddIndex++] = setPWMParams->brushTime;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);    
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_PWM_DATA_BRUSH_FLASH-1,(uint8_t *)eepromValue);
    
    if(status == EEPROM_OK)
    {
      setPWMParams->accelTime              =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->accelTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->accelTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->accelTime             +=  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->decelTime              =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->decelTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->decelTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->decelTime             +=  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->steadyPwm             =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->brushTime             =  (uint32_t)eepromValue[eepromAddIndex++];
    }
  }
  return status;  
}

eEepromStatus EepromSetBrushMotorPolarity(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_BRUSH_MOTOR_POLARITY_STARTADD;
  stBrushMotPolarity *motPolarity = GetSetBrushMotPolarity();
  char eepromValue[JSON_NO_OF_MODE_STATE_DATA-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = motPolarity->mot1Polarity;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_MODE_STATE_DATA-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      motPolarity->mot1Polarity  =  (bool)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromSetMotorPwmAutoMode(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_PWM_MOTOR_AUTO_STARTADD;
  stRobotPwmParam *setPWMParams = GetSetCycleAutoPwmParameters();
  char eepromValue[JSON_NO_OF_PWM_DATA_MOTOR_FLASH_WRITE-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
     eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>8;  
    eepromValue[eepromAddIndex++] = setPWMParams->accelTime;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->decelTime;
    eepromValue[eepromAddIndex++] = (setPWMParams->Accel)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->Accel)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->Accel)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->Accel;
    eepromValue[eepromAddIndex++] = setPWMParams->accelapproachPwm1;
    eepromValue[eepromAddIndex++] = setPWMParams->accelapproachPwm2;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm1;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm2;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm1;
    eepromValue[eepromAddIndex++] = setPWMParams->approachPwm2;
    eepromValue[eepromAddIndex++] = (setPWMParams->Decel)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->Decel)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->Decel)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->Decel;
    eepromValue[eepromAddIndex++] = setPWMParams->acelStartCountDiff;
    eepromValue[eepromAddIndex++] = setPWMParams->decelStartCountDiff;
    
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);    
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_PWM_DATA_MOTOR_FLASH_WRITE-1,(uint8_t *)eepromValue);
  
    if(status == EEPROM_OK)
    {
      setPWMParams->accelTime                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->accelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->accelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->accelTime                   +=  (uint32_t)eepromValue[eepromAddIndex++];
      
      setPWMParams->decelTime                   =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->decelTime                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->decelTime                  +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->decelTime                  +=  (uint32_t)eepromValue[eepromAddIndex++];
      
      setPWMParams->Accel              =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->Accel             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->Accel             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->Accel             +=  (uint32_t)eepromValue[eepromAddIndex++];

      setPWMParams->accelapproachPwm1            =  (uint8_t)eepromValue[eepromAddIndex++];
      setPWMParams->accelapproachPwm2            =  (uint8_t)eepromValue[eepromAddIndex++]; 
      setPWMParams->steadyPwm1                   =  (uint8_t)eepromValue[eepromAddIndex++];
      setPWMParams->steadyPwm2                   =  (uint8_t)eepromValue[eepromAddIndex++];
      setPWMParams->approachPwm1                 =  (uint8_t)eepromValue[eepromAddIndex++];
      setPWMParams->approachPwm2                 =  (uint8_t)eepromValue[eepromAddIndex++];
      
      setPWMParams->Decel              =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->Decel             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->Decel             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->Decel             +=  (uint32_t)eepromValue[eepromAddIndex++];

      setPWMParams->acelStartCountDiff           =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->decelStartCountDiff          =  (uint32_t)eepromValue[eepromAddIndex++];
    }
  }
  return status;  
}

eEepromStatus EepromSetBrushPwmAutoMode(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_PWM_BRUSH_AUTO_STARTADD;
  stBrushPwmParam *setPWMParams = GetSetBrushAutoPwmParameters();
  char eepromValue[JSON_NO_OF_PWM_DATA_BRUSH_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->accelTime)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->accelTime;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>24;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>16;
    eepromValue[eepromAddIndex++] = (setPWMParams->decelTime)>>8;
    eepromValue[eepromAddIndex++] = setPWMParams->decelTime;
    eepromValue[eepromAddIndex++] = setPWMParams->steadyPwm;
    eepromValue[eepromAddIndex++] = setPWMParams->brushTime;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_PWM_DATA_BRUSH_FLASH-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      setPWMParams->accelTime             =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->accelTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->accelTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->accelTime             +=  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->decelTime             =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setPWMParams->decelTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setPWMParams->decelTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setPWMParams->decelTime             +=  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->steadyPwm             =  (uint32_t)eepromValue[eepromAddIndex++];
      setPWMParams->brushTime             =  (uint32_t)eepromValue[eepromAddIndex++];
    }
  }
  return status;  
}

eEepromStatus EepromSetAutoScheduleTime(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_AUTO_SCHEDULE_STARTADD;
  stScheduleTime *setScheduleValues = GetSetScheduleTime();
  char eepromValue[38];
  uint16_t eepromAddIndex = 0;
  
  if(eepromOp == EEPROM_WRITE)
  {
   for(int i=0;i<19;i++)
   {
      eepromValue[eepromAddIndex++] = setScheduleValues->HOUR[i];
      eepromValue[eepromAddIndex++] = setScheduleValues->MINUTE[i];
   }    
   Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,38,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      for(int i=0;i<19;i++)
      { 
        setScheduleValues->HOUR[i]=(uint8_t)eepromValue[eepromAddIndex++]; 
        setScheduleValues->MINUTE[i]=(uint8_t)eepromValue[eepromAddIndex++];
      }     
          
    }
  }
  return status; 
}

eEepromStatus EepromSetEdgeSensorState(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_EDGE_SENSOR_STATE_STARTADD;
  bool *setEdgeSensorState = GetSetEdgeSensorEnabledState();  
  bool *setRotateSensorState = GetSetRotateSensorEnabledState();
  
  char eepromValue[JSON_NO_OF_SENSOR_STATE_DATA];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = *setEdgeSensorState;
    eepromValue[eepromAddIndex++] = *setRotateSensorState;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_MODE_STATE_DATA,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      *setEdgeSensorState               =  (bool)eepromValue[eepromAddIndex++];
      *setRotateSensorState             =  (bool)eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromSetCurrentLimits(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_CURRENT_LIMIT_STARTADD;
  stMaxMotorCurrent *setCurrentValues = GetSetMaxMotorCurrentLimits();
  char eepromValue[JSON_NO_OF_CURRENT_LIMIT-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setCurrentValues->Imot1;
    eepromValue[eepromAddIndex++] = setCurrentValues->Imot2;
    eepromValue[eepromAddIndex++] = setCurrentValues->Imot3;
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_CURRENT_LIMIT-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      setCurrentValues->Imot1             =  eepromValue[eepromAddIndex++];
      setCurrentValues->Imot2             =  eepromValue[eepromAddIndex++];
      setCurrentValues->Imot3             =  eepromValue[eepromAddIndex++];
    }
  }
  return status; 
}

eEepromStatus EepromSetPulseCount(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_PULSE_COUNT_STARTADD;
  stCountRange *setCountValues = GetSetCountRange();
  char eepromValue[JSON_NO_OF_PULSE_COUNT_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = posdistance_int>>24;
    eepromValue[eepromAddIndex++] = posdistance_int>>16;
    eepromValue[eepromAddIndex++] = posdistance_int>>8;
    eepromValue[eepromAddIndex++] = posdistance_int;
    
    eepromValue[eepromAddIndex++] = negdistance_int>>24;
    eepromValue[eepromAddIndex++] = negdistance_int>>16;
    eepromValue[eepromAddIndex++] = negdistance_int>>8;
    eepromValue[eepromAddIndex++] = negdistance_int;
 
    //eepromValue[eepromAddIndex++] = setCountValues->maxPosCountForPVsetup;
    eepromValue[eepromAddIndex++] = setCountValues->maxPosCountForPVsetup>>24;
    eepromValue[eepromAddIndex++] = setCountValues->maxPosCountForPVsetup>>16;
    eepromValue[eepromAddIndex++] = setCountValues->maxPosCountForPVsetup>>8;
    eepromValue[eepromAddIndex++] = setCountValues->maxPosCountForPVsetup;
    
//    eepromValue[eepromAddIndex++] = setCountValues->maxNegCountForPVsetup;  
    eepromValue[eepromAddIndex++] = setCountValues->maxNegCountForPVsetup>>24;
    eepromValue[eepromAddIndex++] = setCountValues->maxNegCountForPVsetup>>16;
    eepromValue[eepromAddIndex++] = setCountValues->maxNegCountForPVsetup>>8;
    eepromValue[eepromAddIndex++] = setCountValues->maxNegCountForPVsetup;
    eepromValue[eepromAddIndex++] = Time_seconds_forward;
    eepromValue[eepromAddIndex++] = Time_seconds_backward;
 
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_PULSE_COUNT_FLASH-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      posdistance_int             =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      posdistance_int             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      posdistance_int             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      posdistance_int             +=  (uint32_t)eepromValue[eepromAddIndex++];
      negdistance_int             =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      negdistance_int             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      negdistance_int             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      negdistance_int             +=  (uint32_t)eepromValue[eepromAddIndex++];

//      setCountValues->maxPosCountForPVsetup     =  (uint16_t)eepromValue[eepromAddIndex++];
      setCountValues->maxPosCountForPVsetup             =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setCountValues->maxPosCountForPVsetup             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setCountValues->maxPosCountForPVsetup             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setCountValues->maxPosCountForPVsetup             +=  (uint32_t)eepromValue[eepromAddIndex++];
      
//      setCountValues->maxNegCountForPVsetup     =  (uint16_t)eepromValue[eepromAddIndex++];
      setCountValues->maxNegCountForPVsetup             =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setCountValues->maxNegCountForPVsetup             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setCountValues->maxNegCountForPVsetup             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setCountValues->maxNegCountForPVsetup             +=  (uint32_t)eepromValue[eepromAddIndex++];
      Time_seconds_forward                      =  (uint16_t)eepromValue[eepromAddIndex++];
      Time_seconds_backward                     =  (uint16_t)eepromValue[eepromAddIndex++];
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
    eepromValue[eepromAddIndex++] = storeHeartbeatConfig->enable;   
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->interval_ms)>>24;
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->interval_ms)>>16;
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->interval_ms)>>8;
    eepromValue[eepromAddIndex++] = storeHeartbeatConfig->interval_ms;
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->noOfMessages)>>24;
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->noOfMessages)>>16;
    eepromValue[eepromAddIndex++] = (storeHeartbeatConfig->noOfMessages)>>8;
    eepromValue[eepromAddIndex++] = storeHeartbeatConfig->noOfMessages;
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
      storeHeartbeatConfig->enable = eepromValue[eepromAddIndex++];
      storeHeartbeatConfig->interval_ms             =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      storeHeartbeatConfig->interval_ms             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      storeHeartbeatConfig->interval_ms             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      storeHeartbeatConfig->interval_ms             +=  (uint32_t)eepromValue[eepromAddIndex++];
   
      storeHeartbeatConfig->noOfMessages             =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      storeHeartbeatConfig->noOfMessages             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      storeHeartbeatConfig->noOfMessages             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      storeHeartbeatConfig->noOfMessages             +=  (uint32_t)eepromValue[eepromAddIndex++];
      
      storeHeartbeatConfig->maxReconnectTime             =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      storeHeartbeatConfig->maxReconnectTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      storeHeartbeatConfig->maxReconnectTime             +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      storeHeartbeatConfig->maxReconnectTime             +=  (uint32_t)eepromValue[eepromAddIndex++]; 
    }
  }
  return status;
}


eEepromStatus EepromSetMotorFaultConditions(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_MOT_FAULT_STARTADD;
  stMaxIloadConfigValues *setMaxIloadConfigValues = GetSetMaxIloadConfigValues();
  char eepromValue[JSON_NO_OF_MOTOR_FAULT_CONDITIONS_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->maxIloadNoiseTimeValue)>>24;
    eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->maxIloadNoiseTimeValue)>>16;
    eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->maxIloadNoiseTimeValue)>>8;
    eepromValue[eepromAddIndex++] = setMaxIloadConfigValues->maxIloadNoiseTimeValue;
     eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->maxIloadFreqTimeValue)>>24;
    eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->maxIloadFreqTimeValue)>>16;
    eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->maxIloadFreqTimeValue)>>8;
    eepromValue[eepromAddIndex++] = setMaxIloadConfigValues->maxIloadFreqTimeValue;
     eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->maxIloadRepeatCountValue)>>24;
    eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->maxIloadRepeatCountValue)>>16;
    eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->maxIloadRepeatCountValue)>>8;
    eepromValue[eepromAddIndex++] = setMaxIloadConfigValues->maxIloadRepeatCountValue;
     eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->motorPauseDelay)>>24;
    eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->motorPauseDelay)>>16;
    eepromValue[eepromAddIndex++] = (setMaxIloadConfigValues->motorPauseDelay)>>8;
    eepromValue[eepromAddIndex++] = setMaxIloadConfigValues->motorPauseDelay;

    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_MOTOR_FAULT_CONDITIONS_FLASH-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    { 
      setMaxIloadConfigValues->maxIloadNoiseTimeValue         =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setMaxIloadConfigValues->maxIloadNoiseTimeValue        +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setMaxIloadConfigValues->maxIloadNoiseTimeValue        +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setMaxIloadConfigValues->maxIloadNoiseTimeValue        +=  (uint32_t)eepromValue[eepromAddIndex++];
      setMaxIloadConfigValues->maxIloadFreqTimeValue          =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setMaxIloadConfigValues->maxIloadFreqTimeValue         +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setMaxIloadConfigValues->maxIloadFreqTimeValue         +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setMaxIloadConfigValues->maxIloadFreqTimeValue         +=  (uint32_t)eepromValue[eepromAddIndex++];
      setMaxIloadConfigValues->maxIloadRepeatCountValue       =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setMaxIloadConfigValues->maxIloadRepeatCountValue      +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setMaxIloadConfigValues->maxIloadRepeatCountValue      +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setMaxIloadConfigValues->maxIloadRepeatCountValue      +=  (uint32_t)eepromValue[eepromAddIndex++];
      setMaxIloadConfigValues->motorPauseDelay                =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
      setMaxIloadConfigValues->motorPauseDelay               +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
      setMaxIloadConfigValues->motorPauseDelay               +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
      setMaxIloadConfigValues->motorPauseDelay               +=  (uint32_t)eepromValue[eepromAddIndex++];
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
  
  eEepromStatus EepromSetWheelDia(eEpromAccessType eepromOp)
{ 
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_WHEEL_DIA_STARTADD;
  stWheelDia *setWheelDia = GetSetwheelDiaLimits();
  char eepromValue[JSON_NO_OF_WHEELDIA_DATA_FLASH-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setWheelDia->Dia;
    eepromValue[eepromAddIndex++] = setWheelDia->Pulse;
    eepromValue[eepromAddIndex++] = setWheelDia->Speed;
    eepromValue[eepromAddIndex++] = CYCLEROTATIONTIME;
    
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_WHEELDIA_DATA_FLASH-1,(uint8_t *)eepromValue);
    
    if(status == EEPROM_OK)
    {
      setWheelDia->Dia = eepromValue[eepromAddIndex++];
      setWheelDia->Pulse = eepromValue[eepromAddIndex++];
      setWheelDia->Speed = eepromValue[eepromAddIndex++];
      CYCLEROTATIONTIME = eepromValue[eepromAddIndex++];
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
    uint16_t eepromAddress = STORE_COMDISTANCE_STARTADD;
    stcomdistance* setcomdistance = GetSetComDistanceLimits();
    char eepromValue[JSON_NO_OF_COMDISTANCE_FLASH+1];
    uint16_t eepromAddIndex = 0;
    
    if(eepromOp == EEPROM_WRITE)
    {
      eepromValue[eepromAddIndex++] = ComDistance_int>>24;
      eepromValue[eepromAddIndex++] = ComDistance_int>>16;
      eepromValue[eepromAddIndex++] = ComDistance_int>>8;  
      eepromValue[eepromAddIndex++] = ComDistance_int;
      eepromValue[eepromAddIndex++] = setcomdistance->CDistance; 
      
      Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex);
    }
    else
    {
      memset(eepromValue,Null,sizeof(eepromValue));
      Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_COMDISTANCE_FLASH,(uint8_t *)eepromValue);
      if(status == EEPROM_OK)
      {
        ComDistance_int         =  (uint32_t)eepromValue[eepromAddIndex++]<<24;
        ComDistance_int       +=  (uint32_t)eepromValue[eepromAddIndex++]<<16;
        ComDistance_int        +=  (uint32_t)eepromValue[eepromAddIndex++]<<8;
        ComDistance_int        +=  (uint32_t)eepromValue[eepromAddIndex++];
        setcomdistance->CDistance  =  eepromValue[eepromAddIndex++]; 
      }
    }
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
  
  
  
eEepromStatus EepromStoreTrackchangerMAC(eEpromAccessType eepromOp)
{
  eEepromStatus status = EEPROM_OK;
  uint16_t eepromAddress = STORE_CHANGER_MAC_STARTADD;
  unChangerMAC* setChangerMAC;
  setChangerMAC = GetSetChangerMAC();
  char eepromValue[JSON_NO_OF_CHANGER_MAC_DATA_flash-1];
  uint16_t eepromAddIndex = 0;
  if(eepromOp == EEPROM_WRITE)
  {
    eepromValue[eepromAddIndex++] = setChangerMAC->changerMAC_16[0];
    eepromValue[eepromAddIndex++] = setChangerMAC->changerMAC_16[1];
    eepromValue[eepromAddIndex++] = setChangerMAC->changerMAC_16[2];
    eepromValue[eepromAddIndex++] = setChangerMAC->changerMAC_16[3];
    eepromValue[eepromAddIndex++] = setChangerMAC->changerMAC_16[4];
    eepromValue[eepromAddIndex++] = setChangerMAC->changerMAC_16[5];
    eepromValue[eepromAddIndex++] = setChangerMAC->changerMAC_16[6];
    eepromValue[eepromAddIndex++] = setChangerMAC->changerMAC_16[7];
    Write_SST_26_VF_016B_Data(eepromAddress,(uint8_t *)&eepromValue,eepromAddIndex); 
  }
  else
  {
    memset(eepromValue,Null,sizeof(eepromValue));
    Read_SST_26_VF_016B_Data( eepromAddress,JSON_NO_OF_CHANGER_MAC_DATA_flash-1,(uint8_t *)eepromValue);
    if(status == EEPROM_OK)
    {
      setChangerMAC->changerMAC_16[0]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setChangerMAC->changerMAC_16[1]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setChangerMAC->changerMAC_16[2]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setChangerMAC->changerMAC_16[3]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setChangerMAC->changerMAC_16[4]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setChangerMAC->changerMAC_16[5]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setChangerMAC->changerMAC_16[6]             =  (uint16_t)eepromValue[eepromAddIndex++];
      setChangerMAC->changerMAC_16[7]             =  (uint16_t)eepromValue[eepromAddIndex++];
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
