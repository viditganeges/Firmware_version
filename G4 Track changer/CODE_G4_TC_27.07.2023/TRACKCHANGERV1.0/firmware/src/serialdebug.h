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

#ifndef _SERIALDEBUG_H    /* Guard against multiple inclusion */
#define _SERIALDEBUG_H

#include "main.h"

//#define ENABLE_SERIAL_DEBUG

typedef enum 
{
  SUCCESS = 0, 
  ERROR = !SUCCESS
} ErrorStatus;

typedef enum 
{
  RESET = 0, 
  SET = !RESET
} FlagStatus, ITStatus;

void Serial_Debug (const char *txdata_buf);
void Serial_Debug_Char (const char txdata_buf);
void Serial_Debug_Bytes (const char *txdata_buf, uint16_t txdata_buf_strlen);
extern ErrorStatus DebugUartTransmit (uint8_t * txBuffer, uint16_t txBufferLength);
extern ErrorStatus ZigbeeTxData (const uint8_t *txData, const uint16_t txDatalen); 
void Serial_Debug_Num(int32_t message_int);
void Serial_Debug_Hex(int32_t message_int);
void Serial_Debug_Float(float message_float, uint8_t noOfDecimal);


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
