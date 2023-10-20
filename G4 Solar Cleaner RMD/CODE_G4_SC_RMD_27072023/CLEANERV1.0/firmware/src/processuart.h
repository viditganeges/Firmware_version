#ifndef _PROCESSUART_H    /* Guard against multiple inclusion */
#define _PROCESSUART_H

#include "main.h"

void ProcessUartData(void);

void DEBUG_UARTTASK(char *Receive_Buffer);
void ZIGBEE_UARTTASK(char *Zigreceive_Buffer);

uint64_t rfidvalue; 
char RFID_10dig_String[10];

uint32_t Usart2_ReadTimeout;

int uartflag;

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
