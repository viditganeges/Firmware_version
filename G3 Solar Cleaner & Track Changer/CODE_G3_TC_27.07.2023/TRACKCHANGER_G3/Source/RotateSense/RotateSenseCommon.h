#ifndef __ROTATE_SENSE_COMMON_H_
#define __ROTATE_SENSE_COMMON_H_

#include <stdbool.h>
#include <stdint.h>

bool RotateSenseInit (void);

int32_t GetRotateSenseCount (void);
int32_t GetRotateSense1Count (void);
int32_t GetRotateSense2Count (void);
void SetRotateSenseCount (uint32_t count);
void ClearRotateSenseCount (void);
void StartRotateSenseCount (void);
void ResumeRotateSenseCount (void);
void StopRotateSenseCount (void);

void RotateSenseTimeIncrement_ms (void);

void DisableRotateSense(void);

#endif