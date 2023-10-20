#ifndef _RFID_H    /* Guard against multiple inclusion */
#define _RFID_H

//#define RFID_UART UART3
//#define RFID_UART_CLKSRC kCLOCK_BusClk
//#define RFID_UART_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
//#define RFID_UART_IRQn UART3_RX_TX_IRQn
//#define RFID_UART_IRQHandler UART3_RX_TX_IRQHandler
//#define RFID_UART_BAUDRATE                  19200
#define RFID_UART_RX_BUFF_SIZE  15
#define RFID_TAG_DATA_SIZE      12
//#define RFID_ENABLE             1

#define RESET_OFF_TIME_MS               2000
#define RESET_ON_TIME_MS                4000
typedef enum eRfidResetState_def
{
  R_RESET_START,  
  R_RESET_OFF,
  R_RESET_WAIT,
  R_RESET_ON,
  R_RESET_WAIT_FOR_ON
}eRfidResetState;

typedef enum eRfidResetStatus_def
{
  R_RESET_IN_PROGRESS,
  R_RESET_DONE
}eRfidResetStatus;

typedef enum eRfidState_def
{
RFID_DISABLE,
RFID_ENABLE
}eRfidState;

uint8_t lastReadRfidData[RFID_TAG_DATA_SIZE];    //changed from char
uint8_t lastReadRfidData1[RFID_TAG_DATA_SIZE];

eRfidState GetRfidState(void);
void SetRfidState(eRfidState state);
void SetDefaultRfidState(void);

void RfidTimerOn(uint32_t timerOn_ms);
void VerifyAndSetData(void);
//void RfidInit(void);
void RfidTimerIncrement_ms(void);
bool RfidDataReceive(uint8_t* rfidBuffer,uint16_t rfidBufferLength);
uint32_t GetLastReadRfidData(void);
void ClearLastReadRfidData(void);
void CombineData(uint8_t rxChar);

uint64_t HEX_to_DEC(char *hex,char len);
void DEC_TEN_ASCII(int rfid,char *ascii_1,char *ascii_2,char *ascii_3,char *ascii_4,char *ascii_5,char *ascii_6,char *ascii_7,char *ascii_8,char *ascii_9, char *ascii_10 );

eRfidResetStatus ResetRfidFSM(void);
void SetRfidResetState(eRfidResetState state);



#endif /* _EXAMPLE_FILE_NAME_H */

