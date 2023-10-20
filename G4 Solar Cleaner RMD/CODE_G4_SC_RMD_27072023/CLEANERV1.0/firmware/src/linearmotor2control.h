#ifndef _MOTOR2CONTROL_H    /* Guard against multiple inclusion */
#define _MOTOR2CONTROL_H

#include "main.h"

typedef enum eMotor2State_def
{
  MOTOR2_IDLE,
  MOTOR2_STOP,
  MOTOR2_POSITIVE,
  MOTOR2_NEGATIVE  
}eMotor2State;

void Motor2Init (void);
void Motor2FSM (void);
void SetMotor2State (eMotor2State motor2StateValue);
eMotor2State GetMotor2State (void);
void SetMotor2TargetState (eMotor2State motor2StateValue);
eMotor2State GetMotor2TargetState (void);
bool SetMotor2AllPwm(uint32_t duty);
bool UpdateMotor2Pwm (uint32_t pwmDuty);
uint8_t GetMotor2Pwm (void);
void Motor2TimeIncrement_ms (void);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
