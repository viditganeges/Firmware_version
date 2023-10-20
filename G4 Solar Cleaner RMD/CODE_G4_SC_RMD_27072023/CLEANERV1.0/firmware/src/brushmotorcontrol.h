#ifndef _BRUSHMOTORCONTROL_H    /* Guard against multiple inclusion */
#define _BRUSHMOTORCONTROL_H

#include "main.h"

#define DEFAULT_BRUSHMOTOR_DEAD_TIME_MS        5

#define STOP_PWM_VALUE          0

#define BRUSH_PWM               PWM0_REGS
#define BRUSH_PWM_CHANNEL       PWM_CHANNEL_3

typedef enum eBrushMotorState_def
{
  BRUSHMOTOR_IDLE,
  BRUSHMOTOR_STOP,
  BRUSHMOTOR_POSITIVE,
  BRUSHMOTOR_NEGATIVE  
}eBrushMotorState;

void BrushMotorInit (void);
void BrushMotorFSM (void);
void SetBrushMotorState (eBrushMotorState brushMotorStateValue);
void OperateBrushMotorForTest(eBrushMotorState targetState);
void BrushMotorTimeIncrement_ms (void);
void SetBrushMotorTargetState (eBrushMotorState brushMotorStateValue);

eBrushMotorState GetBrushMotorState (void);
eBrushMotorState GetBrushMotorTargetState (void);

bool SetBrushMotorAllPwm(uint8_t duty);
bool UpdateBrushMotorPwm (uint8_t pwmDuty);

uint8_t GetBrushMotorPwm (void);

#endif /* _EXAMPLE_FILE_NAME_H */


