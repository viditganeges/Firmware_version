#ifndef _APIPROCESS_H    /* Guard against multiple inclusion */
#define _APIPROCESS_H

#include "main.h"

#define MAX_NO_OF_TRACKS  20

#define MAX_UART_TX_BUFFER_LENGTH             10000//   1500
#define MAX_UART_RX_BUFFER_LENGTH               500
#define MAX_ACK_TX_BUFFER_LENGTH                200

#define MAX_NO_OF_SET_DATA                      20
#define JSON_NO_OF_MODE_STATE_DATA              1+1 //+1 is CRC       
#define JSON_NO_OF_MOTOR_MODE_STATE_DATA        3+1             
#define JSON_NO_OF_SENSOR_STATE_DATA            2+1             
#define JSON_NO_OF_PWM_DATA_MOTOR               12+1       
#define JSON_NO_OF_PWM_DATA_BRUSH               4+1 
#define JSON_NO_OF_RTC_DATA                     6+1
#define JSON_NO_OF_SET_DEVICE_INFO              3+1
#define JSON_NO_OF_GET_DEVICE_INFO              4+1
#define JSON_NO_OF_CURRENT_LIMIT                3+1
#define JSON_NO_OF_ERROR_CODE_DATA              1+1
#define JSON_NO_OF_SOC_DATA                     1+1
#define JSON_NO_OF_ZIGBEE_CONFIG_DATA           6+1
#define JSON_NO_OF_ZIGBEE_NW_RESET_DATA         1+1
#define JSON_NO_OF_MOTOR_FAULT_CONDITIONS       4+1
#define JSON_NO_OF_REQUEST_LOG_VALUES           2+1
#define JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS  4+1
#define JSON_NO_OF_INTERVAL_DATA                     12+1    
#define JSON_NO_OF_CYCLEFREQUENCY_DATA               1+1    
#define JSON_NO_OF_CONTINUE                          1+1
#define JSON_NO_OF_RETURN                            1+1
#define JSON_NO_OF_REQUEST_LOG                       1+1
#define JSON_NO_OF_COMDISTANCE                       1+1 
#define JSON_NO_AUTOSCH                              19+1   
#define JSON_NO_OF_PULSE_COUNT_DATA                  2+1 
#define JSON_NO_OF_PULSE_CYCLE_COUNT_DATA            4+1             
#define JSON_NO_OF_WHEELDIA_DATA                     3+1  
#define JSON_NO_OF_AUTO_SCHED_DATA                   2+1
#define JSON_NO_OF_SETGET_PWM_DATA_MOTOR             7+1
#define JSON_NO_OF_ROW_LENGTH_DATA                  ((MAX_NO_OF_SET_DATA*2)+2+1)
#define JSON_NO_OF_ROWS                              8+1   
#define JSON_NO_OF_GET_TRACK_INFO                    2+1
#define JSON_NO_OF_CHANGER_MAC_DATA                  1+1
#define JSON_NO_OF_CHANGER_MAC_DATA_flash            8+1
#define JSON_NO_OF_RELAY_DATA                        1+1 

//FOR flash usage
#define JSON_NO_OF_PWM_DATA_MOTOR_FLASH_WRITE         24+1   
#define JSON_NO_OF_PWM_DATA_BRUSH_FLASH               10+1
#define JSON_NO_OF_PULSE_COUNT_FLASH                  18+1 //12+1
#define JSON_NO_OF_HEARTBEAT_CONFIG_PARAMETERS_FLASH  13+1
#define JSON_NO_OF_MOTOR_FAULT_CONDITIONS_FLASH       16+1
#define JSON_NO_OF_COMDISTANCE_FLASH                   4+1 
#define JSON_NO_OF_WHEELDIA_DATA_FLASH                 5+1 

