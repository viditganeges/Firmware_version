

#ifndef _MOTOR1CONTROL_H    /* Guard against multiple inclusion */
#define _MOTOR1CONTROL_H


#include "main.h"

#define DEFAULT_MOTOR1_DEAD_TIME_MS        5

#define STEPPER_MOTOR

#define STOP_PWM_VALUE          0

#define STEPPER_PWM_CHANNEL           PWM_CHANNEL_3

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
bool SetMotor1AllPwm(uint16_t duty);
bool UpdateMotor1Pwm (uint16_t pwmDuty);
uint32_t GetMotor1Pwm (void);
void OperateMotor1ForTest(eMotor1State targetState);
void Motor1TimeIncrement_ms (void);

typedef struct {
  bool mot1Polarity;
} stMot1Polarity;

void Motor1Init_hal (void);
stMot1Polarity * GetSetMot1Polarity (void);
void Motor1MoveNegative (void);
void Motor1MoveNegative_stepper(void);
void Motor1MovePositive (void);
void Motor1MovePositive_stepper(void);
bool UpdateMotor1PositivePwm (uint16_t pwmDuty);
bool UpdateMotor1PositiveFrequency(uint16_t pwmDuty);
bool UpdateMotor1NegativePwm (uint16_t pwmDuty);
bool UpdateMotor1NegativeFrequency(uint16_t pwmDuty);
bool UpdateMotor1StopPwm (uint16_t pwmDuty);
bool UpdateMotor1StopFrequency(uint16_t pwmDuty);
uint32_t GetMotor1PositivePwm (void);
uint32_t GetMotorFrequency(void);
uint32_t GetMotor1NegativePwm (void);
void Motor1Break (void);
void Motor1Break_stepper(void);
void Motor1Stop (void);
void Motor1Stop_stepper(void);

void MotorDriveOn(void);
void MotorDriveOff(void);


bool Motor1PwmInit(void);
bool UpdatePulseFrequencyAndRun(uint32_t freq);
ErrorStatus StartPulse(void);
ErrorStatus StopPulse(void);
ErrorStatus SetPulseFrequency(uint32_t frequency);
uint32_t GetPulseFrequency(void);
ErrorStatus SetPwmDutyCycle (uint16_t dutyCycle);
bool SetUpdatePwmDutyCycle (uint16_t dutyCycle);
ErrorStatus StartPwm (void);
ErrorStatus StopPwm (void);

#endif /* _EXAMPLE_FILE_NAME_H */