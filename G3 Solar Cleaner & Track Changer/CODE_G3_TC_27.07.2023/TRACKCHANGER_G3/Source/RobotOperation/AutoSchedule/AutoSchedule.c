#include "AutoSchedule.h"
#include "Rtc.h"
#include "EepromStore.h"
#include "MotCurrentFaultCommon.h"

#define MAX_AUTO_TOLERANCE_TIME_SEC 5

static uint32_t autoDoneDateSeconds = 0;
static stScheduleTime scheduleTime;
extern uint32_t Cumulative_distance;

char Daycount = 0;
extern int trackchangercount;
extern int tc_count;

static bool IsDayChanged (uint32_t dateSeconds);
static bool IsScheduledHrMinTime(uint32_t timeSeconds);

void ClearAutoDoneSeconds(void)
{
autoDoneDateSeconds = 0;
}

stScheduleTime* GetSetScheduleTime(void)
{
  return(&scheduleTime);
}

void SetScheduleTime(stScheduleTime *schdTime)
{
  scheduleTime = *schdTime;
}

uint32_t GetScheduledTimeSeconds (void)
{
  uint32_t ScheduledHrMinSeconds;
  ScheduledHrMinSeconds = (scheduleTime.hour * SECONDS_IN_A_HOUR) + (scheduleTime.minute * SECONDS_IN_A_MINUTE);
  return ScheduledHrMinSeconds;  
}

static bool IsDayChanged (uint32_t dateSeconds)
{
  if(autoDoneDateSeconds != dateSeconds)
  {
    return true;
  }
  else
  {
    return false;
  }  
}

static bool IsScheduledHrMinTime(uint32_t timeSeconds)
{
  uint32_t scheduledHrMinSec = GetScheduledTimeSeconds();
  if(timeSeconds > scheduledHrMinSec)
  {
    if((timeSeconds - scheduledHrMinSec) <= MAX_AUTO_TOLERANCE_TIME_SEC)
    {
      return true;      
    }
    else
    {
      return false;
    }      
  }
  else
  {
    return false;
  }  
}

bool IsSchedueledTime(void)
{
  bool status = false;
  if(IsRTCSecondOccured())
  {
    ClearRTCSecondOccured();
    sDateTime dateTime;
    GetRTCDateTime (&dateTime);
    uint32_t dateSeconds = ConvertRTCDateToSeconds(&dateTime);
    if(IsDayChanged(dateSeconds) == true)
    {
      stContinue *setContinueValue = GetSetContinueLimits();
      uint32_t timeSeconds = ConvertRTCTimeToSeconds(&dateTime);
      if(IsScheduledHrMinTime(timeSeconds) && setContinueValue->Continue1 != 2)
      {      
        autoDoneDateSeconds = dateSeconds;
        Daycount = Daycount + 1;
        Cumulative_distance = 0;
        if (Daycount > 7 || Daycount == 1 || trackchangercount == 0)
        {
//          Cumulative_distance = 0;
          Daycount = 1;
        }
        EepromSetAutoScheduleTime(EEPROM_WRITE);
        status = true;
      }
    }
  }
  return status;  
}
