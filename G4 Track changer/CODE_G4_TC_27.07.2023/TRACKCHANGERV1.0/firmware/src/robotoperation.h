#ifndef _ROBOTOPERATION_H    /* Guard against multiple inclusion */
#define _ROBOTOPERATION_H

#include "main.h"

#define TOTAL_MAX_NO_OF_TRACKS  25
#define MAX_NO_OF_RLTRACKS  10
#define MAX_NO_OF_TRACKS  20
#define TOTAL_NO_OF_COUNT1  2
#define MAX_NO_OF_TRACKINFO  15
#define MAX_NO_OF_TRACKINFODB  30

#define MIN_TO_mSEC(X) X*60*1000
#define SEC_TO_mSEC(X) X*1000

#define TC_WAIT_FOR_PROXIMITY_TIME              SEC_TO_mSEC(60)
#define TC_WAIT_FOR_EDGE_TIME                   SEC_TO_mSEC(60)
#define TC_TOTAL_CLEANER_OPERATION_TIME         MIN_TO_mSEC(3)
#define TC_CLEANER_DETECT_TIME_START            SEC_TO_mSEC(10)
#define TC_WAIT_FOR_ACK_TIME                    SEC_TO_mSEC(10)
#define TC_WAIT_FOR_CLEANER_START               SEC_TO_mSEC(3)
#define TC_WAIT_FOR_CLEAN_COMPLETE_TIME         MIN_TO_mSEC(1)
#define TC_CLEANER_DETECT_TIME_STOP             SEC_TO_mSEC(10)
#define TC_COMMUNICATION_INTERVAL               SEC_TO_mSEC(3)

#define REVERSE_OPERATION_ENABLED               0  //1 to enable,0 to disable

#define MAX_TIME_FOR_ONE_PULSE_CLEANER              SEC_TO_mSEC(30)
#define MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_ENC    SEC_TO_mSEC(2)
#define MAX_TIME_FOR_ONE_PULSE_TRACK_CHANGER_PROX   SEC_TO_mSEC(10)

typedef enum eTcControlMode_def
{
  ENCODER_MODE,
  PROXIMITY_MODE
}eTcControlMode;

typedef enum eMotorCommand_def
{
  ROBOT_CMD_IDLE,
  ROBOT_CMD_NONE,
  ROBOT_CMD_POSITIVE,
  ROBOT_CMD_NEGATIVE,
  ROBOT_CMD_CYCLE,
  ROBOT_CMD_NORMAL_STOP,
  ROBOT_CMD_STOP,
  ROBOT_CMD_EMERGENCY_STOW,
  ROBOT_CMD_GOTO_COUNT,
  ROBOT_CMD_MANUAL,
  ROBOT_CMD_AUTO, 
  ROBOT_CMD_CYCLE_TEST,
  ROBOT_CMD_TRACK_CHANGER,
  ROBOT_CMD_CYCLE_RESUME
}eRobotCommand;

typedef struct 
{
  uint32_t emergencyStowTargetCount;
} stModeCounts;

typedef enum eMode_def
{ 
  AUTO,
  MANUAL,  
  IDLE,
  EMERGENCY_STOW,
  COMM_ERROR_EMERGENCY_STOW,
  CYCLE_TEST,
}eMode;

typedef enum eManualState_def
{
  MAN_IDLE,
  MAN_POSITIVE,
  MAN_NEGATIVE,
  MAN_CYCLE,
  MAN_NORMAL_STOP,
  MAN_STOP,
  MAN_GOTO_COUNT,
  MAN_RESUME,
  MAN_TRACKCHANGER
}eManualState;

typedef enum eMotionPauseState_def
{
  PA_IDLE,
  PA_PAUSE,
  PA_WAIT_FOR_STOP,
  PA_WAIT_FOR_ERROR_CLEAR,
  PA_WAIT_FOR_TIMEOVER_RESUME,
  PA_COMPLETED
}eMotionPauseState;

