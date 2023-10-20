#ifndef _RFID_UART_CONFIG_H_
#define _RFID_UART_CONFIG_H_

#include "board.h"

#define RFID_UART UART3
#define RFID_UART_CLKSRC kCLOCK_BusClk
#define RFID_UART_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define RFID_UART_IRQn UART3_RX_TX_IRQn
#define RFID_UART_IRQHandler UART3_RX_TX_IRQHandler
#define RFID_UART_BAUDRATE                  19200
/*! @brief Ring buffer size (Unit: Byte). */
#define RFID_UART_RX_BUFF_SIZE  15

#define RFID_TAG_DATA_SIZE      12

//#define RFID_ENABLE             1
#endif