//API commands
#define CMD_SET_MODE                          0x0001  
#define CMD_GET_MODE                          0x1001
#define CMD_SET_MOTOR_DIR_MANUAL              0x0003
#define CMD_GET_MOTOR_DIR_MANUAL              0x1003
#define CMD_SET_MOTOR_PWM_MANUAL              0x0004
#define CMD_GET_MOTOR_PWM_MANUAL              0x1004
#define CMD_SET_BRUSH_MOTOR_STATE             0x0005
#define CMD_GET_BRUSH_MOTOR_STATE             0x1005
#define CMD_SET_BRUSH_MOTOR_PWM_MANUAL        0x0006
#define CMD_GET_BRUSH_MOTOR_PWM_MANUAL        0x1006
#define CMD_SET_BRUSH_MOTOR_DIR               0x0007
#define CMD_GET_BRUSH_MOTOR_DIR               0x1007
#define CMD_SET_MOTOR_PWM_AUTO                0x0009
#define CMD_GET_MOTOR_PWM_AUTO                0x1009
#define CMD_SET_BRUSH_MOTOR_PWM_AUTO          0x000A
#define CMD_GET_BRUSH_MOTOR_PWM_AUTO          0x100A
#define CMD_SET_AUTO_SCHEDULE_TIME            0x000B
#define CMD_GET_AUTO_SCHEDULE_TIME            0x100B
#define CMD_SET_EDGE_SENSE_STATE              0x000C
#define CMD_GET_EDGE_SENSE_STATE              0x100C
#define CMD_SET_RTC_VALUE                     0x000D
#define CMD_GET_RTC_VALUE                     0x100D
#define CMD_GET_CURRENT_STAT                  0x100E
#define CMD_SET_OVERCURRENT_LIMIT             0x000F
#define CMD_GET_LAST_CYCLE_STAT               0x100F
#define CMD_CLEAR_ERRORCODES                  0x0010
#define CMD_GET_OVERCURRENT_LIMIT             0x1010
#define CMD_SET_PULSE_COUNT_VALUES            0x0011
#define CMD_GET_MOT_CURRENT_VALUES            0x1011
#define CMD_SET_LOW_BAT_SOC                   0x0012
#define CMD_GET_PULSE_COUNT_VALUES            0x1014
#define CMD_RESET_TO_DEFAULTS                 0x0015
#define CMD_GET_LOW_BAT_SOC                   0x1015
#define CMD_SET_MOTOR_FAULT_CONDITIONS        0x0016
#define CMD_GET_CONTROLLER_STATUS             0x1018
#define CMD_SET_DEVICE_INFO                   0x000E
#define CMD_GET_DEVICE_INFO                   0x1019
#define CMD_GET_BAT_AND_PV_VAL                0x101C
#define CMD_SET_PULSE_COUNT_AND_CYCLE         0x001D
#define CMD_GET_TEMPERATURES                  0x101D
#define CMD_SET_HEARTBEAT_CONFIG              0x001E
#define CMD_GET_MOTOR_FAULT_CONDITIONS        0x101E
#define CMD_REQ_ACTIVITY_LOG                  0x101F 
#define CMD_GET_CURRENT_ROBOT_STATUS          0x1021
#define CMD_GET_HEARTBEAT_CONFIG              0x1022
#define CMD_SET_LOG_INTERVAL                  0x0030
#define CMD_GET_LOG_INTERVAL                  0x1030
#define CMD_SET_DIA_WHEEL                     0x0031
#define CMD_GET_DIA_WHEEL                     0x1031
#define CMD_SET_CYCLE_FREQUENCY               0x0032
#define CMD_GET_CYCLE_FREQUENCY               0x1032
#define CMD_SET_RETURN_STATE                  0x0033
#define CMD_GET_RETURN_STATE                  0x1033
#define CMD_SET_CONTINUE_COUNT                0x0034
#define CMD_GET_CONTINUE_COUNT                0x1034              
#define CMD_SET_COM_DISTANCE                  0x0035
#define CMD_GET_COM_DISTANCE                  0x1035
#define CMD_REQ_LOG                           0x103F
#define CMD_SET_DRIVE_STATE                   0x0050
#define CMD_GET_DRIVE_STATE                   0x1050
#define CMD_SET_CHANGER_MAC                   0x0051 
#define CMD_GET_CHANGER_MAC                   0x1051 
#define CMD_SET_RELAY_INTERVAL                0x0052
#define CMD_GET_RELAY_INTERVAL                0x1052
#define CMD_DEVCIE_WELCOME_MSG                0x1100
#define CMD_ACK_DATA                          0x1111  

