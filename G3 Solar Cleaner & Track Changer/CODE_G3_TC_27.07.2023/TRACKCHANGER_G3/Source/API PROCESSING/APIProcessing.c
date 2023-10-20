#include "APIProcessing.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "NumStrConversion.h"
#include "ValuesValidate.h"
#include "Serial_Debug.h"
#include "TempVariables.h"
#include "RobotOperation.h"
#include "RobotControl.h"
#include "RotateSenseCommon.h"
#include "EdgeSenseCommon.h"
#include "DefaultValues.h"
#include "DefaultValuesConfig.h"
#include "CycleMode.h"
#include "LTC4015_Main.h"
#include "Rtc.h"
#include "Adc.h"
#include "BrushControl.h"
#include "BrushMotorControl_hal.h"
#include "AutoSchedule.h"
#include "MotCurrentFaultCommon.h"
#include "ZigbeeApiMode.h"
#include "FaultProcess.h"
#include "temp_sensors.h"
#include "EventLoggerFlash.h"
#include "EventLogger.h"
#include "ConfigAPIProcess.h"
#include "ResponseAPIProcess.h"
#include "cleanerdetect1.h"
#include "cleanerdetect2.h"
#include "proximity.h"
#include "rfid.h"
#include "cleanerdetectcommon.h"
#include "SDFAT/SDcardoperation.h"
#include "SDFAT/ff.h"      
#include "SDFAT/ffconf.h"
#include "SDFAT/diskio.h"
#include "SDFAT/integer.h"
#include "switch.h"

#define MAX_JASON_VALUE_LENGTH             200U
#define LOG_STR_LEN                        35U    
#define MAX_NO_OF_EVENT_LOGS               50

#define CMD_SET_MODE                          0x0001                                                       
#define CMD_SET_MOTOR_DIR_MANUAL              0x0003
#define CMD_SET_MOTOR_PWM_MANUAL              0x0004
#define CMD_SET_BRUSH_MOTOR_STATE             0x0005
#define CMD_SET_BRUSH_MOTOR_PWM_MANUAL        0x0006
#define CMD_SET_BRUSH_MOTOR_DIR               0x0007
#define CMD_SET_MOTOR_PWM_AUTO                0x0009
#define CMD_SET_BRUSH_MOTOR_PWM_AUTO          0x000A
#define CMD_SET_AUTO_SCHEDULE_TIME            0x000B
#define CMD_SET_EDGE_SENSE_STATE              0x000C
#define CMD_SET_RTC_VALUE                     0x000D
#define CMD_SET_DEVICE_INFO                   0x000E
#define CMD_SET_OVERCURRENT_LIMIT             0x000F
#define CMD_CLEAR_ERRORCODES                  0x0010
#define CMD_SET_PULSE_COUNT_VALUES            0x0011
#define CMD_SET_LOW_BAT_SOC                   0x0012
#define CMD_SET_ZIGBEE_CONF                   0x0013   
#define CMD_RESET_ZIGBEE_NTW                  0x0014
#define CMD_RESET_TO_DEFAULTS                 0x0015
#define CMD_SET_MOTOR_FAULT_CONDITIONS        0x0016
#define CMD_CLEAR_ACTIVITY_LOG                0x0017  
#define CMD_SET_TRACK_COUNT                   0x0018             //not used
//#define CMD_SET_TRACK_INFO                    0x0019        //Track Info One
#define CMD_CLEAR_TRACK_INFO                  0x001A
#define CMD_SET_CLEANER_MAC                   0x001B  
//#define CMD_SET_ROW_LENGTH_DATA               0x001C      //Row length chnaged
#define CMD_SET_ENC_PULSE_TOLERANCE           0x001D
#define CMD_SET_TC_CONTROL_MODE               0x001E
//#define CMD_SET_RFID_DATA                     0x001F
#define CMD_SET_RFID_STATE                    0x0020
#define CMD_SET_HEARTBEAT_CONFIG              0x0021

#define CMD_GET_CURRENT_MODE                  0x1001
#define CMD_GET_MOTOR_DIR_MANUAL              0x1003
#define CMD_GET_MOTOR_PWM_MANUAL              0x1004
#define CMD_GET_BRUSH_MOTOR_STATE             0x1005
#define CMD_GET_BRUSH_MOTOR_PWM_MANUAL        0x1006
#define CMD_GET_BRUSH_MOTOR_DIR               0x1007
#define CMD_GET_MOTOR_PWM_AUTO                0x1009
#define CMD_GET_BRUSH_MOTOR_PWM_AUTO          0x100A
#define CMD_GET_AUTO_SCHEDULE_TIME            0x100B
#define CMD_GET_EDGE_SENSE_STATE              0x100C
#define CMD_GET_RTC_VALUE                     0x100D
#define CMD_GET_CURRENT_STAT                  0x100E
#define CMD_GET_LAST_CYCLE_STAT               0x100F
#define CMD_GET_OVERCURRENT_LIMIT             0x1010
#define CMD_GET_MOT_CURRENT_VALUES            0x1011
#define CMD_GET_PULSE_COUNT_VALUES            0x1014
#define CMD_GET_LOW_BAT_SOC                   0x1015
//#define CMD_GET_SOC                           0x1016      //
#define CMD_GET_CHARGER_STATE                 0x1017
#define CMD_GET_CONTROLLER_STATUS             0x1018
#define CMD_GET_DEVICE_INFO                   0x1019
#define CMD_GET_ZIGBEE_CONF                   0x101A
#define CMD_GET_ZIGBEE_NTW_PARAM              0x101B
#define CMD_GET_BAT_AND_PV_VAL                0x101C      //this is used
#define CMD_GET_TEMPERATURES                  0x101D
#define CMD_GET_MOTOR_FAULT_CONDITIONS        0x101E
#define CMD_REQ_ACTIVITY_LOG                  0x101F 
//#define CMD_GET_TRACK_INFO                    0x1020       //Track Info 
#define CMD_GET_CLEANER_MAC                   0x1022
//#define CMD_GET_ROW_LENGTH_DATA               0x1023   //rowlength changed
//#define CMD_GET_TRACK_CHANGER_STATUS          0x1024      //
#define CMD_GET_SENSOR_STATUS                 0x1025
#define CMD_GET_ENC_PULSE_TOLERANCE           0x1026
#define CMD_GET_TC_CONTROL_MODE               0x1027
//#define CMD_GET_RFID_DATA                     0x1028
#define CMD_GET_RFID_STATE                    0x1029
#define CMD_GET_HEARTBEAT_CONFIG              0x102A


#define CMD_DEVCIE_WELCOME_MSG                0x1100
#define CMD_ACK_DATA                          0x1111 
#define CMD_REPORT_MSG                        0x2001

#define CMD_SET_LOG_INTERVAL                  0x0030
#define CMD_GET_LOG_INTERVAL                  0x1030
#define CMD_SET_DIA_WHEEL                     0x0031
#define CMD_SET_CYCLE_FREQUENCY               0x0032
#define CMD_GET_DIA_WHEEL                     0x1031
#define CMD_GET_CYCLE_FREQUENCY               0x1032
#define CMD_REQ_LOG                           0x103F
#define CMD_SET_CONTINUE                      0x0033
#define CMD_GET_CONTINUE_COUNT                0x1033
#define CMD_SET_RETURN_STATE                  0x0034
#define CMD_GET_RETURN_STATE                  0x1034

#define CMD_SET_RUNRFID                       0x0035
#define CMD_GET_RUNRFID                       0x1035

#define CMD_SET_N0_ROW_DAY                    0x0036
#define CMD_GET_N0_ROW_DAY                    0x1036

#define CMD_SET_TRACK_INFO                    0x0037 
#define CMD_GET_TRACK_INFO                    0x1037 
#define CMD_SET_TRACK_INFO2                   0x0038
#define CMD_GET_TRACK_INFO2                   0x1038

#define CMD_SET_RFID_DATA                     0x0039
#define CMD_GET_RFID_DATA                     0x1039
#define CMD_SET_RFID_DATA2                    0x0040
#define CMD_GET_RFID_DATA2                    0x1040

#define CMD_SET_ROW_LENGTH_DATA               0x0041 
#define CMD_GET_ROW_LENGTH_DATA               0x1041
#define CMD_SET_ROW_LENGTH2_DATA              0x0042
#define CMD_GET_ROW_LENGTH2_DATA              0x1042
#define CMD_SET_ROW_LENGTH3_DATA              0x0043
#define CMD_GET_ROW_LENGTH3_DATA              0x1043
#define CMD_SET_ROW_LENGTH4_DATA              0x0044
#define CMD_GET_ROW_LENGTH4_DATA              0x1044
#define CMD_SET_ROW_LENGTH5_DATA              0x0045
#define CMD_GET_ROW_LENGTH5_DATA              0x1045
#define CMD_SET_ROW_LENGTH6_DATA              0x0046
#define CMD_GET_ROW_LENGTH6_DATA              0x1046
#define CMD_SET_ROW_LENGTH7_DATA              0x0047
#define CMD_GET_ROW_LENGTH7_DATA              0x1047

#define CMD_SET_RFID_DATA3                    0x004A
#define CMD_GET_RFID_DATA3                    0x104A


char Cstarttime[40]={'0'};
char Cstartsoc[5]={'0'};
char Cendtime[40]={'0'};
char Cendsoc[5]={'0'};
char Cerrorcount[5]={'0'};

int distance =0;                        // for Distance calculation
int Distancevalue;
int Row1value=0;
int Row1getvalue = 0;
uint32_t sum = 0;

char getTime_str[40];
char Battemp_str[10],Drivetemp_str[10],Dietemp_str[10];
char Vin_str[10],Vbat_str[10],Iin_str[10],Ibat_str[10],Dtemp_str[10],Soc_Str[10];
char Imot1_Str[10],Imot2_Str[10],Imot3_Str[10];

extern BYTE  data_read_buffer[Data_Read_Cnt];
extern char SDerrorlog[45];
extern char Daycount;
extern int rowdaycount;

extern uint8_t lastReadRfidData[12];

extern int trackchangercount;
extern int tc_count;
extern int rowsActuallyCleaned;

char schedule_log[100];
char cyclelog[100];
char errorcountlog[10];
extern char SDerrorlog[45];
extern char Robot_Error_Count;
extern char Robot_Error_Count1;

uint8_t tempCount_NEW;
extern BYTE  data_read_buffer2[Data_Read_Cnt];
extern uint32_t Cumulative_distance;


extern uint8_t rxDataBuffer[MAX_UART_RX_BUFFER_LENGTH];
unsigned long int bmsvalue;
char* startbms, endbms;
char temp_arr[100][10], temp_arr1[100][10],slave_arr[10],fun_arr[10],byte_arr[10],uart_arr[100][10];
char bat_arr[10],pv_arr[10],mpptcurr_arr[10],mpptout_arr[10],inttemp_arr[10],exttemp_arr[10],batstate_arr[10], loadout_arr[10],loadtrip_arr[10];
unsigned int Slave_address, function_code,BAT_STATE,BAT_STATE_CNT,LOAD_STATE,ENERGY_HI, REG_5V_EN,REG_5V_TRIP,MODBUS_ADR,REG_PWM,MPPT_CNT,MPPT_STATE,REG_DOMINATE,PV_EN,LOAD_EN,LOAD_COUNT,LOAD_TRIPCNT,size,size1,slave_address_cmp, function_cmp,p1,p2,p3,size_pkt,byte1,byte2,byte3,byte_value;
unsigned char Byte_cnt;
float Vbat,VPV,IMPPT,PWR,VN_bat1,VL5V,VLOAD,VN_bat2,HS_TEMP,EXT_TEMP,VR_bat, BAT_SET,BAT_COMP,ENERGY_LO,VMPPT_APP_SET,REG_SET_PV,REG_SET_VBAT,REG_SET_IMPPT,BEST_PV,BEST_PWR,IMPPT_APP_SET;
int byte_int;

extern float pv_vol,pv_curr,pv_power,battery_volt,batt_charg_curr,batt_discharg_curr;
extern int pv_work_status,Batt_SOC,charge_cont_temp,batt_temp,generated_energy;
extern char controller_status;
extern char pv_vol_arr[10],pv_curr_arr[10],pv_power_arr[10],battery_volt_arr[10],batt_charg_curr_arr[10],batt_discharg_curr_arr[10];

uint64_t receivedMACAddr = 0;

