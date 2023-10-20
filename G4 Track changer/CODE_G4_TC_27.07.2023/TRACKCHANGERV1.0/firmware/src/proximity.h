#ifndef _PROXIMITY_H    /* Guard against multiple inclusion */
#define _PROXIMITY_H

#include "main.h"

uint16_t row_detected;

bool ProximityInit(void);
uint32_t GetProximityCount(void);
void ClearProximityCount(void);
void ClearRow_DetectCount (void);
void StartProximityCount(void);
void StopProximityCount(void);
void ResumeProximityCount(void);
bool IsValidProximitySensorState (void);
void SetProximityCount (uint32_t count);
void ProximityTimeIncrement_ms(void);
void Proximity_Irq(void);

bool CleanerDetect1Init(void);
bool CleanerDetect2Init(void);

uint32_t GetCleanerDetect1Count(void);
uint32_t GetCleanerDetect2Count(void);

void ClearCleanerDetect1Count(void);
void ClearCleanerDetect2Count(void);

void StartCleanerDetect1Count(void);
void StartCleanerDetect2Count(void);

void StopCleanerDetect1Count(void);
void StopCleanerDetect2Count(void);

void ResumeCleanerDetect1Count(void);
void ResumeCleanerDetect2Count(void);

bool GetCleanerDetect1PinState(void);
bool GetCleanerDetect2PinState(void);

void CleanerDetect1TimeIncrement_ms(void);
void CleanerDetect2TimeIncrement_ms(void);

void CleanerDetect1_Irq(void);
void CleanerDetect2_Irq(void);

bool CleanerDetectInit(void);
bool IsCleanerPresent(void);


#endif /* _EXAMPLE_FILE_NAME_H */


