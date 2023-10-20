#ifndef _MOTOR1CONTROL_H    /* Guard against multiple inclusion */
#define _MOTOR1CONTROL_H

#include "main.h"

typedef enum eMotor1State_def
{
  MOTOR1_IDLE,
  MOTOR1_STOP,
  MOTOR1_POSITIVE,
  MOTOR1_NEGATIVE  
}eMotor1State;

void Motor1Init (void);
void Motor1FSM (void);
void SetMotor1State (eMotor1State motor1StateValue);
eMotor1State GetMotor1State (void);
void SetMotor1TargetState (eMotor1State motor1StateValue);
eMotor1State GetMotor1TargetState (void);
bool SetMotor1AllPwm(uint8_t duty);
bool UpdateMotor1Pwm (uint8_t pwmDuty);
uint8_t GetMotor1Pwm (void);
void OperateMotor1ForTest(eMotor1State targetState);
void Motor1TimeIncrement_ms (void);





#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
