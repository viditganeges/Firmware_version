#include "main.h"

static volatile bool commTimeOver     = false;
static int32_t comm_timecount         = 0;
static int32_t commTimeoutCount       = 0;
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
    if (zigbeetcflag == 1)
    {
        ZigbeeTx_TC(txBuffer,txBufferLength);  
    }
    else 
    {
        ZigbeeTxPacket_Gateway(0,txBuffer,txBufferLength);
        ConvertHexAndProcess((char *)txBuffer, &txBufferLength);
        status = UART0_Write((void *)txBuffer,txBufferLength);
        while(UART0_WriteIsBusy());
        uartflag = 0;
    }
    if(status == true)
    {
        status = SUCCESS;
    }
    else 
    {
        status = ERROR;
    }
  }
//  else if(uartflag == 4)  //Zigbee TC message
//  {
//    ZigbeeTx_TC(txBuffer,txBufferLength);
//    ConvertHexAndProcess((char *)txBuffer, &txBufferLength);
//    status = UART0_Write((void *)txBuffer,txBufferLength);
//    while(UART0_WriteIsBusy());
//    uartflag = 0;
//    if(status == true)
//    {
//        status = SUCCESS;
//    }
//    else 
//    {
//        status = ERROR;
//    }
//  }
  return status;
}

inline void CommTimerStop (void)
{
  comm_timecount = 0;
}

bool IsCommTimeOver (void)
{
  stHeartbeatConfig *heartBeatconfig = GetSetHeartbeatConfig();
  if(heartBeatconfig->enable == true)
  {
    return (commTimeOver);
  }
  else
  {
    return (false);
  }
}

void ClearCommTimeOver (void)
{
  commTimeOver = false; 
}

static void CommTimerOn (uint32_t setCommTimeCount_ms)
{
  if(setCommTimeCount_ms == 0)
  {
    commTimeOver = true;
    CommTimerStop();
  }
  else
  {
    comm_timecount = setCommTimeCount_ms;
    commTimeOver = false;
  }
}

void CommTimeIncrement_ms (void)
{
  if(comm_timecount)
  {
    if(--comm_timecount <= 0)
    {
      if(--commTimeoutCount <= 0)
      {
        commTimeOver = true;
        CommTimerStop();
      }
      else
      {
        CommTimerOn(heartbeatConfigValues.interval_ms);
      }
    }
    else
    {
      commTimeOver = false;
    }
  }
  else
  {
    commTimeOver = true;
  }
}

void RestartCommTimer (void)
{
  commTimeOver = false;
  CommTimerStop();
  commTimeoutCount = heartbeatConfigValues.noOfMessages;
  CommTimerOn(heartbeatConfigValues.interval_ms);
}

stHeartbeatConfig *GetSetHeartbeatConfig(void)
{
  return(&heartbeatConfigValues);  
}







