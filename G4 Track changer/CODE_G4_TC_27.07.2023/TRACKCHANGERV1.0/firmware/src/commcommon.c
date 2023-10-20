#include "main.h"

//static volatile bool commTimeOver     = false;
//static int32_t comm_timecount         = 0;
//static int32_t commTimeoutCount       = 0;
//static eUartType uartType = UNKNOWN_UART;

static stHeartbeatConfig heartbeatConfigValues;

ErrorStatus UartTransmit (uint8_t * txBuffer, uint16_t txBufferLength)
{
  ErrorStatus status = ERROR;
  
  if(uartflag == 1)         //For Config uart
  {
    ConvertHexAndProcess((char *)txBuffer, &txBufferLength);
    status = UART0_Write((void *)txBuffer,txBufferLength);
     while(UART0_WriteIsBusy());
    uartflag = 0; 
    if(status == true)
    {
        status = SUCCESS;
    }
    else 
    {
        status = ERROR;
    }   
  }
  else if(uartflag == 2)    //For Zigbee uart
  {
    ZigbeeTxPacket_Gateway(0,txBuffer,txBufferLength);
//    ZigbeeTxPacket_Zigbee(0,txBuffer,txBufferLength);
    ConvertHexAndProcess((char *)txBuffer, &txBufferLength);
    status = UART0_Write((void *)txBuffer,txBufferLength);
    while(UART0_WriteIsBusy());
    uartflag = 0;    
    if(status == true)
    {
        status = SUCCESS;
    }
    else 
    {
        status = ERROR;
    } 
  }
  else if(uartflag == 3)  //transmit message
  {
    ZigbeeTxPacket_Gateway(0,txBuffer,txBufferLength);
    ConvertHexAndProcess((char *)txBuffer, &txBufferLength);
    status = UART0_Write((void *)txBuffer,txBufferLength);
    while(UART0_WriteIsBusy());
    uartflag = 0;
    if(status == true)
    {
        status = SUCCESS;
    }
    else 
    {
        status = ERROR;
    }
  } 
  return status;
}
//
//ErrorStatus UartTransmitType (eUartType uartType, uint8_t * txBuffer, uint16_t txBufferLength)
//{
//  ErrorStatus status = ERROR;
//  if(uartType == ZIGBEE_UART)
//  {
//    #ifdef ZIGBEE_JSON_MODE 
//    ConvertHexAndProcess(txBuffer, &txBufferLength);
//    #endif
//    status = ZigbeeApiTxData(GetReceivedMACAddress(),txBuffer, txBufferLength);
//  }
//  else if(uartType == DEBUG_UART)
//  {
//    status = DebugUartTransmit(txBuffer, txBufferLength); 
//  }
//  else if(uartType == CONFIG_UART)
//  {
//   // ConvertHexAndProcess(txBuffer, &txBufferLength);
//    status = ConfigUartTransmit(txBuffer, txBufferLength); 
//  }
//  return status;
//}

//ErrorStatus UartAllTransmit (uint8_t * txBuffer, uint16_t txBufferLength)
//{
//  bool status = true;
//  if(ZigbeeApiTxData(GetReceivedMACAddress(),txBuffer, txBufferLength) == ERROR)
//  {
//    status = false;
//  }
//  if(DebugUartTransmit(txBuffer, txBufferLength) == ERROR)
//  {
//    status = false;
//  }
//  if(ConfigUartTransmit(txBuffer, txBufferLength) == ERROR)
//  {
//    status = false;
//  }
//  if(status == true)
//  {
//    return SUCCESS;
//  }
//  else
//  {
//    return ERROR;
//  }
//}
//
//eUartType GetUartType (void)
//{
//  return uartType;
//}
//
//void SetUartType (eUartType uartRxType)
//{
//  uartType = uartRxType;
//}

stHeartbeatConfig *GetSetHeartbeatConfig(void)
{
  return(&heartbeatConfigValues);  
}







