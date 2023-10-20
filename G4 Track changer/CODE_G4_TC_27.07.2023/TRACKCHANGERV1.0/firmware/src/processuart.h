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

#ifndef _PROCESSUART_H    /* Guard against multiple inclusion */
#define _PROCESSUART_H

#include "main.h"

//#define DEFAULT_RX_MAC_ADDR                  0xFFFFFFFFFFFFFFFF

void ProcessUartData(void);

void DEBUG_UARTTASK(char *Receive_Buffer);
void ZIGBEE_UARTTASK(char *Receive_Buffer);

int uartflag;

uint64_t rfidvalue; 
char RFID_10dig_String[10];

uint32_t Usart2_ReadTimeout;
//void Receiving_packet_processing(void);


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
