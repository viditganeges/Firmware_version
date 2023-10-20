#include "main.h"

#define MAX_AUTO_TOLERANCE_TIME_SEC 15

static uint32_t autoDoneDateSeconds = 0;
static stScheduleTime scheduleTime;
static bool IsDayChanged (uint32_t dateSeconds);
static bool IsScheduledHrMinTime(uint32_t timeSeconds);
 
int schedule_int = 0;
int done_schedule = 0;
int temp_total_cycle=0,total_cycle = 0; 

unsigned int Schedule_Done = 0; 
 
char Daycount = 0;
 
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

char Get_Completed_Schedule()
{
  return done_schedule;
}

void Clear_Schedule_Done()
{
  done_schedule = 0;
}

void Increment_Done_Schedule(void)
{
    Schedule_Done++;
}

uint32_t GetScheduledTimeSeconds (void)
{
  uint32_t ScheduledHrMinSeconds;        
  stCycleFrequency* getCycleFrequency = GetSetcycleFrequencyLimits();   
  total_cycle = getCycleFrequency->Cycle; 
  if(schedule_int <= total_cycle-1) //-1
  {
    ScheduledHrMinSeconds = (scheduleTime.HOUR[schedule_int] * SECONDS_IN_A_HOUR) + (scheduleTime.MINUTE[schedule_int] * SECONDS_IN_A_MINUTE);
    schedule_int++;
  }
  else
  {
    schedule_int = 0;
  }
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
  uint32_t scheduledHrMinSec;
  
  stCycleFrequency* getCycleFrequency = GetSetcycleFrequencyLimits();   
  total_cycle = getCycleFrequency->Cycle; 
  if(schedule_int <= total_cycle-1) //-1
  {
    scheduledHrMinSec = (scheduleTime.HOUR[schedule_int] * SECONDS_IN_A_HOUR) + (scheduleTime.MINUTE[schedule_int] * SECONDS_IN_A_MINUTE);
    schedule_int++;
  }
  else
  {
    schedule_int = 0;
  }
  
//  scheduledHrMinSec = GetScheduledTimeSeconds();
  
  if(timeSeconds > scheduledHrMinSec && (GetCycleModeState() == CYCLE_IDLE || GetCycleModeState() == CYCLE_COMPLETED))
//  if((timeSeconds > ScheduledHrMinSeconds) && (GetCycleModeState() == CYCLE_IDLE || GetCycleModeState() == CYCLE_COMPLETED))
  {
    if((timeSeconds - scheduledHrMinSec ) <= MAX_AUTO_TOLERANCE_TIME_SEC)
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
  bool status = false;      //changed from true
  EepromHeartbeatConfiguration(EEPROM_READ);
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
      done_schedule = schedule_int;     
      SCHEDULE_LOG = 1;
      ClearAutoDoneSeconds();
      status = true;
    }   
    if(status == true && schedule_int == 0)
    {
        Clear_Schedule_Done();
    }
  }
  return status;  
}