typedef enum eTrackChangerState_def
{
  TC_NIL,
  TC_START,
  TC_WAIT_FOR_RELAY_ON,
  TC_START_OPERATION,
  TC_ASSIGN_VALUES,
  TC_NOT_IN_POS_ERROR, 
  TC_WAIT_FOR_PROXIMITY,
  TC_CLEANER_OPERATION,
  TC_CHECK_FOR_REMAINING_ROWS,
  TC_WAIT_FOR_EDGE,
  TC_REVERSE_AND_GOTO_START,
  TC_STOP,
  TC_WAIT_FOR_ROBOT_STOP,
  TC_WAIT_FOR_RELAY_OFF,
  TC_JOG_AND_FIND_POSITION,
  TC_ERROR
}eTrackChangerState;

typedef enum
{
  TC_REPORT_UNKNOWN,
  TC_REPORT_START,
  TC_REPORT_WAIT_FOR_PROXIMITY,
  TC_REPORT_WAIT_FOR_EDGE,
  TC_REPORT_WAIT_FOR_CLEANER,
  TC_REPORT_STOP,
  TC_REPORT_ERROR
}eTrackChangerReportState;

typedef enum eCleanerStatus_def
{
  C_STARTED,
  C_IN_PROGRESS,
  C_DONE,
  C_ERROR,
  C_UNKNOWN
}eCleanerStatus;

typedef enum eJogStatus_def
{
  J_STARTED,
  J_IN_PROGRESS,
  J_DONE,
  J_DONE_EDGE,
  J_ERROR
}eJogStatus;

typedef enum eCleanerState_def
{
  C_START,
  C_CHECK_CMODE_1,
  C_CHECK_PROXOMITY_1,
  C_SEND_CYCLE_CMD,
  C_WAIT_FOR_ACK,
  C_DELAY_FOR_CLEANER_START,
  C_GET_STATUS,
  C_CHECK_PROXIMITY_2,
  C_STOP,
  C_ERROR_STATE,
  C_COMM_RECONNECT,
  C_SEND_RESUME,
  C_WAIT_FOR_RESUME_ACK,
  C_DELAY_FOR_CLEANER_RESUME,
}eCleanerState;

typedef enum eJogState_def
{
  JOG_START,
  JOG_WAIT_FOR_RELAY_ON,
  CHECK_FOR_PROX_OR_EDGE,
  RESET_RFID_AND_WAIT,
  CHECK_RFID_AND_ASSIGN_INDEX,
  JOG_STOP,
  JOG_ERROR
}eJogState;

typedef enum eTrackchangerError_def
{
  TC_NO_ERROR,
  TC_INVALID_DATA = 16,
  TC_MISSED_ROWS ,
  TC_INVALID_DISTANCE_TRAVELLED,
  TC_EDGE_UNKNOWN,
  TC_COMMUNICATION_ERROR,
  TC_CLEANER_NOT_IN_POSITION,    
  TC_CLEANER_ERROR,
  TC_CLEANER_STALL,
  TC_TRACK_CHANGER_STALL,
  TC_NOT_IN_POSITION,
  TC_INVALID_RFID,
  TC_INAPPROPRIATE_ROW_DATA
}eTrackChangerError;

typedef enum eTrackChangerReport_def
{
  TC_REPORT_INDEX_REACHED,
  TC_REPORT_ROW_DONE,
  TC_REPORT_STATUS,
  TC_REPORT_CLEANER_ERROR
}eTrackChangerReport;

typedef enum eRobotDirection_def
{
  POSITIVE,
  NEGATIVE,
  STOP,
  UNKNOWN  
}eRobotDirection;

typedef struct 
{
  int decelStartCountDiff;
  int accelTime;
  int decelTime;
  int steadyPwm1;
  int steadyPwm2;
  int approachPwm1; 
  int approachPwm2; 
} stRobotPwmParam;

typedef struct 
{
  uint32_t count;
  eRobotDirection direction;
  stRobotPwmParam RobotPwmParam;
} stRobotParam;

typedef struct 
{
  bool trackValid[MAX_NO_OF_TRACKS];
  int trackDistance[MAX_NO_OF_TRACKS];
  bool trackValid2[MAX_NO_OF_TRACKS];
  int trackDistance2[MAX_NO_OF_TRACKS];
}stTrackData;