#define CMD_AUTO_GET_VALUES                   0x2222  //new
#define CMD_SOC_LOG                      0x201C  
#define CMD_ERROR_LOG                    0x2018
#define CMD_MOTORCURRENT_LOG             0x2011
#define CMD_AUTO_CYCLE_LOG               0x200F

#define CMD_RESET_ZIGBEE_NTW                  0x0014

#define MAX_NO_OF_VALUES 30
#define MAX_COMMAND_SIZE 4
#define START_CHAR      0x01 
#define DELIMITER_1     0x02
#define DELIMITER_2     0x03
#define STOP_CHAR       0x04         
#define DEVICE_ID               255
#define WILDCARD_DEVICE_ID      0xFFFF
#define STARTID 0x90
#define ENDID  0x2C
#define MAC_ID_STR_LEN 17

typedef enum eJsonCommand_def                                                             
{
  CMD_NUM_SET_MODE, 
  CMD_NUM_GET_MODE,      
  CMD_NUM_SET_MOTOR_DIR_MANUAL,
  CMD_NUM_GET_MOTOR_DIR_MANUAL,        
  CMD_NUM_SET_MOTOR_PWM_MANUAL,
  CMD_NUM_GET_MOTOR_PWM_MANUAL,        
  CMD_NUM_SET_BRUSH_MOTOR_STATE,
  CMD_NUM_GET_BRUSH_MOTOR_STATE,         
  CMD_NUM_SET_BRUSH_MOTOR_PWM_MANUAL,
  CMD_NUM_GET_BRUSH_MOTOR_PWM_MANUAL,        
  CMD_NUM_SET_BRUSH_MOTOR_DIR,
  CMD_NUM_GET_BRUSH_MOTOR_DIR,                
  CMD_NUM_SET_MOTOR_PWM_AUTO,
  CMD_NUM_GET_MOTOR_PWM_AUTO,        
  CMD_NUM_SET_BRUSH_MOTOR_PWM_AUTO,
  CMD_NUM_GET_BRUSH_MOTOR_PWM_AUTO,        
  CMD_NUM_SET_AUTO_SCHEDULE_TIME,
  CMD_NUM_GET_AUTO_SCHEDULE_TIME,         
  CMD_NUM_SET_EDGE_SENSE_STATE,
  CMD_NUM_GET_EDGE_SENSE_STATE,        
  CMD_NUM_SET_RTC_VALUE,
  CMD_NUM_GET_RTC_VALUE,        
  CMD_NUM_SET_OVERCURRENT_LIMIT,        
  CMD_NUM_GET_OVERCURRENT_LIMIT,        
  CMD_NUM_SET_PULSE_COUNT_VALUES,
  CMD_NUM_GET_PULSE_COUNT_VALUES,        
  CMD_NUM_SET_LOW_BAT_SOC,
  CMD_NUM_GET_LOW_BAT_SOC,          
  CMD_NUM_SET_HEARTBEAT_CONFIG,
  CMD_NUM_GET_HEARTBEAT_CONFIG,          
  CMD_NUM_SET_ZIGBEE_CONF,
  CMD_NUM_GET_ZIGBEE_CONF,        
  CMD_NUM_SET_MOTOR_FAULT_CONDITION,
  CMD_NUM_GET_MOTOR_FAULT_CONDITION,        
  CMD_NUM_SET_LOG_INTERVAL,                     
  CMD_NUM_GET_LOG_INTERVAL,                    
  CMD_NUM_SET_DIA_WHEEL,                      
  CMD_NUM_GET_DIA_WHEEL,                       
  CMD_NUM_SET_CYCLE_FREQUENCY,                  
  CMD_NUM_GET_CYCLE_FREQUENCY,                   
  CMD_NUM_SET_CONTINUE_COUNT,                         
  CMD_NUM_GET_CONTINUE_COUNT,                    
  CMD_NUM_SET_EMERGENCY_RETURN,                   
  CMD_NUM_GET_RETURN_STATE,
  CMD_NUM_SET_COM_DISTANCE,
  CMD_NUM_GET_COM_DISTANCE,
  CMD_NUM_SET_N0_ROW_DAY,
  CMD_NUM_GET_N0_ROW_DAY,
  CMD_NUM_SET_ROW_LENGTH_DATA,
  CMD_NUM_GET_ROW_LENGTH_DATA,
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
  CMD_NUM_SET_DEVICE_INFO,
  CMD_NUM_GET_DEVICE_INFO,        
  CMD_NUM_SET_PULSE_COUNT_AND_CYCLE,        
  CMD_NUM_CLEAR_ERRORCODES,
  CMD_NUM_CLEAR_ACTIVITY_LOG, 
  CMD_NUM_RESET_ZIGBEE_NTW,        
  CMD_NUM_RESET_TO_DEFAULTS,
  CMD_NUM_GET_CURRENT_STAT,
  CMD_NUM_GET_LAST_CYCLE_STAT,
  CMD_NUM_GET_MOT_CURRENT_VALUES, 
  CMD_NUM_GET_CURRENT_ROBOT_STATUS,
  CMD_NUM_GET_SOC,
  CMD_NUM_GET_CHARGER_STATE,
  CMD_NUM_GET_CONTROLLER_STATUS,
  CMD_NUM_GET_ZIGBEE_NTW_PARAM, 
  CMD_NUM_GET_BAT_AND_PV_VAL, 
  CMD_NUM_GET_TEMPERATURES,
  CMD_NUM_REQ_ACTIVITY_LOG,
  CMD_NUM_SET_DRIVE_STATE,
  CMD_NUM_GET_DRIVE_STATE,
  CMD_NUM_SET_CHANGER_MAC,
  CMD_NUM_GET_CHANGER_MAC,
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

char SCHEDULE_LOG;
char modeop[2];
char schedule_log[105]; 
char schedule_log_21[105];
char schedule_BYTE;
char schedule_BYTE_flashlog;
char cyclelog[105]; 
char getTime_str[40];
char POS[20],NEG[20];
char Accel_pulse_count[20],Decel_pulse_count[20];
char Battemp_str[10],Drivetemp_str[10],Dietemp_str[10];
char Vin_str[10],Vbat_str[10],Iin_str[10],Ibat_str[10],Dtemp_str[10],Soc_Str[10];
char Imot1_Str[10],Imot2_Str[10],Imot3_Str[10];
char ComPulse[20];
char Cpospulse[20], Cnegpulse[20];
char SDerrorlog[45];

float P;
float pos,neg;
float Accel_speed_pulse,Decel_speed_pulse;
float Rotation_per_second;
float posdistance,negdistance;
float CpositiveDistance,CnegativeDistance;
float Cpositive,Cnegative;

int CpositiveDistance_int,CnegativeDistance_int;
int Autoschcountval;
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
int Cumulative_distance;
int CYCLE_OCCURRED;
int CYCLE_OCCURED_FLASH;
int ERROR_OCCURRED;
int ERROR_OCCURRED_FLASH;
int MOTOR_STARTED;

uint16_t track_no,day_count;
uint16_t cleaner_in_comm;
uint16_t GetCommandProcessingParam (stJsonCommand **jsonCmdParam);

void SD_SOC_Start(void);
void SD_SOC_End(void);
void SD_SOC_Start_21(void);
void SD_SOC_End_21(void);
void PulseCalculation(void);
void TransmitWelcomeMessage (void);
void ClearTxBuff(void);

eJsonStatus ProcessReceivedJsonData_config(char * jsonReceivedData);
eJsonStatus ProcessReceivedJsonData (uint8_t * jsonReceivedData,uint16_t size);
eJsonStatus ExtractData(uint8_t* sourceBuffer,uint8_t size,stMessage* message);
eJsonStatus ConvertHexAndProcess(char * jsonString,uint16_t* size);
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


