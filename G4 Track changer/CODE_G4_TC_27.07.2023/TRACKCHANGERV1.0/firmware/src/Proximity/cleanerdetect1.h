#ifndef __CLEANER_DETECT_1_H_
#define __CLEANER_DETECT_1_H_

#include <stdbool.h>
#include <stdint.h>

#define CLEANER_DETECT_1_GPIO               CLEANER_DETECT_1_PROXIMITY_GPIO
#define CLEANER_DETECT_1_PORT               CLEANER_DETECT_1_PROXIMITY_PORT
#define CLEANER_DETECT_1_PIN                CLEANER_DETECT_1_PROXIMITY_PIN

bool CleanerDetect1Init(void);

uint32_t GetCleanerDetect1Count(void);
void ClearCleanerDetect1Count(void);
void StartCleanerDetect1Count(void);
void StopCleanerDetect1Count(void);
void ResumeCleanerDetect1Count(void);

bool GetCleanerDetect1PinState(void);

void CleanerDetect1TimeIncrement_ms(void);
void CleanerDetect1_Irq(void);

#endif
