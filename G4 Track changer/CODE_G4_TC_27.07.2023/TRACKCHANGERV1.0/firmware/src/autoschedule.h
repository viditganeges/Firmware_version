#ifndef __AUTO_SCHEDULE_H_
#define __AUTO_SCHEDULE_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct{
  uint8_t hour;
  uint8_t minute;
}stScheduleTime;

char Daycount;

void ClearAutoDoneSeconds(void);
bool IsSchedueledTime(void);
uint32_t GetScheduledTimeSeconds (void);
void SetScheduleTime(stScheduleTime *schdTime);
stScheduleTime* GetSetScheduleTime(void);

#endif