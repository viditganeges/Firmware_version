#include "DefaultValues.h"
#include "DefaultValuesConfig.h"
#include "RobotControl.h"
#include "BrushControl.h"
#include "FaultProcess.h"
#include "AutoSchedule.h"
#include "CycleMode.h"
#include "RobotOperation.h"
#include "BrushMotorControl_hal.h"
#include "Motor1Control_hal.h"
#include "Motor2Control_hal.h"
#include "MotCurrentFaultCommon.h"
#include "EdgeSenseCommon.h"
#include "Temp_Sensors.h"
#include "CommCommon.h"
#include "APIProcessing.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

static stDevInfo    manufacturerDeviceInfo;
static void GetDefaultMotionParameter (stRobotPwmParam *robotPwmParam);

void SetDefaultNil(void)
{
  
}

stDevInfo * GetSetDeviceIDInfo (void)
{
  return(&manufacturerDeviceInfo);
}

void SetDefaultDevIDInfo(void)
{
  stDevInfo *defDevInfo = GetSetDeviceIDInfo();
  snprintf(defDevInfo->devID, MAX_DEV_INFO_FIELD_LEN, DEFAULT_DEV_ID);
  snprintf(defDevInfo->hwVersion, MAX_DEV_INFO_FIELD_LEN, DEFAULT_DEV_HW_REV);
  snprintf(defDevInfo->serialNo, MAX_DEV_INFO_FIELD_LEN, DEFAULT_DEV_SERIAL_NO);
}

void SetDefaultPwmCycleManualModeParameter (void)
{
  stRobotPwmParam *robotPwmParam = GetSetCycleManualPwmParameters();
  GetDefaultMotionParameter(robotPwmParam);
  robotPwmParam->steadyPwm1 = DEFAULT_MANUAL_PWM_DUTY;
  robotPwmParam->steadyPwm2 = DEFAULT_MANUAL_PWM_DUTY;
}

void SetDefaultPwmCycleAutoModeParameter (void)
{
  stRobotPwmParam *robotPwmParam = GetSetCycleAutoPwmParameters();
  GetDefaultMotionParameter(robotPwmParam);
  robotPwmParam->steadyPwm1 = DEFAULT_AUTO_PWM_DUTY;
  robotPwmParam->steadyPwm2 = DEFAULT_AUTO_PWM_DUTY;
}


void SetDefaultPwmManulModeParameter (void)
{
  stRobotPwmParam *robotPwmParam = GetSetManualPwmParameters();
  GetDefaultMotionParameter(robotPwmParam);
  robotPwmParam->steadyPwm1 = DEFAULT_MANUAL_PWM_DUTY;
  robotPwmParam->steadyPwm2 = DEFAULT_MANUAL_PWM_DUTY;
  SetDefaultPwmCycleManualModeParameter();
}

void SetDefaultPwmEmergencyModeParameter (void)
{
  stRobotPwmParam *robotPwmParam = GetSetEmergencyPwmParameters();
  GetDefaultMotionParameter(robotPwmParam);
  robotPwmParam->steadyPwm1 = DEFAULT_EMERGENCY_PWM_DUTY;
  robotPwmParam->steadyPwm2 = DEFAULT_EMERGENCY_PWM_DUTY;
}
void SetDefaultBrushPwmNormalModeParameter (void)
{
  stBrushPwmParam *brushPwmParam       = GetSetBrushNormalPwmParameters();
  brushPwmParam->accelTime             = 1;
  brushPwmParam->decelTime             = 1;
  brushPwmParam->steadyPwm             = 1;
}

void SetDefaultBrushPwmAutoModeParameter (void)
{
  stBrushPwmParam *brushPwmParam       = GetSetBrushAutoPwmParameters();
  brushPwmParam->accelTime             = 1;
  brushPwmParam->decelTime             = 1;
  brushPwmParam->steadyPwm             = 1;
}

static void GetDefaultMotionParameter (stRobotPwmParam *robotPwmParam)
{
  robotPwmParam->decelStartCountDiff   = DEFAULT_DECEL_START_COUNT_DIFF;
  robotPwmParam->accelTime             = DEFAULT_PWM_ACCEL_TIME;
  robotPwmParam->decelTime             = DEFAULT_PWM_DECEL_TIME;
  robotPwmParam->steadyPwm1            = DEFAULT_MANUAL_PWM_DUTY;
  robotPwmParam->steadyPwm2            = DEFAULT_MANUAL_PWM_DUTY;
  robotPwmParam->approachPwm1          = DEFAULT_APPROACH_PWM_DUTY;
  robotPwmParam->approachPwm2          = DEFAULT_APPROACH_PWM_DUTY;
}

