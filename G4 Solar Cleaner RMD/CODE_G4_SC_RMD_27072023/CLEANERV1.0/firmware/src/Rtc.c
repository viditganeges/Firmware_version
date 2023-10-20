#include "Rtc.h"

#define DEFAULT_RTC_YEAR                        2022U
#define DEFAULT_RTC_MONTH                       1U
#define DEFAULT_RTC_DAY                         1U
#define DEFAULT_RTC_HOUR                        0U
#define DEFAULT_RTC_MINUTE                      0U
#define DEFAULT_RTC_SECOND                      0U

#define INVALID_CLOCK_VALUE                     0x1011

static bool dateTimeUpdatedFlag = false;
static bool rtcSecondsUpdatedFlag = false;
static bool RTCFreshStartFlag = true;
static bool checkRTCTimeValid (void);
static void SetDateTimeUpdated (void);


void RTC_Seconds_IRQHandler(void)
{
   rtcSecondsUpdatedFlag = true;
}

bool IsRTCSecondOccured (void)
{
  return rtcSecondsUpdatedFlag;
}

void ClearRTCSecondOccured (void)
{
  rtcSecondsUpdatedFlag = false;  
}

bool IsRTCFreshStart (void)
{
  return (RTCFreshStartFlag == true);  
}

//static void StoreRTCInvalidClock (void)
//{
//  volatile uint32_t * rtcInvalidValue = GetRTCInvalidClockRegPointer();
// *rtcInvalidValue = INVALID_CLOCK_VALUE;
//}
//
//static void ClearRTCInvalidClock (void)
//{
//  volatile uint32_t * rtcInvalidValue = GetRTCInvalidClockRegPointer();
// *rtcInvalidValue = 0xFFFF;
//}
//
//static bool IsRTCInvalidClock (void)
//{
//  volatile uint32_t * rtcInvalidValue = GetRTCInvalidClockRegPointer();
//  return (*rtcInvalidValue == INVALID_CLOCK_VALUE);
//}

bool RTCInit (void)                                         //done//
{
  bool status = true;           //sfalse;
  
  if(checkRTCTimeValid())
  {
      status = true;
      RTCFreshStartFlag = true;
  }
  else
  {
      SetRTCDefaultDateTime();  
      status = false;
      RTCFreshStartFlag = false;
  }
  
  return status;
}

static bool checkRTCTimeValid (void)                        //done//
{ 
  if((RTC_GetStatusFlags() & RTC_SR_TDERR_ERR_TIMEDATE))
  {
      return(false);
  }
  else
  {
    return (true);
  }
}

void GetRTCDateTime (sDateTime * dateTime)                  //done//
{
  struct tm read_time;
  external_rtc_read(&read_time);
  
  dateTime->date = read_time.tm_mday;
  dateTime->year = read_time.tm_year;
  dateTime->month = read_time.tm_mon;
  dateTime->hour = read_time.tm_hour;
  dateTime->minute = read_time.tm_min;
  dateTime->second = read_time.tm_sec;
}

bool SetRTCDateTime (sDateTime * dateTime)                  //done//
{
    bool status = true;
    
    struct tm write_time;
    
    write_time.tm_year = (uint16_t)dateTime->year;
    write_time.tm_mon = (uint8_t)dateTime->month;
    write_time.tm_mday = (uint8_t)dateTime->date;
    write_time.tm_hour = (uint8_t)dateTime->hour;
    write_time.tm_min = (uint8_t)dateTime->minute;
    write_time.tm_sec = (uint8_t)dateTime->second;

    if (!external_rtc_write(&write_time))
    {
       status = false;
       Serial_Debug("\nDate Time Validity Error");
    }
    else
    {
       SetDateTimeUpdated();
//       ClearRTCInvalidClock();
       status = true;
       Serial_Debug("\n RTC Set Success");
    }
    
    return(status);
}

bool CheckRTCDateTimeFormat(const sDateTime *dateTime)              //done//
{
  struct tm check_time;
  
  check_time.tm_year = (uint16_t)dateTime->year;
  check_time.tm_mon = (uint8_t)dateTime->month;
  check_time.tm_mday = (uint8_t)dateTime->date;
  check_time.tm_hour = (uint8_t)dateTime->hour;
  check_time.tm_min = (uint8_t)dateTime->minute;
  check_time.tm_sec = (uint8_t)dateTime->second;
  return RTC_CheckDatetimeFormat(&check_time);
}

