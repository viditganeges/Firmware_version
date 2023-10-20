#include "fsl_uart.h"
#include "BmsUart.h"
#include "Serial_Debug.h"
#include "switch.h"
#include <string.h>
#include "APIProcessing.h"


//extern uint8_t rxDataBuffer[MAX_UART_RX_BUFFER_LENGTH];

typedef struct {
  uint8_t txBuffer[MAX_BMS_UART_TX_BUFFER];
  volatile uint16_t txBufferLength;
  volatile uint16_t txBufferIndex;
  volatile ITStatus txCompleteFlag;  
} sUartTxdef;

typedef struct {
  uint8_t rxBuffer[MAX_BMS_UART_RX_BUFFER];
  uint16_t rxBufferLength;
  uint16_t Rxdata_Count_End;
  volatile ITStatus rxCompleteFlag;
  volatile ITStatus rxStartFlag;
} sUartRxdef;


static sUartTxdef bmsUartTx;
static sUartRxdef bmsUartRx;

static void EnableBmsUartIT (void);
static void DisableBmsUartIT (void);

ErrorStatus BmsUartInit (void)
{
  uart_config_t bms;
  /*
 * config.baudRate_Bps = 115200U;
 * config.parityMode = kUART_ParityDisabled;
 * config.stopBitCount = kUART_OneStopBit;
 * config.txFifoWatermark = 0;
 * config.rxFifoWatermark = 1;
 * config.enableTx = false;
 * config.enableRx = false;
 */
  UART_GetDefaultConfig(&bms);
  bms.baudRate_Bps = BMS_UART_BAUDRATE;
  bms.enableTx = true;
  bms.enableRx = true;
  
  if(UART_Init(BMS_UART_BASE, &bms, BMS_UART_CLK_FREQ) != kStatus_Success)
  {
    return ERROR;
  }
  
  bmsUartTx.txCompleteFlag = SET;
  bmsUartRx.rxCompleteFlag = RESET;
  UART_EnableInterrupts(BMS_UART_BASE, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable ); 
  EnableBmsUartIT();
  return SUCCESS;
}

ErrorStatus BmsUartDeInit (void)
{
 UART_Deinit(BMS_UART_BASE);
 return SUCCESS;  
}

ErrorStatus BmsUartTransmit (uint8_t * txBuffer, uint16_t txBufferLength)
{
  if(!txBufferLength)
  {
    return SUCCESS;
  }
  while(bmsUartTx.txCompleteFlag == RESET);                                        /* Wait for Previous Transmission Completion */
  if((sizeof(bmsUartTx.txBuffer)/sizeof(bmsUartTx.txBuffer[0])) >= txBufferLength)
  {
    memcpy(bmsUartTx.txBuffer, txBuffer, txBufferLength);
    bmsUartTx.txBufferLength = txBufferLength;
    bmsUartTx.txBufferIndex = 0;
    bmsUartTx.txCompleteFlag = RESET;
    UART_EnableInterrupts(BMS_UART_BASE, kUART_TxDataRegEmptyInterruptEnable);   
  }
  else
  {
    return ERROR;
  }
  return SUCCESS;
}

ErrorStatus BmsUartReceive (uint8_t * rxBuffer, uint16_t rxBufferLength)
{
  ErrorStatus status;
  if(bmsUartRx.rxCompleteFlag == RESET)
  {
    status = ERROR;
  }
  else
  {
    DisableBmsUartIT();
    if(bmsUartRx.rxCompleteFlag == RESET)
    {
      status = ERROR;
    }
    else
    {
      if(bmsUartRx.Rxdata_Count_End <= rxBufferLength)
      {
        memcpy(rxBuffer, bmsUartRx.rxBuffer, bmsUartRx.Rxdata_Count_End);
        status = SUCCESS;
      }
      else
      {
        status = ERROR;
      }      
      bmsUartRx.rxCompleteFlag = RESET;
    }
   EnableBmsUartIT();
  }
  return status; 
}

