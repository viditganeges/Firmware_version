#ifndef _APIPROCESS_H    /* Guard against multiple inclusion */
#define _APIPROCESS_H

#include "main.h"

#define MAC_ID_STR_LEN  16+1

#define MAX_UART_TX_BUFFER_LENGTH             10000//   1500
#define MAX_UART_RX_BUFFER_LENGTH               500
#define MAX_ACK_TX_BUFFER_LENGTH                200

#define MAX_NO_OF_SET_DATA                      20
#define JSON_NO_OF_MODE_STATE_DATA              1+1 //+1 is CRC 
#define JSON_NO_OF_DIR_STATE_DATA               2+1 
//#define JSON_NO_OF_MOTOR_MODE_STATE_DATA        3+1             
//#define JSON_NO_OF_SENSOR_STATE_DATA            2+1             
#define JSON_NO_OF_PWM_DATA_MOTOR               7+1 //12+1       
#define JSON_NO_OF_PWM_DATA_BRUSH               3+1 //4+1 
#define JSON_NO_OF_RTC_DATA                     6+1
#define JSON_NO_OF_SET_DEVICE_INFO              3+1
#define JSON_NO_OF_GET_DEVICE_INFO              4+1
#define JSON_NO_OF_CURRENT_LIMIT                3+1
#define JSON_NO_OF_ERROR_CODE_DATA              1+1

#define JSON_NO_OF_PULSE_COUNT_DATA             2+1 //

#define JSON_NO_OF_SOC_DATA                     1+1
//#define JSON_NO_OF_ZIGBEE_CONFIG_DATA           6+1
#define JSON_NO_OF_ZIGBEE_NW_RESET_DATA         1+1
#define JSON_NO_OF_MOTOR_FAULT_CONDITIONS       4+1
#define JSON_NO_OF_REQUEST_LOG_VALUES           2+1
//#define JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS  4+1


#define JSON_NO_OF_INTERVAL_DATA                     12+1    
#define JSON_NO_OF_CYCLEFREQUENCY_DATA               1+1    
#define JSON_NO_OF_CONTINUE                          1+1
#define JSON_NO_OF_RETURN                            1+1
#define JSON_NO_OF_REQUEST_LOG                       1+1
//#define JSON_NO_OF_COMDISTANCE                       1+1 
//#define JSON_NO_AUTOSCH                              19+1
//#define JSON_NO_OF_PULSE_COUNT_DATA                  6+1         
//#define JSON_NO_OF_PULSE_CYCLE_COUNT_DATA            4+1         
#define JSON_NO_OF_WHEELDIA_DATA                     3+1      
#define JSON_NO_OF_AUTO_SCHED_DATA                   5+1            //2+1
#define JSON_NO_OF_SETGET_PWM_DATA_MOTOR (JSON_NO_OF_PWM_DATA_MOTOR -2)        /* We are using single steady and approach pwm instaed of two */
#define JSON_NO_OF_ROW_LENGTH_DATA              ((MAX_NO_OF_SET_DATA*2)+2+1)
#define JSON_NO_OF_ROWS                              8+1   
#define JSON_NO_OF_GET_TRACK_INFO                    2+1
#define JSON_NO_OF_SET_TRACK_INFO                    20+2+1

#define JSON_NO_OF_RELAY_DATA                     1+1 
#define JSON_NO_OF_MOTOR_MODE_STATE_DATA        2+1
#define JSON_NO_OF_ROWS                     8+1    //NEW

//FOR flash usage
#define JSON_NO_OF_PWM_DATA_MOTOR_FLASH_WRITE         24+1   
#define JSON_NO_OF_PWM_DATA_BRUSH_FLASH               10+1
#define JSON_NO_OF_PULSE_COUNT_FLASH                  12+1
//#define JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS_FLASH  13+1
#define JSON_NO_OF_MOTOR_FAULT_CONDITIONS_FLASH       16+1
#define JSON_NO_OF_COMDISTANCE_FLASH                   4+1 
#define JSON_NO_OF_ROWLENGTHCOUNT_FLASH               40+1
#define JSON_NO_OF_RUNRFID                           1+1
#define JSON_NO_OF_RFIDDATA_FLASH               20+1
#define JSON_NO_OF_ENC_PULSE_TOLERANCE_DATA     1+1
//#define JSON_NO_OF_CLEANER_MAC_DATA             4+1
#define JSON_NO_OF_ROWLEN_FLASH                72+1
#define JSON_NO_OF_TRACKINFO_FLASH             40+1
#define JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS_FLASH  8+1
#define JSON_NO_OF_ENC_PULSE_TOLERANCE_DATA_FLASH     4+1

