#ifndef _TEMPERATURE_H    /* Guard against multiple inclusion */
#define _TEMPERATURE_H

uint8_t TempSIGN_BIT;
uint8_t TempHi,TempLow;
float Temperature_fl;

uint8_t Motor1tempFault;
uint8_t Motor2tempFault;
uint8_t Motor3tempFault;

uint8_t TEMPERATURE_SENSOR_REG_ADDR[1];
uint8_t TEMPERATURE_SENSOR_READ[2];

float temp_read_A(void);
float temp_read_B(void);
float temp_read_C(void);

typedef struct 
{
  float maxTempSensorOne;
  float maxTempSensorTwo;
  float maxTempSensorThree;
} stBoardTemperature;

stBoardTemperature *GetSetMaxBoardTemperature(void);

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