stJsonCommand jsonCommandParams[] = {
  {CMD_NUM_SET_MODE,                            CMD_SET_MODE,                   ProcessSetMode,                 EepromSetMode,                          SetDefaultOperationMode},
  {CMD_NUM_SET_MOTOR_DIR_MANUAL,                CMD_SET_MOTOR_DIR_MANUAL,       ProcessSetManualDirection,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_MOTOR_PWM_MANUAL,                CMD_SET_MOTOR_PWM_MANUAL,       ProcessSetMotorPWMManual,       EepromSetMotorPwmManualMode,            SetDefaultPwmManulModeParameter},
  {CMD_NUM_SET_BRUSH_MOTOR_STATE,               CMD_SET_BRUSH_MOTOR_STATE,      ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_BRUSH_MOTOR_PWM_MANUAL,          CMD_SET_BRUSH_MOTOR_PWM_MANUAL, ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_BRUSH_MOTOR_DIR,                 CMD_SET_BRUSH_MOTOR_DIR,        ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_MOTOR_PWM_AUTO,                  CMD_SET_MOTOR_PWM_AUTO,         ProcessSetMotorPWMAuto,         EepromSetMotorPwmAutoMode,              SetDefaultPwmCycleAutoModeParameter},
  {CMD_NUM_SET_BRUSH_MOTOR_PWM_AUTO,            CMD_SET_BRUSH_MOTOR_PWM_AUTO,   ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_AUTO_SCHEDULE_TIME,              CMD_SET_AUTO_SCHEDULE_TIME,     ProcessSetAutoScheduleTime,     EepromSetAutoScheduleTime,              SetDefaultAutoScheduledTime},
  {CMD_NUM_SET_EDGE_SENSE_STATE,                CMD_SET_EDGE_SENSE_STATE,       ProcessSetEdgeSensorState,      EepromSetEdgeSensorState,               SetDefaultEdgeSensorEnabledState},
  {CMD_NUM_SET_RTC_VALUE,                       CMD_SET_RTC_VALUE,              ProcessSetRTCValue,             EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_DEVICE_INFO,                     CMD_SET_DEVICE_INFO,            ProcessSetDeviceInfo,           EepromDevIDInfo,                        SetDefaultDevIDInfo},
  {CMD_NUM_SET_OVERCURRENT_LIMIT,               CMD_SET_OVERCURRENT_LIMIT,      ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultMaxMotCurrentLimits},
  {CMD_NUM_CLEAR_ERRORCODES,                    CMD_CLEAR_ERRORCODES,           ProcessClearErrorCodes,         EepromDataNil,                          SetDefaultNil},
  //{CMD_NUM_GET_SOC,                             CMD_GET_SOC,                    ProcessGetSoC,                  EepromDataNil, //07D                          SetDefaultNil},
  {CMD_NUM_GET_CHARGER_STATE,                   CMD_GET_CHARGER_STATE,          ProcessGetChargerState,         EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_CONTROLLER_STATUS,               CMD_GET_CONTROLLER_STATUS,      ProcessGetErrorStatus,          EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_DEVICE_INFO,                     CMD_GET_DEVICE_INFO,            ProcessGetDeviceInfo,           EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_CURRENT_MODE,                    CMD_GET_CURRENT_MODE,           ProcessGetCurrentMode,          EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_MOTOR_DIR_MANUAL,                CMD_GET_MOTOR_DIR_MANUAL,       ProcessGetMotorDirectionManual, EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_MOTOR_PWM_MANUAL,                CMD_GET_MOTOR_PWM_MANUAL,       ProcessGetMotorPWMManual,       EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_BRUSH_MOTOR_STATE,               CMD_GET_BRUSH_MOTOR_STATE,      ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_BRUSH_MOTOR_PWM_MANUAL,          CMD_GET_BRUSH_MOTOR_PWM_MANUAL, ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_BRUSH_MOTOR_DIR,                 CMD_GET_BRUSH_MOTOR_DIR,        ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_MOTOR_PWM_AUTO,                  CMD_GET_MOTOR_PWM_AUTO,         ProcessGetMotorPWMAuto,         EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_BRUSH_MOTOR_PWM_AUTO,            CMD_GET_BRUSH_MOTOR_PWM_AUTO,   ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_AUTO_SCHEDULE_TIME,              CMD_GET_AUTO_SCHEDULE_TIME,     ProcessGetAutoScheduleTime,     EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_EDGE_SENSE_STATE,                CMD_GET_EDGE_SENSE_STATE,       ProcessGetEdgeSenseEnabledState,EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_RTC_VALUE,                       CMD_GET_RTC_VALUE,              ProcessGetRTCValue,             EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_CURRENT_STAT,                    CMD_GET_CURRENT_STAT,           ProcessGetCurrentStat,          EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_LAST_CYCLE_STAT,                 CMD_GET_LAST_CYCLE_STAT,        ProcessGetLastOperationStat,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_OVERCURRENT_LIMIT,               CMD_GET_OVERCURRENT_LIMIT,      ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_MOT_CURRENT_VALUES,              CMD_GET_MOT_CURRENT_VALUES,     ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_PULSE_COUNT_VALUES,              CMD_SET_PULSE_COUNT_VALUES,     ProcessSetPulseCount,           EepromSetPulseCount,                    SetDefaultCountRange},
  {CMD_NUM_SET_LOW_BAT_SOC,                     CMD_SET_LOW_BAT_SOC,            ProcessSetLowBatSoC,            EepromSetLowBatSoC,                     SetDefaultMinBatSoc},
  {CMD_NUM_GET_PULSE_COUNT_VALUES,              CMD_GET_PULSE_COUNT_VALUES,     ProcessGetPulseCountValues,     EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_LOW_BAT_SOC,                     CMD_GET_LOW_BAT_SOC,            ProcessGetLowBatSoC,            EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_ZIGBEE_CONF,                     CMD_SET_ZIGBEE_CONF,            ProcessSetZigbeeConfig,         EepromZigbeeConfiguration,              SetDefaultZigbeeConfig},
  {CMD_NUM_GET_ZIGBEE_CONF,                     CMD_GET_ZIGBEE_CONF,            ProcessGetZigbeeConfig,         EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_ZIGBEE_NTW_PARAM,                CMD_GET_ZIGBEE_NTW_PARAM,       ProcessGetZigbeeNetworkParams,  EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_RESET_ZIGBEE_NTW,                    CMD_RESET_ZIGBEE_NTW,           ProcessResetZigbeeNetwork,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_BAT_AND_PV_VAL,                  CMD_GET_BAT_AND_PV_VAL,         ProcessGetBatteryAndPVvalues,   EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_RESET_TO_DEFAULTS,                   CMD_RESET_TO_DEFAULTS,          ProcessResetToDefaults,         EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_TEMPERATURES,                    CMD_GET_TEMPERATURES,           ProcessGetTemperaturevalues,    EepromDataNil,                          SetDefaultNil},  
  {CMD_NUM_SET_MOTOR_FAULT_CONDITION,           CMD_SET_MOTOR_FAULT_CONDITIONS, ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultIloadConfigValues},  
  {CMD_NUM_GET_MOTOR_FAULT_CONDITION,           CMD_GET_MOTOR_FAULT_CONDITIONS, ProcessSendInvalidCommand,      EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_REQ_ACTIVITY_LOG,                    CMD_REQ_ACTIVITY_LOG,           RequestActivityLog,             EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_CLEAR_ACTIVITY_LOG,                  CMD_CLEAR_ACTIVITY_LOG,         ClearActivityLog,               EepromDataNil,                          SetDefaultNil},
  //{CMD_NUM_SET_TRACK_COUNT,                     CMD_SET_TRACK_COUNT,            ProcessSetTrackCount,           EepromDataNil,                        SetDefaultNil},
  {CMD_NUM_SET_TRACK_INFO,                      CMD_SET_TRACK_INFO,             ProcessSetTrackInfo,            EepromStoreTrackInfo,                   SetDefaultTrackInfo},
  {CMD_NUM_GET_TRACK_INFO,                      CMD_GET_TRACK_INFO,             ProcessGetTrackInfo,            EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_CLEAR_TRACK_INFO,                    CMD_CLEAR_TRACK_INFO,           ClearTrackInfo,                 EepromStoreTrackInfo,                   SetDefaultNil},
  {CMD_NUM_SET_CLEANER_MAC,                     CMD_SET_CLEANER_MAC,            ProcessSetCleanerMAC,           EepromStoreCleanerMAC,                  SetDefaultNil},
  {CMD_NUM_SET_ROW_LENGTH_DATA,                 CMD_SET_ROW_LENGTH_DATA,        ProcessSetRowLengthData,        EepromStoreRowLength,                   SetDefaultRowLength},
  {CMD_NUM_GET_CLEANER_MAC,                     CMD_GET_CLEANER_MAC,            ProcessGetCleanerMAC,           EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_GET_ROW_LENGTH_DATA,                 CMD_GET_ROW_LENGTH_DATA,        ProcessGetRowLengthData,        EepromDataNil,                          SetDefaultNil},
  //{CMD_NUM_GET_TRACK_CHANGER_STATUS,            CMD_GET_TRACK_CHANGER_STATUS,   ProcessGetTrackChangerStatus,   EepromDataNil,     //07D                     SetDefaultNil},
  {CMD_NUM_GET_SENSOR_STATUS,                   CMD_GET_SENSOR_STATUS,          ProcessGetSensorStatus,         EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_ENC_PULSE_TOLERANCE,             CMD_SET_ENC_PULSE_TOLERANCE,    ProcessSetEncoderPulseTolerance,EepromStoreEncoderPulseTolerance,       SetDefaultEncoderPulseTolerance},
  {CMD_NUM_GET_ENC_PULSE_TOLERANCE,             CMD_GET_ENC_PULSE_TOLERANCE,    ProcessGetEncoderPulseTolerance,EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_TC_CONTROL_MODE,                 CMD_SET_TC_CONTROL_MODE,        ProcessSetTcControlMode,        EepromSetTcControlMode,                 SetDefaultTcControlMode},
  {CMD_NUM_SET_TC_CONTROL_MODE,                 CMD_GET_TC_CONTROL_MODE,        ProcessGetTcControlMode,        EepromDataNil,                          SetDefaultNil},
  {CMD_NUM_SET_RFID_DATA,                       CMD_SET_RFID_DATA,              ProcessSetRfidData,             EepromStoreRfidData,                    SetDefaultRfidData},  
  {CMD_NUM_GET_RFID_DATA,                       CMD_GET_RFID_DATA,              ProcessGetRfidData,             EepromDataNil,                          SetDefaultNil},   
  {CMD_NUM_SET_RFID_STATE,                      CMD_SET_RFID_STATE,             ProcessSetRfidState,            EepromStoreRfidState,                   SetDefaultRfidState},  
  {CMD_NUM_GET_RFID_STATE,                      CMD_GET_RFID_STATE,             ProcessGetRfidState,            EepromDataNil,                          SetDefaultNil},   
  {CMD_NUM_SET_HEARTBEAT_CONFIG,                CMD_SET_HEARTBEAT_CONFIG,       ProcessSetHeartbeatConfig,      EepromHeartbeatConfiguration,           SetDefaultHeartbeatConfig},  
  {CMD_NUM_GET_HEARTBEAT_CONFIG,                CMD_GET_HEARTBEAT_CONFIG,       ProcessGetHeartbeatConfig,      EepromDataNil,              SetDefaultNil},   
  
  {CMD_NUM_SET_LOG_INTERVAL,                    CMD_SET_LOG_INTERVAL,           ProcessSetLogInterval,          EepromSetLogInterval,       SetDefaultLogInterval},     
  {CMD_NUM_GET_LOG_INTERVAL,                    CMD_GET_LOG_INTERVAL,           ProcessGetLogInterval,          EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_DIA_WHEEL,                       CMD_SET_DIA_WHEEL,              ProcessSetWheelDia,             EepromSetWheelDia,          SetDefaultWheelDia},         
  {CMD_NUM_GET_DIA_WHEEL,                       CMD_GET_DIA_WHEEL,              ProcessGetWheelDia,             EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_CYCLE_FREQUENCY,                 CMD_SET_CYCLE_FREQUENCY,        ProcessSetCycleFrequency,       EepromSetCycleFrequency,    SetDefaultCycleFrequency},   
  {CMD_NUM_GET_CYCLE_FREQUENCY,                 CMD_GET_CYCLE_FREQUENCY,        ProcessGetCycleFrequency,       EepromDataNil,              SetDefaultNil},  
  {CMD_NUM_SET_CONTINUE,                        CMD_SET_CONTINUE,               ProcessSetContinue,             EepromSetContinue,          SetDefaultContinue},  
  {CMD_NUM_GET_CONTINUE_COUNT,                  CMD_GET_CONTINUE_COUNT,         ProcessGetContinueCount,        EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_EMERGENCY_RETURN,                CMD_SET_RETURN_STATE,           ProcessSetReturnState,          EepromSetReturnState,       SetDefaultReturnState},  
  {CMD_NUM_GET_RETURN_STATE,                    CMD_GET_RETURN_STATE,           ProcessGetReturnState,          EepromDataNil,              SetDefaultNil},
  {CMD_NUM_REQ_LOG,                             CMD_REQ_LOG,                    RequestLog,                     EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_COMMDISTANCE,                    CMD_SET_RUNRFID,             ProcessSetCommDistance,         EepromSetCommDistance,      SetDefaultCommDistance},   
  {CMD_NUM_GET_COMMDISTANCE,                    CMD_GET_RUNRFID,             ProcessGetCommDistance,         EepromDataNil,              SetDefaultNil},
  {CMD_NUM_SET_N0_ROW_DAY,                      CMD_SET_N0_ROW_DAY,             ProcessSetNoofRowday,           EepromSetNoofRowday,        SetDefaultNoofRowday},   
  {CMD_NUM_GET_N0_ROW_DAY,                      CMD_GET_N0_ROW_DAY,             ProcessGetNoofRowday,           EepromDataNil,              SetDefaultNil},

  {CMD_NUM_SET_TRACK_INFO2,                      CMD_SET_TRACK_INFO2,             ProcessSetTrackInfo2,            EepromStoreTrackInfo2,                   SetDefaultTrackInfo2},
  {CMD_NUM_GET_TRACK_INFO2,                      CMD_GET_TRACK_INFO2,             ProcessGetTrackInfo2,            EepromDataNil,                          SetDefaultNil},
  
   {CMD_NUM_SET_RFID_DATA2,                       CMD_SET_RFID_DATA2,              ProcessSetRfidData2,             EepromStoreRfidData2,                    SetDefaultRfidData2},  
  {CMD_NUM_GET_RFID_DATA2,                       CMD_GET_RFID_DATA2,              ProcessGetRfidData2,             EepromDataNil,                          SetDefaultNil},    
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
  {CMD_NUM_SET_RFID_DATA3,                       CMD_SET_RFID_DATA3,              ProcessSetRfidData3,             EepromStoreRfidData3,                    SetDefaultRfidData3},  
  {CMD_NUM_GET_RFID_DATA3,                       CMD_GET_RFID_DATA3,              ProcessGetRfidData3,             EepromDataNil,                          SetDefaultNil},    
};

void SetReceivedMACAddress(uint64_t macAddr)
{
receivedMACAddr = macAddr;
}

uint64_t GetReceivedMACAddress(void)
{
return receivedMACAddr;
}


static eJsonStatus ProcessSetState (const char * jsonString, bool* tempSetBool);
static eJsonStatus ProcessSetMotorPwm (const char * jsonString, stRobotPwmParam *tempSetPwmParam);
//static eJsonStatus ProcessSetBrushPwm (const char * jsonString, stBrushPwmParam *tempSetPwmParam);
void ClearTxBuff(void);

//extern stTrackOperationData tempSetTrackOpData;

uint16_t C_ROW;

