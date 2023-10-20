#ifndef _ROTATESENSE_H    /* Guard against multiple inclusion */
#define _ROTATESENSE_H

uint32_t prev_rotateCount;

bool RotateSense1Init (void);
uint32_t GetRotateSense1CountValue (void);
uint32_t GetAcelRotateCountValue (void);
void SetRotateSense1Count (uint32_t count);
void ClearRotateSense1Count (void);
void ClearAcelRotateCount (void);
void StartRotateSense1Count (void);
void ResumeRotateSense1Count (void);
void StopRotateSense1Count (void);
void RotateSense1TimeIncrement_ms (void);
void Rotate_Sense1_Irq(void);
char GetRotateSense1State(void);

bool RotateSense2Init (void);
uint32_t GetRotateSense2CountValue (void);
uint32_t GetBrushRotateSense2CountValue (void);
void SetRotateSense2Count (uint32_t count);
void ClearRotateSense2Count (void);
void ClearBrushRotateSense2Count (void);
void StartRotateSense2Count (void);
void ResumeRotateSense2Count (void);
void StopRotateSense2Count (void);
void RotateSense2TimeIncrement_ms (void);
void Rotate_Sense2_Irq(void);
char GetRotateSense2State(void);

bool RotateSenseInit (void);
uint32_t GetRotateSenseCount (void);
int GetRotateSense1Count (void);
int GetRotateSense2Count (void);
void SetRotateSenseCount (uint32_t count);
void ClearRotateSenseCount (void);
void StartRotateSenseCount (void);
void ResumeRotateSenseCount (void);
void StopRotateSenseCount (void);
bool IsRotateSensorEnabled (void);
bool *GetSetRotateSensorEnabledState(void);
void RotateSenseTimeIncrement_ms (void);

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
