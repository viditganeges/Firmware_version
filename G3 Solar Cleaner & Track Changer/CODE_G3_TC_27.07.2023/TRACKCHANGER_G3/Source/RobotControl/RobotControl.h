#ifndef __ROBOTCONTROL_H_
#define __ROBOTCONTROL_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_NO_OF_TRACKS 20

typedef enum {
  TRACK_INFO_INVALID, 
  TRACK_INFO_VALID
}eTrackInfoStat;

typedef enum {
  FORWARD_COUNT, 
  REVERSE_COUNT,
  TOTAL_NO_OF_COUNT
}eRowLengthData;


typedef struct {
  bool trackValid[MAX_NO_OF_TRACKS];
  uint32_t trackDistance[MAX_NO_OF_TRACKS];
  bool trackValid2[MAX_NO_OF_TRACKS];
  uint32_t trackDistance2[MAX_NO_OF_TRACKS];
}stTrackData;

typedef union {
uint64_t cleanerMAC_64;
uint16_t cleanerMAC_32[4];
}unCleanerMAC;

typedef struct {
//uint32_t rowLength[MAX_NO_OF_TRACKS][TOTAL_NO_OF_COUNT];
uint32_t rowLength[60][TOTAL_NO_OF_COUNT];
uint32_t rowLength2[MAX_NO_OF_TRACKS][TOTAL_NO_OF_COUNT];
uint32_t rowLength3[MAX_NO_OF_TRACKS][TOTAL_NO_OF_COUNT];
uint32_t rowLength4[MAX_NO_OF_TRACKS][TOTAL_NO_OF_COUNT];
uint32_t rowLength5[MAX_NO_OF_TRACKS][TOTAL_NO_OF_COUNT];
uint32_t rowLength6[MAX_NO_OF_TRACKS][TOTAL_NO_OF_COUNT];
uint32_t rowLength7[MAX_NO_OF_TRACKS][TOTAL_NO_OF_COUNT];
}stRowLengthData;

typedef struct {
uint32_t rfidData[MAX_NO_OF_TRACKS]; 
uint32_t rfidData2[MAX_NO_OF_TRACKS];
uint32_t rfidData3[MAX_NO_OF_TRACKS];
}stRfidData;

typedef enum eRobotState_def
{
  ROBOT_IDLE,
  ROBOT_START,
  ROBOT_DIR_CHANGE_STOP,
  ROBOT_WAIT_FOR_VALID_STATE,
  ROBOT_ACCELERATION_SLOW_START,
  ROBOT_ACCELERATION_START,
  ROBOT_ACCELERATION,
  ROBOT_STEADYSTATE,
  ROBOT_NORMAL_STOP,
  ROBOT_DECELERATION_START,
  ROBOT_DECELERATION,
  ROBOT_APPROACH_COUNT,
  ROBOT_IMMEDIATE_STOP,
  ROBOT_HOME_RETURN,
  ROBOT_END,
  ROBOT_WAIT_FOR_BRUSH_STOP,
  ROBOT_COMPLETED,
  ROBOT_WAIT_FOR_RELAY_ON,
  ROBOT_WAIT_FOR_RELAY_OFF,
  ROBOT_RE_START
}eRobotState;

typedef enum eRobotDirection_def
{
  POSITIVE,
  NEGATIVE,
  STOP,
  UNKNOWN  
}eRobotDirection;

typedef struct {
  uint32_t decelStartCountDiff;
  uint32_t accelTime;
  uint32_t decelTime;
  uint32_t steadyPwm1;
  uint32_t steadyPwm2;
  uint32_t approachPwm1; 
  uint32_t approachPwm2; 
} stRobotPwmParam;

typedef struct {
  uint32_t count;
  eRobotDirection direction;
  stRobotPwmParam RobotPwmParam;
} stRobotParam;

typedef struct {
  uint32_t maxNegCountForPVsetup;
  uint32_t maxPosCountForPVsetup;
} stCountRange;

typedef struct {
  uint32_t startTime;
  uint32_t endTime;
  float startSOC;
  float endSOC;
  uint32_t totalRotateCount1;
  uint32_t totalRotateCount2;
  uint32_t errorCode;
} stLastOperationStatus;



bool RobotInit (void);
void RobotFSM (void);
void SetRobotState (eRobotState setRobotState);
eRobotState GetRobotState (void);
eRobotDirection GetRobotActualDirection (void);
eRobotDirection CalculateRobotDirection (float startCount, float tarCount); 
void PwmChangeTimeIncrement_ms (void);
void SetCurrentModeRobotParameter (stRobotParam *motionParam);
void RobotTimeIncrement_ms (void);
stCountRange *GetCountRange(void);
stCountRange *GetSetCountRange(void);
stLastOperationStatus * GetLastOperationData (void);
bool IsEdgeReached (eRobotDirection dir);
bool IsFinalTargetCountReached (void);
bool IsProximityDetected(void);
#endif