/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _LED_H    /* Guard against multiple inclusion */
#define _LED_H

typedef enum eLed1OperateState_def
{
  LED_1_INITIAL,
  LED_1_NORMAL,
  LED_1_ERROR1,
  LED_1_ERROR2  
}eLed1OperateState;

void Led1InitPins(void);
void Led1TimeIncrement_ms (void);
void OperateLed1 (void);
void SwitchLed1Off (void);
void SwitchLed1On (void);
void SwitchLed1Toggle (void);
void Led1Test (bool state);

typedef enum eLed2OperateState_def
{
  LED_2_INITIAL,
  LED_2_NORMAL,
  LED_2_ERROR1,
}eLed2OperateState;

void Led2InitPins(void);
void Led2TimeIncrement_ms (void);
void OperateLed2 (void);
void SwitchLed2Off (void);
void SwitchLed2On (void);
void SwitchLed2Toggle (void);
void Led2Test (bool state);
void SetLed2OperateState(eLed2OperateState state);

typedef enum eLed3OperateState_def
{
  LED_3_INITIAL,
  LED_3_NORMAL,
  LED_3_ERROR1,
  LED_3_ERROR2  
}eLed3OperateState;

void SetVAL1var(int16_t );
void SetVAL2var(int16_t );
void SetVAL3var(int16_t );
int16_t GetVAL1var(void);
int16_t GetVAL2var(void);
int16_t GetVAL3var(void);

void Led3InitPins(void);
void Led3TimeIncrement_ms (void);
void OperateLed3 (void);
void SwitchLed3Off (void);
void SwitchLed3On (void);
void SwitchLed3Toggle (void);
void Led3Test (bool state);


void LedInit (void);
void OperateLeds (void);
void LedTimeIncrement_ms (void);

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
