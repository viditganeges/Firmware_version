#ifndef _MOTOR2PWM_H    /* Guard against multiple inclusion */
#define _MOTOR2PWM_H

#include "main.h"

#define SERIAL_DEBUG_PWM1

#define STOP_PWM_VALUE          0

#define MOTOR2_PWM          PWM0_REGS
#define MOTOR2_PWM_CHANNEL  PWM_CHANNEL_0

#define SERIAL_DEBUG_MOTOR1CONTROL

#define DEFAULT_MOTOR2_DEAD_TIME_MS        5

bool Motor2PwmInit(void);
ErrorStatus SetMotor2ForwardDutyCycle (uint32_t dutyCycle);
ErrorStatus SetMotor2ReverseDutyCycle (uint32_t dutyCycle);
ErrorStatus SetUpdateMotor2ForwardDutyCycle (uint32_t dutyCycle);
ErrorStatus SetUpdateMotor2ReverseDutyCycle (uint32_t dutyCycle);
uint8_t GetMotor2ForwardDutyCycle (void);
uint8_t GetMotor2ReverseDutyCycle (void);
ErrorStatus StartMotor2Forward (void);
ErrorStatus StartMotor2Reverse (void);
ErrorStatus StopMotor2 (void);


#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
