#include "main.h"

/* LED Operating Timings

  |--|__|--|__|--|__________________|--|__|--|__|--|____________________

  |  |                                -> On Time
     |  |                             -> Off time
                 |                  | -> Long Off Time
  |              |                    -> No of Blinks (eg. 3)

*/

#define LED_START_TIME_MS         50
#define LED_ON_TIME_MS            0
#define LED_OFF_TIME_NORMAL_MS    100
#define LED_ON_TIME_ERROR1_MS     100
#define LED_OFF_TIME_ERROR1_MS    100
#define LED_OFF_LONG_TIME_MS      1000
#define NO_OF_BLINKS_NORMAL       1
#define NO_OF_BLINKS_ERROR1       2

#define DEFAULT_LED_STATE       false


static void DoLedBlinkFSM(void);
static inline void LedTimerStop (void);
static inline bool IsLedTimeOver (void);
static inline void ClearLedTimeOver (void);
//static void SetLed2State (eLed2State ledState);
//static  eLed2State GetLed2State (void);
static void LedTimerOn (uint32_t setLed4TimeCount_ms);
//static void LedTimerOn (uint32_t setLed2TimeCount_ms);
static eLed4OperateState  GetCurrentOperateState(void);
static void DoLedOperate(void);

typedef enum eLEDState_def
{
  LED_OFF,
  LED_ON
}eLEDState;

typedef enum eLedBlinkState_def
{
  LED_BLINK_IDLE,
  LED_BLINK_LED_ON,
  LED_ON_TIME_WAIT,
  LED_OFF_TIME_WAIT,
  LED_OFF_LONG_TIME_WAIT
}eLedBlinkState;

typedef enum eLed4State_def
{
  LED_IDLE,
  LED_START,
  LED_START_TIME_WAIT,
  LED_OPERATE
}eLed4State;

typedef struct stLedOperateParams_def {
  eLed4OperateState ledOperateState;
  uint8_t noOfSetBlink;
  uint32_t ledOnTime;
  uint32_t ledOffTime;
  uint32_t ledLongOffTime;    
}stLedOperateParams;


static uint8_t noOfSetBlink        = NO_OF_BLINKS_NORMAL;
static uint32_t ledOnTime          = LED_ON_TIME_MS;
static uint32_t ledOffTime         = LED_OFF_TIME_NORMAL_MS;
static uint32_t ledLongOffTime     = LED_OFF_LONG_TIME_MS;

static volatile bool ledTimeOver     = false;
static int32_t led_timecount         = 0;
static bool ledTestMode              = false;
eLed4OperateState errorState4 = LED_4_NORMAL;

static stLedOperateParams ledOperateParams[] = {
  {LED_4_INITIAL, NO_OF_BLINKS_NORMAL, LED_ON_TIME_MS, LED_OFF_TIME_NORMAL_MS, LED_OFF_LONG_TIME_MS},
  {LED_4_NORMAL,  NO_OF_BLINKS_NORMAL, LED_ON_TIME_MS, LED_OFF_TIME_NORMAL_MS, LED_OFF_LONG_TIME_MS},
  {LED_4_ERROR1,  NO_OF_BLINKS_ERROR1, LED_ON_TIME_ERROR1_MS, LED_OFF_TIME_ERROR1_MS, LED_OFF_LONG_TIME_MS}
};

void Led4InitPins(void)
{
//  gpio_pin_config_t led_config = {
//    kGPIO_DigitalOutput, 0,
//  };
  
//  GPIO_PinInit(LED_2_GPIO, LED_2_GPIO_PIN, &led_config);
#if DEFAULT_LED_STATE == true
  SwitchLed4On();
#else
  SwitchLed4Off();
#endif
} 

void SwitchLed4Off (void)
{
//  GPIO_PortClear(LED_2_GPIO, 1U << LED_2_GPIO_PIN);
  GPIO_PC7_LED4_Clear(); 
}

void SwitchLed4On (void)
{
//  GPIO_PortSet(LED_2_GPIO, 1U << LED_2_GPIO_PIN); 
  GPIO_PC7_LED4_Set();
}

void SwitchLed4Toggle (void)
{
//  GPIO_PortToggle(LED_2_GPIO, 1U << LED_2_GPIO_PIN);
  GPIO_PC7_LED4_Toggle();
}

void OperateLed4 (void)
{
  static eLed4State redLedState = LED_IDLE;
  if(ledTestMode == true)
  {
    return;    
  }
  switch (redLedState)
  {
   case LED_IDLE:
 //   Led2InitPins();
    SwitchLed4On();
    redLedState = LED_START;
    break;
   case LED_START:
    SwitchLed4On();
    LedTimerOn(LED_START_TIME_MS);
    redLedState = LED_START_TIME_WAIT;
    break;
   case LED_START_TIME_WAIT:
    if(IsLedTimeOver() == true)
    {
      SwitchLed4Off();
      redLedState = LED_OPERATE;
    }
    break;
   case LED_OPERATE:
        DoLedOperate();
    break;
   default:
    break;
  }
}

