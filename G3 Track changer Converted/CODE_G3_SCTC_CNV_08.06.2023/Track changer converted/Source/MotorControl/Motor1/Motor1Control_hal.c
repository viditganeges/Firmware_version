#include "Motor1Control_hal.h"
#include "Pwm1.h"
#include "board.h"
#include "Serial_Debug.h"

#ifdef SERIAL_DEBUG_MOTOR1CONTROL
#define Serial_Debug_Motor1Control Serial_Debug
#else
#define Serial_Debug_Motor1Control (void)
#endif

static stMot1Polarity motor1Polarity;

#ifdef STEPPER_MOTOR
void Motor1MoveNegative_stepper(void);
void Motor1MovePositive_stepper(void);
bool UpdateMotor1PositiveFrequency(float pwmDuty);
bool UpdateMotor1NegativeFrequency(float pwmDuty);
bool UpdateMotor1StopFrequency(float pwmDuty);
uint32_t GetMotorFrequency(void);
void Motor1Break_stepper(void);
void Motor1Stop_stepper(void);
#endif

static void Motor1InitPins()
{
  gpio_pin_config_t out_config = {
    kGPIO_DigitalOutput, 0,
  };
  
  
  GPIO_PinInit(BOARD_MOTOR_ENABLE_GPIO, BOARD_MOTOR1_ENABLE_GPIO_PIN, &out_config);
#ifdef STEPPER_MOTOR
  GPIO_PinInit(STEPPER_DIRECTION_GPIO, STEPPER_DIRECTION_PIN, &out_config);
#if (HARDWARE_VERSION != HVER_1_0)
  gpio_pin_config_t out_config_1 = {
    kGPIO_DigitalOutput, 1,
  };
  GPIO_PinInit(STEPPER_DRIVE_EN_GPIO, STEPPER_DRIVE_EN_PIN, &out_config_1);
#endif
#endif
  Motor1Disable();
}  

void Motor1Init_hal (void)
{
  motor1Polarity.mot1Polarity = 0;
  Pwm1Init();
  Motor1InitPins();
  Motor1Stop();
  Motor1Enable();
}

stMot1Polarity * GetSetMot1Polarity (void)
{
  return &motor1Polarity;
}

void Motor1MoveNegative (void)                                                   
{
#ifdef STEPPER_MOTOR
  Motor1MoveNegative_stepper();
#else
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    StopPwm1Ch1();
    StartPwm1Ch2(); 
  }
  else
  {
    StopPwm1Ch2();
    StartPwm1Ch1(); 
  }
  Motor1Enable();
#endif
}

void Motor1MoveNegative_stepper(void)
{
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    MotorNegative();
    StartPulse();
  }
  else
  {
    MotorPositive();
    StartPulse();
  }
}
void Motor1MovePositive (void)                                                   
{
#ifdef STEPPER_MOTOR
  Motor1MovePositive_stepper();
#else
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    StopPwm1Ch2();
    StartPwm1Ch1(); 
  }
  else
  {
    StopPwm1Ch1();
    StartPwm1Ch2();  
  }
  Motor1Enable();
#endif
  
}

void Motor1MovePositive_stepper(void)
{
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    MotorPositive();
    StartPulse();
  }
  else
  {
    MotorNegative();
    StartPulse();
  }
}

bool UpdateMotor1PositivePwm (float pwmDuty)                                                   
{
#ifdef STEPPER_MOTOR
  return(UpdateMotor1PositiveFrequency(pwmDuty));
#else
  bool status = false;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    status = SetUpdatePwm1Ch1DutyCycle(pwmDuty);
  }
  else
  {
    status = SetUpdatePwm1Ch2DutyCycle(pwmDuty);
  }
  return status;
#endif
}

bool UpdateMotor1PositiveFrequency(float pwmDuty)
{
  bool status = false;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    MotorPositive();
    status = UpdatePulseFrequencyAndRun((uint32_t)pwmDuty);
  }
  else
  {
    MotorNegative();
    status = UpdatePulseFrequencyAndRun((uint32_t)pwmDuty);
  }
  return status;
}

bool UpdateMotor1NegativePwm (float pwmDuty)                                                   
{
#ifdef STEPPER_MOTOR
  return(UpdateMotor1NegativeFrequency(pwmDuty));
#else
  bool status = false;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    status = SetUpdatePwm1Ch2DutyCycle(pwmDuty);
  }
  else
  {
    status = SetUpdatePwm1Ch1DutyCycle(pwmDuty);
  }
  return status;
#endif
}