//void SetDefaultMotorConditions(void)
//{
//  stMotorConditions * defaultMotorOnCondition = GetSetMotorConditions();
//  defaultMotorOnCondition->minBattVoltage_motor           = (float)DEFAULT_MIN_BATT_VOLTAGE_MOTOR;
//  defaultMotorOnCondition->maxMotorCurrent                = (float)DEFAULT_MAX_MOTOR_CURRENT_MOTOR;  
//  stMaxIloadConfigValues* defaultMaxIloadConfigValues     = GetSetMaxIloadConfigValues();
//  defaultMaxIloadConfigValues->maxIloadNoiseTimeValue     = DEFAULT_ILOAD_MAX_NOISE_TIME;
//  defaultMaxIloadConfigValues->motorPauseDelay            = DEFAULT_PAUSE_DELAY_TIME;    
//  defaultMaxIloadConfigValues->maxIloadRepeatCountValue   = DEFAULT_ILOAD_MAX_REPEAT_COUNT;
//  defaultMaxIloadConfigValues->maxIloadFreqTimeValue      = DEFAULT_ILOAD_MAX_FREQ_TIME;
//}

void SetDefaultCountRange(void)
{
  stCountRange * defaultCountRange = GetSetCountRange();
  defaultCountRange->maxPosCountForPVsetup        = (float)DEFAULT_MAX_POS_COUNT_FOR_PV_SETUP;
  defaultCountRange->maxNegCountForPVsetup        = (float)DEFAULT_MAX_NEG_COUNT_FOR_PV_SETUP;
}

void SetDefaultMaxBoardTemperature(void)
{
  stBoardTemperature *defaultMaxBoardTemperature = GetSetMaxBoardTemperature();
  defaultMaxBoardTemperature->maxTempSensorOne   = (float)DEFAULT_MAX_TEMP_SENSOR_ONE;
  defaultMaxBoardTemperature->maxTempSensorTwo   = (float)DEFAULT_MAX_TEMP_SENSOR_TWO;
}

void SetDefaultAutoScheduledTime(void)
{
  stScheduleTime *defaultScheduleValues = GetSetScheduleTime();
  defaultScheduleValues->hour             =  DEFAULT_AUTO_SCHEDULED_TIME_HOUR;
  defaultScheduleValues->minute           =  DEFAULT_AUTO_SCHEDULED_TIME_MINUTE;
}

void SetDefaultBrushMotPolarity(void)
{
  stBrushMotPolarity *defaultMotPolarity = GetSetBrushMotPolarity();
  defaultMotPolarity->mot1Polarity   = (bool)DEFAULT_BRUSH_MOT_DIRECTION;
}

void SetDefaultMot1Polarity(void)
{
  stMot1Polarity *defaultMotPolarity = GetSetMot1Polarity();
  defaultMotPolarity->mot1Polarity   = (bool)DEFAULT_MOT_1_DIRECTION;
}

void SetDefaultMot2Polarity(void)
{
  stMot2Polarity *defaultMotPolarity = GetSetMot2Polarity();
  defaultMotPolarity->mot2Polarity   = (bool)DEFAULT_MOT_2_DIRECTION;
}

void SetDefaultIloadConfigValues(void)
{
  stMaxIloadConfigValues* defaultMaxIloadConfigValues     = GetSetMaxIloadConfigValues();
  defaultMaxIloadConfigValues->maxIloadNoiseTimeValue     = DEFAULT_ILOAD_MAX_NOISE_TIME;
  defaultMaxIloadConfigValues->motorPauseDelay            = DEFAULT_PAUSE_DELAY_TIME;    
  defaultMaxIloadConfigValues->maxIloadRepeatCountValue   = DEFAULT_ILOAD_MAX_REPEAT_COUNT;
  defaultMaxIloadConfigValues->maxIloadFreqTimeValue      = DEFAULT_ILOAD_MAX_FREQ_TIME;
}

void SetDefaultMaxMotCurrentLimits(void)
{
    stMaxMotorCurrent* defaultMaxIloadLimits = GetSetMaxMotorCurrentLimits();
    defaultMaxIloadLimits->Imot1 = DEFAULT_ILOAD1_MAX_VALUE;
    defaultMaxIloadLimits->Imot2 = DEFAULT_ILOAD2_MAX_VALUE;
    defaultMaxIloadLimits->Imot3 = DEFAULT_ILOAD3_MAX_VALUE;
}

void SetDefaultZigbeeConfig(void)
{
  
}