static eLed4OperateState  GetCurrentOperateState(void)
{
  errorState4 = LED_4_NORMAL;
  if(IsAnyFault())                           //change for motor running
  {
    errorState4 = LED_4_ERROR1;
  }
  else
  {
    errorState4 = LED_4_NORMAL;
  }
 return errorState4; 
}

void SetLed4OperateState(eLed4OperateState state)
{
errorState4 = state;
}
static void DoLedOperate(void)
{
   static eLed4OperateState redLedPrevOperateState = LED_4_INITIAL;
   eLed4OperateState  redLedOperateState =  GetCurrentOperateState();
   if(redLedPrevOperateState == redLedOperateState)
   {
     DoLedBlinkFSM(); 
     return;
   }
   else
   {
     redLedPrevOperateState = redLedOperateState;     
   }
//  Serial_Debug("\nLED Operate State :");
//  Serial_Debug_Num(redLedOperateState);   
  uint16_t ledOpParamsCount = 0;
  uint16_t ledOpParamsTotalCount = sizeof(ledOperateParams)/sizeof(ledOperateParams[0]);
  for(ledOpParamsCount =0; ledOpParamsCount < ledOpParamsTotalCount; ledOpParamsCount++)
  {
    if(ledOperateParams[ledOpParamsCount].ledOperateState == redLedOperateState)
    {
      noOfSetBlink       = ledOperateParams[ledOpParamsCount].noOfSetBlink;
      ledOnTime          = ledOperateParams[ledOpParamsCount].ledOnTime;
      ledOffTime         = ledOperateParams[ledOpParamsCount].ledOffTime;
      ledLongOffTime     = ledOperateParams[ledOpParamsCount].ledLongOffTime;
      break;
    }
  }
  if(ledOpParamsCount >= ledOpParamsTotalCount)
  {
    noOfSetBlink          = NO_OF_BLINKS_NORMAL;
    ledOnTime          = LED_ON_TIME_MS;
    ledOffTime         = LED_OFF_TIME_NORMAL_MS;
    ledLongOffTime     = LED_OFF_LONG_TIME_MS;
  }
  DoLedBlinkFSM(); 
}

static void DoLedBlinkFSM(void)
{
  static eLedBlinkState redLedBlinkState = LED_BLINK_IDLE;
  static uint8_t noOfBlink = 0;
  switch(redLedBlinkState)
  {
   case LED_BLINK_IDLE:
    SwitchLed4Off();
    redLedBlinkState = LED_BLINK_LED_ON;
    noOfBlink = 0;
    break;
   case LED_BLINK_LED_ON:
    if(ledOnTime)
    {
      SwitchLed4On();
      LedTimerOn(ledOnTime);
      redLedBlinkState = LED_ON_TIME_WAIT;
    }
    break;
   case LED_ON_TIME_WAIT:
    if(IsLedTimeOver() == true)
    {
      if(ledOffTime)
      {
        SwitchLed4Off();
        LedTimerOn(ledOffTime);
        redLedBlinkState = LED_OFF_TIME_WAIT;
      }
    }
    break;
   case LED_OFF_TIME_WAIT:
    if(IsLedTimeOver() == true)
    {
      if(++noOfBlink < noOfSetBlink)
      {
        redLedBlinkState = LED_BLINK_LED_ON;
      }
      else
      {
        if(ledLongOffTime)
        {
          SwitchLed4Off();
          LedTimerOn(ledLongOffTime);
          redLedBlinkState = LED_OFF_LONG_TIME_WAIT;
        }
        else
        {
          redLedBlinkState = LED_BLINK_IDLE;
        }
      }
    }
    break;
   case LED_OFF_LONG_TIME_WAIT:
    if(IsLedTimeOver() == true)
    {
      redLedBlinkState = LED_BLINK_IDLE;
    }
    break;
   default:
    break;
  }
}


static void LedTimerOn (uint32_t setLed4TimeCount_ms)
{
  if(setLed4TimeCount_ms == 0)
  {
    ledTimeOver = true;
    LedTimerStop();
  }
  else
  {
    led_timecount = setLed4TimeCount_ms;
    ledTimeOver = false;
  }
}

static inline void LedTimerStop (void)
{
  led_timecount = 0;
}

static inline bool IsLedTimeOver (void)
{
  return (ledTimeOver);
}

static inline void ClearLedTimeOver (void)
{
  ledTimeOver = false; 
}

void Led4TimeIncrement_ms (void)
{
  if(led_timecount)
  {
    if(--led_timecount <= 0)
    {
      ledTimeOver = true;
      LedTimerStop();
    }
    else
    {
      ClearLedTimeOver();
    }
  }
}

void Led4Test (bool state)
{
  ledTestMode = true;
  if(state == true)
  {
    SwitchLed4On();
  }
  else
  {
    SwitchLed4Off();
  }   
}