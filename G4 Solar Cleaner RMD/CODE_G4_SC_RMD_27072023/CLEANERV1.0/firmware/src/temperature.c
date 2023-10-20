#include "main.h"

static stBoardTemperature maxBoardTemperature;

stBoardTemperature *GetSetMaxBoardTemperature(void)
{
  return(&maxBoardTemperature);
}

float temp_read_A(void)
{
//    TEMPERATURE_SENSOR_REG_ADDR[0] = 0x00;
//    
//    memset(TEMPERATURE_SENSOR_READ, 0, sizeof(TEMPERATURE_SENSOR_READ));
//    TWIHS2_WriteRead(TEMP_SENSOR_SLAVE_ID_A,TEMPERATURE_SENSOR_REG_ADDR,1,TEMPERATURE_SENSOR_READ,2); 
//    Twihs2_BusywaitandRecover();
//    SYSTICK_DelayMs(10);
//    
//    TempSIGN_BIT = (TEMPERATURE_SENSOR_READ[0] & 0x80)?NEGATIVE:POSITIVE;
//    TempHi = TEMPERATURE_SENSOR_READ[0] & 0x7F;   // Remove sign
//    TempLow = TEMPERATURE_SENSOR_READ[1] & 0xF0;   // Filter out last nibble
//    TempLow = TempLow >>4;              // Shift right 4 times to remove last four bits
//    Temperature_fl = TempLow * 0.0625;    // Each bit = 0.0625 degree C
//    Temperature_fl += TempHi;
//    Temperature_fl = (TempSIGN_BIT == 1)?Temperature_fl*(-1):Temperature_fl;
    
    return 0.0;
}

float temp_read_B(void)
{
//    TEMPERATURE_SENSOR_REG_ADDR[0] = 0x00;
//    
//    memset(TEMPERATURE_SENSOR_READ, 0, sizeof(TEMPERATURE_SENSOR_READ));
//    TWIHS2_WriteRead(TEMP_SENSOR_SLAVE_ID_B,TEMPERATURE_SENSOR_REG_ADDR,1,TEMPERATURE_SENSOR_READ,2); 
//    Twihs2_BusywaitandRecover();
//    SYSTICK_DelayMs(10);
//    
//    TempSIGN_BIT = (TEMPERATURE_SENSOR_READ[0] & 0x80)?NEGATIVE:POSITIVE;
//    TempHi = TEMPERATURE_SENSOR_READ[0] & 0x7F;   // Remove sign
//    TempLow = TEMPERATURE_SENSOR_READ[1] & 0xF0;   // Filter out last nibble
//    TempLow = TempLow >>4;              // Shift right 4 times to remove last four bits
//    Temperature_fl = TempLow * 0.0625;    // Each bit = 0.0625 degree C
//    Temperature_fl += TempHi;
//    Temperature_fl = (TempSIGN_BIT == 1)?Temperature_fl*(-1):Temperature_fl;
    
    return 0.0;
}

float temp_read_C(void)
{
//    TEMPERATURE_SENSOR_REG_ADDR[0] = 0x00;
//    
//    memset(TEMPERATURE_SENSOR_READ, 0, sizeof(TEMPERATURE_SENSOR_READ));
//    TWIHS2_WriteRead(TEMP_SENSOR_SLAVE_ID_C,TEMPERATURE_SENSOR_REG_ADDR,1,TEMPERATURE_SENSOR_READ,2); 
//    Twihs2_BusywaitandRecover();
//    SYSTICK_DelayMs(10);
//    
//    TempSIGN_BIT = (TEMPERATURE_SENSOR_READ[0] & 0x80)?NEGATIVE:POSITIVE;
//    TempHi = TEMPERATURE_SENSOR_READ[0] & 0x7F;   // Remove sign
//    TempLow = TEMPERATURE_SENSOR_READ[1] & 0xF0;   // Filter out last nibble
//    TempLow = TempLow >>4;              // Shift right 4 times to remove last four bits
//    Temperature_fl = TempLow * 0.0625;    // Each bit = 0.0625 degree C
//    Temperature_fl += TempHi;
//    Temperature_fl = (TempSIGN_BIT == 1)?Temperature_fl*(-1):Temperature_fl;
    
    return 0.0;
}

void CheckMot1Temp (void)
{
  float boardTemperatue = temp_read_A();
  stBoardTemperature* boardSetTemperature = GetSetMaxBoardTemperature();
  if(boardTemperatue > boardSetTemperature->maxTempSensorOne)
  {
      Motor1tempFault = 1;
  }
  else
  {
      Motor1tempFault = 0;
  }
}

void CheckMot2Temp (void)
{
  float boardTemperatue = temp_read_B();
  stBoardTemperature* boardSetTemperature = GetSetMaxBoardTemperature();
  if(boardTemperatue > boardSetTemperature->maxTempSensorTwo)
  {
      Motor2tempFault = 1;
  }
  else
  {
      Motor2tempFault = 0;
  }
}

void CheckMot3Temp (void)
{
  float boardTemperatue = temp_read_C();
  stBoardTemperature* boardSetTemperature = GetSetMaxBoardTemperature();
  if(boardTemperatue > boardSetTemperature->maxTempSensorThree)
  {
      Motor3tempFault = 1;
  }
  else
  {
      Motor3tempFault = 0;
  }
}





