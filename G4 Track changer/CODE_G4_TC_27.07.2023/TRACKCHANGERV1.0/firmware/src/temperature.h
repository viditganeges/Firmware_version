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

#ifndef _TEMPERATURE_H    /* Guard against multiple inclusion */
#define _TEMPERATURE_H


uint8_t TempSIGN_BIT;
uint8_t TempHi,TempLow;
float Temperature_fl;

uint8_t TEMPERATURE_SENSOR_REG_ADDR[1];
uint8_t TEMPERATURE_SENSOR_READ[2];

float temp_read_A(void);
float temp_read_B(void);
float temp_read_C(void);

typedef struct 
{
  float maxTempSensorOne;
  float maxTempSensorTwo;
} stBoardTemperature;
//float GetTemperatureSensorData(eTempSensors);
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