static eJsonStatus ProcessSendInvalidCommand (const char * jsonString)
{
Serial_Debug("\r\nIn ProcessSetMode->");
  Serial_Debug(jsonString);
  return JSON_INVALID_CMD_VALUE;
}
static eJsonStatus ProcessSetMode (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetMode->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  uint8_t mode;
  char *setStateStr[JSON_NO_OF_MODE_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_MODE_STATE_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setStateStr[0], &mode, IsModeValid);
    
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
//      case 2 : SetMotionCommand(ROBOT_CMD_TRACK_CHANGER);
//      break;
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

static eJsonStatus ProcessSetManualDirection (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetManualDirection->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  uint8_t mode;
  char *setStateStr[JSON_NO_OF_DIR_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  
  uint16_t size_ch = strlen(setStateStr);
  stnoofrows* getNoofrows = GetSetNoofrowsLimits();
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
    status &= SetParameterValue_int(setStateStr[0], &mode, IsManualCommandValid);   
    int commandrun =  atoi(setStateStr[1]);
      
    if(mode == 6)
    {
      if(commandrun <= getNoofrows->TRow)
      {
         status &= SetParameterValue_int(setStateStr[1], &C_ROW, Row_Num_Valid);
      }
      else
      {
        status = false;
      }
    }
    
    stContinue *setContinueValue = GetSetContinueLimits();
    strunrfid* getrunrfid = GetSetrunrfidLimits();
    strunrfid tempSetstrunrfid = *getrunrfid;
    
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
  //stRobotPwmParam tempSetPWMCycleParams = *setPWMCycleParams;
  error_code = ProcessSetMotorPwm(jsonString,&tempSetPWMParams);
  //error_code = ProcessSetMotorPwm(jsonString,&tempSetPWMCycleParams);
  if(error_code == JSON_NO_ERROR)
  {
    *setPWMParams = tempSetPWMParams;
    *setPWMCycleParams = tempSetPWMParams;
    char dispstring [100];
    snprintf(dispstring, sizeof dispstring, "\nPwm Manual Mode SetValues: %d, %d, %ld, %ld, %ld, %ld, %d",
             setPWMParams->accelTime, setPWMParams->decelTime, setPWMParams->steadyPwm1, setPWMParams->steadyPwm2,
             setPWMParams->approachPwm1, setPWMParams->approachPwm2,setPWMParams->decelStartCountDiff);
    Serial_Debug(dispstring);
  }
  return error_code;
}


//static eJsonStatus ProcessSetBrushMotorState (const char * jsonString)
//{
//  Serial_Debug("\r\nIn ProcessSetBrushMotorState->");
//  Serial_Debug(jsonString); 
//  eJsonStatus error_code = JSON_NO_ERROR;
//  bool mode;
//  error_code = ProcessSetState(jsonString, &mode);
//  if(error_code == JSON_NO_ERROR)
//  {
//    Serial_Debug("\r\nBrush motor state set as ");
//    Serial_Debug_Num(mode);
//    bool *brushEnState = GetSetBrushEnabledState();
//    *brushEnState = mode;
//  }
//  return error_code;
//}
//
//
//static eJsonStatus ProcessSetBrushPWMManual (const char * jsonString)
//{
//  Serial_Debug("\r\nIn ProcessSetMotorPWMManual->");
//  Serial_Debug(jsonString); 
//  eJsonStatus error_code = JSON_NO_ERROR;
//  stBrushPwmParam* setBrushParams;
//  setBrushParams = GetSetBrushNormalPwmParameters();
//  stBrushPwmParam tempSetBrushParams = *setBrushParams;
//  error_code = ProcessSetBrushPwm(jsonString,&tempSetBrushParams);
//  if(error_code == JSON_NO_ERROR)
//  {
//    *setBrushParams = tempSetBrushParams;
//    char dispstring [100];
//    snprintf(dispstring, sizeof dispstring, "\nBrush Pwm Manual Mode SetValues: %d, %d, %d",
//             setBrushParams->accelTime, setBrushParams->decelTime, setBrushParams->steadyPwm);
//    Serial_Debug(dispstring);
//  }
//  return error_code;
//  
//}
//
//static eJsonStatus ProcessSetBrushDirection (const char * jsonString)
//{
//  Serial_Debug("\r\nIn ProcessSetBrushDirection->");
//  Serial_Debug(jsonString); 
//  eJsonStatus error_code = JSON_NO_ERROR;
//  bool mode;
//  error_code = ProcessSetState(jsonString, &mode);
//  if(error_code == JSON_NO_ERROR)
//  {
//    Serial_Debug("\r\nBrush motor direction set as ");
//    Serial_Debug_Num(mode);
//    stBrushMotPolarity *motPolarity = GetSetBrushMotPolarity();
//    motPolarity->mot1Polarity = mode;
//  }
//  return error_code;
//}

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


//static eJsonStatus ProcessSetBrushPWMAuto (const char * jsonString)
//{
//  Serial_Debug("\r\nIn ProcessSetBrushPWMAuto->");
//  Serial_Debug(jsonString); 
//  eJsonStatus error_code = JSON_NO_ERROR;
//  stBrushPwmParam* setBrushParams;
//  setBrushParams = GetSetBrushAutoPwmParameters();
//  stBrushPwmParam tempSetBrushParams = *setBrushParams;
//  error_code = ProcessSetBrushPwm(jsonString,&tempSetBrushParams);
//  if(error_code == JSON_NO_ERROR)
//  {
//    *setBrushParams = tempSetBrushParams;
//    char dispstring [100];
//    snprintf(dispstring, sizeof dispstring, "\nBrush Pwm Auto Mode SetValues: %d, %d, %d",
//             setBrushParams->accelTime, setBrushParams->decelTime, setBrushParams->steadyPwm);
//    Serial_Debug(dispstring);
//  }
//  return error_code;
//}

static eJsonStatus ProcessSetAutoScheduleTime (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetAutoScheduleTime->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  char* autoScheduleStr[JSON_NO_OF_MODE_STATE_DATA];
  uint8_t maxNoStrings=(sizeof autoScheduleStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, autoScheduleStr, ',',maxNoStrings);
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

static eJsonStatus ProcessSetRTCValue (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetRTCValue->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  char* setRTCValueStr[JSON_NO_OF_RTC_DATA];
  uint8_t maxNoStrings=(sizeof setRTCValueStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, setRTCValueStr, ',',maxNoStrings);
  Serial_Debug("\nAuto schedule Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setRTCValueStr[dispcount]);
    Serial_Debug(",");
  }
  
  sDateTime apisec;
  
  apisec.year = atoi(setRTCValueStr[2]);
  apisec.month = atoi(setRTCValueStr[1]);
  apisec.date = atoi(setRTCValueStr[0]);
  apisec.hour = atoi(setRTCValueStr[3]);
  apisec.minute = atoi(setRTCValueStr[4]);
  apisec.second = atoi(setRTCValueStr[5]);
  
  uint32_t IST_to_Sec = ConvertRTCDatetimeToSeconds(&apisec);

  if(totalResultStringCount == JSON_NO_OF_RTC_DATA)
  {
    //uint32_t time = strtol(setRTCValueStr[0],NULL,10);
    uint32_t time = IST_to_Sec;
    
    if(time >= JAN_2000_START_SECONDS && time <= DEC_2099_END_SECONDS)
    {
      char getTime_str[40];
      sDateTime setTime;
      ConvertRTCSecondsToDatetime(time,&setTime);
      ConvertRTCDateTimeToString(&setTime, getTime_str, sizeof getTime_str);
      Serial_Debug(getTime_str);
      if(SetRTCDateTime(&setTime))
      {
        ClearAutoDoneSeconds();
        ClearFault(RTC_FAULT);
        error_code = JSON_NO_ERROR;
        Serial_Debug("\n RTC SetValue/s Set");
      } 
      else
      {
        
        Serial_Debug("\n RTC  SetValue/s Invalid");
        error_code = JSON_INVALID_DATA;  
      }
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

static eJsonStatus ProcessSetDeviceInfo (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetDeviceInfo->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  char* devIDInfoStr[JSON_NO_OF_SET_DEVICE_INFO];
  uint8_t maxNoStrings=(sizeof devIDInfoStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, devIDInfoStr, ',',maxNoStrings);
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
      snprintf(devIDInfoValues->devID, MAX_DEV_INFO_FIELD_LEN, devIDInfoStr[0]);
    }
    if(strlen(devIDInfoStr[1]) != 0)
    {
      snprintf(devIDInfoValues->hwVersion, MAX_DEV_INFO_FIELD_LEN, devIDInfoStr[1]);
    }
    if(strlen(devIDInfoStr[2]) != 0)
    {
      snprintf(devIDInfoValues->serialNo, MAX_DEV_INFO_FIELD_LEN, devIDInfoStr[2]);
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

//static eJsonStatus ProcessSetMotorCurrentLimits (const char * jsonString)
//{
//  Serial_Debug("\r\nIn ProcessSetMotorCurrentLimits->");
//  Serial_Debug(jsonString); 
//  eJsonStatus error_code = JSON_NO_ERROR;
//  stMaxMotorCurrent* setMotorcurrent = GetSetMaxMotorCurrentLimits();
//  stMaxMotorCurrent tempSetMotorCurrent = *setMotorcurrent;
//  char *MotorCurrentLimitStr[JSON_NO_OF_CURRENT_LIMIT];
//  uint8_t maxNoStrings=(sizeof MotorCurrentLimitStr)/4;
//  uint8_t totalResultStringCount =  SplitString (jsonString, MotorCurrentLimitStr, ',',maxNoStrings);
//  Serial_Debug("\nPWM Set Received Values :");
//  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
//  {
//    Serial_Debug(MotorCurrentLimitStr[dispcount]);
//    Serial_Debug(",");
//  }  
//  if(totalResultStringCount == JSON_NO_OF_CURRENT_LIMIT)
//  {
//    bool status = true;
//    status &= SetParameterValue_float(MotorCurrentLimitStr[0], &(tempSetMotorCurrent.Imot1), IsMotorCurrentLimitValid);
//    status &= SetParameterValue_float(MotorCurrentLimitStr[1], &(tempSetMotorCurrent.Imot2) , IsMotorCurrentLimitValid);
//    status &= SetParameterValue_float(MotorCurrentLimitStr[2], &(tempSetMotorCurrent.Imot3), IsMotorCurrentLimitValid);
//    
//    if(status == false)
//    {
//      error_code = JSON_DATA_OUT_OF_RANGE;
//      Serial_Debug("\n IMOT current limit SetValue/s Out of Range");
//    }
//    else
//    {
//      error_code = JSON_NO_ERROR;
//      Serial_Debug("\n IMOT current limit SetValue/s set : ");
//      *setMotorcurrent = tempSetMotorCurrent;
//      
//      Serial_Debug_Float(setMotorcurrent->Imot1,3);
//      Serial_Debug(" ,");
//      Serial_Debug_Float(setMotorcurrent->Imot2,3);
//      Serial_Debug(" ,");
//      Serial_Debug_Float(setMotorcurrent->Imot3,3);
//    }            
//  }
//  else
//  {
//    Serial_Debug("\n IMOT current limit SetValue/s Invalid");
//    error_code = JSON_INVALID_DATA;
//  }
//  return error_code;
//}


static eJsonStatus ProcessClearErrorCodes (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessClearErrorCodes->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR;
  char* clearErrorCodeStr[JSON_NO_OF_ERROR_CODE_DATA];
  uint8_t maxNoStrings=(sizeof clearErrorCodeStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, clearErrorCodeStr, ',',maxNoStrings);
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
  uint8_t totalResultStringCount =  SplitString (jsonString, setLowBatSoCValueStr, ',',maxNoStrings);
  Serial_Debug("\nLow Battery Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setLowBatSoCValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_SOC_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setLowBatSoCValueStr[0], &(tempSetLowBatSoC), IsLowBatSoCValid);
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
  uint8_t totalResultStringCount =  SplitString (jsonString, PulseCountStr, ',',maxNoStrings);
  Serial_Debug("\nPulse Count Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(PulseCountStr[dispcount]);
    Serial_Debug(",");
  }  
  if(totalResultStringCount == JSON_NO_OF_PULSE_COUNT_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(PulseCountStr[0], &(tempSetCountValues.maxPosCountForPVsetup), IsPulseCountValid);
    status &= SetParameterValue_int(PulseCountStr[1], &(tempSetCountValues.maxNegCountForPVsetup) , IsPulseCountValid);
    
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

//static eJsonStatus ProcessGetSoC (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_SOC);
//  dataCount = dataCount+bufferLength;    
//  bufferLength = sizeof(txBuff)-dataCount;
//  char Soc_Str[10];
//  ftoa(GetSOCAsPercentage(),Soc_Str,2);
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%d",Soc_Str,GetChargingOrNot());
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes(txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}


static eJsonStatus ProcessGetChargerState (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CHARGER_STATE);
  dataCount = dataCount+bufferLength;   
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",GetChargingOrNot());
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%000X", GetMotorFaultCode());
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%s,%s,%s",
                               devInfo->devID, devInfo->hwVersion, devInfo->serialNo, DEFAULT_DEV_SW_REV);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessGetCurrentMode (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_CURRENT_MODE);
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
  tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",modeValue);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
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
  tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",manualValue);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%ld,%ld,%d",getPWMVal->accelTime,getPWMVal->decelTime,getPWMVal->steadyPwm1,getPWMVal->approachPwm1,getPWMVal->decelStartCountDiff);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

//static eJsonStatus ProcessGetBrushMotorEnabledState (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_BRUSH_MOTOR_STATE);
//  dataCount = dataCount+bufferLength;    
//  bufferLength = sizeof(txBuff)-dataCount;
//  bool *brushEnabledState = GetSetBrushEnabledState();
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d", *brushEnabledState);
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes(txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}
//
//static eJsonStatus ProcessGetBrushMotorPWMManual (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_BRUSH_MOTOR_PWM_MANUAL);
//  dataCount = dataCount+bufferLength;    
//  bufferLength = sizeof(txBuff)-dataCount;
//  stBrushPwmParam* getBrushPWMVal = GetSetBrushNormalPwmParameters();
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d",getBrushPWMVal->accelTime,
//                               getBrushPWMVal->decelTime,getBrushPWMVal->steadyPwm);
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes(txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}
//
//static eJsonStatus ProcessGetBrushMotorPolarity (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_BRUSH_MOTOR_DIR);
//  dataCount = dataCount+bufferLength;    
//  bufferLength = sizeof(txBuff)-dataCount;
//  stBrushMotPolarity *setMotPolarity = GetSetBrushMotPolarity();
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",setMotPolarity->mot1Polarity);
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes(txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}


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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d",
                               getPWMVal->accelTime,getPWMVal->decelTime,getPWMVal->steadyPwm1,
                               getPWMVal->approachPwm1,getPWMVal->decelStartCountDiff);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

//static eJsonStatus ProcessGetBrushMotorPWMAuto (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_BRUSH_MOTOR_PWM_AUTO);
//  dataCount = dataCount+bufferLength;    
//  bufferLength = sizeof(txBuff)-dataCount;
//  stBrushPwmParam* getBrushPWMVal = GetSetBrushAutoPwmParameters();
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d",getBrushPWMVal->accelTime,
//                               getBrushPWMVal->decelTime,getBrushPWMVal->steadyPwm);
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes(txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}
//
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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%02d%02d,%d",getScheduleTime->hour,getScheduleTime->minute,Daycount);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",IsEdgeSensorEnabled());
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
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
  uint32_t getTime = GetRTCDateTimeSeconds();
  
  //uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",getTime);
  
  GetRTCDateTimeString(&getTime_str, sizeof getTime_str);
  strcat(txBuff,getTime_str);
  char tempCount = strlen(getTime_str);
  
  Serial_Debug("\r\nTime-->");
  Serial_Debug(getTime_str);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
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
  
  //int DistanceCount;
  // Distancevalue;
  
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
  
 // DistanceCount = (GetRotateSense1Count()/setWheelDia->Pulse);
  Distancevalue = (distance/setWheelDia->Pulse);
  
  
  //DistanceCount = DistanceCount * distance;

 // uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%d", modeValue, robotState, GetRotateSense1Count(), GetRotateSense2Count(),GetProximityCount(),getRfidData->rfidData[GetProximityCount()-1]);
  uint8_t tempCount;
  
  stnoofrows* setNoofrows = GetSetNoofrowsLimits();
  stnoofrows tempsetNoofrows = *setNoofrows;
  

  if(lastReadRfidData[0] == NULL)
  {
     strcpy(lastReadRfidData,"0");
  }
  if(trackchangercount<=10) 
  {
    tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d", modeValue, robotState, GetRotateSense1Count(),GetRotateSense1Count()*Distancevalue,Cumulative_distance,man_day,trackchangercount,getRfidData->rfidData[trackchangercount-1],rowsActuallyCleaned,lastReadRfidData,tempsetNoofrows.TRow  - trackchangercount);
    dataCount = dataCount+tempCount; 
  }
  else if((trackchangercount>=11)&&(trackchangercount<=20)) 
  {
    tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d", modeValue, robotState, GetRotateSense1Count(),GetRotateSense1Count()*Distancevalue,Cumulative_distance,man_day,trackchangercount,getRfidData->rfidData2[trackchangercount-11],rowsActuallyCleaned,lastReadRfidData,tempsetNoofrows.TRow  - trackchangercount);
    dataCount = dataCount+tempCount; 
  }
  else if((trackchangercount>=21)&&(trackchangercount<=25)) 
  {
    tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d", modeValue, robotState, GetRotateSense1Count(),GetRotateSense1Count()*Distancevalue,Cumulative_distance,man_day,trackchangercount,getRfidData->rfidData3[trackchangercount-21],rowsActuallyCleaned,lastReadRfidData,tempsetNoofrows.TRow  - trackchangercount);
    dataCount = dataCount+tempCount; 
  }
  
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
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
  char daycount;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_LAST_CYCLE_STAT);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  stLastOperationStatus *lastOpStatus = GetLastOperationData();
  char startSoc_Str[10],endSoc_Str[10];
  ftoa(lastOpStatus->startSOC,startSoc_Str,2);
  ftoa(lastOpStatus->endSOC,endSoc_Str,2);
//  int Currentdistance; 
//  PulseCalculation();
//  Currentdistance = GetRotateSense1Count() * distance;
  eRobotDirection robotState = GetRobotActualDirection();
  eMode getMode =  GetOperationMode ();  
  int DAY = Daycount - 1;
    uint8_t modeValue = 0;
    switch(getMode)
    {
    case AUTO: 
      modeValue = 0;
      daycount = Daycount;
      //mode_now =  Get_Completed_Schedule();
      break;
    case MANUAL:
      modeValue = 1;
      daycount = 1;
      //mode_now = 0;
      break;
    default : 
      modeValue = 0;
      //mode_now =  Get_Completed_Schedule();
      break;
    }
  //uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%s",modeValue,mode_now,GetRotateSense1Count(),Currentdistance,schedule_log);
    uint8_t tempCount; 
    stnoofrows* setNoofrows = GetSetNoofrowsLimits();
    stnoofrows tempsetNoofrows = *setNoofrows;
    stWheelDia* setWheelDia = GetSetwheelDiaLimits();
    Distancevalue = (distance/setWheelDia->Pulse);
    
//  if(lastReadRfidData[0] == NULL)
//  {
//     strcpy(lastReadRfidData,"0"); "%000X", GetMotorFaultCode()
//  }
  if(trackchangercount<=10) 
  {
//    tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%000X,%d,%d,%d,%d,%s",modeValue,robotState,GetRotateSense1Count()*Distancevalue,Cumulative_distance,GetMotorFaultCode(),daycount,trackchangercount,rowdaycount,tempsetNoofrows.Row[daycount]-rowdaycount,cyclelog);
    tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%s,%s,%s,%s,%000X,%d",robotState,daycount,trackchangercount,rowdaycount,tempsetNoofrows.Row[DAY]-rowdaycount,Cstarttime,Cstartsoc,Cendtime,Cendsoc,GetMotorFaultCode(),Cumulative_distance);  
    dataCount = dataCount+tempCount; 
  }
  else if((trackchangercount>=11)&&(trackchangercount<=20)) 
  {
    tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%s,%s,%s,%s,%000X,%d",robotState,daycount,trackchangercount,rowdaycount,tempsetNoofrows.Row[DAY]-rowdaycount,Cstarttime,Cstartsoc,Cendtime,Cendsoc,GetMotorFaultCode(),Cumulative_distance);
    dataCount = dataCount+tempCount; 
  }
  else if((trackchangercount>=21)&&(trackchangercount<=25)) 
  {
//    tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%000X,%d,%d,%d,%d,%s",modeValue,robotState,GetRotateSense1Count()*Distancevalue,Cumulative_distance,GetMotorFaultCode(),daycount,trackchangercount,rowdaycount,tempsetNoofrows.Row[daycount]-rowdaycount,cyclelog);
    tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%s,%s,%s,%s,%000X,%d",robotState,daycount,trackchangercount,rowdaycount,tempsetNoofrows.Row[DAY]-rowdaycount,Cstarttime,Cstartsoc,Cendtime,Cendsoc,GetMotorFaultCode(),Cumulative_distance); 
    dataCount = dataCount+tempCount; 
  }
  
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;

}  
  
//static eJsonStatus ProcessGetLastOperationStat (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_LAST_CYCLE_STAT);
//  dataCount = dataCount+bufferLength;    
//  bufferLength = sizeof(txBuff)-dataCount;
//  stLastOperationStatus *lastOpStatus = GetLastOperationData();
//  char startSoc_Str[10],endSoc_Str[10];
//  ftoa(lastOpStatus->startSOC,startSoc_Str,2);
//  ftoa(lastOpStatus->endSOC,endSoc_Str,2);
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%s,%s,%d,%d,%d",lastOpStatus->startTime, 
//                               lastOpStatus->endTime,startSoc_Str, endSoc_Str, lastOpStatus->totalRotateCount1, 
//                               lastOpStatus->totalRotateCount2,lastOpStatus->errorCode);
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes(txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}
//static eJsonStatus ProcessGetOverCurrentLimits (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_OVERCURRENT_LIMIT);
//  dataCount = dataCount+bufferLength;    
//  bufferLength = sizeof(txBuff)-dataCount;
//  stMaxMotorCurrent* getMotorCurrent = GetSetMaxMotorCurrentLimits();
//  char Imot1_Str[10],Imot2_Str[10],Imot3_Str[10];
//  ftoa(getMotorCurrent->Imot1,Imot1_Str,2);
//  ftoa(getMotorCurrent->Imot2,Imot2_Str,2);
//  ftoa(getMotorCurrent->Imot3,Imot3_Str,2);
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%s,%s",Imot1_Str,Imot2_Str,Imot3_Str);
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes(txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}
//static eJsonStatus ProcessGetMotorCurrentValues (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_MOT_CURRENT_VALUES);
//  dataCount = dataCount+bufferLength;    
//  bufferLength = sizeof(txBuff)-dataCount;
//  char Imot1_Str[10],Imot2_Str[10],Imot3_Str[10];
//  ftoa(GetImot1Value(),Imot1_Str,2);
//  ftoa(GetImot2Value(),Imot2_Str,2);
//  ftoa(GetImot3Value(),Imot3_Str,2);
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%s,%s",Imot1_Str,Imot2_Str,Imot3_Str);
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes(txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}
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
  
//  Positivedistance = (float) ((float) 3.14 * (float) WheelDia);
//  Positive = Positivedistance * (getCountValue->maxPosCountForPVsetup);
   
 // uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d",getCountValue->maxPosCountForPVsetup,getCountValue->maxNegCountForPVsetup,Positive);
  
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d",getCountValue->maxPosCountForPVsetup,getCountValue->maxNegCountForPVsetup);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",*lowBatSoc);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%X%X%X%X",getSetCleanerMAC->cleanerMAC_32[3],getSetCleanerMAC->cleanerMAC_32[2],getSetCleanerMAC->cleanerMAC_32[1],getSetCleanerMAC->cleanerMAC_32[0]);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
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
static eJsonStatus ProcessGetZigbeeConfig (const char * jsonString)
{
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  bool status = ParseHeaderData(&txBuff[dataCount], &bufferLength, CMD_GET_ZIGBEE_CONF);
  dataCount = dataCount+bufferLength; 
  stZigbeeConfigRead zigbeeReadConfigData;
  GetZigbeeParameters(&zigbeeReadConfigData);
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%s,%s,%s,%s",(char *)zigbeeReadConfigData.AssocStatus,
                               (char *)zigbeeReadConfigData.configPanID,(char *)zigbeeReadConfigData.encryptionOptions,
                               (char *)zigbeeReadConfigData.encryptionEnable, (char *)zigbeeReadConfigData.joinVerifyEnable);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return JSON_NO_ERROR;
}

static eJsonStatus ProcessGetZigbeeNetworkParams (const char * jsonString)
{
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  bool status = ParseHeaderData(&txBuff[dataCount], &bufferLength, CMD_GET_ZIGBEE_NTW_PARAM);
  dataCount = dataCount+bufferLength; 
  stZigbeeNetworkParamRead zigbeeNetworkParamRead;
  GetZigbeeNetworkParameters(&zigbeeNetworkParamRead);
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%s,%s,%s",
                               (char *)zigbeeNetworkParamRead.opPanID,(char *)zigbeeNetworkParamRead.opPan16BitID,
                               (char *)zigbeeNetworkParamRead.opChannel, (char *)zigbeeNetworkParamRead.stackProfile);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return JSON_NO_ERROR;
}

static eJsonStatus ProcessGetBatteryAndPVvalues (const char * jsonString)
{
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  stBatCurrent BatteryCurrent;float Vin_f,Vbat_f,Iin_f,Ibat_f,Dtemp_f;
  char Vin_str[10],Vbat_str[10],Iin_str[10],Ibat_str[10],Dtemp_str[10];
  Vin_f = GetInputVoltage();
  ftoa(Vin_f,Vin_str,1);
  Vbat_f = GetBatteryVoltage();
  ftoa(Vbat_f,Vbat_str,1);
  Iin_f = GetInputCurrent();
  ftoa(Iin_f,Iin_str,1);
  Dtemp_f = GetDieTemperature();
  ftoa(Dtemp_f,Dtemp_str,1);
  GetBatteryCurrent(&BatteryCurrent);
  Ibat_f = BatteryCurrent.current;
  if(!BatteryCurrent.direction)
    Ibat_f = 0-Ibat_f;
  ftoa(Ibat_f,Ibat_str,1);
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  bool status = ParseHeaderData(&txBuff[dataCount], &bufferLength, CMD_GET_BAT_AND_PV_VAL);
  dataCount = dataCount+bufferLength; 
  bufferLength = sizeof(txBuff)-dataCount;
  //uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%s,%s,%s,%s",Vin_str,Iin_str,Vbat_str, Ibat_str,Dtemp_str);
  
  char Soc_Str[10];
  ftoa(GetSOCAsPercentage(),Soc_Str,2);
 // uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%d",Soc_Str,GetChargingOrNot());
  
  
  
  eTrackChangerReportState robotState =GetReportTrackChangerState();
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
 // uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%X,%X", modeValue, robotState,GetRotateSenseCount(), GetNextIndex(),GetMotorFaultCode(), IsErrorFromCleaner());

  
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%s,%s,%s,%s,%s,%s,%s,%s",bat_arr,pv_arr,mpptcurr_arr,mpptout_arr,inttemp_arr,exttemp_arr,batstate_arr,loadout_arr,loadtrip_arr); 
  
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%X,%X",Vin_str,Iin_str,Vbat_str, Ibat_str,Dtemp_str,Soc_Str,GetChargingOrNot(), modeValue, robotState,GetRotateSenseCount(), GetNextIndex(),GetMotorFaultCode(), IsErrorFromCleaner()); 
  
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%X,%X,%s,%s,%s,%s,%s,%s,%s,%s,%s",Vin_str,Iin_str,Vbat_str, Ibat_str,Dtemp_str,Soc_Str,GetChargingOrNot(), modeValue, robotState,
//                               GetRotateSenseCount(), GetNextIndex(),GetMotorFaultCode(), IsErrorFromCleaner(),bat_arr,pv_arr,mpptcurr_arr,mpptout_arr,inttemp_arr,exttemp_arr,batstate_arr,loadout_arr,loadtrip_arr);
  
    uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%X,%X,%s,%s,%s,%s,%d,%s,%s,%d,%d,%d,%d,%d",modeValue,robotState,GetRotateSenseCount(),GetNextIndex(),GetMotorFaultCode(),IsErrorFromCleaner(),pv_vol_arr,pv_curr_arr,pv_power_arr,battery_volt_arr,batt_temp,batt_charg_curr_arr,batt_discharg_curr_arr,pv_work_status,Batt_SOC,charge_cont_temp,generated_energy,controller_status);
  
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  //UartTransmit(txBuff,dataCount);
//  SetUartType(CONFIG_UART);
//  UartTransmit(txBuff,dataCount);
//  SetUartType(ZIGBEE_UART);
//  UartTransmit(txBuff,dataCount);
    if(GetSwitchState() == MANUAL_CONTROL)
    {
      UartTransmitType(CONFIG_UART,txBuff,dataCount);
    }
    else if(GetSwitchState() == AUTO_CONTROL)
    {
      UartTransmitType(ZIGBEE_UART,txBuff,dataCount);
    }
  ClearTxBuff();
  return JSON_NO_ERROR;
}

static eJsonStatus ProcessGetTemperaturevalues (const char * jsonString)
{
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  float Battemp_f,Drivetemp_f,Dietemp_f;
  char Battemp_str[10],Drivetemp_str[10],Dietemp_str[10];
  Battemp_f = GetTemperatureSensorData(TEMPSENSOR_1);
  ftoa(Battemp_f,Battemp_str,2);
  Drivetemp_f = GetTemperatureSensorData(TEMPSENSOR_2);
  ftoa(Drivetemp_f,Drivetemp_str,2);
  Dietemp_f = GetDieTemperature();
  ftoa(Dietemp_f,Dietemp_str,2);
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  bool status = ParseHeaderData(&txBuff[dataCount], &bufferLength, CMD_GET_TEMPERATURES);
  dataCount = dataCount+bufferLength; 
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%s,%s",Battemp_str,Drivetemp_str,Dietemp_str);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return JSON_NO_ERROR;
}

char* GetAcknowledgementData (eJsonStatus errorCode)
{
  //eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txAckBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txAckBuff)-dataCount;
  ParseHeaderData(&txAckBuff[dataCount],&bufferLength,CMD_ACK_DATA);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txAckBuff)-dataCount;
  uint8_t tempCount = snprintf(&txAckBuff[dataCount],bufferLength,"%X",errorCode);
  dataCount = dataCount+tempCount; 
  txAckBuff[dataCount] = STOP_CHAR;
  dataCount++;
  txAckBuff[dataCount] = 0;
  dataCount++;
  //  Serial_Debug("\r\ndataCount->");
  //  Serial_Debug_Num(dataCount); 
  //  Serial_Debug("\r\nData->");
  //  Serial_Debug_Bytes(txAckBuff,dataCount);
  //  Serial_Debug("\r\nEnd");
  //ClearTxAckBuff();
  return txAckBuff;
}

