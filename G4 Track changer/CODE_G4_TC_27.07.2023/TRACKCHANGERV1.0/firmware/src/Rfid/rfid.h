#ifndef _RFID_H_
#define _RFID_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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


eRfidState GetRfidState(void);
void SetRfidState(eRfidState state);
void SetDefaultRfidState(void);

void RfidInit(void);
void RfidTimerIncrement_ms(void);
bool RfidDataReceive(uint8_t* rfidBuffer,uint16_t rfidBufferLength);
uint32_t GetLastReadRfidData(void);
void ClearLastReadRfidData(void);


uint64_t HEX_to_DEC(char *hex,char len);
void DEC_TEN_ASCII(int rfid,char *ascii_1,char *ascii_2,char *ascii_3,char *ascii_4,char *ascii_5,char *ascii_6,char *ascii_7,char *ascii_8,char *ascii_9, char *ascii_10 );

eRfidResetStatus ResetRfidFSM(void);
void SetRfidResetState(eRfidResetState state);
#endif