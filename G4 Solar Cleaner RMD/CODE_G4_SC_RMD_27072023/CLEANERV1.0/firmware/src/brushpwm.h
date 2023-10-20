#ifndef _BRUSHPWM_H    /* Guard against multiple inclusion */
#define _BRUSHPWM_H

#include "main.h"

bool BrushPwmInit(void);
bool SetUpdateBrushMotorForwardDutyCycle (uint8_t dutyCycle);
bool SetUpdateBrushMotorReverseDutyCycle (uint8_t dutyCycle);

uint8_t GetBrushMotorForwardDutyCycle (void);
uint8_t GetBrushMotorReverseDutyCycle (void);

ErrorStatus SetBrushMotorForwardDutyCycle (uint8_t dutyCycle);
ErrorStatus SetBrushMotorReverseDutyCycle (uint8_t dutyCycle);
ErrorStatus StartBrushMotorForward (void);
ErrorStatus StartBrushMotorReverse (void);
ErrorStatus StopBrushMotor (void);


#endif /* _EXAMPLE_FILE_NAME_H */