void ClearTxBuff(void)
{
  for(int i =0;i<MAX_UART_TX_BUFFER_LENGTH;i++)
  {
    txBuff[i] = 0;
  }
}
bool ParseHeaderData(uint8_t* tempBuff, uint16_t* headerBufferLength, uint16_t command )
{
  uint16_t buffCount = 0;
  
  tempBuff[0] = START_CHAR;
  
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  uint64_t macAddr = GetZigbeeMacAddress();                                     //  uint16_t deviceID = GetDeviceID();
  uint16_t maxStrLength = MAC_ID_STR_LEN;
  char macStr[MAC_ID_STR_LEN];
  Int64ToHexString (macAddr, macStr, &maxStrLength);
  uint16_t snData = snprintf(&tempBuff[buffCount],*headerBufferLength,"%02X",DEVICE_ID);
  buffCount = buffCount+snData;
  *headerBufferLength = *headerBufferLength - buffCount;
  //printf("Data->%s",tempBuff);
  tempBuff[buffCount] = DELIMITER_1;
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  snData = snprintf(&tempBuff[buffCount],*headerBufferLength,"%04X",command);
  buffCount = buffCount+snData;
  *headerBufferLength = *headerBufferLength - buffCount;
  tempBuff[buffCount] = DELIMITER_2;
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  *headerBufferLength = buffCount;
  // Serial_Debug_Bytes(tempBuff,buffCount);
  //Serial_Debug(tempBuff);
  //      if(*headerBufferLength < MAX_UART_TX_BUFFER_LENGTH)
  //  {
  //    return false;
  //  }
  //  else
  //  {
  return true;
  //  }
  
}

//static eJsonStatus ProcessSetBrushPwm (const char * jsonString, stBrushPwmParam *tempSetPwmParam)
//{
//  Serial_Debug(jsonString);
//  eJsonStatus error_code = JSON_NO_ERROR;
//  
//  char *pwmValuesStr[JSON_NO_OF_PWM_DATA_BRUSH];
//  uint8_t maxNoStrings=(sizeof pwmValuesStr)/4;
//  uint8_t totalResultStringCount =  SplitString (jsonString, pwmValuesStr, ',',maxNoStrings);
//  Serial_Debug("\nPWM Set Received Values :");
//  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
//  {
//    Serial_Debug(pwmValuesStr[dispcount]);
//    Serial_Debug(",");
//  }
//  if(totalResultStringCount == JSON_NO_OF_PWM_DATA_BRUSH)
//  {
//    bool status = true;
//    status &= SetParameterValue_int(pwmValuesStr[0], (uint32_t *)&(tempSetPwmParam->accelTime), IsAccelTimeValid);
//    status &= SetParameterValue_int(pwmValuesStr[1], (uint32_t *)&(tempSetPwmParam->decelTime) , IsDecelTimeValid);
//    status &= SetParameterValue_int(pwmValuesStr[2], (uint32_t *)&(tempSetPwmParam->steadyPwm), IsPWMvalueValid);
//    
//    if(status == false)
//    {
//      error_code = JSON_DATA_OUT_OF_RANGE;
//      Serial_Debug("\n Pwm SetValue/s Out of Range");
//    }
//    else
//    {
//      error_code = JSON_NO_ERROR;
//    }            
//  }
//  else
//  {
//    Serial_Debug("\n Pwm SetValue/s Invalid");
//    error_code = JSON_INVALID_DATA;
//  }
//  return error_code;  
//}


