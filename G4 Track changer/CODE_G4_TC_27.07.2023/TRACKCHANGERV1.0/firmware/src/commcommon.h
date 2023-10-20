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

#ifndef _COMMCOMMON_H    /* Guard against multiple inclusion */
#define _COMMCOMMON_H

#include "main.h"

//#include "Customtypedefs.h"
//#include <stdint.h>
//#include <stdbool.h>

//typedef enum Uart_type_def
//{
//  ZIGBEE_UART,
//  DEBUG_UART,
//  CONFIG_UART,
//  UNKNOWN_UART  
//}eUartType;
//
typedef struct{
//  bool enable;
  int interval_ms;
//  int noOfMessages;
  int maxReconnectTime;
}stHeartbeatConfig;
//
void CommTimeIncrement_ms (void);
void CommTimerStop (void);
bool IsCommTimeOver (void);

  
ErrorStatus UartTransmit (uint8_t * txBuffer, uint16_t txBufferLength);
//ErrorStatus UartTransmitType (eUartType uartType, uint8_t * txBuffer, uint16_t txBufferLength);
//ErrorStatus UartAllTransmit (uint8_t * txBuffer, uint16_t txBufferLength);
//void SetUartType (eUartType uartRxType);
stHeartbeatConfig* GetSetHeartbeatConfig(void);
void RestartCommTimer (void);
void ClearCommTimeOver (void);
//static void CommTimerOn (uint32_t setCommTimeCount_ms);
//eUartType GetUartType (void);



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