stTrackData trackData;

typedef union 
{
uint64_t cleanerMAC_64;
uint16_t cleanerMAC_32[4];
uint8_t cleanerMAC_16[8];
}unCleanerMAC;

typedef struct 
{
int rowLength[TOTAL_MAX_NO_OF_TRACKS][TOTAL_NO_OF_COUNT1];
int rowLength2[MAX_NO_OF_RLTRACKS][TOTAL_NO_OF_COUNT1];
int rowLength3[MAX_NO_OF_RLTRACKS][TOTAL_NO_OF_COUNT1];
int rowLength4[MAX_NO_OF_RLTRACKS][TOTAL_NO_OF_COUNT1];
int rowLength5[MAX_NO_OF_RLTRACKS][TOTAL_NO_OF_COUNT1];
int rowLength6[MAX_NO_OF_RLTRACKS][TOTAL_NO_OF_COUNT1];
int rowLength7[MAX_NO_OF_RLTRACKS][TOTAL_NO_OF_COUNT1];
}stRowLengthData;

stRowLengthData rowLengthData;

typedef struct 
{
int rfidData[MAX_NO_OF_RLTRACKS]; 
int rfidData2[MAX_NO_OF_RLTRACKS];
int rfidData3[MAX_NO_OF_RLTRACKS];
}stRfidData;

stRfidData storeRfidData;

void SetMotionCommand (eRobotCommand command);
eRobotCommand GetMotionCommand (void);
void RobotOperate (void);
void SetManualPwmParameters (stRobotPwmParam *robotPwmParam);
void SetEmergencyPwmParameters (stRobotPwmParam *robotPwmParam);
void SetModeCounts (stModeCounts *modeSetCounts);
stModeCounts * GetSetModeCounts (void);
stRobotPwmParam * GetSetEmergencyPwmParameters (void);
stRobotPwmParam * GetSetManualPwmParameters (void);
void SetMotionPauseState (eMotionPauseState pauseSt);
eMotionPauseState GetMotionPauseState (void);
eMotionPauseState GetPrevPauseState(void);
void SetPrevPauseState(eMotionPauseState state);
void SetOperationMode (eMode setMode, eManualState opManualState);
eMode GetOperationMode (void);
eManualState GetManualState (void);
void LoadRobotParameters(uint32_t count, stRobotPwmParam * modePwmParam);
uint32_t GetGotoCount(void);
bool SetGotoCount(uint32_t);
bool IsCountValid(uint32_t count);
void RobotOperationTimeIncrement_ms (void);
void SetInitialOperationMode (eMode eepromReadMode);

eTrackChangerState GetTrackChangerState(void);
void TrackChangerOperate(void);
eCleanerStatus OperateCleaner(uint16_t index);
eJogStatus JogAndFindPosition(uint8_t* index);

void CheckAndStopRobotOnProximity(void);

void SetTrackData(stTrackData *trackData);
stTrackData *GetSetTrackData(void);
void SetCleanerMAC(unCleanerMAC *macData);
unCleanerMAC *GetSetCleanerMAC(void);
void SetRowLengthData(stRowLengthData *rowData);
stRowLengthData *GetSetRowLengthData(void);
void SetEncoderPulseTolerance(uint32_t* tolerance);
uint32_t *GetSetEncoderPulseTolerance(void);
void SetRfidData(stRfidData *rfidData);
stRfidData *GetSetRfidData(void);

uint64_t getCurrentCleanerMac(void);
void SetReportTrackChangerState(eTrackChangerReportState state);
eTrackChangerReportState GetReportTrackChangerState(void);
void ClearReportTrackChangerState(void);
void SetNextIndex(uint8_t index);
uint8_t GetNextIndex(void);
eTrackChangerError GetTrackChangerError(void);

eTcControlMode GetTcControlMode(void);
void SetTcControlMode(eTcControlMode mode);
uint32_t GetPrevRotateSense1Count(void);

void RFID_Sense_Time_Increment(void);
void TC_Reverse_Time_Increment(void);

void SetTrackChangerError2(eTrackChangerError errorCode);

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */


