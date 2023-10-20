#ifndef _MOTOR2CONTROL_HAL_H    /* Guard against multiple inclusion */
#define _MOTOR2CONTROL_HAL_H

#include "main.h"

#define MOTOR2_CURRENT_LEVEL_1   1.0
#define MOTOR2_CURRENT_LEVEL_2   1.5
#define MOTOR2_CURRENT_LEVEL_3   2.0
#define MOTOR2_CURRENT_LEVEL_4   4.0

#define MOTOR2_LEVEL_12_TIME     180*1000 //120 SECONDS
#define MOTOR2_LEVEL_23_TIME     60*1000  //60 SECONDS
#define MOTOR2_LEVEL_34_TIME     10*1000  //10 SECONDS

bool motor2_lvl_12_flag;
bool motor2_lvl_23_flag;
bool motor2_lvl_34_flag;
bool motor2_lvl_40_flag;

typedef struct {
  bool mot2Polarity;
} stMot2Polarity;

uint8_t Motor2_Fault;

uint32_t Motor2RuntimeCurrentTimer;
bool Motor2CurrrentTimerError;

void Motor2Init_hal (void);
stMot2Polarity * GetSetMot2Polarity (void);
void Motor2MoveNegative (void);
void Motor2MovePositive (void);
bool UpdateMotor2PositivePwm (uint32_t pwmDuty);
bool UpdateMotor2NegativePwm (uint32_t pwmDuty);
bool UpdateMotor2StopPwm (void);
uint8_t GetMotor2PositivePwm (void);
uint8_t GetMotor2NegativePwm (void);
void Motor2Break (void);
void Motor2Stop (void);
ErrorStatus SetMotor2Forward (void);
ErrorStatus SetMotor2Reverse (void);
uint8_t Motor2Fault(void);

void Motor2CurrentRuntime_inc(void);
uint32_t Motor2Runtime_get(void);
void Motor2Runtime_clear(void);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