void SetDefaultMinBatSoc(void)
{
  uint8_t *setLowBatSoC = GetSetMinBattSoc();
  *setLowBatSoC = DEFAULT_MIN_BATT_SOC;
}

void SetDefaultBrushEnabledState(void)
{
  bool *defaultBrushEnabledState = GetSetBrushEnabledState();
  *defaultBrushEnabledState = DEFAULT_BRUSH_ENABLED_STATE;
}

void SetDefaultEdgeSensorEnabledState(void)
{
  bool *defaultEdgeSensorState = GetSetEdgeSensorEnabledState();
  *defaultEdgeSensorState = DEFAULT_EDGE_SENSOR_ENABLED_STATE;
}

void SetDefaultOperationMode (void)
{
  SetInitialOperationMode(IDLE);
}

void SetDefaultHeartbeatConfig(void)
{
  stHeartbeatConfig *defaultHeartbeatConfig = GetSetHeartbeatConfig();
//  defaultHeartbeatConfig->enable = HEARTBEAT_ENABLE_DEFAULT;
  defaultHeartbeatConfig->interval_ms = HEARTBEAT_INTERVAL_MS_DEFAULT;
  //defaultHeartbeatConfig->noOfMessages = NO_OF_HEARTBEAT_MESSAGES_DEFAULT;
  defaultHeartbeatConfig->maxReconnectTime = MAX_ZIGBEE_RECONNECT_TIME_DEFAULT;
}

void DefaultValuesInit (void)
{
  SetDefaultPwmEmergencyModeParameter();
  SetDefaultMot1Polarity();
  SetDefaultMot2Polarity();
  //SetDefaultIloadConfigValues();
  SetDefaultMaxBoardTemperature();
  SetDefaultBrushEnabledState();
  SetDefaultBrushPwmNormalModeParameter();
  SetDefaultBrushMotPolarity();
  SetDefaultBrushPwmAutoModeParameter();
}
void SetDefaultTrackInfo(void)
{
  stTrackData *setTrackData = GetSetTrackData();
  for(int i=0;i<MAX_NO_OF_TRACKS;i++)
  {
    setTrackData->trackValid[i] = DEFAULT_TRACK_COUNT;
  }
  for(int i=0;i<MAX_NO_OF_TRACKS;i++)
  {
    setTrackData->trackDistance[i] = DEFAULT_TRACK_COUNT;
  }
}

void SetDefaultTrackInfo2(void)
{
  stTrackData *setTrackData = GetSetTrackData();
  for(int i=0;i<MAX_NO_OF_TRACKS;i++)
  {
    setTrackData->trackValid2[i] = DEFAULT_TRACK_COUNT;
  }
  for(int i=0;i<MAX_NO_OF_TRACKS;i++)
  {
    setTrackData->trackDistance2[i] = DEFAULT_TRACK_COUNT;
  }
}

void SetDefaultRfidData(void)
{
stRfidData *setRfidData = GetSetRfidData();
  for(int i=0;i<MAX_NO_OF_TRACKS;i++)
  {
    setRfidData->rfidData[i] = DEFAULT_RFID_DATA;
  }
}

void SetDefaultRfidData2(void)
{
stRfidData *setRfidData = GetSetRfidData();
  for(int i=0;i<MAX_NO_OF_TRACKS;i++)
  {
    setRfidData->rfidData2[i] = DEFAULT_RFID_DATA;
  }
}

void SetDefaultRfidData3(void)
{
stRfidData *setRfidData = GetSetRfidData();
  for(int i=0;i<MAX_NO_OF_TRACKS;i++)
  {
    setRfidData->rfidData3[i] = DEFAULT_RFID_DATA;
  }
}

void SetDefaultRowLength(void)
{
stRowLengthData *setRowLengthData = GetSetRowLengthData();
for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      setRowLengthData->rowLength[i][FORWARD_COUNT] = DEFAULT_FORWARD_COUNT;
      setRowLengthData->rowLength[i][REVERSE_COUNT] = DEFAULT_REVERSE_COUNT;
    }
}

void SetDefaultEncoderPulseTolerance(void)
{
  uint32_t *setEncoderPulseTolerance = GetSetEncoderPulseTolerance();
  
  *setEncoderPulseTolerance = DEFAULT_ENCODER_PULSE_TOLERANCE;
}

void SetDefaultTcControlMode(void)
{
SetTcControlMode(PROXIMITY_MODE);    
}

void SetDefaultWheelDia(void)
{
  stWheelDia *defaultWheelDia = GetSetwheelDiaLimits();
//  defaultWheelDia->Dia = (float)DEFAULT_WHEEL_DIA;  
//  defaultWheelDia->Pulse = (float)DEFAULT_PULSE;  
//  defaultWheelDia->Speed = DEFAULT_TIME_PWM;
//  CYCLEROTATIONTIME = DEFAULT_TIME;
  circumference = (float) ((float) 3.14 * (float) defaultWheelDia->Dia);
}