#define JSON_NO_OF_PWM_DATA_MOTOR_flash               25+1 
#define JSON_NO_OF_SETGET_PWM_DATA_MOTOR_tc           5+1 
#define JSON_NO_OF_TRACKINFO_FLASH_tc            120+1
#define JSON_NO_OF_WHEELDIA_DATA                     3+1    //NEW
#define JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS  2+1
#define JSON_NO_OF_RFIDDATA_FLASH_tc               40+1
#define JSON_NO_OF_CLEANER_MAC_DATA             1+1
#define JSON_NO_OF_CLEANER_MAC_DATA_flash             8+1

//API commands
#define CMD_SET_MODE                          0x0001                                                       
#define CMD_SET_MOTOR_DIR_MANUAL              0x0003
#define CMD_SET_MOTOR_PWM_MANUAL              0x0004
#define CMD_SET_MOTOR_PWM_AUTO                0x0009
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
#define CMD_SET_TRACK_COUNT                   0x0018             //not use
#define CMD_CLEAR_TRACK_INFO                  0x001A
#define CMD_SET_CLEANER_MAC                   0x001B  
#define CMD_SET_ENC_PULSE_TOLERANCE           0x001D
#define CMD_SET_TC_CONTROL_MODE               0x001E
#define CMD_SET_RFID_STATE                    0x0020
#define CMD_SET_HEARTBEAT_CONFIG              0x0021
#define CMD_GET_MODE                          0x1001
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
#define CMD_GET_CHARGER_STATE                 0x1017
#define CMD_GET_CONTROLLER_STATUS             0x1018
#define CMD_GET_DEVICE_INFO                   0x1019
#define CMD_GET_ZIGBEE_CONF                   0x101A
#define CMD_GET_ZIGBEE_NTW_PARAM              0x101B
#define CMD_GET_BAT_AND_PV_VAL                0x101C     
#define CMD_GET_TEMPERATURES                  0x101D
#define CMD_GET_MOTOR_FAULT_CONDITIONS        0x101E
#define CMD_REQ_ACTIVITY_LOG                  0x101F 
#define CMD_GET_CLEANER_MAC                   0x1022
#define CMD_GET_SENSOR_STATUS                 0x1025
#define CMD_GET_ENC_PULSE_TOLERANCE           0x1026
#define CMD_GET_TC_CONTROL_MODE               0x1027
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
#define CMD_SET_CONTINUE_COUNT                0x0033
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

#define CMD_SET_RELAY_INTERVAL                  0x0052
#define CMD_GET_RELAY_INTERVAL                  0x1052

#define CMD_AUTO_GET_VALUES                   0x3333  //new
#define CMD_SOC_LOG                      0x301C  
#define CMD_ERROR_LOG                    0x3018
#define CMD_CLEANER_LOG                  0x300E
#define CMD_AUTO_CYCLE_LOG               0x300F

#define MAX_NO_OF_VALUES 30
#define MAX_COMMAND_SIZE 4
#define START_CHAR      0x01    //0x7B 
#define STOP_CHAR       0x04    //0x7D 
#define DELIMITER_1     0x02    //0x2C  
#define DELIMITER_2     0x03
#define DEVICE_ID               255
#define WILDCARD_DEVICE_ID      0xFFFF

#define STARTID 0x90
#define ENDID  0x2C

