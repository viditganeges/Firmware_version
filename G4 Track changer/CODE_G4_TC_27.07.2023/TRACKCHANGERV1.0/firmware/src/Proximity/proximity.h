#ifndef __PROXIMITY_H_
#define __PROXIMITY_H_

#include <stdbool.h>
#include <stdint.h>

#define PROXIMITY_GPIO               ROW_DETECT_PROXIMITY_GPIO
#define PROXIMITY_PORT               ROW_DETECT_PROXIMITY_PORT
#define PROXIMITY_PIN                ROW_DETECT_PROXIMITY_PIN

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

#endif
