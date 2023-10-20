/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BMSUART_CONFIG_H_
#define __BMSUART_CONFIG_H_

/* Includes ------------------------------------------------------------------*/
#include "board.h"

#define ENABLE_BMS_RESET                     /* Enable bms Reset Pin */

#define MAX_BMS_UART_TX_BUFFER            1500
#define MAX_BMS_UART_RX_BUFFER            500

#define BMS_UART_RxStartChar              ':'     
#define BMS_UART_RxEndChar               '\n' 

//#define SlaveADD      ':'
//#define Slaveend      '\n'

#define BMS_UART_BASE                      UART4
#define BMS_UART_CLKSRC                    BUS_CLK //kCLOCK_BusClk kCLOCK_CoreSysClk //
#define BMS_UART_CLK_FREQ                  CLOCK_GetFreq(BMS_UART_CLKSRC)
#define BMS_UART_IRQn                      UART4_RX_TX_IRQn
#define BMS_UART_IRQHandler                UART4_RX_TX_IRQHandler

#define BMS_UART_BAUDRATE                  9600

#endif