typedef enum eJsonCommand_def                                                             
{
  CMD_NUM_SET_MODE,                                                      
  CMD_NUM_SET_MOTOR_DIR_MANUAL,
  CMD_NUM_SET_MOTOR_PWM_MANUAL,
  CMD_NUM_SET_BRUSH_MOTOR_STATE,
  CMD_NUM_SET_BRUSH_MOTOR_PWM_MANUAL,
  CMD_NUM_SET_BRUSH_MOTOR_DIR,
  CMD_NUM_SET_AUTO_MODE_STATE,
  CMD_NUM_SET_MOTOR_PWM_AUTO,
  CMD_NUM_SET_BRUSH_MOTOR_PWM_AUTO,
  CMD_NUM_SET_AUTO_SCHEDULE_TIME,
  CMD_NUM_SET_EDGE_SENSE_STATE,
  CMD_NUM_SET_RTC_VALUE,
  CMD_NUM_SET_DEVICE_INFO,
  CMD_NUM_SET_OVERCURRENT_LIMIT,
  CMD_NUM_CLEAR_ERRORCODES,
  CMD_NUM_GET_SOC,
  CMD_NUM_GET_CHARGER_STATE,
  CMD_NUM_GET_CONTROLLER_STATUS,
  CMD_NUM_GET_DEVICE_INFO,
  CMD_NUM_GET_MODE,
  CMD_NUM_GET_MOTOR_DIR_MANUAL,
  CMD_NUM_GET_MOTOR_PWM_MANUAL,
  CMD_NUM_GET_BRUSH_MOTOR_STATE,
  CMD_NUM_GET_BRUSH_MOTOR_PWM_MANUAL,
  CMD_NUM_GET_BRUSH_MOTOR_DIR,
  CMD_NUM_GET_MOTOR_PWM_AUTO,
  CMD_NUM_GET_BRUSH_MOTOR_PWM_AUTO,
  CMD_NUM_GET_AUTO_SCHEDULE_TIME,
  CMD_NUM_GET_EDGE_SENSE_STATE,
  CMD_NUM_GET_RTC_VALUE,
  CMD_NUM_GET_CURRENT_STAT,
  CMD_NUM_GET_LAST_CYCLE_STAT,
  CMD_NUM_GET_OVERCURRENT_LIMIT,
  CMD_NUM_GET_MOT_CURRENT_VALUES, 
  CMD_NUM_SET_PULSE_COUNT_VALUES,
  CMD_NUM_SET_LOW_BAT_SOC,
  CMD_NUM_GET_PULSE_COUNT_VALUES,
  CMD_NUM_GET_LOW_BAT_SOC,
  CMD_NUM_SET_ZIGBEE_CONF,
  CMD_NUM_GET_ZIGBEE_CONF,
  CMD_NUM_GET_ZIGBEE_NTW_PARAM,
  CMD_NUM_RESET_ZIGBEE_NTW,
  CMD_NUM_GET_BAT_AND_PV_VAL,
  CMD_NUM_RESET_TO_DEFAULTS,
  CMD_NUM_GET_TEMPERATURES,
  CMD_NUM_SET_MOTOR_FAULT_CONDITION,
  CMD_NUM_GET_MOTOR_FAULT_CONDITION,
  CMD_NUM_REQ_ACTIVITY_LOG,
  CMD_NUM_CLEAR_ACTIVITY_LOG,
  //CMD_NUM_SET_TRACK_COUNT,
  CMD_NUM_SET_TRACK_INFO,
  CMD_NUM_GET_TRACK_INFO,
  CMD_NUM_CLEAR_TRACK_INFO,
  CMD_NUM_SET_CLEANER_MAC,
  CMD_NUM_SET_ROW_LENGTH_DATA,
  CMD_NUM_GET_CLEANER_MAC,
  CMD_NUM_GET_ROW_LENGTH_DATA,
  CMD_NUM_GET_TRACK_CHANGER_STATUS,
  CMD_NUM_GET_SENSOR_STATUS,
  CMD_NUM_SET_ENC_PULSE_TOLERANCE,
  CMD_NUM_GET_ENC_PULSE_TOLERANCE,
  CMD_NUM_SET_TC_CONTROL_MODE,
  CMD_NUM_GET_TC_CONTROL_MODE,
  CMD_NUM_SET_RFID_DATA,
  CMD_NUM_GET_RFID_DATA,
  CMD_NUM_SET_RFID_STATE,
  CMD_NUM_GET_RFID_STATE,
  CMD_NUM_SET_HEARTBEAT_CONFIG,
  CMD_NUM_GET_HEARTBEAT_CONFIG,
  CMD_NUM_INVALID,                              //07D included comma
  CMD_NUM_SET_LOG_INTERVAL,                     //54 
  CMD_NUM_GET_LOG_INTERVAL,                     //new
  CMD_NUM_SET_DIA_WHEEL,                        //new
  CMD_NUM_GET_DIA_WHEEL,                         //new
  CMD_NUM_SET_CYCLE_FREQUENCY,                    //new
  CMD_NUM_GET_CYCLE_FREQUENCY,                    //new
  CMD_NUM_SET_CONTINUE_COUNT,                           //new
  CMD_NUM_GET_CONTINUE_COUNT,                     //new
  CMD_NUM_SET_EMERGENCY_RETURN,                   //new
  CMD_NUM_GET_RETURN_STATE,
  CMD_NUM_SET_COMMDISTANCE,
  CMD_NUM_GET_COMMDISTANCE,
  CMD_NUM_SET_N0_ROW_DAY,
  CMD_NUM_GET_N0_ROW_DAY,
  CMD_NUM_SET_TRACK_INFO2,
  CMD_NUM_GET_TRACK_INFO2,
  CMD_NUM_SET_RFID_DATA2,
  CMD_NUM_GET_RFID_DATA2,
  
  CMD_NUM_SET_ROW_LENGTH2_DATA,
  CMD_NUM_GET_ROW_LENGTH2_DATA,
  CMD_NUM_SET_ROW_LENGTH3_DATA,
  CMD_NUM_GET_ROW_LENGTH3_DATA,
  CMD_NUM_SET_ROW_LENGTH4_DATA,
  CMD_NUM_GET_ROW_LENGTH4_DATA,
  CMD_NUM_SET_ROW_LENGTH5_DATA,
  CMD_NUM_GET_ROW_LENGTH5_DATA,
  CMD_NUM_SET_ROW_LENGTH6_DATA,
  CMD_NUM_GET_ROW_LENGTH6_DATA,
  CMD_NUM_SET_ROW_LENGTH7_DATA,
  CMD_NUM_GET_ROW_LENGTH7_DATA,
  CMD_NUM_SET_RFID_DATA3,
  CMD_NUM_GET_RFID_DATA3,
  CMD_NUM_SET_RELAY_INTERVAL,
  CMD_NUM_GET_RELAY_INTERVAL,
  CMD_NUM_REQ_LOG

}eJsonCommand;