static eJsonStatus ProcessSetMotorPwm (const char * jsonString, stRobotPwmParam *tempSetPwmParam)
{
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  
  char *pwmValuesStr[JSON_NO_OF_SETGET_PWM_DATA_MOTOR];
  uint8_t maxNoStrings=(sizeof pwmValuesStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, pwmValuesStr, ',',maxNoStrings);
  Serial_Debug("\nPWM Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(pwmValuesStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_SETGET_PWM_DATA_MOTOR)
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


static eJsonStatus ProcessSetState (const char * jsonString, bool* tempSetBool)
{
  Serial_Debug("\r\nIn ProcessSetState->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char *setStateStr[JSON_NO_OF_MODE_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, setStateStr, ',',maxNoStrings); 
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

static eJsonStatus  ProcessSetZigbeeConfig (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetZigbeeConfig->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char *zigbeeConfigStr[JSON_NO_OF_ZIGBEE_CONFIG_DATA];
  uint8_t maxNoStrings=(sizeof zigbeeConfigStr)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, zigbeeConfigStr, ',', maxNoStrings );
  Serial_Debug("\nZigbee Config Parameters Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(zigbeeConfigStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_ZIGBEE_CONFIG_DATA)
  {
    bool status = true;
    stZigbeeConfig *setZigbeeConfig = GetSetZigbeeConfig();
    status &= CheckHexStrValidity(zigbeeConfigStr[0], (PAN_ID_STR_LEN-1));
    status &= CheckHexStrValidity(zigbeeConfigStr[1], (LINK_KEY_STR_LEN-1));
    status &= CheckHexStrValidity(zigbeeConfigStr[2], (NETWORK_OPTIONS_STR_LEN-1));
    status &= ((zigbeeConfigStr[3][0] == NULL)||
               ((zigbeeConfigStr[3][0] == '0')||(zigbeeConfigStr[3][0] == '1')&&(zigbeeConfigStr[3][1] == NULL)));
    status &= ((zigbeeConfigStr[4][0] == NULL)||
               ((zigbeeConfigStr[4][0] == '0')||(zigbeeConfigStr[4][0] == '1')&&(zigbeeConfigStr[4][1] == NULL)));
    status &= ((zigbeeConfigStr[5][0] == NULL)||
               ((zigbeeConfigStr[5][0] == '0')||(zigbeeConfigStr[5][0] == '1')&&(zigbeeConfigStr[5][1] == NULL)));
    if(status == true)
    {
      snprintf((char *)setZigbeeConfig->panID, PAN_ID_STR_LEN, zigbeeConfigStr[0]);
      snprintf((char *)setZigbeeConfig->encryptionLinkKey, LINK_KEY_STR_LEN, zigbeeConfigStr[1]);
      snprintf((char *)setZigbeeConfig->encryptionOptions, NETWORK_OPTIONS_STR_LEN, zigbeeConfigStr[2]);
      setZigbeeConfig->encryptionEnable = zigbeeConfigStr[3][0];
      setZigbeeConfig->joinVerifyEnable = zigbeeConfigStr[4][0];
      setZigbeeConfig->writeEnable = zigbeeConfigStr[5][0];
      UpdateZigbeeConfigChangeFlag(true);
      Serial_Debug("\nZigbee Config  Set");
      error_code = JSON_NO_ERROR;
    }
    else
    {
      Serial_Debug("\nZigbee Config SetValue/s: Out of Range");
      error_code = JSON_DATA_OUT_OF_RANGE;   
    } 
  }
  else
  {
    Serial_Debug("\nZigbee Config SetValue/s : Invalid Data");
    error_code = JSON_INVALID_DATA;
  }
  return error_code; 
}

static eJsonStatus ProcessResetZigbeeNetwork (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessResetZigbeeNW->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char *zigbeeConfigStr[JSON_NO_OF_ZIGBEE_NW_RESET_DATA];
  uint8_t maxNoStrings=(sizeof zigbeeConfigStr)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, zigbeeConfigStr, ',', maxNoStrings );
  Serial_Debug("\nZigbee Nw Reset Parameters Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(zigbeeConfigStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_ZIGBEE_NW_RESET_DATA)
  {
    bool status = ((zigbeeConfigStr[0][0] == NULL)||
                   ((zigbeeConfigStr[0][0] == '0')||(zigbeeConfigStr[0][0] == '1')&&(zigbeeConfigStr[0][1] == NULL)));
    if(status == true)
    {
      if(zigbeeConfigStr[0][0] == NULL)
      {
        UpdateZigbeeNetworkResetFlag(NW_RESET_IDLE);
      }
      else
      {
        UpdateZigbeeNetworkResetFlag((eZigNwReset)(zigbeeConfigStr[0][0] - '0'));         
      }
      Serial_Debug("\nZigbee  Nw Reset  Set");
      error_code = JSON_NO_ERROR;
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

//static eJsonStatus ProcessSetMotorFaultConditions (const char * jsonString)
//{
//  Serial_Debug("\r\nIn ProcessSetMotor fault conditions->");
//  Serial_Debug(jsonString); 
//  eJsonStatus error_code = JSON_NO_ERROR;
//  stMaxIloadConfigValues* setMaxIloadConfigValues;
//  setMaxIloadConfigValues = GetSetMaxIloadConfigValues();
//  stMaxIloadConfigValues tempSetMaxIloadConfigValues = *setMaxIloadConfigValues;
//  char *MotorFaultConditionsStr[JSON_NO_OF_MOTOR_FAULT_CONDITIONS];
//  uint8_t maxNoStrings=(sizeof MotorFaultConditionsStr)/4;
//  uint8_t totalResultStringCount =  SplitString (jsonString, MotorFaultConditionsStr, ',',maxNoStrings);
//  Serial_Debug("\nMotor fault conditions Received Values :");
//  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
//  {
//    Serial_Debug(MotorFaultConditionsStr[dispcount]);
//    Serial_Debug(",");
//  }  
//  if(totalResultStringCount == JSON_NO_OF_MOTOR_FAULT_CONDITIONS)
//  {
//    bool status = true;
//    status &= SetParameterValue_int(MotorFaultConditionsStr[0], &(tempSetMaxIloadConfigValues.maxIloadNoiseTimeValue), IsIloadMaxConfigTimeValid);
//    status &= SetParameterValue_int(MotorFaultConditionsStr[1], &(tempSetMaxIloadConfigValues.maxIloadFreqTimeValue) , IsIloadMaxConfigTimeValid);
//    status &= SetParameterValue_int(MotorFaultConditionsStr[2], &(tempSetMaxIloadConfigValues.maxIloadRepeatCountValue), IsIloadMaxConfigTimeValid);
//    status &= SetParameterValue_int(MotorFaultConditionsStr[3], &(tempSetMaxIloadConfigValues.motorPauseDelay) , IsIloadMaxConfigTimeValid);
//    
//    if(status == false)
//    {
//      error_code = JSON_DATA_OUT_OF_RANGE;
//      Serial_Debug("\n Motor fault conditions SetValue/s Out of Range");
//    }
//    else
//    {
//      error_code = JSON_NO_ERROR;
//      Serial_Debug("\n Motor fault conditions SetValue/s set : ");
//      *setMaxIloadConfigValues = tempSetMaxIloadConfigValues;
//      Serial_Debug_Num(setMaxIloadConfigValues->maxIloadNoiseTimeValue);
//      Serial_Debug(",");
//      Serial_Debug_Num(setMaxIloadConfigValues->maxIloadFreqTimeValue);
//      Serial_Debug(",");
//      Serial_Debug_Num(setMaxIloadConfigValues->maxIloadRepeatCountValue);
//      Serial_Debug(",");
//      Serial_Debug_Num(setMaxIloadConfigValues->motorPauseDelay);
//    }            
//  }
//  else
//  {
//    Serial_Debug("\n Motor fault conditions SetValue/s Invalid");
//    error_code = JSON_INVALID_DATA;
//  }
//  return error_code;
//}
//
//
//static eJsonStatus ProcessGetMotorFaultConditions (const char * jsonString)
//{
//  
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_PULSE_COUNT_VALUES);
//  dataCount = dataCount+bufferLength;    
//  bufferLength = sizeof(txBuff)-dataCount;
//  stMaxIloadConfigValues* getMaxIloadConfigValues = GetSetMaxIloadConfigValues();
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d",getMaxIloadConfigValues->maxIloadNoiseTimeValue,getMaxIloadConfigValues->maxIloadFreqTimeValue,getMaxIloadConfigValues->maxIloadRepeatCountValue,getMaxIloadConfigValues->motorPauseDelay);
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes(txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}

eJsonStatus SendProximityReached(uint8_t index)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_REPORT_MSG);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",index);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  SetUartType(CONFIG_UART);
  UartTransmit(txBuff,dataCount);
  SetUartType(ZIGBEE_UART);
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%000X",reportType,reportValue);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
//  SetUartType(ZIGBEE_UART);
//  UartTransmit(txBuff,dataCount);
//  SetUartType(CONFIG_UART);
//  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus RequestActivityLog (const char * jsonString)
{
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  uint32_t startIndex = 0, endIndex = 0;
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *reqLogStr[JSON_NO_OF_REQUEST_LOG_VALUES];
  uint8_t maxNoStrings=(sizeof reqLogStr)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, reqLogStr, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqLogStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_REQUEST_LOG_VALUES)
  {
    if((reqLogStr[0][0] != NULL) && (reqLogStr[1][0] != NULL))
    {
      startIndex = strtol(reqLogStr[0], NULL, 10);
      endIndex = strtol(reqLogStr[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_STORE_LOG_DATA && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
        Serial_Debug(dispstring);
        error_code = JSON_NO_ERROR;
      }
      else
      {
        Serial_Debug("\nRequest Logs Value/s: Out of Range");
        error_code = JSON_DATA_OUT_OF_RANGE;
      }
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
  //  }
  //  else
  //  {
  //    Serial_Debug("\nRequest Logs Value/s : Parse Error");
  //    error_code = JSON_PARSE_ERROR;
  //  }
  if(error_code == JSON_NO_ERROR)
  {
    
    
    char values[MAX_NO_OF_EVENT_LOGS][LOG_STR_LEN+1+1];
    uint8_t logIndex = 0;
    //  uint16_t logArrayStartIndex= 0;
    uint8_t logStoreData[MAX_NO_OF_EVENT_LOGS][SINGLE_STORE_BYTE_COUNT];
    uint32_t noOfReadLogs = ReadFromLogFlash(&logStoreData[0][0], MAX_NO_OF_EVENT_LOGS, startIndex, endIndex);
    Serial_Debug("\r\nIn API Processing->");
    for(logIndex = 0; logIndex < noOfReadLogs && logIndex < MAX_NO_OF_EVENT_LOGS; logIndex++)
    {
      // logArrayStartIndex = logIndex * SINGLE_STORE_BYTE_COUNT;
      uint32_t utcTime =  ((uint32_t)logStoreData[logIndex][0] << 24)  | ((uint32_t)logStoreData[logIndex][1] << 16) 
        | ((uint32_t)logStoreData[logIndex][2] << 8) | logStoreData[logIndex][3];
      uint16_t eventValue = (logStoreData[logIndex][5]<<8)|logStoreData[logIndex][6];
      if(logIndex == 0)
      {
        snprintf(&values[logIndex][0],(LOG_STR_LEN+1+1),"[%ld,%d,%d]",utcTime,logStoreData[logIndex][4], (logStoreData[logIndex][5]<<8)|logStoreData[logIndex][6]);
      }
      else
      {
        snprintf(&values[logIndex][0],(LOG_STR_LEN+1),"[%ld,%d,%d]",utcTime,logStoreData[logIndex][4],eventValue );
      }
      //      Serial_Debug(values[logIndex]);
      //      Serial_Debug(" ,");
    }
    
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    if(GetUartType()==CONFIG_UART)
    {
      bool status = ParseHeaderDataJson(&txBuff[dataCount], &bufferLength, CMD_REQ_ACTIVITY_LOG,GetZigbeeMacAddress());
      dataCount = dataCount+bufferLength;
      bufferLength = sizeof(txBuff)-dataCount;
      txBuff[dataCount]=0;
      Serial_Debug(txBuff);
      UartTransmitType(CONFIG_UART,txBuff,dataCount-1);
      uint16_t sentLogCount = 0;
      Serial_Debug(",\"VALUES\":\"[");
      UartTransmitType(CONFIG_UART,",\"VALUES\":\"[",strlen(",\"VALUES\":\"["));
      while(sentLogCount<noOfReadLogs)
      {
        Serial_Debug(values[sentLogCount]);
        UartTransmitType(CONFIG_UART,values[sentLogCount],strlen(values[sentLogCount]));
        if((noOfReadLogs-sentLogCount)>1)
        {
          UartTransmitType(CONFIG_UART,",",1);
          Serial_Debug(",");
        }
        sentLogCount++;
      }
      
      ClearTxBuff();
      txBuff[0]=']';
      txBuff[1]='"';
      txBuff[2]='}';
      txBuff[3]=0;
      Serial_Debug(txBuff);
      UartTransmitType(CONFIG_UART,txBuff,3);
    }
    else
    {
      bool status = ParseHeaderData(&txBuff[dataCount], &bufferLength, CMD_REQ_ACTIVITY_LOG);
      dataCount = dataCount+bufferLength;
      bufferLength = sizeof(txBuff)-dataCount;
      UartTransmitType(ZIGBEE_UART,txBuff,dataCount);
      txBuff[dataCount]=0;
      Serial_Debug(txBuff);
      uint16_t sentLogCount = 0;
      while(sentLogCount<noOfReadLogs)
      {
        Serial_Debug(values[sentLogCount]);
        UartTransmitType(ZIGBEE_UART,values[sentLogCount],strlen(values[sentLogCount]));
        if((noOfReadLogs-sentLogCount)>1)
        {
          UartTransmitType(ZIGBEE_UART,",",1);
          Serial_Debug(",");
        }
        sentLogCount++;
      }
      
      ClearTxBuff();
      txBuff[0]=']';
      txBuff[1]=STOP_CHAR;
      txBuff[2]=0;
      
      UartTransmitType(ZIGBEE_UART,txBuff,2);
      Serial_Debug(txBuff);
    }
    
  }
  ClearTxBuff();
  return error_code;
}

//static eJsonStatus ProcessSetTrackCount (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR; 
//  Serial_Debug("\r\nIn ProcessSetTrackCount->");
//  Serial_Debug(jsonString);
//  uint8_t mode;
//  char *setStateStr[JSON_NO_OF_MODE_STATE_DATA];
//  uint8_t maxNoStrings=(sizeof setStateStr)/4;
//  uint8_t totalResultStringCount =  SplitString (jsonString, setStateStr, ',',maxNoStrings); 
//  Serial_Debug("\nTrack count Values :");
//  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
//  {
//    Serial_Debug(setStateStr[dispcount]);
//    Serial_Debug(",");
//  }
//  if(totalResultStringCount == JSON_NO_OF_MODE_STATE_DATA)
//  {
//    bool status = true;
//    status &= SetParameterValue_int(setStateStr[0], &mode, IsManualCommandValid);
//    
//    if(status == false)
//    {
//      error_code = JSON_DATA_OUT_OF_RANGE;
//      Serial_Debug("\n Track count SetValue/s Out of Range");
//    }
//    else
//    {
//      stTrackData* trackCount_temp = GetSetTrackData();
//      trackCount_temp->trackCount = mode; 
//    }            
//  }
//  else
//  {
//    Serial_Debug("\n Track count SetValue/s Invalid");
//    error_code = JSON_INVALID_DATA;
//  }
//  return error_code;
//}

static eJsonStatus ProcessSetTrackInfo (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetTrackInfo->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *trackDataStr[JSON_NO_OF_TRACK_INFO_DATA];
  int i;
  uint8_t maxNoStrings=(sizeof trackDataStr)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, trackDataStr, ',', maxNoStrings );
  
  Serial_Debug("\nTrack Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(trackDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_TRACK_INFO_DATA)
  {
    if((trackDataStr[0][0] != NULL) && (trackDataStr[1][0] != NULL))
    {
      startIndex = strtol(trackDataStr[0], NULL, 10);
      endIndex = strtol(trackDataStr[1], NULL, 10);
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
          for(i = (startIndex-1);i<(difference+startIndex-1);i++)
          {
            status &= SetParameterValue_int(trackDataStr[i+2], &(tempSetTrackInfo.trackDistance[i]), IsTrackDistanceValid);
            if(status)
            {
              tempSetTrackInfo.trackValid[i] = TRACK_INFO_VALID;
            }
          }
          for(i;i<15;i++)
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
  uint32_t startIndex = 0, endIndex = 0;
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *reqTrackInfo[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqTrackInfo)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, reqTrackInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqTrackInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqTrackInfo[0][0] != NULL) && (reqTrackInfo[1][0] != NULL))
    {
      startIndex = strtol(reqTrackInfo[0], NULL, 10);
      endIndex = strtol(reqTrackInfo[1], NULL, 10);
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
  //  }
  //  else
  //  {
  //    Serial_Debug("\nRequest Track Info Value/s : Parse Error");
  //    error_code = JSON_PARSE_ERROR;
  //  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_TRACK_INFO);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    //stMaxIloadConfigValues* getMaxIloadConfigValues = GetSetMaxIloadConfigValues();
    stTrackData* setTrackData = GetSetTrackData();
    for(int i=startIndex;i<=endIndex;i++)
    {
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,",setTrackData->trackDistance[i-1]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
    Serial_Debug("\r\nEnd");
 //   UartTransmit(txBuff,dataCount);
     if(GetSwitchState() == MANUAL_CONTROL)
        {
          UartTransmitType(CONFIG_UART,txBuff,dataCount);
        }
        else if(GetSwitchState() == AUTO_CONTROL)
        {
          UartTransmitType(ZIGBEE_UART,txBuff,dataCount);
        }
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
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *trackDataStr[JSON_NO_OF_TRACK_INFO_DATA];
  int i;
  uint8_t maxNoStrings=(sizeof trackDataStr)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, trackDataStr, ',', maxNoStrings );
  
  Serial_Debug("\nTrack Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(trackDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_TRACK_INFO_DATA)
  {
    if((trackDataStr[0][0] != NULL) && (trackDataStr[1][0] != NULL))
    {
      startIndex = strtol(trackDataStr[0], NULL, 10);
      endIndex = strtol(trackDataStr[1], NULL, 10);
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
          for(i = (startIndex-1);i<(difference+startIndex-1);i++)
          {
            status &= SetParameterValue_int(trackDataStr[i+2], &(tempSetTrackInfo.trackDistance2[i]), IsTrackDistanceValid);
            if(status)
            {
              tempSetTrackInfo.trackValid2[i] = TRACK_INFO_VALID;
            }
          }
          for(i;i<15;i++)
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
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *reqTrackInfo[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqTrackInfo)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, reqTrackInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqTrackInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqTrackInfo[0][0] != NULL) && (reqTrackInfo[1][0] != NULL))
    {
      startIndex = strtol(reqTrackInfo[0], NULL, 10);
      endIndex = strtol(reqTrackInfo[1], NULL, 10);
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
  //  }
  //  else
  //  {
  //    Serial_Debug("\nRequest Track Info Value/s : Parse Error");
  //    error_code = JSON_PARSE_ERROR;
  //  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_TRACK_INFO2);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    //stMaxIloadConfigValues* getMaxIloadConfigValues = GetSetMaxIloadConfigValues();
    stTrackData* setTrackData = GetSetTrackData();
    for(int i=startIndex;i<=endIndex;i++)
    {
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,",setTrackData->trackDistance2[i-1]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
    Serial_Debug("\r\nEnd");
 //   UartTransmit(txBuff,dataCount);
    if(GetSwitchState() == MANUAL_CONTROL)
        {
          UartTransmitType(CONFIG_UART,txBuff,dataCount);
        }
        else if(GetSwitchState() == AUTO_CONTROL)
        {
          UartTransmitType(ZIGBEE_UART,txBuff,dataCount);
        }
        ClearTxBuff();
  }
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
  uint8_t totalResultStringCount =  SplitString (jsonString, CleanerMACdataStr, ',',maxNoStrings);
  Serial_Debug("\nCleaner MAC Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(CleanerMACdataStr[dispcount]);
    Serial_Debug(",");
  }  
  if((totalResultStringCount == JSON_NO_OF_CLEANER_MAC_DATA)&&(strlen(CleanerMACdataStr[0])!=0))
  {
    bool status = true;
    if(strlen(CleanerMACdataStr[0])>=MAC_ID_STR_LEN)
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
      tempSetCleanerMAC.cleanerMAC_64 = HexStringtoInt(CleanerMACdataStr[0]);
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


static eJsonStatus ProcessSetRowLengthData (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRowLengthData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *rowLengthDataStr[JSON_NO_OF_ROW_LENGTH_DATA];
  uint8_t maxNoStrings=(sizeof rowLengthDataStr)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, rowLengthDataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLengthDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLengthDataStr[0][0] != NULL) && (rowLengthDataStr[1][0] != NULL))
    {
      startIndex = strtol(rowLengthDataStr[0], NULL, 10);
      endIndex = strtol(rowLengthDataStr[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0)
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
          
          //memset(tempSetRowLengthData.rowLength,0,sizeof(tempSetRowLengthData.rowLength[MAX_NO_OF_TRACKS][TOTAL_NO_OF_COUNT]));
          if(Row1value==1) 
          {
            for(int i = (startIndex-1),j=3;(i<(difference+startIndex-1)&&j<=noOfdata+1);i++,j+=2)
            {
              status &= SetParameterValue_int(rowLengthDataStr[j], &(tempSetRowLengthData.rowLength[i][FORWARD_COUNT]), IsPulseCountValid);
              status &= SetParameterValue_int(rowLengthDataStr[j+1], &(tempSetRowLengthData.rowLength[i][REVERSE_COUNT]), IsPulseCountValid);
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
              status &= SetParameterValue_int(rowLengthDataStr[j], &(tempSetRowLengthData.rowLength[i+9][FORWARD_COUNT]), IsPulseCountValid);
              status &= SetParameterValue_int(rowLengthDataStr[j+1], &(tempSetRowLengthData.rowLength[i+9][REVERSE_COUNT]), IsPulseCountValid);
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
              status &= SetParameterValue_int(rowLengthDataStr[j], &(tempSetRowLengthData.rowLength[i+18][FORWARD_COUNT]), IsPulseCountValid);
              status &= SetParameterValue_int(rowLengthDataStr[j+1], &(tempSetRowLengthData.rowLength[i+18][REVERSE_COUNT]), IsPulseCountValid);
              //          Serial_Debug("\r\ni - >");
              //          Serial_Debug_Num(i);
              //          Serial_Debug("   j - >");
              //          Serial_Debug_Num(j);
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
 // int Row1getvalue =0;
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *reqRowLengthInfo[JSON_NO_OF_GET_TRACK_INFO+1];
  uint8_t maxNoStrings=(sizeof reqRowLengthInfo)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO+1)
  {
    if((reqRowLengthInfo[0][0] != NULL) && (reqRowLengthInfo[1][0] != NULL))
    {
      startIndex = strtol(reqRowLengthInfo[0], NULL, 10);
      endIndex = strtol(reqRowLengthInfo[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS+MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA+MAX_NO_OF_SET_DATA))  //changed
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
  //  }
  //  else
  //  {
  //    Serial_Debug("\nRequest Track Info Value/s : Parse Error");
  //    error_code = JSON_PARSE_ERROR;
  //  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_ROW_LENGTH_DATA);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    //stMaxIloadConfigValues* getMaxIloadConfigValues = GetSetMaxIloadConfigValues();
    stRowLengthData *setRowlengthData = GetSetRowLengthData();
    
    if(Row1getvalue == 1)
    {  
      for(int i=startIndex-1;i<=endIndex-1;i++)
      {
        uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength[i][FORWARD_COUNT]);
        dataCount = dataCount+tempCount; 
        tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength[i][REVERSE_COUNT]);
        dataCount = dataCount+tempCount; 
      }
    }
   else  if(Row1getvalue == 2)
    {  
      for(int i=startIndex-1;i<=endIndex-1;i++)
      {
        uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength[i+9][FORWARD_COUNT]);
        dataCount = dataCount+tempCount; 
        tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength[i+9][REVERSE_COUNT]);
        dataCount = dataCount+tempCount; 
      }
    }
   else if(Row1getvalue == 3)
    {  
      for(int i=startIndex-1;i<=endIndex-1;i++)
      {
        uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength[i+18][FORWARD_COUNT]);
        dataCount = dataCount+tempCount; 
        tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength[i+18][REVERSE_COUNT]);
        dataCount = dataCount+tempCount; 
      }
    }
    
    else if(Row1getvalue == 4)
    {  
      for(int i=startIndex-1;i<=endIndex-1;i++)
      {
        uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength[i][FORWARD_COUNT]);
        dataCount = dataCount+tempCount; 
        tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength[i][REVERSE_COUNT]);
        dataCount = dataCount+tempCount; 
      }
    }
   
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
    Serial_Debug("\r\nEnd");
   // UartTransmit(txBuff,dataCount);
       if(GetSwitchState() == MANUAL_CONTROL)
        {
          UartTransmitType(CONFIG_UART,txBuff,dataCount);
        }
        else if(GetSwitchState() == AUTO_CONTROL)
        {
          UartTransmitType(ZIGBEE_UART,txBuff,dataCount);
        }
        ClearTxBuff();
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
  uint8_t totalResulStringCount =  SplitString (jsonString, rowLength2DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength2DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength2DataStr[0][0] != NULL) && (rowLength2DataStr[1][0] != NULL))
    {
      startIndex = strtol(rowLength2DataStr[0], NULL, 10);
      endIndex = strtol(rowLength2DataStr[1], NULL, 10);
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
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength2DataStr[j], &(tempSetRowLengthData.rowLength2[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength2DataStr[j+1], &(tempSetRowLengthData.rowLength2[i][REVERSE_COUNT]), IsPulseCountValid);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != NULL) && (reqRowLengthInfo[1][0] != NULL))
    {
      startIndex = strtol(reqRowLengthInfo[0], NULL, 10);
      endIndex = strtol(reqRowLengthInfo[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
    stRowLengthData *setRowlengthData = GetSetRowLengthData();
    for(int i=startIndex-1;i<=endIndex-1;i++)
    {
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength2[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength2[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, rowLength3DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength3DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength3DataStr[0][0] != NULL) && (rowLength3DataStr[1][0] != NULL))
    {
      startIndex = strtol(rowLength3DataStr[0], NULL, 10);
      endIndex = strtol(rowLength3DataStr[1], NULL, 10);
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
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength3DataStr[j], &(tempSetRowLengthData.rowLength3[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength3DataStr[j+1], &(tempSetRowLengthData.rowLength3[i][REVERSE_COUNT]), IsPulseCountValid);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != NULL) && (reqRowLengthInfo[1][0] != NULL))
    {
      startIndex = strtol(reqRowLengthInfo[0], NULL, 10);
      endIndex = strtol(reqRowLengthInfo[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength3[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength3[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, rowLength4DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength4DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength4DataStr[0][0] != NULL) && (rowLength4DataStr[1][0] != NULL))
    {
      startIndex = strtol(rowLength4DataStr[0], NULL, 10);
      endIndex = strtol(rowLength4DataStr[1], NULL, 10);
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
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength4DataStr[j], &(tempSetRowLengthData.rowLength4[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength4DataStr[j+1], &(tempSetRowLengthData.rowLength4[i][REVERSE_COUNT]), IsPulseCountValid);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != NULL) && (reqRowLengthInfo[1][0] != NULL))
    {
      startIndex = strtol(reqRowLengthInfo[0], NULL, 10);
      endIndex = strtol(reqRowLengthInfo[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength4[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength4[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, rowLength5DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength5DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength5DataStr[0][0] != NULL) && (rowLength5DataStr[1][0] != NULL))
    {
      startIndex = strtol(rowLength5DataStr[0], NULL, 10);
      endIndex = strtol(rowLength5DataStr[1], NULL, 10);
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
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength5DataStr[j], &(tempSetRowLengthData.rowLength5[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength5DataStr[j+1], &(tempSetRowLengthData.rowLength5[i][REVERSE_COUNT]), IsPulseCountValid);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != NULL) && (reqRowLengthInfo[1][0] != NULL))
    {
      startIndex = strtol(reqRowLengthInfo[0], NULL, 10);
      endIndex = strtol(reqRowLengthInfo[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength5[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength5[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, rowLength6DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength6DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength6DataStr[0][0] != NULL) && (rowLength6DataStr[1][0] != NULL))
    {
      startIndex = strtol(rowLength6DataStr[0], NULL, 10);
      endIndex = strtol(rowLength6DataStr[1], NULL, 10);
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
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength6DataStr[j], &(tempSetRowLengthData.rowLength6[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength6DataStr[j+1], &(tempSetRowLengthData.rowLength6[i][REVERSE_COUNT]), IsPulseCountValid);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != NULL) && (reqRowLengthInfo[1][0] != NULL))
    {
      startIndex = strtol(reqRowLengthInfo[0], NULL, 10);
      endIndex = strtol(reqRowLengthInfo[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength6[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength6[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, rowLength7DataStr, ',', maxNoStrings );
  Serial_Debug("\nRow Length Info Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rowLength7DataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_ROW_LENGTH_DATA)
  {
    if((rowLength7DataStr[0][0] != NULL) && (rowLength7DataStr[1][0] != NULL))
    {
      startIndex = strtol(rowLength7DataStr[0], NULL, 10);
      endIndex = strtol(rowLength7DataStr[1], NULL, 10);
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
        if((difference*2) == (noOfdata))
        {
          stRowLengthData* setRowLengthData = GetSetRowLengthData();
          stRowLengthData tempSetRowLengthData = *setRowLengthData;
          bool status = true;
          for(int i = (startIndex-1),j=2;(i<(difference+startIndex-1)&&j<=noOfdata);i++,j+=2)
          {
            status &= SetParameterValue_int(rowLength7DataStr[j], &(tempSetRowLengthData.rowLength7[i][FORWARD_COUNT]), IsPulseCountValid);
            status &= SetParameterValue_int(rowLength7DataStr[j+1], &(tempSetRowLengthData.rowLength7[i][REVERSE_COUNT]), IsPulseCountValid);
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
  uint8_t totalResulStringCount =  SplitString (jsonString, reqRowLengthInfo, ',', maxNoStrings );
  Serial_Debug("\nRequest Row Length Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRowLengthInfo[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRowLengthInfo[0][0] != NULL) && (reqRowLengthInfo[1][0] != NULL))
    {
      startIndex = strtol(reqRowLengthInfo[0], NULL, 10);
      endIndex = strtol(reqRowLengthInfo[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 && ((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength7[i][FORWARD_COUNT]);
      dataCount = dataCount+tempCount; 
      tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRowlengthData->rowLength7[i][REVERSE_COUNT]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    UartTransmit(txBuff,dataCount);
    ClearTxBuff();
  }
  return error_code;
}

//static eJsonStatus ProcessGetTrackChangerStatus (const char * jsonString)
//{
//  eJsonStatus error_code = JSON_NO_ERROR;
//  uint16_t dataCount=0;
//  txBuff[dataCount]=0;
//  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_TRACK_CHANGER_STATUS);
//  dataCount = dataCount+bufferLength;    
//  bufferLength = sizeof(txBuff)-dataCount;
//  eTrackChangerReportState robotState =GetReportTrackChangerState();
//  eMode getMode =  GetOperationMode ();
//  uint8_t modeValue = 0;
//  switch(getMode)
//  {
//  case AUTO: 
//    modeValue = 0;
//    break;
//  case MANUAL:
//    modeValue = 1;
//    break;
//  default : 
//    modeValue = 1;
//    break;
//  }
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%X,%X", modeValue, robotState,GetRotateSenseCount(), GetNextIndex(),GetMotorFaultCode(), IsErrorFromCleaner());
//  dataCount = dataCount+tempCount; 
//  txBuff[dataCount] = STOP_CHAR;
//  dataCount++;
//  Serial_Debug("\r\ndataCount->");
//  Serial_Debug_Num(dataCount); 
//  Serial_Debug("\r\nData->");
//  Serial_Debug_Bytes(txBuff,dataCount);
//  Serial_Debug("\r\nEnd");
//  UartTransmit(txBuff,dataCount);
//  ClearTxBuff();
//  return error_code;
//}

static eJsonStatus ProcessGetSensorStatus (const char * jsonString)
{
   eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_SENSOR_STATUS);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;

  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d", IsEdgeSensor1Detected(), IsEdgeSensor2Detected(), IsValidProximitySensorState(),GetCleanerDetect1PinState(), GetCleanerDetect2PinState());
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
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
  uint32_t* pulseTolerance = GetSetEncoderPulseTolerance();
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld", *pulseTolerance);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
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
  uint32_t* setEncPulTolerance = GetSetEncoderPulseTolerance();
  uint32_t tempSetEncPulTolerance = *setEncPulTolerance;
  uint32_t totalResultStringCount =  SplitString (jsonString, setEncPulToleranceValueStr, ',',maxNoStrings);
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
static eJsonStatus ClearTrackInfo (const char * jsonString)
{
//  Serial_Debug("\r\nClearTrackInfo");
//  Serial_Debug(jsonString);
//  stTrackData* setTrackInfo = GetSetTrackData();
//  for(int i = 0;i<MAX_NO_OF_TRACKS;i++)
//  {
//    setTrackInfo->trackDistance[i] = 0;
//    setTrackInfo->trackValid[i] = TRACK_INFO_INVALID;
//  }
  
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
//
//for(int i=0;i<MAX_NO_OF_TRACKS;i++)
//    {
//      setRowLengthData->rowLength3[i][FORWARD_COUNT] = DEFAULT_FORWARD_COUNT;
//      setRowLengthData->rowLength3[i][REVERSE_COUNT] = DEFAULT_REVERSE_COUNT;
//    }
  return JSON_NO_ERROR;
}

static eJsonStatus ProcessSetTcControlMode (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetTcControlMode->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  uint8_t mode;
  char *setStateStr[JSON_NO_OF_MODE_STATE_DATA];
  uint8_t maxNoStrings=(sizeof setStateStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_MODE_STATE_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setStateStr[0], &mode, IsModeValid);
    
    if(status == false)
    {
      error_code = JSON_DATA_OUT_OF_RANGE;
      Serial_Debug("\n Set Mode SetValue/s Out of Range");
    }
    else
    {
      switch(mode)
      {
      case ENCODER_MODE : 
        DisableRotateSense();
        SetTcControlMode(ENCODER_MODE);
        RotateSenseInit();
        break;
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
  tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",getMode);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ProcessSetRfidData (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessSetRfidData->");
  Serial_Debug(jsonString);
  uint32_t startIndex = 0, endIndex = 0,difference = 0, noOfdata = 0;
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *rfidDataStr[JSON_NO_OF_TRACK_INFO_DATA];
 
  uint8_t maxNoStrings=(sizeof rfidDataStr)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, rfidDataStr, ',', maxNoStrings );
  Serial_Debug("\nRFID Data Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rfidDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_TRACK_INFO_DATA)
  {
    if((rfidDataStr[0][0] != NULL) && (rfidDataStr[1][0] != NULL))
    {
      startIndex = strtol(rfidDataStr[0], NULL, 10);
      endIndex = strtol(rfidDataStr[1], NULL, 10);
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
        if(difference) //== noOfdata)
        {
          stRfidData* setRfidData = GetSetRfidData();
          stRfidData tempSetRfidData = *setRfidData;
          bool status = true;
           memset(tempSetRfidData.rfidData,NULL,sizeof(tempSetRfidData.rfidData));
          for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
          {
            status &= SetParameterValue_int(rfidDataStr[i+2], &(tempSetRfidData.rfidData[i]), IsTrackDistanceValid);
         //   status &= SetParameterValue_double(rfidDataStr[i+2], &(tempSetRfidData.rfidData[i]), IsTrackDistanceValid);
          //  SetParameterValue_double
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
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *reqRfidData[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRfidData)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, reqRfidData, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRfidData[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRfidData[0][0] != NULL) && (reqRfidData[1][0] != NULL))
    {
      startIndex = strtol(reqRfidData[0], NULL, 10);
      endIndex = strtol(reqRfidData[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 &&((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
  //  }
  //  else
  //  {
  //    Serial_Debug("\nRequest RFID Data Value/s : Parse Error");
  //    error_code = JSON_PARSE_ERROR;
  //  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RFID_DATA);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    //stMaxIloadConfigValues* getMaxIloadConfigValues = GetSetMaxIloadConfigValues();
    stRfidData* setRfidData = GetSetRfidData();
    for(int i=startIndex;i<=endIndex;i++)
    {
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRfidData->rfidData[i-1]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
    Serial_Debug("\r\nEnd");
  //  UartTransmit(txBuff,dataCount);
    if(GetSwitchState() == MANUAL_CONTROL)
        {
          UartTransmitType(CONFIG_UART,txBuff,dataCount);
        }
        else if(GetSwitchState() == AUTO_CONTROL)
        {
          UartTransmitType(ZIGBEE_UART,txBuff,dataCount);
        }
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
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *rfidDataStr[JSON_NO_OF_TRACK_INFO_DATA];
  uint8_t maxNoStrings=(sizeof rfidDataStr)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, rfidDataStr, ',', maxNoStrings );
  Serial_Debug("\nRFID Data Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rfidDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_TRACK_INFO_DATA)
  {
    if((rfidDataStr[0][0] != NULL) && (rfidDataStr[1][0] != NULL))
    {
      startIndex = strtol(rfidDataStr[0], NULL, 10);
      endIndex = strtol(rfidDataStr[1], NULL, 10);
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
        if(difference) //== noOfdata)
        {
          stRfidData* setRfidData = GetSetRfidData();
          stRfidData tempSetRfidData = *setRfidData;
          bool status = true;
          for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
          {
            status &= SetParameterValue_int(rfidDataStr[i+2], &(tempSetRfidData.rfidData2[i]), IsTrackDistanceValid);
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
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *reqRfidData[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRfidData)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, reqRfidData, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRfidData[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRfidData[0][0] != NULL) && (reqRfidData[1][0] != NULL))
    {
      startIndex = strtol(reqRfidData[0], NULL, 10);
      endIndex = strtol(reqRfidData[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 &&((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
  //  }
  //  else
  //  {
  //    Serial_Debug("\nRequest RFID Data Value/s : Parse Error");
  //    error_code = JSON_PARSE_ERROR;
  //  }
  if( error_code == JSON_NO_ERROR)
  {
    uint16_t dataCount=0;
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RFID_DATA2);
    dataCount = dataCount+bufferLength;    
    bufferLength = sizeof(txBuff)-dataCount;
    //stMaxIloadConfigValues* getMaxIloadConfigValues = GetSetMaxIloadConfigValues();
    stRfidData* setRfidData = GetSetRfidData();
    for(int i=startIndex;i<=endIndex;i++)
    {
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRfidData->rfidData2[i-1]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
    Serial_Debug("\r\nEnd");
   // UartTransmit(txBuff,dataCount);
    if(GetSwitchState() == MANUAL_CONTROL)
        {
          UartTransmitType(CONFIG_UART,txBuff,dataCount);
        }
        else if(GetSwitchState() == AUTO_CONTROL)
        {
          UartTransmitType(ZIGBEE_UART,txBuff,dataCount);
        }
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
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *rfidDataStr[JSON_NO_OF_TRACK_INFO_DATA];
  uint8_t maxNoStrings=(sizeof rfidDataStr)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, rfidDataStr, ',', maxNoStrings );
  Serial_Debug("\nRFID Data Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(rfidDataStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount <= JSON_NO_OF_TRACK_INFO_DATA)
  {
    if((rfidDataStr[0][0] != NULL) && (rfidDataStr[1][0] != NULL))
    {
      startIndex = strtol(rfidDataStr[0], NULL, 10);
      endIndex = strtol(rfidDataStr[1], NULL, 10);
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
        if(difference) //== noOfdata)
        {
          stRfidData* setRfidData = GetSetRfidData();
          stRfidData tempSetRfidData = *setRfidData;
          bool status = true;
          for(int i = (startIndex-1);i<(difference+startIndex-1);i++)
          {
            status &= SetParameterValue_int(rfidDataStr[i+2], &(tempSetRfidData.rfidData3[i]), IsTrackDistanceValid);
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
  //char value[MAX_JASON_VALUE_LENGTH+1];
  //if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
  //{
  char *reqRfidData[JSON_NO_OF_GET_TRACK_INFO];
  uint8_t maxNoStrings=(sizeof reqRfidData)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, reqRfidData, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResulStringCount; dispcount++)
  {
    Serial_Debug(reqRfidData[dispcount]);
    Serial_Debug(",");
  }
  if(totalResulStringCount == JSON_NO_OF_GET_TRACK_INFO)
  {
    if((reqRfidData[0][0] != NULL) && (reqRfidData[1][0] != NULL))
    {
      startIndex = strtol(reqRfidData[0], NULL, 10);
      endIndex = strtol(reqRfidData[1], NULL, 10);
      if((startIndex <= endIndex) && endIndex <= MAX_NO_OF_TRACKS && startIndex > 0 &&((endIndex-startIndex)<=MAX_NO_OF_SET_DATA))
      {
        char dispstring[50];
        snprintf (dispstring, sizeof dispstring, "\nStartIndex = %d, EndIndex = %d", startIndex, endIndex);
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
  //  }
  //  else
  //  {
  //    Serial_Debug("\nRequest RFID Data Value/s : Parse Error");
  //    error_code = JSON_PARSE_ERROR;
  //  }
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
      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRfidData->rfidData3[i-1]);
      dataCount = dataCount+tempCount; 
    }
    dataCount-=1;
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes(txBuff,dataCount);
    Serial_Debug("\r\nEnd");
   // UartTransmit(txBuff,dataCount);
    if(GetSwitchState() == MANUAL_CONTROL)
        {
          UartTransmitType(CONFIG_UART,txBuff,dataCount);
        }
        else if(GetSwitchState() == AUTO_CONTROL)
        {
          UartTransmitType(ZIGBEE_UART,txBuff,dataCount);
        }
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
  uint8_t totalResultStringCount =  SplitString (jsonString, setStateStr, ',',maxNoStrings); 
  Serial_Debug("\nstate Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setStateStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_MODE_STATE_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setStateStr[0], &mode, IsModeValid);
    
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
  tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",getMode);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
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
    uint8_t totalResulStringCount =  SplitString (jsonString, heartbeatConfigStr, ',', maxNoStrings );
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
      status &= SetParameterValue_int(heartbeatConfigStr[0], &(tempSetHeartbeatConfig.interval_ms) , IsHeartBeatIntervalValid);
      status &= SetParameterValue_int(heartbeatConfigStr[1], &(tempSetHeartbeatConfig.maxReconnectTime) , IsMaxReconnectTimeValid);
      
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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,%ld",defaultHeartbeatConfig->interval_ms,defaultHeartbeatConfig->maxReconnectTime);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus ClearActivityLog (const char * jsonString)
{
  Serial_Debug(jsonString);
  ClearAllLogData();
  return JSON_NO_ERROR;
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
  error_code = ExtractData(jsonReceivedData,size,&message);
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
      Error_Handler();
    }
  }
  else
  {
    Error_Handler();
  }
  if((parseErrorCode != JSON_WILDCARD_DEV_ID) && (parseErrorCode != JSON_INVALID_DEV_ID))
  {
    char * AckBuffer = GetAcknowledgementData(error_code);
    Serial_Debug(AckBuffer);
    
    UartTransmit(AckBuffer,strlen(AckBuffer));
  }
  return error_code;
}


static eJsonStatus ProcessJsonCommand (const uint8_t * jsonString, uint16_t presentCommand_Hex) 
{
  eJsonStatus jsonStatus;
  jsonStatus = AssignJsonCommand (presentCommand_Hex); 
  if(presentJsonCommandParams != NULL)
  {
    jsonStatus = presentJsonCommandParams->executeCommand(jsonString);
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
        //Serial_Debug("\nEeprom Stored");
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
    //Serial_Debug("\n Invalid Device ID");
    // error_code = JSON_INVALID_DEV_ID; 
    error_code = JSON_NO_ERROR;
  }
  ////  }
  ////  else
  ////  {
  ////    error_code = JSON_PARSE_ERROR;
  ////  }  
  return error_code; 
}


//void SetDefaultNil(void)
//{
//  
//}

static void Error_Handler(void)
{
  /* Do Nothing */
}

//void ExtractData(uint8_t* sourceString, uint8_t* destString, uint8_t delimiter1, uint8_t delimiter2)
//{
//uint16_t startAddress,endAddress;
//}

eJsonStatus ExtractCommand(uint8_t* sourceBuffer,uint16_t size, uint16_t* command)
{
  uint8_t tempBuffer[MAX_COMMAND_SIZE+1];
  int i,j = 0;
  for(i=0;sourceBuffer[i]!=DELIMITER_1;i++)
  {
    if(i>=size)
    {
      return JSON_INVALID_CMD_VALUE;
    }
  }
  i++;
  for(j=0;(j<MAX_COMMAND_SIZE)&&(i<=size);i++,j++)
  {
    tempBuffer[j] = sourceBuffer[i];  
  }
  if(sourceBuffer[i]!=DELIMITER_2)
  {
    return JSON_INVALID_CMD_VALUE;
  }
  tempBuffer[j] = 0;
  Serial_Debug(tempBuffer);
  *command = strtol(tempBuffer,NULL,16);
  return JSON_NO_ERROR;
}

eJsonStatus ExtractData(uint8_t* data,uint8_t size,stMessage* finalMessage)
{
  uint16_t startAddress = 0;
  uint16_t destAddress = 0;
  //Serial_Debug("\nExtract Data");
  //Serial_Debug("");
  for(startAddress = 0; ((data[startAddress]!=START_CHAR)&&(startAddress<size));startAddress++);
  if(data[startAddress]!=START_CHAR)
  {
    //Serial_Debug("START_CHAR not found");
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
    //Serial_Debug("DELIMITER1 not found");
    return JSON_PARSE_ERROR;
  }
  tempBuffer[destAddress] = 0;
  //Serial_Debug("\nDevice ID-->");Serial_Debug(tempBuffer);
  finalMessage->deviceID = strtoull(tempBuffer,NULL,16);
  startAddress++;
  tempBuffer[0] = 0;
  for(destAddress = 0; (data[startAddress]!=DELIMITER_2)&&(startAddress<size);startAddress++,destAddress++)
  {
    
    tempBuffer[destAddress] = data[startAddress];
  }
  if(data[startAddress]!=DELIMITER_2)
  {
    //Serial_Debug("DELIMITER2 not found");
    return JSON_PARSE_ERROR;
  }
  tempBuffer[destAddress] = 0;
  //Serial_Debug("\nCommand-->");Serial_Debug(tempBuffer);
  finalMessage->CMD = strtol(tempBuffer,NULL,16);
  //Serial_Debug_Num(finalMessage->CMD);
  startAddress++;
  tempBuffer[0] = 0;
  for(destAddress = 0; (data[startAddress]!=STOP_CHAR)&&(startAddress<size);startAddress++,destAddress++)
  {
    
    tempBuffer[destAddress] = data[startAddress];
  }
  if(data[startAddress]!=STOP_CHAR)
  {
    // Serial_Debug("STOP_CHAR not found");
    return JSON_PARSE_ERROR;
  }
  tempBuffer[destAddress] = 0;
  // Serial_Debug("\nValues-->");Serial_Debug(tempBuffer);
  snprintf(finalMessage->values,300,"%s",tempBuffer);
  char* tempValues[MAX_NO_OF_VALUES];
  uint8_t totalNoOfStr = SplitString(tempBuffer,tempValues,',',MAX_NO_OF_VALUES);
  // Serial_Debug("\r\ntotalNoOfStr-->");
  // Serial_Debug_Num(totalNoOfStr);
  if(totalNoOfStr == 0)
  {
    return JSON_INVALID_DATA;
  }
  
  for(int i=0; i<totalNoOfStr;i++)
  {
    //    Serial_Debug("\r\nValue_");
    //    Serial_Debug_Num(i+1);
    //    Serial_Debug("-->");
    //    Serial_Debug(tempValues[i]);
  }
  finalMessage->CRC = strtol(tempValues[totalNoOfStr-1],NULL,16);
  //Serial_Debug("\r\nCRC-->");
  //Serial_Debug_Num(finalMessage->CRC);
  return JSON_NO_ERROR;
}

static bool  SetParameterValue_float(const char * inputString, float * number, CheckValidity_Ptr_float ExecuteCheckValidity)
{
  if(inputString[0] != NULL)
  {
    float convertedNumber = strtof(inputString, NULL);
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
  if(inputString[0] != NULL)
  {
    long int convertedNumber = strtol(inputString, NULL, 10);
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
  if(inputString[0] != NULL)
  {
    bool convertedNumber = strtol(inputString, NULL, 10);
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
//  if(inputString[0] != NULL)
//  {
//    double convertedNumber = strtod(inputString, NULL);
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

void TransmitWelcomeMessage (eUartType uarttType)
{
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_DEVCIE_WELCOME_MSG);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s",DEFAULT_DEVICE_WELCOME_MSG);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  //  Serial_Debug("\r\ndataCount->");
  //  Serial_Debug_Num(dataCount); 
  //  Serial_Debug("\r\nData->");
  //  Serial_Debug_Bytes(txBuff,dataCount);
  //  Serial_Debug("\r\nEnd");
  UartTransmitType(uarttType,(uint8_t*)txBuff, dataCount);
  ClearTxBuff();
}

//07D 
static eJsonStatus ProcessSetWheelDia (const char * jsonString)
{
  Serial_Debug("\r\nIn ProcessSetWheelDia->");
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR; 
  stWheelDia* setWheelDia = GetSetwheelDiaLimits();
  stWheelDia tempsetWheelDia = *setWheelDia;
  char* setWheelDiaValueStr[JSON_NO_OF_WHEELDIA_DATA]; 
  uint8_t maxNoStrings=(sizeof setWheelDiaValueStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, setWheelDiaValueStr, ',',maxNoStrings);
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
    
    status &= SetParameterValue_int(setWheelDiaValueStr[1], &(tempsetWheelDia.Pulse), IsLowBatSoCValid);
    
 //   status &= SetParameterValue_int(setWheelDiaValueStr[1], &(tempsetWheelDia.Pulse), IsPulseCountValid);            //change   
    
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
  char WheelDia1_Str[10];char Pulse_str[10];
  ftoa(getWheelDia->Dia,WheelDia1_Str,0);
 
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%d,%d",WheelDia1_Str,getWheelDia->Pulse,distance);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
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
  uint8_t totalResultStringCount =  SplitString (jsonString, setcyclefrequencyValueStr, ',',maxNoStrings);
  Serial_Debug("\nCycleFrequency Set Received Value :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setcyclefrequencyValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_CYCLEFREQUENCY_DATA)
  {
    bool status = true;
    status &= SetParameterValue_int(setcyclefrequencyValueStr[0], &(tempsetCycleFrequency.Cycle), IscycleFrequencyLimitValid);
    
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
      //Autoschcountval=setCycleFrequency->Cycle;
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
  
 //  stContinue *setContinueValue = GetSetContinueLimits();
//  char Cycle_Str[10]; 
//  ftoa(getCycleFrequency->Cycle,Cycle_Str,0);
  
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",getCycleFrequency->Cycle);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
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
  uint8_t totalResultStringCount =  SplitString (jsonString, setIntervalValueStr, ',',maxNoStrings);
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
      Serial_Debug_Num(setInterval->I1);
      Serial_Debug(",");
      Serial_Debug_Num(setInterval->P1);
      Serial_Debug(",");
      Serial_Debug_Num(setInterval->I2);Serial_Debug_Num(setInterval->P2);
      Serial_Debug_Num(setInterval->I3);Serial_Debug_Num(setInterval->P3);
      Serial_Debug_Num(setInterval->I4);Serial_Debug_Num(setInterval->P4);
      Serial_Debug_Num(setInterval->I5);Serial_Debug_Num(setInterval->P5);
      Serial_Debug_Num(setInterval->I6);Serial_Debug_Num(setInterval->P6);
     // Serial_Debug_Float(setInterval->Interval7,2);Serial_Debug_Float(setInterval->Parameter7,1);
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
//  char I1_Str[10],I2_Str[10],I3_Str[10],I4_Str[10],I5_Str[10],I6_Str[10];
//  char P1_Str[10],P2_Str[10],P3_Str[10],P4_Str[10],P5_Str[10],P6_Str[10];
//  
//  ftoa(getInterval->I1,I1_Str,0); ftoa(getInterval->P1,P1_Str,0);
//  
//  ftoa(getInterval->I2,I2_Str,0); ftoa(getInterval->P2,P2_Str,0);
//  ftoa(getInterval->I3,I3_Str,0); ftoa(getInterval->P3,P3_Str,0);
//  ftoa(getInterval->I4,I4_Str,0); ftoa(getInterval->P4,P4_Str,0);
//  ftoa(getInterval->I5,I5_Str,0); ftoa(getInterval->P5,P5_Str,0);
//  ftoa(getInterval->I6,I6_Str,0); ftoa(getInterval->P6,P6_Str,0);
 // ftoa(getInterval->Interval7,Interval1_Str,1); ftoa(getInterval->Parameter7,Interval2_Str,1);
  
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",getInterval->I1,getInterval->P1,getInterval->I2,getInterval->P2,
  getInterval->I3,getInterval->P3,getInterval->I4,getInterval->P4,getInterval->I5,getInterval->P5,getInterval->I6,getInterval->P6);  
  
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
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
  char state,state1;
  
  uint8_t maxNoStrings=(sizeof setContinueValueStr)/4;  
 
  uint8_t totalResultStringCount =  SplitString (jsonString, setContinueValueStr, ',',maxNoStrings);
  Serial_Debug("\nContinue Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setContinueValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_CONTINUE)
  {
    bool status = true;
    status &= SetParameterValue_int(setContinueValueStr[0], &(tempSetContinue.Continue1), IsContinue1Valid);
    
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
    else if(state == 1)
    {   
      unCleanerMAC *getCleanerMAC = GetSetCleanerMAC();
      
      SendCommandToCleaner(getCleanerMAC->cleanerMAC_64,HOME_RETURN,"1,2");
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
//      ClearRow_DetectCount();
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
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",getContinueValue->Continue1);
  //uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d",setContinueValue->Continue1,setContinueValue->Count1);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
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
 
  uint8_t totalResultStringCount =  SplitString (jsonString, setContinueValueStr, ',',maxNoStrings);
  Serial_Debug("\nContinue Set Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(setContinueValueStr[dispcount]);
    Serial_Debug(",");
  }
  if(totalResultStringCount == JSON_NO_OF_RETURN)
  {
    bool status = true;
    status &= SetParameterValue_int(setContinueValueStr[0], &(tempSetReturn.Return1), IsReturn1Valid);
    
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
    
   //if(state1 == 0)
   //   ClearRoboterror();
    
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
  
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",getReturnValue->Return1);
  
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

static eJsonStatus RequestLog (const char * jsonString)
{
  Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  char *reqLogparaStr[JSON_NO_OF_REQUEST_LOG];
  int Parameter=0;
  uint8_t maxNoStrings=(sizeof reqLogparaStr)/4;
  uint8_t totalResulStringCount =  SplitString (jsonString, reqLogparaStr, ',', maxNoStrings );
  Serial_Debug("\nRequest Log Received Values :");
   char only_once = 0;
   
  memset(txBuff,NULL,strlen(txBuff));
  
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
//      uint8_t tempCount ;
      uint16_t dataCount=0;
      
      uint16_t dataCount1;      //07D
      
      txBuff[dataCount]=0;

      uint16_t bufferLength = sizeof(txBuff)-dataCount;
      bool status = ParseHeaderData(&txBuff[dataCount], &bufferLength, CMD_REQ_LOG);
      dataCount = dataCount+bufferLength; 
      bufferLength = sizeof(txBuff)-dataCount;   
//      GetRTCDateTimeString(&getTime_str, sizeof getTime_str);
//      
      memset(data_read_buffer2,'\0',sizeof(data_read_buffer2));
      memset(data_read_buffer,'\0',sizeof(data_read_buffer));
//      
       if(Parameter == 1 && only_once == 0)
      {       
        GETLog_Temperature();
//        uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s",data_read_buffer);
        error_code = JSON_NO_ERROR;
        only_once = 1;
        snprintf(&txBuff[dataCount],bufferLength,"%s",data_read_buffer2);
      }
      else if(Parameter == 2 && only_once == 0)
      {         
        GETLog_SOC();
//        uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s",data_read_buffer);
        error_code = JSON_NO_ERROR; 
        only_once = 1;
        snprintf(&txBuff[dataCount],bufferLength,"%s",data_read_buffer2);
      }
      else if(Parameter == 3 && only_once == 0)
      { 
        GETLog_Cleaner();
//        uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s",data_read_buffer);
        error_code = JSON_NO_ERROR;  
        only_once = 1;
        snprintf(&txBuff[dataCount],bufferLength,"%s",data_read_buffer2);
      }
      
      else if(Parameter == 4 && only_once == 0)
      {  
        GETLog_AUTOCYCLE();
//        uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s",data_read_buffer);
        error_code = JSON_NO_ERROR;
        only_once = 1;
        snprintf(&txBuff[dataCount],bufferLength,"%s",data_read_buffer2);
      }
      
      else if(Parameter == 5 && only_once == 0)
      {     
        GETLog_ERROR();
//        uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s",data_read_buffer);
        error_code = JSON_NO_ERROR;  
        only_once = 1;
        snprintf(&txBuff[dataCount],bufferLength,"%s",data_read_buffer2);
      }
      
      else if(Parameter == 6)
      {          
           f_unlink("Temperat.txt");
           f_unlink("SOClog.txt");
           f_unlink("CLOG.txt");
           f_unlink("AUTOCYCL.txt");
           f_unlink("ERRORLOG.txt");        
           SDcardMount();    
      }
            
        dataCount1  = strlen(data_read_buffer2);          
        dataCount = dataCount+dataCount1; 
        txBuff[dataCount] = STOP_CHAR;
        dataCount++;
        Serial_Debug("\r\ndataCount->");
        Serial_Debug_Num(dataCount); 
        Serial_Debug("\r\nData->");
        Serial_Debug_Bytes(txBuff,dataCount);
        Serial_Debug("\r\nEnd");       
        if(GetSwitchState() == MANUAL_CONTROL)
        {
//          UartTransmit(txBuff,dataCount);
          UartTransmitType(CONFIG_UART,txBuff,dataCount);     //CONFIG_UART 
        }
        else if(GetSwitchState() == AUTO_CONTROL)
        {
          UartTransmitType(ZIGBEE_UART,txBuff,dataCount);
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

void PulseCalculation()
{
  
//    distance =  (circumference * 1);  
//    stWheelDia* setWheelDia = GetSetwheelDiaLimits();
//    
//    distance = distance / (setWheelDia->Pulse);
    
//    Rotation_per_second = distance / CYCLEROTATIONTIME;
//    
//    if (pulse_flag == 1)
//    {
//      pos = (float) ((float) posdistance /(float) distance);
//      neg = (float) ((float) negdistance /(float) distance);
//      
//      forward_remaining_distance = (posdistance_int % distance);         
//      backward_remaining_distance = (negdistance_int % distance);  
//      
//      ftoa(pos, POS, 0);  
//      ftoa(neg, NEG, 0);
//      
//      Time_seconds_forward = forward_remaining_distance / Rotation_per_second;
//      Time_seconds_backward = backward_remaining_distance / Rotation_per_second;
//      
//      pulse_flag = 0;
//    }
//    
//    else if(pulse_flag == 2)
//    {  
//      Accel_speed_pulse = (float) ((float) Accel_speed /(float) distance);
//      Decel_speed_pulse = (float) ((float) Decel_speed /(float) distance);
//     
//      Accel_remaining_distance = (Accel_speed % distance);
//      Decel_remaining_distance = (Decel_speed % distance);
//
//      ftoa(Accel_speed_pulse,Accel_pulse_count,0); 
//      ftoa(Decel_speed_pulse,Decel_pulse_count,0);
//      
//      Time_seconds_accel = Accel_remaining_distance / Rotation_per_second;
//      Time_seconds_decel = Decel_remaining_distance / Rotation_per_second;
//        
//      pulse_flag = 0;
//    }
}


static eJsonStatus ProcessSetCommDistance (const char * jsonString)                  //SPECIFIC ROW COMMAND
{
  Serial_Debug("\r\nIn ProcessSetPulseCount->");
  Serial_Debug(jsonString); 
  eJsonStatus error_code = JSON_NO_ERROR; 

  strunrfid* setrunrfid = GetSetrunrfidLimits();
  strunrfid tempSetstrunrfid = *setrunrfid;
  
  char *ComdistanceStr[JSON_NO_OF_RUNRFID];
  uint8_t maxNoStrings=(sizeof ComdistanceStr)/4;
  uint8_t totalResultStringCount =  SplitString (jsonString, ComdistanceStr, ',',maxNoStrings);
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
    status &= SetParameterValue_int(ComdistanceStr[0], &(tempSetstrunrfid.runrfid), IsRTCvalueValid);
  
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
       // Serial_Debug_Num(setCountValues->maxNegCountForPVsetup);
    }  
  } 
  else
  {
    Serial_Debug("\n Pulse Count SetValue/s Invalid");
    error_code = JSON_INVALID_DATA;
  }
  return error_code;
}

static eJsonStatus ProcessGetCommDistance (const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  ParseHeaderData(&txBuff[dataCount],&bufferLength,CMD_GET_RUNRFID);
  dataCount = dataCount+bufferLength;    
  bufferLength = sizeof(txBuff)-dataCount;
  
   strunrfid* getrunrfid = GetSetrunrfidLimits();
//  strunrfid tempSetstrunrfid = *setrunrfid;
 // stcomdistance* getcomdistance = GetSetComDistanceLimits();
  //stContinue *setContinueValue = GetSetContinueLimits();
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",getrunrfid->runrfid);
  
  //uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d",setContinueValue->Continue1,setContinueValue->Count1);
  dataCount = dataCount+tempCount; 
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
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
  uint8_t totalResultStringCount =  SplitString (jsonString, setNoofrowsValueStr, ',',maxNoStrings);
  Serial_Debug("\nLogInterval Set Received Value :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    
    Serial_Debug(setNoofrowsValueStr[dispcount]);
    Serial_Debug(",");
    
    //sum = (sum + setNoofrowsValueStr[dispcount]);
  }
  if(totalResultStringCount == JSON_NO_OF_ROWS-1)
  {
    bool status = true;
    tempsetNoofrows.TRow = 0;
    
      
    for(int i=0;i<7;i++)
    {
      status &= SetParameterValue_int(setNoofrowsValueStr[i], &(tempsetNoofrows.Row[i]), IsparameterLimitValid);     //Changed for zero
     
    
//      status &= SetParameterValue_int(setNoofrowsValueStr[0], &(tempsetNoofrows.R1), IsparameterLimitValid);
//      status &= SetParameterValue_int(setNoofrowsValueStr[1], &(tempsetNoofrows.R2), IsparameterLimitValid);
//      status &= SetParameterValue_int(setNoofrowsValueStr[2], &(tempsetNoofrows.R3), IsparameterLimitValid);     
//      status &= SetParameterValue_int(setNoofrowsValueStr[3], &(tempsetNoofrows.R4), IsparameterLimitValid);  
//      status &= SetParameterValue_int(setNoofrowsValueStr[4], &(tempsetNoofrows.R5), IsparameterLimitValid);    
//      status &= SetParameterValue_int(setNoofrowsValueStr[5], &(tempsetNoofrows.R6), IsparameterLimitValid);
//      status &= SetParameterValue_int(setNoofrowsValueStr[6], &(tempsetNoofrows.R7), IsparameterLimitValid);
      
//      tempsetNoofrows.TRow = tempsetNoofrows.R1 + tempsetNoofrows.R2 + tempsetNoofrows.R3 +tempsetNoofrows.R4 + tempsetNoofrows.R5 + tempsetNoofrows.R6 + tempsetNoofrows.R7;
      
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
//        ClearRow_DetectCount();
        rowsActuallyCleaned = trackchangercount;
        EepromSetAutoScheduleTime(EEPROM_WRITE);
        error_code = JSON_NO_ERROR;
        Serial_Debug("\n LogInterval SetValue/s set : "); 
        *setNoofrows = tempsetNoofrows;
        for(int i=0;i<7;i++)
        {
          Serial_Debug_Num(setNoofrows->Row[i]);
        }
//        Serial_Debug_Num(setNoofrows->R1);
//        Serial_Debug(",");
//        Serial_Debug_Num(setNoofrows->R2);
//        Serial_Debug(",");
//        Serial_Debug_Num(setNoofrows->R3);Serial_Debug_Num(setNoofrows->R4);
//        Serial_Debug_Num(setNoofrows->R5);Serial_Debug_Num(setNoofrows->R6);
//        Serial_Debug_Num(setNoofrows->R7);
      
      }
      else 
      {
        error_code = JSON_INVALID_DATA;
        sum = 0;
        
      }
//      Serial_Debug_Num(setNoofrows->P4);
//      Serial_Debug_Num(setNoofrows->I5);Serial_Debug_Num(setNoofrows->P5);
//      Serial_Debug_Num(setNoofrows->I6);Serial_Debug_Num(setNoofrows->P6);
//      Serial_Debug_Float(setInterval->Interval7,2);Serial_Debug_Float(setInterval->Parameter7,1);
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
   
//  stnoofrows tempsetNoofrows = *setNoofrows;
//  stInterval* getInterval = GetSetIntervalLimits(); 
   
//    for(int i=0;i<7;i++)
//    {
//     // uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%ld,",setRfidData->rfidData[i-1]);
//      uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,",getNoofrows->Row[i]);
//      dataCount = dataCount+tempCount; 
//    }
    
  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%d,%d,%d",getNoofrows->Row[0],getNoofrows->Row[1],getNoofrows->Row[2],getNoofrows->Row[3],
  getNoofrows->Row[4],getNoofrows->Row[5],getNoofrows->Row[6],getNoofrows->TRow);
                               
//  uint8_t tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d,%d,%d,%d,%d,%d,%d",getNoofrows->R1,getNoofrows->R2,getNoofrows->R3,getNoofrows->R4,
//  getNoofrows->R5,getNoofrows->R6,getNoofrows->R7);  
  
  dataCount = dataCount+tempCount; 
                               
  txBuff[dataCount] = STOP_CHAR;
  dataCount++;
  Serial_Debug("\r\ndataCount->");
  Serial_Debug_Num(dataCount); 
  Serial_Debug("\r\nData->");
  Serial_Debug_Bytes(txBuff,dataCount);
  Serial_Debug("\r\nEnd");
  UartTransmit(txBuff,dataCount);
  ClearTxBuff();
  return error_code;
}

char Get_SD_Card_Temps()
{
  float Battemp_f,Drivetemp_f,Dietemp_f;
  Battemp_f = GetTemperatureSensorData(TEMPSENSOR_1);
  ftoa(Battemp_f,Battemp_str,2);
  Drivetemp_f = GetTemperatureSensorData(TEMPSENSOR_2);
  ftoa(Drivetemp_f,Drivetemp_str,2);
  Dietemp_f = GetDieTemperature();
  ftoa(Dietemp_f,Dietemp_str,2);
}

char Get_SD_Card_SOC()
{

  stBatCurrent BatteryCurrent;float Vin_f,Vbat_f,Iin_f,Ibat_f,Dtemp_f;
 
  Vin_f = GetInputVoltage();
  ftoa(Vin_f,Vin_str,1);
  Vbat_f = GetBatteryVoltage();
  ftoa(Vbat_f,Vbat_str,1);
  Iin_f = GetInputCurrent();
  ftoa(Iin_f,Iin_str,1);
  Dtemp_f = GetDieTemperature();
  ftoa(Dtemp_f,Dtemp_str,1);
  GetBatteryCurrent(&BatteryCurrent);
  Ibat_f = BatteryCurrent.current;
  if(!BatteryCurrent.direction)
    Ibat_f = 0-Ibat_f;
  ftoa(Ibat_f,Ibat_str,1);

  ftoa(GetSOCAsPercentage(),Soc_Str,2);
  
  
}

char Get_SD_Card_MOTORCRNT(){
  
  ftoa(GetImot1Value(),Imot1_Str,2);
  ftoa(GetImot2Value(),Imot2_Str,2);
  ftoa(GetImot3Value(),Imot3_Str,2); 
}

void SD_SOC_Start(){
         char getTime_str1[40];
         eRobotDirection robotState = GetRobotActualDirection();
        memset(schedule_log,'\0',sizeof(schedule_log));
        
        snprintf(schedule_log,3,"%d,",robotState);
        GetRTCDateTimeString(&getTime_str1, sizeof getTime_str1);
        
        strcat(schedule_log,getTime_str1);
        strcat(schedule_log,",");
        
        float startSOC = GetSOCAsPercentage();
        char startSoc_Str[10];
        ftoa(startSOC,startSoc_Str,2);
        strcat(schedule_log,startSoc_Str);
        strcat(schedule_log,",");
}

void SD_SOC_End(){
         char getTime_str1[40],Error_Cnt[2];
         memset(Error_Cnt,'\0',2);
         Error_Cnt[0] =  (Robot_Error_Count1) + 0x30; 
        Robot_Error_Count1 = 0; 
        GetRTCDateTimeString(&getTime_str1, sizeof getTime_str1);
        strcat(schedule_log,getTime_str1);
        strcat(schedule_log,",");
       // strcat(cyclelog,getTime_str1);
        //strcat(cyclelog,",");
        float endSOC = GetSOCAsPercentage();
        char startSoc_Str[10];
        ftoa(endSOC,startSoc_Str,2);
        strcat(schedule_log,startSoc_Str);
        strcat(schedule_log,",");
        strcat(schedule_log,Error_Cnt);

}

void tcstarttime()
{
  char getTime_str1[40];
  memset(cyclelog,'\0',sizeof(cyclelog));
  GetRTCDateTimeString(&getTime_str1, sizeof getTime_str1);
  strcat(cyclelog,getTime_str1);
  strcat(cyclelog,",");
  strcpy(Cstarttime,getTime_str1);
  
  float startSOC = Batt_SOC;
  char startSoc_Str[10];
  ftoa(startSOC,startSoc_Str,2);
  strcpy(Cstartsoc,startSoc_Str);
}
void tcendtime()
{
  char getTime_str1[40],Error_Cnt[2];
//  memset(Error_Cnt,'\0',2);
//  Error_Cnt[0] =  (Robot_Error_Count1) + 0x30; 
//  Robot_Error_Count1 = 0;
  GetRTCDateTimeString(&getTime_str1, sizeof getTime_str1);
  strcat(cyclelog,getTime_str1);
//  strcat(cyclelog,",");
//  memset(errorcountlog,'\0',sizeof(errorcountlog));
//  strcat(errorcountlog,Error_Cnt);
  
   strcpy(Cendtime,getTime_str1);
   float endSOC = Batt_SOC;
   char startSoc_Str[10];
   ftoa(endSOC,startSoc_Str,2);
   strcpy(Cendsoc,startSoc_Str);
//   strcpy(Cerrorcount,Error_Cnt);
}



//void Bmscalculation(void)
//{
////    size_pkt = strlen(pkt);
////    start = strstr(pkt,":");
////    start+=1;
////    start = strstr(pkt,"11");
//    
//    size_pkt = strlen(rxDataBuffer);
//    startbms = strstr(rxDataBuffer,":");
//    startbms+=1;
//    startbms = strstr(rxDataBuffer,"11");
//    
//    strncpy(slave_arr,startbms,2);
//    slave_address_cmp = strcmp(slave_arr,"11");
//    startbms+=2;
//    strncpy(fun_arr,startbms,2);
//    function_cmp = strcmp(fun_arr,"03");
//    startbms+=2;
//    strncpy(byte_arr,startbms,2);
//    byte_int =  hex2dec(byte_arr,2);
//    byte_int = byte_int/2;
//    startbms+=2;
//    // strncpy(temp_arr,start,4);
//    // end = strstr(pkt,"/n"); 
//    // strncpy(temp_arr,start,(end-start));
//    // printf("%s\n",temp_arr);
//    if( slave_address_cmp==0 && function_cmp==0)
//    {
//      switch(byte_int)
//      { 
//        case 16:
//          for(int i=0;i<byte_int;i++)
//          {
//            strncpy(temp_arr1[i],startbms,4);
//            startbms+=4;
//            //  printf("%s\n",temp_arr1[i]);
//            bmsvalue =  hex2dec(temp_arr1[i],2); 
//            switch(i)
//            {
//              case 0:
//                Vbat = bmsvalue*0.01;
//                //    printf("Vbat is %.2f\n",Vbat);
//                ftoa(Vbat,bat_arr, 2);
//                //    printf("Vbat is %s\n",uart_arr[i]);
//                //  snprintf(uart_arr[i], sizeof(Vbat), "%f",Vbat);
//                //  strcpy(temp[i],uart_arr[i]);
//                break;
//              case 1:
//                VPV = bmsvalue*0.01;
//                //   printf("VPV is %.2f\n",VPV);
//                ftoa(VPV, pv_arr,2);
//                //    printf("Vbat is %s\n",uart_arr[i]);
//                //  snprintf(uart_arr[i], sizeof(VPV), "%f",VPV);
//                break;
//              case 2:
//                IMPPT = bmsvalue*0.01;
//                //     printf("IMPPT is %.2f\n",IMPPT);
//                ftoa(IMPPT, mpptcurr_arr, 2);
//                //     printf("Vbat is %s\n",uart_arr[i]);
//                //  snprintf(uart_arr[i], sizeof(IMPPT), "%f",IMPPT);
//                break;
//              case 3:
//                PWR = bmsvalue*0.1;
//                //    printf("PWR is %.2f\n",PWR);
//                ftoa(PWR, mpptout_arr, 2);
//                //    printf("Vbat is %s\n",uart_arr[i]);
//                //  snprintf(uart_arr[i], sizeof(PWR), "%f",PWR);
//                break;
//              case 4:
//                VN_bat1 = bmsvalue*0.01;
//                //    printf("VN_bat1is %.2f\n",VN_bat1);
//                ftoa(VN_bat1, uart_arr[i], 2);
//                //    printf("Vbat is %s\n",uart_arr[i]);
//                //  snprintf(uart_arr[i], sizeof(VN_bat1), "%f",VN_bat1);
//                break;
//              case 5:
//                VL5V = bmsvalue*0.01;
//                //     printf("VL5V is %.2f\n",VL5V);
//                ftoa(VL5V, uart_arr[i], 2);
//                //    printf("Vbat is %s\n",uart_arr[i]);
//                //  snprintf(uart_arr[i], sizeof(VL5V), "%f",VL5V);
//                break;
//              case 6:
//                VLOAD = bmsvalue*0.01;
//                //    printf("VLOAD is %.2f\n",VLOAD);
//                ftoa(VLOAD, uart_arr[i], 2);
//                //    printf("Vbat is %s\n",uart_arr[i]);
//                //  snprintf(uart_arr[i], sizeof(VLOAD), "%f",VLOAD);
//                break;
//              case 7:
//                VN_bat2 = bmsvalue*0.01;
//                //    printf("VN_bat2 is %.2f\n",VN_bat2);
//                ftoa(VN_bat2, uart_arr[i], 2);
//                //    printf("Vbat is %s\n",uart_arr[i]);
//                //  snprintf(uart_arr[i], sizeof(VN_bat2), "%f",VN_bat2);
//                break;
//              case 8:
//                HS_TEMP = bmsvalue*1;
//                //     printf("HS_TEMP is %.2f\n",HS_TEMP);
//                ftoa(HS_TEMP, inttemp_arr, 2);
//                //     printf("Vbat is %s\n",uart_arr[i]);
//                //  snprintf(uart_arr[i], sizeof(HS_TEMP), "%f",HS_TEMP);
//                break;
//              case 9:
//                EXT_TEMP = bmsvalue*1;
//                //     printf("EXT_TEMP is %.2f\n",EXT_TEMP);
//                ftoa(EXT_TEMP, exttemp_arr, 2);
//                //     printf("Vbat is %s\n",uart_arr[i]);
//                // snprintf(uart_arr[i], sizeof(EXT_TEMP), "%f",EXT_TEMP);
//                break;
//              case 10:
//                VR_bat = bmsvalue*0.01;
//                //     printf("VR_bat is %.2f\n",VR_bat);
//                ftoa(VR_bat, uart_arr[i], 4);
//                //    printf("Vbat is %s\n",uart_arr[i]);
//                // snprintf(uart_arr[i], sizeof(VR_bat), "%f",VR_bat);
//                break;
//              case 11:
//                BAT_STATE  = bmsvalue*1;
//                //      printf("BAT_STATE is %d\n",BAT_STATE);
//                ftoa(BAT_STATE,batstate_arr, 0);
//                //snprintf(batstate_arr, sizeof(BAT_STATE), "%d",BAT_STATE);
//                //     printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 12:
//                BAT_STATE_CNT = bmsvalue*1;
//                //    printf("BAT_STATE_CNT is %d\n",BAT_STATE_CNT);
//                snprintf(uart_arr[i], sizeof(BAT_STATE_CNT), "%d",BAT_STATE_CNT);
//                //     printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 13:
//                BAT_SET = bmsvalue*0.01;
//                //      printf("BAT_SET is %.2f\n",BAT_SET);
//
//                ftoa(BAT_SET, uart_arr[i], 2);
//                //       printf("Vbat is %s\n",uart_arr[i]);
//                // snprintf(uart_arr[i], sizeof(BAT_SET), "%f",BAT_SET);
//                break;
//              case 14:
//                BAT_COMP = bmsvalue*0.01;
//                //       printf("BAT_COMP is %.2f\n",BAT_COMP);
//                ftoa(BAT_COMP, uart_arr[i], 2);
//                //       printf("Vbat is %s\n",uart_arr[i]);
//                //  snprintf(uart_arr[i], sizeof(BAT_COMP), "%f",BAT_COMP);
//                break;
//              case 15:
//                LOAD_STATE = bmsvalue*1;
//                //      printf("LOAD_STEATE is %d\n",LOAD_STATE);
//               // snprintf(loadout_arr, sizeof(LOAD_STATE), "%d",LOAD_STATE);
//                 ftoa(LOAD_STATE,loadout_arr, 0);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
////              case 16:
////                LOAD_TRIPCNT = bmsvalue*1;
////                //      printf("LOAD_STEATE is %d\n",LOAD_STATE);
////                snprintf(loadtrip_arr, sizeof(LOAD_TRIPCNT), "%d",LOAD_TRIPCNT);
////                //        printf("Vbat is %s\n",uart_arr[i]);
////                break;
//              default:
//                printf("packet error");
//                break;
//          }
//        }
//
//        break;
//
//        case 6:
//          for(int i=0;i<byte_int;i++)
//          {
//
//            strncpy(temp_arr1[i],startbms,4);
//            startbms+=4;
//            //    printf("%s\n",temp_arr1[i]);
//            bmsvalue =  hex2dec(temp_arr1[i],2); 
//
//            switch(i)
//            {
//              case 0:
//                ENERGY_LO = bmsvalue*0.01;
//                //        printf("ENERGY_LO is %.2f\n",ENERGY_LO);
//                ftoa(ENERGY_LO, uart_arr[i], 2);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 1:
//                ENERGY_HI = bmsvalue*1;
//                //       printf("ENERGY_HI is %d\n",ENERGY_HI);
//                snprintf(uart_arr[i], sizeof(ENERGY_HI), "%d",ENERGY_HI);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 2:
//                MODBUS_ADR = bmsvalue*1;
//                //       printf("MODBUS_ADR is %d\n",MODBUS_ADR);
//                snprintf(uart_arr[i], sizeof(MODBUS_ADR), "%d",MODBUS_ADR);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 3:
//                VMPPT_APP_SET = bmsvalue*0.01;
//                //       printf("VMPPT_APP_SET is %.2f\n",VMPPT_APP_SET);
//                ftoa(VMPPT_APP_SET, uart_arr[i], 2);
//                //         printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 4:
//                REG_5V_EN = bmsvalue*1;
//                //       printf("REG_5V_EN is %d\n",REG_5V_EN);
//                snprintf(uart_arr[i], sizeof(REG_5V_EN), "%d",REG_5V_EN);
//                //       printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 5:
//                REG_5V_TRIP = bmsvalue*1;
//                //       printf("REG_5V_TRIP is %d\n",REG_5V_TRIP);
//                snprintf(uart_arr[i], sizeof(REG_5V_TRIP), "%d",REG_5V_TRIP);
//                //       printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              default:
//                printf("packet error");
//                break;
//            }
//          }
//          break;
//        case 14:
//          for(int i=0;i<byte_int;i++)
//          {
//
//            strncpy(temp_arr1[i],startbms,4);
//            startbms+=4;
//            //     printf("%s\n",temp_arr1[i]);
//            bmsvalue =  hex2dec(temp_arr1[i],2); 
//
//            switch(i)
//            {
//              case 0:
//                REG_SET_PV = bmsvalue*0.01;
//                //        printf("REG_SET_PV is %.2f\n",REG_SET_PV);
//                ftoa(REG_SET_PV, uart_arr[i], 2);
//                //         printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 1:
//                REG_SET_VBAT = bmsvalue*0.01;
//                //        printf("REG_SET_VBAT is %.2f\n",REG_SET_VBAT);
//                ftoa(REG_SET_VBAT, uart_arr[i], 2);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 2:
//                REG_SET_IMPPT = bmsvalue*0.01;
//                //       printf("REG_SET_IMPPT is %.2f\n",REG_SET_IMPPT);
//                ftoa(REG_SET_IMPPT, uart_arr[i], 2);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 3:
//                REG_PWM = bmsvalue*1;
//                //        printf("REG_PWM is %d\n",REG_PWM);
//                snprintf(uart_arr[i], sizeof(REG_PWM), "%d",REG_PWM);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 4:
//                MPPT_STATE = bmsvalue*1;
//                //       printf("MPPT_STATE is %d\n",MPPT_STATE);
//                snprintf(uart_arr[i], sizeof(MPPT_STATE), "%d",MPPT_STATE);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 5:
//                BEST_PWR = bmsvalue*0.1;
//                //       printf("BEST_PWR is %.2f\n",BEST_PWR);
//                ftoa(BEST_PWR, uart_arr[i], 2);
//                //       printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 6:
//                BEST_PV = bmsvalue*0.01;
//                //        printf("BEST_PV is %.2f\n",BEST_PV);
//                ftoa(BEST_PV, uart_arr[i], 2);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 7:
//                MPPT_CNT = bmsvalue*1;
//                //        printf("MPPT_CNT is %d\n",MPPT_CNT);
//                snprintf(uart_arr[i], sizeof(MPPT_CNT), "%d",MPPT_CNT);
//                //         printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 8:
//                REG_DOMINATE = bmsvalue*1;
//                //        printf("REG_DOMINATE is %d\n",REG_DOMINATE);
//                snprintf(uart_arr[i], sizeof(REG_DOMINATE), "%d",REG_DOMINATE);
//                //         printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 9:
//                PV_EN = bmsvalue*1;
//                //        printf("PV_EN is %d\n",PV_EN);
//                snprintf(uart_arr[i], sizeof(PV_EN), "%d",PV_EN);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 10:
//                IMPPT_APP_SET = bmsvalue*0.01;
//                //        printf("IMPPT_APP_SET is %.2f\n",IMPPT_APP_SET);
//                ftoa(IMPPT_APP_SET, uart_arr[i], 2);
//                //       printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 11:
//                LOAD_EN  = bmsvalue*1;
//                //        printf("LOAD_EN is %d\n",LOAD_EN);
//                snprintf(uart_arr[i], sizeof(LOAD_EN), "%d",LOAD_EN);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 12:
//                LOAD_COUNT = bmsvalue*1;
//                //       printf("LOAD_COUNT is %d\n",LOAD_COUNT);
//                snprintf(uart_arr[i], sizeof(LOAD_COUNT), "%d",LOAD_COUNT);
//                //       printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              case 13:
//                LOAD_TRIPCNT = bmsvalue*1;
//                //      printf("LOAD_STEATE is %d\n",LOAD_STATE);
//                snprintf(loadtrip_arr, sizeof(LOAD_TRIPCNT), "%d",LOAD_TRIPCNT);
//                //        printf("Vbat is %s\n",uart_arr[i]);
//                break;
//              default:
//                printf("packet error");
//                break;
//            }
//
//          }
//        break;
//        default:
//        //         printf("byte count error");
//        break;
//
//      }
//    }
//}