void SetDefaultLogInterval(void)
{
  stInterval *defaultInterval = GetSetIntervalLimits();
  defaultInterval->I1 =  DEFAULT_INTERVAL;
  defaultInterval->I2 =  DEFAULT_INTERVAL;
  defaultInterval->I3 =  DEFAULT_INTERVAL;
  defaultInterval->I4 =  DEFAULT_INTERVAL;
  defaultInterval->I5 =  DEFAULT_INTERVAL;
  defaultInterval->I6 =  DEFAULT_INTERVAL;
  defaultInterval->P1 = 1;
  defaultInterval->P2 = 2;
  defaultInterval->P3 = 3;
  defaultInterval->P4 = 4;
  defaultInterval->P5 = 5;
  defaultInterval->P6 = 6;
}

void SetDefaultCycleFrequency(void)
{
  stCycleFrequency *defaultCycleFrequency = GetSetcycleFrequencyLimits();
  defaultCycleFrequency->Cycle = DEFAULT_CYCLE_FREQUENCY;
}

void SetDefaultContinue(void)
{
   stContinue *defaultContinueValue = GetSetContinueLimits();
   defaultContinueValue->Continue1 = (float)DEFAULT_COUNTINUE;
   defaultContinueValue->Count1 = DEFAULT_COUNT;
}

void SetDefaultReturnState(void)
{
  stReturn *defaultReturnValue = GetSetReturnLimits();
  defaultReturnValue->Return1 = DEFAULT_COUNT;
}

//SetDefaultCommDistance
void SetDefaultCommDistance(void)
{
 // stReturn *defaultReturnValue = GetSetReturnLimits();
  strunrfid* defaultrunrfid = GetSetrunrfidLimits();
  defaultrunrfid->runrfid = DEFAULT_COUNTINUE;
}

void SetDefaultNoofRowday(void)
{
   stnoofrows* defaultNoofrows = GetSetNoofrowsLimits();
  
  for(int i=0;i<7;i++)
      {
        defaultNoofrows->Row[i] = 0;
      }
//  defaultNoofrows->R1 = 0;defaultNoofrows->R2 = 0;defaultNoofrows->R3 = 0;defaultNoofrows->R4 = 0;
//  defaultNoofrows->R5 = 0;defaultNoofrows->R6 = 0;defaultNoofrows->R7 = 0;
  
  defaultNoofrows->TRow = 0;
}

void SetDefaultRowLength2(void)
{
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      setRowLengthData->rowLength2[i][FORWARD_COUNT] = DEFAULT_FORWARD_COUNT;
      setRowLengthData->rowLength2[i][REVERSE_COUNT] = DEFAULT_REVERSE_COUNT;
    }
}

void SetDefaultRowLength3(void)
{
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      setRowLengthData->rowLength3[i][FORWARD_COUNT] = DEFAULT_FORWARD_COUNT;
      setRowLengthData->rowLength3[i][REVERSE_COUNT] = DEFAULT_REVERSE_COUNT;
    }
}

void SetDefaultRowLength4(void)
{
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      setRowLengthData->rowLength4[i][FORWARD_COUNT] = DEFAULT_FORWARD_COUNT;
      setRowLengthData->rowLength4[i][REVERSE_COUNT] = DEFAULT_REVERSE_COUNT;
    }
}

void SetDefaultRowLength5(void)
{
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      setRowLengthData->rowLength5[i][FORWARD_COUNT] = DEFAULT_FORWARD_COUNT;
      setRowLengthData->rowLength5[i][REVERSE_COUNT] = DEFAULT_REVERSE_COUNT;
    }
}

void SetDefaultRowLength6(void)
{
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      setRowLengthData->rowLength6[i][FORWARD_COUNT] = DEFAULT_FORWARD_COUNT;
      setRowLengthData->rowLength6[i][REVERSE_COUNT] = DEFAULT_REVERSE_COUNT;
    }
}

void SetDefaultRowLength7(void)
{
  stRowLengthData *setRowLengthData = GetSetRowLengthData();
for(int i=0;i<MAX_NO_OF_TRACKS;i++)
    {
      setRowLengthData->rowLength7[i][FORWARD_COUNT] = DEFAULT_FORWARD_COUNT;
      setRowLengthData->rowLength7[i][REVERSE_COUNT] = DEFAULT_REVERSE_COUNT;
    }
}
