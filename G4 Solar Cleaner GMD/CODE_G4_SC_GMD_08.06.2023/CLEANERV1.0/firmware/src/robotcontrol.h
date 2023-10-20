#ifndef _ROBOTCONTROL_H    /* Guard against multiple inclusion */
#define _ROBOTCONTROL_H

#include "main.h"
                                           
typedef enum eBrushState_def                 //brush control
{
  BRUSH_IDLE,
  BRUSH_START,
  BRUSH_DIR_CHANGE_STOP,
  BRUSH_WAIT_FOR_VALID_STATE,
  BRUSH_ACCELERATION_START,
  BRUSH_ACCELERATION,
  BRUSH_STEADYSTATE,
  BRUSH_NORMAL_STOP,
  BRUSH_DECELERATION_START,
  BRUSH_DECELERATION,
  BRUSH_APPROACH_COUNT,
  BRUSH_IMMEDIATE_STOP,
  BRUSH_END,
  BRUSH_COMPLETED,
  BRUSH_RE_START,
  BRUSH_DISABLEDSTATE,
}eBrushState;

typedef enum eBrushDirection_def
{
  BRUSH_POSITIVE,
  BRUSH_NEGATIVE,
  BRUSH_STOP,
  BRUSH_UNKNOWN
}eBrushDirection;

typedef struct {
  int accelTime;
  int decelTime;
  uint8_t steadyPwm;
  int brushTime;         
} stBrushPwmParam;

typedef struct {
  eBrushDirection direction;
  stBrushPwmParam BrushPwmParam;
} stBrushParam;


void SetBrushNormalPwmParameters (stBrushPwmParam *brushPwmParam);
void SetBrushAutoPwmParameters (stBrushPwmParam *brushPwmParam);

stBrushPwmParam * GetSetBrushNormalPwmParameters (void);
stBrushPwmParam * GetSetBrushAutoPwmParameters (void);

bool RobotInit (void);
void RobotFSM (void);

bool BrushInit (void);
void BrushFSM (void);
void SetBrushState (eBrushState setBrushState);
eBrushState GetBrushState (void);
void BrushPwmChangeTimeIncrement_ms (void);

void StartBrush (eBrushDirection dir);
void StopBrush (void);
void ImmediateStopBrush (void);

typedef enum eRobotState_def                     //robot control
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
  uint32_t count;
  eRobotDirection direction;
  stRobotPwmParam RobotPwmParam;
} stRobotParam;

typedef struct {
  uint32_t startTime;
  uint32_t endTime;
  float startSOC;
  float endSOC;
  uint32_t totalRotateCount1;
  uint32_t totalRotateCount2;
  uint32_t errorCode;
} stLastOperationStatus;

bool GetTargetCount(void);
void SetRobotState (eRobotState setRobotState);
eRobotState GetRobotState (void);
eRobotDirection GetRobotActualDirection (void);
eRobotDirection CalculateRobotDirection (float startCount, float tarCount); 
void PwmChangeTimeIncrement_ms (void);
void SetCurrentModeRobotParameter (stRobotParam *motionParam);
void RobotTimeIncrement_ms (void);
stLastOperationStatus * GetLastOperationData (void);

uint32_t Current_Brush_Count(uint32_t);
int Get_Current_Brush_Count(void);

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
