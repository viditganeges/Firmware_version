#ifndef __CLEANER_DETECT_2_H_
#define __CLEANER_DETECT_2_H_

#include <stdbool.h>
#include <stdint.h>

#define CLEANER_DETECT_2_GPIO               CLEANER_DETECT_2_PROXIMITY_GPIO
#define CLEANER_DETECT_2_PORT               CLEANER_DETECT_2_PROXIMITY_PORT
#define CLEANER_DETECT_2_PIN                CLEANER_DETECT_2_PROXIMITY_PIN

bool CleanerDetect2Init(void);

uint32_t GetCleanerDetect2Count(void);
void ClearCleanerDetect2Count(void);
void StartCleanerDetect2Count(void);
void StopCleanerDetect2Count(void);
void ResumeCleanerDetect2Count(void);

bool GetCleanerDetect2PinState(void);

void CleanerDetect2TimeIncrement_ms(void);
void CleanerDetect2_Irq(void);

#endif
