#ifndef _AUTOSCHEDULE_H    /* Guard against multiple inclusion */
#define _AUTOSCHEDULE_H

#include "main.h"

#define SECONDS_IN_A_DAY (86400U)
#define SECONDS_IN_A_HOUR (3600U)
#define SECONDS_IN_A_MINUTE (60U)

typedef struct{
  uint8_t hour;
  uint8_t minute;
  uint8_t HOUR[30];
  uint8_t MINUTE[30];
}stScheduleTime;

void ClearAutoDoneSeconds(void);
void Increment_Done_Schedule(void);
char Get_Completed_Schedule(void);
void Clear_Schedule_Done(void);
void SetScheduleTime(stScheduleTime *schdTime);

bool IsSchedueledTime(void);
uint32_t GetScheduledTimeSeconds (void);
stScheduleTime* GetSetScheduleTime(void);

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