typedef enum eJsonStatus_def {
  JSON_NO_ERROR,
  JSON_PARSE_ERROR,
  JSON_INVALID_CMD_VALUE,
  JSON_INVALID_DATA,  
  JSON_DATA_OUT_OF_RANGE,
  JSON_EEPROM_ERROR,
  JSON_INVALID_DEV_ID, 
  JSON_WILDCARD_DEV_ID,
  JSON_NULL_DATA
}eJsonStatus;

//typedef struct{
//  uint32_t Continue1;
//  uint32_t Count1;
//}stContinue;

typedef eJsonStatus (*RxCommandProcess_Ptr)(const char * jsonString);
typedef void (*AssignDefaultProcess_Ptr)(void);

typedef struct stJsonCommand_def 
{
  eJsonCommand command;
  const uint16_t command_Hex;
  RxCommandProcess_Ptr executeCommand;
  EepromStoreProcess_Ptr executeStoreProcess;
  AssignDefaultProcess_Ptr executeAssignDefaultProcess;
}stJsonCommand;

typedef struct stMessage_def
{
uint64_t deviceID;
uint16_t CMD;
uint8_t values[100];
uint8_t CRC;
}stMessage;

int CYCLE_OCCURRED;
int CYCLE_OCCURED_FLASH;
int ERROR_OCCURRED;
int ERROR_OCCURRED_FLASH;
char SDerrorlog[45];

int Trackinfoend1;
int Trackinfoend1read;

uint32_t C_ROW;        //track changer
int Distancevalue;
int trackchangercount;
int rowsActuallyCleaned;
int rowdaycount;
int Row1value;
int Row1getvalue;
int tc_count;
int sum;

int Autoschcountval;
char SCHEDULE_LOG;

char modeop[2];
char schedule_log[105]; 
char cyclelog[105]; 
char getTime_str[40];
char POS[20],NEG[20];
char Accel_pulse_count[20],Decel_pulse_count[20];
char Battemp_str[10],Drivetemp_str[10],Dietemp_str[10];
char Vin_str[10],Vbat_str[10],Iin_str[10],Ibat_str[10],Dtemp_str[10],Soc_Str[10];
char Imot1_Str[10],Imot2_Str[10],Imot3_Str[10];
char ComPulse[20];

uint32_t Cumulative_distance;

