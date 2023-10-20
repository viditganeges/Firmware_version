#include "main.h"

static stMot2Polarity motor2Polarity;

static void Motor2InitPins(void);
static inline void Motor2Disable (void);
static inline void Motor2Enable (void);
 
static void Motor2InitPins(void)        //done//
{
  Motor2Disable();
}  

void Motor2Init_hal (void)      //done//
{
  motor2Polarity.mot2Polarity = 0;
  Motor2PwmInit();
  Motor2InitPins();
  Motor2Stop();
//  Motor2Enable();
//  Motor2Disable();
}

stMot2Polarity * GetSetMot2Polarity (void)      //done//
{
  return &motor2Polarity;
}

void Motor2MoveNegative (void)         //done//                                          
{
  const stMot2Polarity *motPolarity = GetSetMot2Polarity();
  
  if(motPolarity->mot2Polarity == 0)
  {
    SetMotor2Reverse();
    StartMotor2Reverse(); 
  }
  else
  {
    SetMotor2Forward();
    StartMotor2Forward(); 
  }
  
//  SYSTICK_DelayMs(300);
  
//  Motor2Enable();
  MOTORB_IN1_Set();
  MOTORB_PD11_nsleep_Clear();
}

void Motor2MovePositive (void)       //done//                                            
{
  const stMot2Polarity *motPolarity = GetSetMot2Polarity();
  
  if(motPolarity->mot2Polarity == 0)
  {
    SetMotor2Forward();
    StartMotor2Forward(); 
  }
  else
  {
    SetMotor2Reverse();
    StartMotor2Reverse();  
  }
  
//  Motor2Enable();
  MOTORB_IN1_Clear();
  MOTORB_PD11_nsleep_Set();
}

bool UpdateMotor2PositivePwm (uint32_t pwmDuty)               //done//                                      
{
  bool status = false;
  const stMot2Polarity *motPolarity = GetSetMot2Polarity();
  if(motPolarity->mot2Polarity == 0)
  {
    status = SetUpdateMotor2ForwardDutyCycle(pwmDuty);
  }
  else
  {
    status = SetUpdateMotor2ReverseDutyCycle(pwmDuty);
  }
  return status;
}

bool UpdateMotor2NegativePwm (uint32_t pwmDuty)               //done//                                    
{
  bool status = false;
  const stMot2Polarity *motPolarity = GetSetMot2Polarity();
  if(motPolarity->mot2Polarity == 0)
  {
    status = SetUpdateMotor2ReverseDutyCycle(pwmDuty);
  }
  else
  {
    status = SetUpdateMotor2ForwardDutyCycle(pwmDuty);
  }
  return status;
}

bool UpdateMotor2StopPwm (void)                  //done//                                               
{
  bool status = true;
  status &= StopMotor2();
  return status;
}

uint8_t GetMotor2PositivePwm (void)                       //done//                                  
{
  uint8_t pwmDuty;
  const stMot2Polarity *motPolarity = GetSetMot2Polarity();
  if(motPolarity->mot2Polarity == 0)
  {
    pwmDuty = GetMotor2ForwardDutyCycle();
  }
  else
  {
    pwmDuty = GetMotor2ReverseDutyCycle();
  }
  return pwmDuty;
}

uint8_t GetMotor2NegativePwm (void)                      //done//                             
{
  uint8_t pwmDuty;
  const stMot2Polarity *motPolarity = GetSetMot2Polarity();
  if(motPolarity->mot2Polarity == 0)
  {
    pwmDuty = GetMotor2ReverseDutyCycle();
  }
  else
  {
    pwmDuty = GetMotor2ForwardDutyCycle();
  }
  return pwmDuty;
}

void Motor2Break (void)              //done//                                     
{
//    Motor2Disable();
//    StopMotor2();
    
    MOTORB_IN1_Clear();
    MOTORB_PD11_nsleep_Clear();
}

void Motor2Stop (void)         //done//                                   
{
  ErrorStatus status = SUCCESS;
  Motor2Break();
//  StopMotor2(); 
  if(status != SUCCESS)
  {
//    Error_Handler();
  }
}

ErrorStatus SetMotor2Forward (void)     //done//
{
  ErrorStatus status = SUCCESS;
  
  MOTORB_IN1_Clear();
  
  return status;
}

ErrorStatus SetMotor2Reverse (void)     //done//
{
  ErrorStatus status = SUCCESS;
  
  MOTORB_IN1_Set();
  
  return status;
}

static inline void Motor2Disable (void)     //done//
{
   MOTORB_PD11_nsleep_Clear();
}

static inline void Motor2Enable (void)  //done//
{
   MOTORB_PD11_nsleep_Set(); 
}

uint8_t Motor2Fault(void)
{
    if(MOTORB_PC16_lowfault_Get() == 0)
    {
        return 1; 
    }
    else
    {
        return 0;
    }
}


void Motor2CurrentRuntime_inc(void)
{   
    if(motor2A<MOTOR2_CURRENT_LEVEL_1)
    {
        Motor2Runtime_clear();
        motor2_lvl_12_flag=false;
        motor2_lvl_23_flag=false;
        motor2_lvl_34_flag=false;
        motor2_lvl_40_flag=false;
    }
    else if((motor2_lvl_12_flag == false)&&(motor2A>MOTOR2_CURRENT_LEVEL_1)&&(motor2A<MOTOR2_CURRENT_LEVEL_2))
    {
        Motor2Runtime_clear();
        motor2_lvl_12_flag=true;
        motor2_lvl_23_flag=false;
        motor2_lvl_34_flag=false;
        motor2_lvl_40_flag=false;
    }
    else if((motor2_lvl_23_flag == false)&&(motor2A>MOTOR2_CURRENT_LEVEL_2)&&(motor2A<MOTOR2_CURRENT_LEVEL_3))
    {
        Motor2Runtime_clear();
        motor2_lvl_12_flag=false;
        motor2_lvl_23_flag=true;
        motor2_lvl_34_flag=false;
        motor2_lvl_40_flag=false;
    }
    else if((motor2_lvl_34_flag == false)&&(motor2A>MOTOR2_CURRENT_LEVEL_3)&&(motor2A<MOTOR2_CURRENT_LEVEL_4))
    {
        Motor2Runtime_clear();
        motor2_lvl_12_flag=false;
        motor2_lvl_23_flag=false;
        motor2_lvl_34_flag=true;
        motor2_lvl_40_flag=false;
    }
    else if(motor2A>MOTOR2_CURRENT_LEVEL_4)
    {
        Motor2Runtime_clear();
        motor2_lvl_12_flag=false;
        motor2_lvl_23_flag=false;
        motor2_lvl_34_flag=false;
        motor2_lvl_40_flag=true;
    }
    
    if((motor2_lvl_12_flag == true)||(motor2_lvl_23_flag == true)||(motor2_lvl_34_flag == true))
    {
        if(Motor2RuntimeCurrentTimer < 0xFFFFFFFF)
        {
            Motor2RuntimeCurrentTimer++;
        }
        else
        {
            Motor2RuntimeCurrentTimer = 0;
        }
    }
}

uint32_t Motor2Runtime_get(void)
{
    return Motor2RuntimeCurrentTimer;
}

void Motor2Runtime_clear(void)
{
    Motor2RuntimeCurrentTimer = 0;
}