//#include <string.h>
//#include <stdio.h>
#include "main.h"


void Serial_Debug (const char *txdata_buf)
{
  Serial_Debug_Bytes(txdata_buf, strlen(txdata_buf));  
}

void Serial_Debug_Char (const char txdata_buf)
{
  Serial_Debug_Bytes(&txdata_buf, 1); 
}

void Serial_Debug_Bytes (const char *txdata_buf, uint16_t txdata_buf_strlen)
{
#ifdef ENABLE_SERIAL_DEBUG
// DebugUartTransmit ((uint8_t *) txdata_buf, txdata_buf_strlen);
 UART0_Write((void *)txdata_buf,txdata_buf_strlen);
 while(UART0_WriteIsBusy());
 //ZigbeeTxData ((uint8_t *) txdata_buf, txdata_buf_strlen);   // Can be used Only when Zigbee in API Mode.
#endif 
}

void Serial_Debug_Num(int32_t message_int)
{
#ifdef ENABLE_SERIAL_DEBUG
  static char dispstring[50];
  snprintf((char *)dispstring, sizeof dispstring, "%d",(int) message_int);
  Serial_Debug_Bytes((const char *)dispstring, strlen(dispstring));
#endif 
}

void Serial_Debug_Hex(int32_t message_int)
{
#ifdef ENABLE_SERIAL_DEBUG
  static char dispstring[50];
  snprintf((char *)dispstring, sizeof dispstring, "%x",(unsigned int) message_int);
  Serial_Debug_Bytes((const char *)dispstring, strlen(dispstring));
#endif 
}
void Serial_Debug_Float(float message_float, uint8_t noOfDecimal)
{
#ifdef ENABLE_SERIAL_DEBUG
  char dispstring[20];
  ftoa(message_float, dispstring, noOfDecimal);
  Serial_Debug(dispstring);
#endif 
}



