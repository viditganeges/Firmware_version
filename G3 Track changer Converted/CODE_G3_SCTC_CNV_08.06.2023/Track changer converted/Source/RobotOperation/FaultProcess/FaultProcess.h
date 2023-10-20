#ifndef _FAULT_PROCESS_H_
#define _FAULT_PROCESS_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum eFaultFlags_def
{
  NO_FAULT                       = 0x0000,
  OVER_CURRENT_MOT1_FAULT,
  OVER_CURRENT_MOT2_FAULT,
  OVER_CURRENT_MOT3_FAULT,
  MIN_BAT_VOLTAGE_FAULT, 
  OVER_BTEMP_FAULT,
  BATTERY_FAULT,
  ZIGBEE_FAULT,
  RTC_FAULT,
  OVER_DTEMP_FAULT,               
  MOTOR_STALL_FAULT, 
  COMMUNICATION_FAULT,
  SPI_FLASH_FAULT,
  OTAA_FAILURE,
  EEPROM_FAULT,
  MAX_BAT_VOLTAGE_FAULT, 
  UNKNOWN_FAULT,
  MAX_FAULT,                                    /* This should be the last enum above ALL_FAULT. Its used to keep track maximum no of fault codes */
  ALL_FAULT                     = 0xFFFF        /* This should be the last enum */
}eFaultFlags;

typedef enum eErrorState_def
{
  ER_IDLE,
  ER_NO_ERROR,
  ER_CLEAR_ERROR,
  ER_ERROR
}eErrorState;

typedef struct {
  uint32_t errorCodeTime[MAX_FAULT];
  int8_t errorCodes[MAX_FAULT];
  uint8_t noOffaults;
} stFaultTimeData;

void ClearAllFaults (void);
void ClearFaultsOnRobotCommand (void);
bool IsOnlyNonMotorError (void);
void CheckError (void);
eErrorState GetErrorState (void);
stFaultTimeData * GetFaultTimeData(void);
void ReportErrorOnZigeeStart (void);
uint8_t * GetSetMinBattSoc (void);
uint32_t GetMotorFaultCode (void);
void SetFault(eFaultFlags faultFlag);
void ClearFault(eFaultFlags faultFlag);
void ClearFaultsOnRobotCommand (void);
bool IsRTCFault (void);
bool IsAnyFault(void);
void ClearTrackChangerFaults(void);

char Error_Count(void);
char ClearRoboterror(void);

#endif