bool UpdateMotor1NegativeFrequency(float pwmDuty)
{
  bool status = false;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    MotorNegative();
    status = UpdatePulseFrequencyAndRun((uint32_t)pwmDuty);
  }
  else
  {
    MotorPositive();
    status = UpdatePulseFrequencyAndRun((uint32_t)pwmDuty);
  }
  return status;
}

bool UpdateMotor1StopPwm (float pwmDuty)                                                   
{
#ifdef STEPPER_MOTOR
  return (UpdateMotor1StopFrequency(pwmDuty));
#else
  bool status = true;
  status &= SetUpdatePwm1Ch2DutyCycle(STOP_PWM_VALUE);
  status &= SetUpdatePwm1Ch1DutyCycle(STOP_PWM_VALUE);
  return status;
#endif
}

bool UpdateMotor1StopFrequency(float pwmDuty)
{
  bool status = true;
  //status &= SetUpdatePwm1Ch2DutyCycle(STOP_PWM_VALUE);
  status &= StopPulse();//SetUpdatePwm1Ch1DutyCycle(STOP_PWM_VALUE);
  return status;
}
uint32_t GetMotor1PositivePwm (void)                                                   
{
#ifdef STEPPER_MOTOR
  return(GetMotorFrequency());
#else
  uint8_t pwmDuty;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    pwmDuty = GetPwm1Ch1DutyCycle();
  }
  else
  {
    pwmDuty = GetPwm1Ch2DutyCycle();
  }
  return pwmDuty;
#endif
}

uint32_t GetMotorFrequency(void)
{
  uint32_t pwmDuty;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    pwmDuty = GetPulseFrequency();
  }
  else
  {
    pwmDuty = GetPulseFrequency();
  }
  return pwmDuty;
}
uint32_t GetMotor1NegativePwm (void)                                                   
{
#ifdef STEPPER_MOTOR
  return(GetMotorFrequency());
#else
  uint8_t pwmDuty;
  const stMot1Polarity *motPolarity = GetSetMot1Polarity();
  if(motPolarity->mot1Polarity == 0)
  {
    pwmDuty = GetPwm1Ch2DutyCycle();
  }
  else
  {
    pwmDuty = GetPwm1Ch1DutyCycle();
  }
  return pwmDuty;
#endif
}

void Motor1Break (void)                                                   
{
#ifdef STEPPER_MOTOR
  Motor1Break_stepper();
#else
  Motor1Disable();
  StopPwm1Ch1();
  StopPwm1Ch2(); 
#endif
}

void Motor1Break_stepper(void)
{
  Motor1Disable();
  StopPulse();
  //StopPwm1Ch1();
  //StopPwm1Ch2(); 
}
void Motor1Stop (void)                                            
{
#ifdef STEPPER_MOTOR
  Motor1Stop_stepper();
#else
  ErrorStatus status = SUCCESS;
  StopPwm1Ch1();
  StopPwm1Ch2(); 
  if(status != SUCCESS)
  {
    Error_Handler();
  }
#endif
}

void Motor1Stop_stepper(void)
{
  ErrorStatus status = SUCCESS;
  //StopPwm1Ch1();
  //StopPwm1Ch2();
  StopPulse();
  if(status != SUCCESS)
  {
    Error_Handler();
  }
}


#if (HARDWARE_VERSION != HVER_1_0)
void MotorDriveOn(void)
{
  GPIO_PortClear(STEPPER_DRIVE_EN_GPIO, 1U << STEPPER_DRIVE_EN_PIN); 
}

void MotorDriveOff(void)
{
  GPIO_PortSet(STEPPER_DRIVE_EN_GPIO, 1U << STEPPER_DRIVE_EN_PIN); 
}
#endif

static inline void Motor1Disable (void)
{
  GPIO_PortClear(BOARD_MOTOR_ENABLE_GPIO, 1U << BOARD_MOTOR1_ENABLE_GPIO_PIN); 
}

static inline void Motor1Enable (void)
{
  GPIO_PortSet(BOARD_MOTOR_ENABLE_GPIO, 1U << BOARD_MOTOR1_ENABLE_GPIO_PIN); 
}

static inline void MotorPositive(void)
{
  GPIO_PortSet(STEPPER_DIRECTION_GPIO, 1U << STEPPER_DIRECTION_PIN); 
}

static inline void MotorNegative(void)
{
  GPIO_PortClear(STEPPER_DIRECTION_GPIO, 1U << STEPPER_DIRECTION_PIN); 
}
/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
static void Error_Handler(void)
{
  Serial_Debug_Motor1Control("\nMotor1 Control HAl error ");
}
