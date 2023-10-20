#ifndef _MOTOR1CONTROL_HAL_H    /* Guard against multiple inclusion */
#define _MOTOR1CONTROL_HAL_H

#include "main.h"

#define MOTOR1_CURRENT_LEVEL_1   1.0
#define MOTOR1_CURRENT_LEVEL_2   1.5
#define MOTOR1_CURRENT_LEVEL_3   2.0
#define MOTOR1_CURRENT_LEVEL_4   4.0

#define MOTOR1_LEVEL_12_TIME     180*1000 //120 SECONDS
#define MOTOR1_LEVEL_23_TIME     60*1000  //60 SECONDS
#define MOTOR1_LEVEL_34_TIME     10*1000  //10 SECONDS

bool motor1_lvl_12_flag;
bool motor1_lvl_23_flag;
bool motor1_lvl_34_flag;
bool motor1_lvl_40_flag;

typedef struct {
  bool mot1Polarity;
} stMot1Polarity;

uint8_t Motor1_Fault;

uint32_t Motor1RuntimeCurrentTimer;
bool Motor1CurrrentTimerError;

void Motor1Init_hal (void);
stMot1Polarity * GetSetMot1Polarity (void);
void Motor1MoveNegative (void);
void Motor1MovePositive (void);
bool UpdateMotor1PositivePwm (uint16_t pwmDuty);
bool UpdateMotor1NegativePwm (float pwmDuty);
bool UpdateMotor1Stop (void);
uint8_t GetMotor1PositivePwm (void);
uint8_t GetMotor1NegativePwm (void);
void Motor1Break (void);
void Motor1Stop (void);
ErrorStatus SetMotor1Forward (void);
ErrorStatus SetMotor1Reverse (void);
uint8_t Motor1Fault(void);

void Motor1CurrentRuntime_inc(void);
uint32_t Motor1Runtime_get(void);
void Motor1Runtime_clear(void);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
