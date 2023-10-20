#include "main.h"

float GetImot1Value (void)
{
//    return (ADC_ILOAD_CALC(Imot1AdcValue) + Imot1OffsetValue);
    return current_read_A();
}

float GetImot2Value (void)
{
//  return (ADC_ILOAD_CALC(Imot2AdcValue) + Imot2OffsetValue);
    return current_read_B();
}

float GetImot3Value (void)
{
//  return (ADC_ILOAD_CALC(Imot3AdcValue) + Imot3OffsetValue);
    return current_read_C();
}

uint8_t CURRENT_CH1_CONFIG[] = {0x98};
uint8_t CURRENT_CH2_CONFIG[] = {0xB8};

float AbsoluteValue(float value)
{
    if(value < 0)
    {
        return (value*(-1));
    }
    else
    {
        return value;
    }
}

float current_read_A(void)
{
    static uint8_t tempch1[3];
    static uint8_t tempch2[3];
    
    static uint32_t reading1;
    static uint32_t reading2;
    
    static float current_A1;
    static float current_A2;
    
    
    TWIHS2_Write(CURRENT_AADR_A, CURRENT_CH1_CONFIG, 1);
    
    while(1)
    {
        memset(tempch1, 0, sizeof(tempch1));
        TWIHS2_Read(CURRENT_AADR_A, tempch1, 3);
        
        SYSTICK_DelayMs(75);
        
        if(tempch1[2] == 0x18)
        {
            reading1 = ((tempch1[0]&0x07)<<8) + tempch1[1];
            reading1 = reading1 * 62.5;
            if(tempch1[0]&0x80)
            {
                reading1 = reading1*(-1);
            }
            break;
        }
    }
    
    TWIHS2_Write(CURRENT_AADR_A, CURRENT_CH2_CONFIG, 1);
    
    while(1)
    {
        memset(tempch2, 0, sizeof(tempch2));
        TWIHS2_Read(CURRENT_AADR_A, tempch2, 3);
        
        SYSTICK_DelayMs(75);
        
        if(tempch2[2] == 0x38)
        {
            reading2 = ((tempch2[0]&0x07)<<8) + tempch2[1];
            reading2 = reading2 * 62.5;
            if(tempch2[0]&0x80)
            {
                reading2 = reading2*(-1);
            }
            break;
        }
    }
    
//    current_A = ((reading1 + reading2)/2)*20;                         //((reading1+reading2)/2)*10
      current_A1 = (float)reading1/100000;
      current_A2 = (float)reading2/100000;
    
     return current_A1+current_A2;
}

float current_read_B(void)
{
    static uint8_t tempch1[3];
    static uint8_t tempch2[3];
    
    static uint32_t reading1;
    static uint32_t reading2;
    
    static float current_B1;
    static float current_B2;
    
    
    TWIHS2_Write(CURRENT_AADR_B, CURRENT_CH1_CONFIG, 1);
    
    while(1)
    {
        memset(tempch1, 0, sizeof(tempch1));
        TWIHS2_Read(CURRENT_AADR_B, tempch1, 3);
        
        SYSTICK_DelayMs(75);
        
        if(tempch1[2] == 0x18)
        {
            reading1 = ((tempch1[0]&0x07)<<8) + tempch1[1];
            reading1 = reading1 * 62.5;
            if(tempch1[0]&0x80)
            {
                reading1 = reading1*(-1);
            }
            break;
        }
    }
    
    TWIHS2_Write(CURRENT_AADR_B, CURRENT_CH2_CONFIG, 1);
    
    while(1)
    {
        memset(tempch2, 0, sizeof(tempch2));
        TWIHS2_Read(CURRENT_AADR_B, tempch2, 3);
        
        SYSTICK_DelayMs(75);
        
        if(tempch2[2] == 0x38)
        {
            reading2 = ((tempch2[0]&0x07)<<8) + tempch2[1];
            reading2 = reading2 * 62.5;
            if(tempch2[0]&0x80)
            {
                reading2 = reading2*(-1);
            }
            break;
        }
    }
    
//    current_A = ((reading1 + reading2)/2)*20;                         //((reading1+reading2)/2)*10
      current_B1 = (float)reading1/100000;
      current_B2 = (float)reading2/100000;
    
     return current_B1+current_B2;
}

float current_read_C(void)
{
    static uint8_t tempch1[3];
    static uint8_t tempch2[3];
    
    static uint32_t reading1;
    static uint32_t reading2;
    
    static float current_C1;
    static float current_C2;
    
    
    TWIHS2_Write(CURRENT_AADR_C, CURRENT_CH1_CONFIG, 1);
    
    while(1)
    {
        memset(tempch1, 0, sizeof(tempch1));
        TWIHS2_Read(CURRENT_AADR_C, tempch1, 3);
        
        SYSTICK_DelayMs(75);
        
        if(tempch1[2] == 0x18)
        {
            reading1 = ((tempch1[0]&0x07)<<8) + tempch1[1];
            reading1 = reading1 * 62.5;
            if(tempch1[0]&0x80)
            {
                reading1 = reading1*(-1);
            }
            break;
        }
    }
    
    TWIHS2_Write(CURRENT_AADR_C, CURRENT_CH2_CONFIG, 1);
    
    while(1)
    {
        memset(tempch2, 0, sizeof(tempch2));
        TWIHS2_Read(CURRENT_AADR_C, tempch2, 3);
        
        SYSTICK_DelayMs(75);
        
        if(tempch2[2] == 0x38)
        {
            reading2 = ((tempch2[0]&0x07)<<8) + tempch2[1];
            reading2 = reading2 * 62.5;
            if(tempch2[0]&0x80)
            {
                reading2 = reading2*(-1);
            }
            break;
        }
    }
    
//    current_A = ((reading1 + reading2)/2)*20;                         //((reading1+reading2)/2)*10
      current_C1 = (float)reading1/100000;
      current_C2 = (float)reading2/100000;
    
     return current_C1+current_C2;
}

//bool IsMotorOverCurrentFault (void)
//{
//  bool status = true;
//  status &= IsMot1OverCurrentFault();
//  status &= IsMot2OverCurrentFault();
//  status &= IsMot3OverCurrentFault();
//  return status;  
//}
//
//void ClearMotorOverCurrentFault (void)
//{
//  ClearMotor1OverCurrentFault();
//  ClearMotor2OverCurrentFault();
//  ClearMotor3OverCurrentFault();  
//}
//
//bool IsMotor1OverCurrentFault (void)
//{
//  return IsMot1OverCurrentFault();
//}
//
//bool IsMotor2OverCurrentFault (void)
//{
//  return IsMot2OverCurrentFault();
//}
//
//bool IsMotor3OverCurrentFault (void)
//{
//  return IsMot3OverCurrentFault();
//}
//
void ClearMotor1OverCurrentFault (void)
{
  ClearMot1OverCurrentFault();  
  ClearMot1MaxOverCurrentCount();
}

void ClearMotor2OverCurrentFault (void)
{
  ClearMot2OverCurrentFault();  
  ClearMot2MaxOverCurrentCount();
}

void ClearMotor3OverCurrentFault (void)
{
  ClearMot3OverCurrentFault();  
  ClearMot3MaxOverCurrentCount();
}
  
void CheckMotorOverLoad (void)
{
//  CheckADCValues();
  CheckMot1OverLoad();
  CheckMot2OverLoad();
  CheckMot3OverLoad();
}
//
//void CheckADCValues (void)
//{
//  CheckADC();  
//}
//
void IloadMotMaxTimeIncrement_ms (void)
{
  IloadMot1MaxTimeIncrement_ms();
  IloadMot2MaxTimeIncrement_ms();
  IloadMot3MaxTimeIncrement_ms();
}
//
//void FindAndUpdateImotOffsetValue (void)
//{
//  CheckADCValues();
//  CheckADCValues();
//  CheckADCValues();  
//  UpdateImotOffsetValue();  
//}
//
//stnoofrows* GetSetNoofrowsLimits(void)
//{
//  return(&maxNoofrowsLimits);
//}
//
//void GetNoofrowsLimits(stnoofrows * NoofrowsLimits)
//{
//  *NoofrowsLimits = maxNoofrowsLimits;
//}