bool SetRTCDefaultDateTime (void)                               //done//
{
  struct tm default_time;
  
  default_time.tm_year = DEFAULT_RTC_YEAR;
  default_time.tm_mon = DEFAULT_RTC_MONTH;
  default_time.tm_mday = DEFAULT_RTC_DAY;
  default_time.tm_hour = DEFAULT_RTC_HOUR;
  default_time.tm_min = DEFAULT_RTC_MINUTE;
  default_time.tm_sec = DEFAULT_RTC_SECOND;
  default_time.tm_isdst = 0;
  default_time.tm_wday = 0;
  
  if(external_rtc_write(&default_time) == true)
  {
    Serial_Debug("\nDefault RTCC Set");
    return true;
  }
  else
  {    
    Serial_Debug("\nDefault RTCC Set ERROR");
    return false;
  }   
}

uint32_t GetRTCDateTimeSeconds (void)
{
    struct tm current_datetime;
    external_rtc_read(&current_datetime);
    
    return RTC_ConvertDatetimeToSeconds(&current_datetime);
}

void GetRTCDateTimeString(char * rtcString, uint8_t rtcStringLength)                        //done//
{
   sDateTime currentDateTime;
   GetRTCDateTime(&currentDateTime);
   snprintf((char *)rtcString, rtcStringLength, "%02d-%02d-%04d %02d:%02d:%02d", currentDateTime.date, 
        currentDateTime.month,currentDateTime.year, currentDateTime.hour, 
        currentDateTime.minute, currentDateTime.second);
}

void ConvertRTCDateTimeToString(const sDateTime *dateTime, char * rtcString, uint8_t rtcStringLength)       //done//
{
  snprintf((char *)rtcString, rtcStringLength, "%02d-%02d-%04d %02d:%02d:%02d", dateTime->date, 
          dateTime->month,dateTime->year, dateTime->hour, 
          dateTime->minute, dateTime->second);
}

void ConvertRTCSecondsToDatetime(uint32_t seconds, sDateTime *dateTime)                                     //done//
{
    struct tm secondstodatetime_time;
    RTC_ConvertSecondsToDatetime(seconds, &secondstodatetime_time);
    dateTime->date = secondstodatetime_time.tm_mday;
    dateTime->year = secondstodatetime_time.tm_year;
    dateTime->month = secondstodatetime_time.tm_mon;
    dateTime->hour = secondstodatetime_time.tm_hour;
    dateTime->minute = secondstodatetime_time.tm_min;
    dateTime->second = secondstodatetime_time.tm_sec;
}

uint32_t ConvertRTCDatetimeToSeconds(const sDateTime *datetime)
{
    struct tm datetimetoseconds_time;
    datetimetoseconds_time.tm_year = (uint16_t)datetime->year;
    datetimetoseconds_time.tm_mon = (uint8_t)datetime->month;
    datetimetoseconds_time.tm_mday = (uint8_t)datetime->date;
    datetimetoseconds_time.tm_hour = (uint8_t)datetime->hour;
    datetimetoseconds_time.tm_min = (uint8_t)datetime->minute;
    datetimetoseconds_time.tm_sec = (uint8_t)datetime->second;
    return RTC_ConvertDatetimeToSeconds(&datetimetoseconds_time);
}

uint32_t ConvertRTCDateToSeconds(sDateTime * datetime)                  
{
    struct tm datetoseconds_time;
    datetoseconds_time.tm_year = (uint16_t)datetime->year;
    datetoseconds_time.tm_mon = (uint8_t)datetime->month;
    datetoseconds_time.tm_mday = (uint8_t)datetime->date;
    datetoseconds_time.tm_hour = 0;
    datetoseconds_time.tm_min = 0;
    datetoseconds_time.tm_sec = 0;
    return RTC_ConvertDatetimeToSeconds(&datetoseconds_time);
}

uint32_t ConvertRTCTimeToSeconds(sDateTime * datetime)                  
{
   uint32_t seconds = (datetime->hour * SECONDS_IN_A_HOUR) +
              (datetime->minute * SECONDS_IN_A_MINUTE) + datetime->second;

   return seconds;
}

static void SetDateTimeUpdated (void)
{
  dateTimeUpdatedFlag = true;  
}

bool IsDateTimeUpdated (void)
{
  return dateTimeUpdatedFlag;
}

void ClearDateTimeUpdated (void)
{
  dateTimeUpdatedFlag = false;  
}




