#ifndef _RTC_H    /* Guard against multiple inclusion */
#define _RTC_H

#include "main.h"

#define SECONDS_IN_A_DAY (86400U)
#define SECONDS_IN_A_HOUR (3600U)
#define SECONDS_IN_A_MINUTE (60U)

typedef struct 
{
  uint16_t year;  /*!< Range from 1970 to 2099.*/
  uint8_t month;  /*!< Range from 1 to 12.*/
  uint8_t date;    /*!< Range from 1 to 31 (depending on month).*/
  uint8_t hour;   /*!< Range from 0 to 23.*/
  uint8_t minute; /*!< Range from 0 to 59.*/
  uint8_t second; /*!< Range from 0 to 59.*/
}sDateTime;

typedef struct 
{
  uint16_t year;  /*!< Range from 1970 to 2099.*/
  uint8_t month;  /*!< Range from 1 to 12.*/
  uint8_t date;    /*!< Range from 1 to 31 (depending on month).*/
  uint8_t hour;   /*!< Range from 0 to 23.*/
  uint8_t minute; /*!< Range from 0 to 59.*/
  uint8_t second; /*!< Range from 0 to 59.*/
}sd_DateTime;

void RTC_Seconds_IRQHandler(void);
bool IsRTCSecondOccured (void);
void ClearRTCSecondOccured (void);
bool IsRTCFreshStart (void);

bool RTCInit (void);
void GetRTCDateTime (sDateTime * dateTime);
bool SetRTCDateTime (sDateTime * dateTime);
bool CheckRTCDateTimeFormat(const sDateTime *dateTime);
bool SetRTCDefaultDateTime (void);
uint32_t GetRTCDateTimeSeconds (void);
void GetRTCDateTimeString(char * rtcString, uint8_t rtcStringLength);
void ConvertRTCDateTimeToString(const sDateTime *dateTime, char * rtcString, uint8_t rtcStringLength);
void ConvertRTCSecondsToDatetime(uint32_t seconds, sDateTime *dateTime);
uint32_t ConvertRTCDatetimeToSeconds(const sDateTime *datetime);
uint32_t ConvertRTCDateToSeconds(sDateTime * datetime) ;
uint32_t ConvertRTCTimeToSeconds(sDateTime * datetime);

bool IsDateTimeUpdated (void);
void ClearDateTimeUpdated (void);

#endif /* _EXAMPLE_FILE_NAME_H */