uint8_t rxDataBuffer[MAX_UART_RX_BUFFER_LENGTH];
unsigned long int bmsvalue;
char* startbms, endbms;
char temp_arr[100][10], temp_arr1[100][10],slave_arr[10],fun_arr[10],byte_arr[10],uart_arr[100][10];
char bat_arr[10],pv_arr[10],mpptcurr_arr[10],mpptout_arr[10],inttemp_arr[10],exttemp_arr[10],batstate_arr[10], loadout_arr[10],loadtrip_arr[10];
//unsigned int Slave_address, function_code,BAT_STATE,BAT_STATE_CNT,LOAD_STATE,ENERGY_HI, REG_5V_EN,REG_5V_TRIP,MODBUS_ADR,REG_PWM,MPPT_CNT,MPPT_STATE,REG_DOMINATE,PV_EN,LOAD_EN,LOAD_COUNT,LOAD_TRIPCNT,size,size1,slave_address_cmp, function_cmp,p1,p2,p3,size_pkt,byte1,byte2,byte3,byte_value;
unsigned char Byte_cnt;
float Vbat,VPV,IMPPT,PWR,VN_bat1,VL5V,VLOAD,VN_bat2,HS_TEMP,EXT_TEMP,VR_bat, BAT_SET,BAT_COMP,ENERGY_LO,VMPPT_APP_SET,REG_SET_PV,REG_SET_VBAT,REG_SET_IMPPT,BEST_PV,BEST_PWR,IMPPT_APP_SET;
int byte_int;


float P;
float pos,neg;
float Accel_speed_pulse,Decel_speed_pulse;
float Rotation_per_second;
float posdistance,negdistance;
float circumference;

float CpositiveDistance,CnegativeDistance;
int CpositiveDistance_int,CnegativeDistance_int;
char Cpospulse[20], Cnegpulse[20];
float Cpositive,Cnegative;
int Cpositivedistance,Cnegativedistance;
int Timesecondscpos,Timesecondscneg;

int pulse_flag ;
int distancecalc, pulsecalc, distance;
int forward_remaining_distance,backward_remaining_distance;
int posdistance_int,negdistance_int;
int ComDistance_int;
int Accel_speed,Decel_speed;
int Accel_remaining_distance,Decel_remaining_distance;
int Time_seconds,Time_seconds_forward,Time_seconds_backward;
int Time_seconds_accel,Time_seconds_decel;
int CYCLEROTATIONTIME;
int ComDistance,Compulse;
int Getpos,Getneg;


//static eJsonStatus AssignJsonCommand (uint16_t  presentCommand_Hex); 
//static eJsonStatus ProcessJsonCommand (const uint8_t * jsonString, uint16_t presentCommand_Hex);
//static eJsonStatus ParseRxdata(stMessage* message);
//static const stJsonCommand * presentJsonCommandParams = NULL; 
//static eJsonStatus ProcessSetMode (const char * jsonString);

eJsonStatus ProcessReceivedJsonData_config(char * jsonReceivedData);
eJsonStatus ProcessReceivedJsonData (uint8_t * jsonReceivedData,uint16_t size);
eJsonStatus ExtractData(uint8_t* sourceBuffer,uint8_t size,stMessage* message);
eJsonStatus ConvertHexAndProcess(char * jsonString,uint16_t* size);


uint16_t GetCommandProcessingParam (stJsonCommand **jsonCmdParam);
void PulseCalculation(void);
void TransmitWelcomeMessage (void);
void ClearTxBuff(void);
void tcstarttime(void);
void tcendtime(void);
eJsonStatus ReportTrackChangerStatus(uint8_t reportType,uint32_t reportValue);
//ResponseApiprocess

#define JSON_NO_OF_STATUS_RESPONSE      8       
#define SET_AND_START_CYCLE             0x001D
#define GET_CLEANER_STATUS              0x1021
#define STOP_CLEANER                    0x0003
#define CYCLE_RESUME                    0x0003
#define GET_CLEANER_MODE                0x1001
#define HOME_RETURN                     0x0033

typedef eJsonStatus (*RxResponseProcess_Ptr)(uint64_t macAddress, const char * jsonString);

typedef enum eResponseCommand_def 
{
  CMD_NUM_ACK_RES,
  CMD_NUM_STATUS_RES,
  CMD_NUM_INVALID_RES
}eResponseCommand;

typedef struct stResponseCommand_def {
  eResponseCommand command;
  const uint16_t command_Hex;
  RxResponseProcess_Ptr executeCommand;
}stResponseCommand;

bool IsAckReceived(void);
void ClearIsAckReceived(void);
void SetIsAckReceived(void);
bool IsCleanerDone(void);
void ClearIsCleanerDone(void);
uint32_t IsErrorFromCleaner(void);
void ClearErrorFromCleaner(void);
void SetIsCleanerDone(void);

ErrorStatus SendCommandToCleaner(uint64_t macAddress,uint16_t command,char* values);

eJsonStatus ProcessResponse (uint64_t macAddress, uint8_t * jsonReceivedData, uint16_t size);


eJsonStatus ExtractDatafromzigbee(uint8_t* sourceBuffer,uint8_t size,stMessage* message);



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */


