#include "main.h"

float GetImot1Value (void)
{
    return current_read_A();
}

float GetImot2Value (void)
{
    return current_read_B();
}

float GetImot3Value (void)
{
    return current_read_C();
}

uint8_t CURRENT_CH1_CONFIG[] = {0x90};
uint8_t CURRENT_CH2_CONFIG[] = {0xB0};

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

//float current_read_A(void)
//{
//    static uint8_t tempch1[3];
//    static uint8_t tempch2[3];
//    
//    static int reading1;
//    static int reading2;
//    
//    static float current_A1;
//    static float current_A2;
//    
//
//    TWIHS2_Write(CURRENT_AADR_A, CURRENT_CH1_CONFIG, 1);
//    Twihs2_BusywaitandRecover();
//    memset(tempch1, 0, sizeof(tempch1));
//    SYSTICK_DelayMs(10);
//
//    TWIHS2_Read(CURRENT_AADR_A, tempch1, 3);
//    Twihs2_BusywaitandRecover();
//
//    if(tempch1[2] == 0x10)
//    {
//        if(tempch1[0]&0x08)
//        {
//            reading1 = ~(tempch1[0]&0x07);
//            reading1 =  reading1<<8;
//            reading1 |= ~(tempch1[1]);
//            reading1 = reading1+1;
//        }
//        else
//        {
//            reading1 = tempch1[0]&0x07;
//            reading1 =  reading1<<8;
//            reading1 |= tempch1[1];
//        }
//        
//        if(reading1>0)
//        {
//            reading1 = reading1-2;
//        }
//        if(reading1<0)
//        {
//            reading1 = 0;
//        }
//    }
//    else 
//    {
//        reading1 = 0;
//    }
//
//
//    TWIHS2_Write(CURRENT_AADR_A, CURRENT_CH2_CONFIG, 1);
//    Twihs2_BusywaitandRecover();
//    memset(tempch2, 0, sizeof(tempch2));
//    SYSTICK_DelayMs(10);
//
//    TWIHS2_Read(CURRENT_AADR_A, tempch2, 3);
//    Twihs2_BusywaitandRecover();
//
//
//
//    if(tempch2[2] == 0x30)
//    {
//        if(tempch2[0]&0x08)
//        {
//            reading2 = ~(tempch2[0]&0x07);
//            reading2 =  reading2<<8;
//            reading2 |= ~(tempch2[1]);
//            reading2 = reading2+1;
//        }
//        else
//        {
//            reading2 = tempch2[0]&0x07;
//            reading2 =  reading2<<8;
//            reading2 |= tempch2[1];
//        }
//        
//        if(reading2>0)
//        {
//            reading2 = reading2-2;
//        }
//        if(reading2<0)
//        {
//            reading2 = 0;
//        }
//    }
//    else 
//    {
//        reading2 = 0;
//    }
//    
//    current_A1 = (float)reading1;
//    current_A2 = (float)reading2;
//
//    float motorA_current;
//
//    motorA_current = current_A1-current_A2;
//    if(motorA_current < 0)
//    {
//        motorA_current *= -1;
//    }
//    motorA_current = trunc((motorA_current*100)/133.0)/100; 
//    return motorA_current;
//}
//
//float current_read_B(void)
//{
//    static uint8_t tempch1[3];
//    static uint8_t tempch2[3];
//    
//    static int reading1;
//    static int reading2;
//    
//    static float current_B1;
//    static float current_B2;
//        
//    TWIHS2_Write(CURRENT_AADR_B, CURRENT_CH1_CONFIG, 1);
//    Twihs2_BusywaitandRecover();
//    memset(tempch1, 0, sizeof(tempch1));
//    SYSTICK_DelayMs(10);
//
//    TWIHS2_Read(CURRENT_AADR_B, tempch1, 3);
//    Twihs2_BusywaitandRecover();
//
//    if(tempch1[2] == 0x10)
//    {
//        if(tempch1[0]&0x08)
//        {
//            reading1 = ~(tempch1[0]&0x07);
//            reading1 =  reading1<<8;
//            reading1 |= ~(tempch1[1]);
//            reading1 = reading1+1;
//        }
//        else
//        {
//            reading1 = tempch1[0]&0x07;
//            reading1 =  reading1<<8;
//            reading1 |= tempch1[1];
//        }
//        if(reading1>0)
//        {
//            reading1 = reading1-2;
//        }
//        if(reading1<0)
//        {
//            reading1 = 0;
//        }
//    }
//    else 
//    {
//        reading1 = 0;
//    } 
//
//
//    TWIHS2_Write(CURRENT_AADR_B, CURRENT_CH2_CONFIG, 1);
//    Twihs2_BusywaitandRecover();
//    memset(tempch2, 0, sizeof(tempch2));
//    SYSTICK_DelayMs(10);
//
//    TWIHS2_Read(CURRENT_AADR_B, tempch2, 3);
//    Twihs2_BusywaitandRecover();
//
//    if(tempch2[2] == 0x30)
//    {
//        if(tempch2[0]&0x08)
//        {
//            reading2 = ~(tempch2[0]&0x07);
//            reading2 =  reading2<<8;
//            reading2 |= ~(tempch2[1]);
//            reading2 = reading2+1;
//        }
//        else
//        {
//            reading2 = tempch2[0]&0x07;
//            reading2 =  reading2<<8;
//            reading2 |= tempch2[1];
//        }
//        if(reading2>0)
//        {
//            reading2 = reading2-2;
//        }
//        if(reading2<0)
//        {
//            reading2 = 0;
//        }
//        
//    }
//    else 
//    {
//        reading2 = 0;
//    }
//    
//    current_B1 = (float)reading1;
//    current_B2 = (float)reading2;
//
//    float motorB_current;
//
//    motorB_current = current_B1-current_B2;
//    if(motorB_current < 0)
//    {
//        motorB_current *= -1;
//    }
//    motorB_current = trunc((motorB_current*100)/133.0)/100;
//    return motorB_current;
//}
//
//float current_read_C(void)
//{
//    static uint8_t tempch1[3];
//    static uint8_t tempch2[3];
//    
//    static int reading1;
//    static int reading2;
//    
//    static float current_C1;
//    static float current_C2;
//    
//    TWIHS2_Write(CURRENT_AADR_C, CURRENT_CH1_CONFIG, 1);
//    Twihs2_BusywaitandRecover();
//    memset(tempch1, 0, sizeof(tempch1));
//    SYSTICK_DelayMs(10);
//
//    TWIHS2_Read(CURRENT_AADR_C, tempch1, 3);
//    Twihs2_BusywaitandRecover();
//
//    if(tempch1[2] == 0x10)
//    {
//        if(tempch1[0]&0x08)
//        {
//            reading1 = ~(tempch1[0]&0x07);
//            reading1 =  reading1<<8;
//            reading1 |= ~(tempch1[1]);
//            reading1 = reading1+1;
//        }
//        else
//        {
//            reading1 = tempch1[0]&0x07;
//            reading1 =  reading1<<8;
//            reading1 |= tempch1[1];
//        }
//        if(reading1>0)
//        {
//            reading1 = reading1-2;
//        }
//        if(reading1<0)
//        {
//            reading1 = 0;
//        }
//    }
//    else 
//    {
//        reading1 = 0;
//    }
//
//    TWIHS2_Write(CURRENT_AADR_C, CURRENT_CH2_CONFIG, 1);
//    Twihs2_BusywaitandRecover();
//    memset(tempch2, 0, sizeof(tempch2));
//    SYSTICK_DelayMs(10); 
//
//    TWIHS2_Read(CURRENT_AADR_C, tempch2, 3);
//    Twihs2_BusywaitandRecover();
//
//    if(tempch2[2] == 0x30)
//    {
//        if(tempch2[0]&0x08)
//        {
//            reading2 = ~(tempch2[0]&0x07);
//            reading2 =  reading2<<8;
//            reading2 |= ~(tempch2[1]);
//            reading2 = reading2+1;
//        }
//        else
//        {
//            reading2 = tempch2[0]&0x07;
//            reading2 =  reading2<<8;
//            reading2 |= tempch2[1];
//        }
//        if(reading2>0)
//        {
//            reading2 = reading2-2;
//        }
//        if(reading2<0)
//        {
//            reading2 = 0;
//        }
//    }
//    else 
//    {
//        reading2 = 0;
//    }
//    
//    current_C1 = (float)reading1;
//    current_C2 = (float)reading2;
//
//    float motorC_current;
//
//    motorC_current = current_C1-current_C2;
//    if(motorC_current < 0)
//    {
//        motorC_current *= -1;
//    }
//    motorC_current = trunc((motorC_current*100)/133.0)/100;      
//    return motorC_current;
//}

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
bool IsMotor1OverCurrentFault (void)
{
  return IsMot1OverCurrentFault();
}

bool IsMotor2OverCurrentFault (void)
{
  return IsMot2OverCurrentFault();
}

bool IsMotor3OverCurrentFault (void)
{
  return IsMot3OverCurrentFault();
}

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
  CheckMot1OverLoad();
  CheckMot2OverLoad();
  CheckMot3OverLoad();
}

void CheckMotorTempFault(void)
{
  CheckMot1Temp();
  CheckMot2Temp();
  CheckMot3Temp();   
}

void CheckMotorDriverFault(void)
{
    Motor1FaultCheck();
    Motor2FaultCheck();
    BrushMotorFaultCheck();
}

void CheckRunFault(void)
{
    Motor1run();
    Motor2run();
    BrushMotorrun();
}

void CheckMotorCurrentDurationFault(void)
{
    Motor1CurrentDurationCheck();
    Motor2CurrentDurationCheck();
    Motor3CurrentDurationCheck();
}

void IloadMotMaxTimeIncrement_ms (void)
{
  IloadMot1MaxTimeIncrement_ms();
  IloadMot2MaxTimeIncrement_ms();
  IloadMot3MaxTimeIncrement_ms();
}




//---------------------------------------------------------------------------------------------
//new current sensor

void INA226_Init(uint8_t slaveID)               //new current sensor initialization
{
    static uint8_t controlbyte[3];
    
    controlbyte[0] = 0x00;
    controlbyte[1] = 0x47;
    controlbyte[2] = 0x25;
    
    TWIHS2_Write(slaveID, controlbyte, 3);
    Twihs2_BusywaitandRecover();
}

uint16_t INA226_read(uint8_t slaveID, uint8_t regPointer)
{
    static uint8_t controlbyte[2];
    static uint16_t calculated_reading = 0;
    
    memset(controlbyte, 0, sizeof(controlbyte));
    controlbyte[0] = regPointer;
    
    TWIHS2_Write(slaveID, controlbyte, 1);
    Twihs2_BusywaitandRecover();
    
    SYSTICK_DelayMs(10);
    
    memset(controlbyte, 0, sizeof(controlbyte));
    TWIHS2_Read(slaveID, controlbyte, 2);
    Twihs2_BusywaitandRecover();
    
    SYSTICK_DelayMs(10);
    
    calculated_reading = 0;
    if((controlbyte[0]>>7)&0x01)
    {
        calculated_reading = controlbyte[0];
        calculated_reading <<= 8;
        calculated_reading |= controlbyte[1];
        calculated_reading = ~calculated_reading;
        calculated_reading++;
    }
    else
    {
        calculated_reading = controlbyte[0];
        calculated_reading <<= 8;
        calculated_reading |= controlbyte[1];
    }
    
    return calculated_reading;
}

float current_read_A(void)
{
    static uint16_t temp_reading;
    static float currentMeasured = 0.0;
    
    temp_reading = INA226_read(CURRENT_AADR_A, 0x01);
    currentMeasured =(float)((temp_reading*25*1000)/(1000*10*SHUNT_RESISTOR));
    currentMeasured = currentMeasured/1000;
    
    return currentMeasured;
}

float current_read_B(void)
{
    static uint16_t temp_reading;
    static float currentMeasured = 0.0;
    
    temp_reading = INA226_read(CURRENT_AADR_B, 0x01);
    currentMeasured =(float)((temp_reading*25*1000)/(1000*10*SHUNT_RESISTOR));
    currentMeasured = currentMeasured/1000;
    
    return currentMeasured;
}

float current_read_C(void)
{
    static uint16_t temp_reading;
    static float currentMeasured = 0.0;
    
    temp_reading = INA226_read(CURRENT_AADR_C, 0x01);
    currentMeasured =(float)((temp_reading*25*1000)/(1000*10*SHUNT_RESISTOR));
    currentMeasured = currentMeasured/1000;
    
    return currentMeasured;
}