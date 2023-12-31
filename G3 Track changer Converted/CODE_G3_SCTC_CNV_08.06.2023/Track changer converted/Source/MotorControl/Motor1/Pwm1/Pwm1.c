#include "Pwm1.h"
#include "fsl_ftm.h"
#include "Serial_Debug.h"
#include <math.h>
#include <stdbool.h>

#ifdef SERIAL_DEBUG_PWM1
#define Serial_Debug_Pwm1     Serial_Debug
#define Serial_Debug_Pwm1_Num Serial_Debug_Num
#else
#define Serial_Debug_Pwm1     (void)
#define Serial_Debug_Pwm1_Num (void)
#endif

#define MINIMUM_PULSE_FREQUENCY                 8U
#define MAXIMUM_PULSE_FREQUENCY                 200000U

#define DEFAULT_PWM1_CH1_DUTY_CYCLE             50
#define DEFAULT_PWM1_CH2_DUTY_CYCLE             50

static uint8_t pwm1Ch1DutyCycle = DEFAULT_PWM1_CH1_DUTY_CYCLE;
static uint8_t pwm1Ch2DutyCycle = DEFAULT_PWM1_CH2_DUTY_CYCLE;

static uint32_t pulseFrequency = MINIMUM_PULSE_FREQUENCY;
//static bool stopFlag = false;
static ftm_config_t ftmInfo;

static ftm_chnl_pwm_signal_param_t ftmParam[2];

bool Pwm1Init(void)
{  
  
  
  /* Configure ftm params with frequency 24kHZ */
  ftmParam[0].chnlNumber = (ftm_chnl_t)PWM1_CHANNEL1;
  ftmParam[0].level = kFTM_HighTrue;
  ftmParam[0].dutyCyclePercent = 50U;
  ftmParam[0].firstEdgeDelayPercent = 0U;
  
  ftmParam[1].chnlNumber = (ftm_chnl_t)PWM1_CHANNEL2;
  ftmParam[1].level = kFTM_HighTrue;
  ftmParam[1].dutyCyclePercent = 50U;
  ftmParam[1].firstEdgeDelayPercent = 0U;
  
  /*
  * ftmInfo.prescale = kFTM_Prescale_Divide_1;
  * ftmInfo.bdmMode = kFTM_BdmMode_0;
  * ftmInfo.pwmSyncMode = kFTM_SoftwareTrigger;
  * ftmInfo.reloadPoints = 0;
  * ftmInfo.faultMode = kFTM_Fault_Disable;
  * ftmInfo.faultFilterValue = 0;
  * ftmInfo.deadTimePrescale = kFTM_Deadtime_Prescale_1;
  * ftmInfo.deadTimeValue = 0;
  * ftmInfo.extTriggers = 0;
  * ftmInfo.chnlInitState = 0;
  * ftmInfo.chnlPolarity = 0;
  * ftmInfo.useGlobalTimeBase = false;
  */
  FTM_GetDefaultConfig(&ftmInfo);
  /* Initialize FTM module */
  ftmInfo.prescale = kFTM_Prescale_Divide_128;
  FTM_Init(PWM1_FTM_BASEADDR, &ftmInfo);
  
  FTM_SetupPwm(PWM1_FTM_BASEADDR, ftmParam, 1U, kFTM_EdgeAlignedPwm, 8U, FTM_SOURCE_CLOCK);
  
  //FTM_StartTimer(PWM1_FTM_BASEADDR, kFTM_SystemClock);
  //  if(SetPwm1Ch1DutyCycle(pwm1Ch1DutyCycle) != SUCCESS)
  //  {
  //    Error_Handler();
  //    return false;
  //  }
  //  if(SetPwm1Ch2DutyCycle(pwm1Ch2DutyCycle) != SUCCESS)
  //  {
  //    Error_Handler();
  //    return false;
  //  }
  return true;
}

