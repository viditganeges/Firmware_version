#ifndef _BRUSHMOTORCONTROL_HAL_H    /* Guard against multiple inclusion */
#define _BRUSHMOTORCONTROL_HAL_H

#include "main.h"

#define MOTOR3_CURRENT_LEVEL_1   3.5
#define MOTOR3_CURRENT_LEVEL_2   4.0
#define MOTOR3_CURRENT_LEVEL_3   4.5
#define MOTOR3_CURRENT_LEVEL_4   6.0

#define MOTOR3_LEVEL_12_TIME     120*1000 //120 SECONDS
#define MOTOR3_LEVEL_23_TIME     60*1000  //60 SECONDS
#define MOTOR3_LEVEL_34_TIME     10*1000  //10 SECONDS

bool motor3_lvl_12_flag;
bool motor3_lvl_23_flag;
bool motor3_lvl_34_flag;
bool motor3_lvl_40_flag;

typedef struct {
  bool mot1Polarity;
} stBrushMotPolarity;

void BrushMotorInit_hal (void);
void BrushMotorMoveNegative (void);
void BrushMotorMovePositive (void);
void BrushMotorBreak (void);
void BrushMotorStop (void);
void Motor3CurrentRuntime_inc(void);
void Motor3Runtime_clear(void);

uint8_t BrushMotorFault(void);
uint8_t BrushMotor_Fault;
uint8_t GetBrushMotorPositivePwm (void);
uint8_t GetBrushMotorNegativePwm (void);

uint32_t Motor3Runtime_get(void);
uint32_t Motor3RuntimeCurrentTimer;

bool Motor3CurrrentTimerError;
bool UpdateBrushMotorPositivePwm (uint8_t pwmDuty);
bool UpdateBrushMotorNegativePwm (uint8_t pwmDuty);
bool UpdateBrushMotorStopPwm (void);

ErrorStatus SetBrushMotorForward (void);
ErrorStatus SetBrushMotorReverse (void);

stBrushMotPolarity * GetSetBrushMotPolarity (void);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
