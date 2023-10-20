#ifndef _ROBOTOPERATION_H    /* Guard against multiple inclusion */
#define _ROBOTOPERATION_H

#include "main.h"

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
  ROBOT_CMD_CYCLE_RESUME
}eRobotCommand;

typedef struct {
  uint32_t emergencyStowTargetCount;
} stModeCounts;

typedef enum eMode_def
{
  AUTO,
  MANUAL,  
  IDLE,
  EMERGENCY_STOW,
  COMM_ERROR_EMERGENCY_STOW,
  CYCLE_TEST
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
  MAN_RESUME
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

void SetMotionCommand (eRobotCommand command);
eRobotCommand GetMotionCommand (void);

void RobotOperate (void);

typedef struct {
  int accelTime;
  int decelTime; 
  int Accel;
  int acelStartCountDiff;
  uint8_t accelapproachPwm1; 
  uint8_t accelapproachPwm2;
  uint8_t steadyPwm1;
  uint8_t steadyPwm2;
  uint8_t approachPwm1; 
  uint8_t approachPwm2;
  int Decel;
  int decelStartCountDiff;
  int timePwm;
} stRobotPwmParam;

unsigned char Schedulestart;

void SetManualPwmParameters (stRobotPwmParam *robotPwmParam);
stRobotPwmParam * GetSetManualPwmParameters (void);
stRobotPwmParam * GetSetEmergencyPwmParameters (void);

void SetMotionPauseState (eMotionPauseState pauseSt);
eMotionPauseState GetMotionPauseState (void);

eMotionPauseState GetPrevPauseState(void);
void SetPrevPauseState(eMotionPauseState state);

uint32_t GetPrevRotateSense2Count(void);
uint32_t GetPrevRotateSense1Count(void);
bool GetResumeBit(void);
void SetResumeBit(bool value);

void SetOperationMode (eMode setMode, eManualState opManualState);
eMode GetOperationMode (void);
void SetInitialOperationMode (eMode eepromReadMode);

eManualState GetManualState (void);
void LoadRobotParameters(uint32_t count, stRobotPwmParam * modePwmParam);
uint32_t GetGotoCount(void);
bool SetGotoCount(uint32_t);
bool IsCountValid(uint32_t count);
void RobotOperationTimeIncrement_ms (void);

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