bool UpdatePulseFrequencyAndRun(uint32_t freq)
{
  SetPwm1Ch1DutyCycle(50);
  pulseFrequency = freq;
// Serial_Debug("Frequecy to Set->");
// Serial_Debug_Num(pulseFrequency);
  StopPwm1Ch1();
  //StopPwm1Ch2();  
  FTM_StopTimer(PWM1_FTM_BASEADDR);
  FTM_SetupPwm(PWM1_FTM_BASEADDR, ftmParam, 1U, kFTM_EdgeAlignedPwm, pulseFrequency, FTM_SOURCE_CLOCK);
  FTM_StartTimer(PWM1_FTM_BASEADDR, kFTM_SystemClock);
  StartPwm1Ch1();
  //StartPwm1Ch2();
  return true;
}

ErrorStatus StartPulse(void)
{
  SetPwm1Ch1DutyCycle(50);
  StopPwm1Ch1(); 
  FTM_StopTimer(PWM1_FTM_BASEADDR);
  FTM_SetupPwm(PWM1_FTM_BASEADDR, ftmParam, 1U, kFTM_EdgeAlignedPwm, pulseFrequency, FTM_SOURCE_CLOCK);
  FTM_StartTimer(PWM1_FTM_BASEADDR, kFTM_SystemClock);
  StartPwm1Ch1();
  return SUCCESS;
}

ErrorStatus StopPulse(void)
{
  //StopPwm1Ch1();
  //StopPwm1Ch2();  
  //FTM_StopTimer(PWM1_FTM_BASEADDR);
  
  SetUpdatePwm1Ch1DutyCycle(100);
  
  return SUCCESS;
}

