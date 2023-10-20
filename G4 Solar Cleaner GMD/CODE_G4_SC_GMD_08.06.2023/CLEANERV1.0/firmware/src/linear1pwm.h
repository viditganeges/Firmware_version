#ifndef _MOTOR1PWM_H    /* Guard against multiple inclusion */
#define _MOTOR1PWM_H

#include "main.h"

#define SERIAL_DEBUG_PWM1

#define STOP_PWM_VALUE          0

#define MOTOR1_PWM          PWM0_REGS
#define MOTOR1_PWM_CHANNEL  PWM_CHANNEL_1

#define SERIAL_DEBUG_MOTOR1CONTROL

#define DEFAULT_MOTOR1_DEAD_TIME_MS        5

bool Motor1PwmInit(void);
ErrorStatus SetMotor1ForwardDutyCycle (uint16_t dutyCycle);
ErrorStatus SetMotor1ReverseDutyCycle (uint16_t dutyCycle);
bool SetUpdateMotor1ForwardDutyCycle (uint16_t dutyCycle);
bool SetUpdateMotor1ReverseDutyCycle (uint16_t dutyCycle);
uint8_t GetMotor1ForwardDutyCycle (void);
uint8_t GetMotor1ReverseDutyCycle (void);
ErrorStatus StartMotor1Forward (void);
ErrorStatus StartMotor1Reverse (void);
ErrorStatus StopMotor1 (void);



#endif /* _EXAMPLE_FILE_NAME_H */