void BmsRxDataReceive (uint8_t * rxBuffer, uint16_t rxBufferLength)
{
    for(uint16_t rxDataCount = 0; rxDataCount < rxBufferLength; rxDataCount++)
    {
      CombineBmsUartReceiveData(rxBuffer[rxDataCount]);
    }  
}

void BMS_UART_IRQHandler(void)
{
  /* If new data arrived. */
  if (((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(BMS_UART_BASE)) &&
    (UART_GetEnabledInterrupts(BMS_UART_BASE) & kUART_RxDataRegFullInterruptEnable))
  {
      uint8_t data = UART_ReadByte(BMS_UART_BASE);

      BmsUartReceive_Callback(data);
  }
  if((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(BMS_UART_BASE)) && 
     (kUART_TxDataRegEmptyInterruptEnable & UART_GetEnabledInterrupts(BMS_UART_BASE)))
  {
    if(bmsUartTx.txBufferLength)
    {
      UART_WriteByte(BMS_UART_BASE, bmsUartTx.txBuffer[bmsUartTx.txBufferIndex++]);
      bmsUartTx.txBufferLength--;
      if(!bmsUartTx.txBufferLength)
      {
        UART_DisableInterrupts(BMS_UART_BASE,kUART_TxDataRegEmptyInterruptEnable);
        BmsUartTransmit_Callback();
      }
    }
    else
    {
      UART_DisableInterrupts(BMS_UART_BASE,kUART_TxDataRegEmptyInterruptEnable);
      BmsUartTransmit_Callback();
    }
  }
}

void BmsUartError_Callback(void)
{
  /* Handle Transfer Error */
//  (void)HAL_UART_EnableReceive(&ConfigUartHandle);                      /* Enable reception that was disabled on error by the hal routine.  */
}

void BmsUartTransmit_Callback(void)
{
  /* Set transmission flag: trasfer complete*/
  bmsUartTx.txCompleteFlag = SET;
}

void BmsUartReceive_Callback(uint8_t rxData)
{
  if(GetSwitchState()==MANUAL_CONTROL)
  {  
    CombineBmsUartReceiveData(rxData);
  }
}

static void CombineBmsUartReceiveData (uint8_t rxData)
{
  
  if(rxData == BMS_UART_RxStartChar)
  {
    bmsUartRx.rxBufferLength = 0;
    bmsUartRx.rxStartFlag = SET;
    bmsUartRx.rxCompleteFlag = RESET;
    bmsUartRx.rxBuffer[bmsUartRx.rxBufferLength++] = rxData;
  }
  else if(rxData == BMS_UART_RxEndChar)
  {
    bmsUartRx.rxBuffer[bmsUartRx.rxBufferLength++] = rxData;
    bmsUartRx.rxBuffer[bmsUartRx.rxBufferLength++] = 0x00;           /* Just add null character at the end     */
    bmsUartRx.Rxdata_Count_End = bmsUartRx.rxBufferLength;
    bmsUartRx.rxCompleteFlag = SET;
    bmsUartRx.rxBufferLength = 0;
    bmsUartRx.rxStartFlag = RESET;
  }
  
  else if(bmsUartRx.rxStartFlag == SET)
  {
    if(bmsUartRx.rxBufferLength <  MAX_BMS_UART_RX_BUFFER-1)
    {
      bmsUartRx.rxBuffer[bmsUartRx.rxBufferLength++] = rxData;  
    }
    else
    {
      bmsUartRx.rxBufferLength = 0;
      bmsUartRx.rxStartFlag = RESET;
    }
  }  
}


ITStatus * GetBmsUartTxCompleteFlagPtr (void)
{
  return ((ITStatus *)&bmsUartTx.txCompleteFlag);
}

static void DisableBmsUartIT(void)
{
    DisableIRQ(BMS_UART_IRQn);
}

static void EnableBmsUartIT(void)
{
    EnableIRQ(BMS_UART_IRQn);
}