ErrorStatus SetPulseFrequency(uint32_t frequency)
{
ErrorStatus status = SUCCESS;
  if(frequency < MINIMUM_PULSE_FREQUENCY)
  {
  pulseFrequency = MINIMUM_PULSE_FREQUENCY;
  StopPulse();
  }
  else if(frequency >= MINIMUM_PULSE_FREQUENCY && frequency <= MAXIMUM_PULSE_FREQUENCY)
  {
    pulseFrequency = (uint32_t)(frequency);
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return status;
}

uint32_t GetPulseFrequency(void)
{
return (pulseFrequency);
}

ErrorStatus SetPwm1Ch1DutyCycle (float dutyCycle)
{
  ErrorStatus status = SUCCESS;
  int16_t duty = (int16_t)(round(dutyCycle));
  if(duty >= 0 && duty <= 100)
  {
    pwm1Ch1DutyCycle = (uint8_t)duty;
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return status;
}

ErrorStatus SetPwm1Ch2DutyCycle (float dutyCycle)
{
  ErrorStatus status = SUCCESS;
  int16_t duty = (int16_t)(round(dutyCycle));
  if(duty >= 0 && duty <= 100)
  {
    pwm1Ch2DutyCycle = (uint8_t)duty;
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }
  return status;
}

bool SetUpdatePwm1Ch1DutyCycle (float dutyCycle)
{
  ErrorStatus status = SUCCESS;
  status = SetPwm1Ch1DutyCycle(dutyCycle);
  if(status == SUCCESS)
  {
    StartPwm1Ch1();
  }
  return (status == SUCCESS);
}

bool SetUpdatePwm1Ch2DutyCycle (float dutyCycle)
{
  ErrorStatus status = SUCCESS;
  status = SetPwm1Ch2DutyCycle(dutyCycle);
  if(status == SUCCESS)
  {
    StartPwm1Ch2();
  }
  return (status == SUCCESS);
}

uint8_t GetPwm1Ch1DutyCycle (void)
{
  return pwm1Ch1DutyCycle;
}

uint8_t GetPwm1Ch2DutyCycle (void)
{
  return pwm1Ch2DutyCycle;
}

ErrorStatus StartPwm1Ch1 (void)
{
  ErrorStatus status = SUCCESS;
  
  /* Start PWM mode with updated duty cycle */
  FTM_UpdatePwmDutycycle(PWM1_FTM_BASEADDR, (ftm_chnl_t)PWM1_CHANNEL1, kFTM_EdgeAlignedPwm,
                         pwm1Ch1DutyCycle);
  /* Software trigger to update registers */
  FTM_SetSoftwareTrigger(PWM1_FTM_BASEADDR, true);
  // Serial_Debug_Pwm1("\n PWM1 CH1 Duty Cycle Output: ");
  // Serial_Debug_Pwm1_Num(pwm1Ch1DutyCycle);
  return status;  
}

ErrorStatus StartPwm1Ch2 (void)
{
  ErrorStatus status = SUCCESS;
  
  /* Start PWM mode with updated duty cycle */
  FTM_UpdatePwmDutycycle(PWM1_FTM_BASEADDR, (ftm_chnl_t)PWM1_CHANNEL2, kFTM_EdgeAlignedPwm,
                         pwm1Ch2DutyCycle);
  /* Software trigger to update registers */
  FTM_SetSoftwareTrigger(PWM1_FTM_BASEADDR, true);
  // Serial_Debug_Pwm1("\n PWM1 CH2 Duty Cycle Output: ");
  // Serial_Debug_Pwm1_Num(pwm1Ch2DutyCycle);
  return status;  
}

ErrorStatus StopPwm1Ch1 (void)
{
  ErrorStatus status = SUCCESS;
  
  uint8_t duty_cycle = 0;
  /* Start PWM mode with updated duty cycle */
  FTM_UpdatePwmDutycycle(PWM1_FTM_BASEADDR, (ftm_chnl_t)PWM1_CHANNEL1, kFTM_EdgeAlignedPwm,
                         duty_cycle);
  /* Software trigger to update registers */
  FTM_SetSoftwareTrigger(PWM1_FTM_BASEADDR, true);
  // Serial_Debug_Pwm1("\n PWM1 CH1 Stop Duty Cycle Output: ");
  // Serial_Debug_Pwm1_Num(duty_cycle);
  return status;  
}

ErrorStatus StopPwm1Ch2 (void)
{
  ErrorStatus status = SUCCESS;
  
  uint8_t duty_cycle = 0;
  /* Start PWM mode with updated duty cycle */
  FTM_UpdatePwmDutycycle(PWM1_FTM_BASEADDR, (ftm_chnl_t)PWM1_CHANNEL2, kFTM_EdgeAlignedPwm,
                         duty_cycle);
  /* Software trigger to update registers */
  FTM_SetSoftwareTrigger(PWM1_FTM_BASEADDR, true);
  // Serial_Debug_Pwm1("\n PWM1 CH2 Stop Duty Cycle Output: ");
  // Serial_Debug_Pwm1_Num(duty_cycle);
  return status;  
}

//ErrorStatus StopPwm1 (void)
//{
//  ErrorStatus status;
//  
//  sConfig.Pulse = 0;
//  stopFlag = true;
//  if (HAL_TIM_PWM1_ConfigChannel(&Pwm1TimHandle, &sConfig, TIM_CHANNEL_3) != HAL_OK)
//  {
//    Error_Handler();
//    status = ERROR; 
//  }
//  else
//  {
//    if(HAL_TIM_PWM1_Start(&Pwm1TimHandle, TIM_CHANNEL_3) != HAL_OK)
//    {
//      Error_Handler();
//      status = ERROR; 
//    }
//    else
//    {
//      status = SUCCESS; 
//    }
//  }
//  return status;  
//}

//uint8_t GetCurrentPwm1Duty (void)
//{
//  if(stopFlag == true)
//  {
//    return 0;
//  }
//  else
//  {
//    return(pwm1DutyCycle);
//  }
//}

/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
static void Error_Handler(void)
{
  Serial_Debug_Pwm1("\n PWM1 Error ");
}
