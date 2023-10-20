/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EDGESENSE_H    /* Guard against multiple inclusion */
#define _EDGESENSE_H

#include "main.h"

bool EdgeSense1Init (void);
bool EdgeSense2Init (void);

bool IsEdgeSense1Detected (void);
bool IsEdgeSense2Detected (void);

void SetEdgeSense1State (bool state);
void SetEdgeSense2State (bool state);

void UpdateSense1State (void);
void UpdateSense2State(void);

void EdgeSense1TimeIncrement_ms (void);
void EdgeSense2TimeIncrement_ms (void);

void Edge_Sense1_Irq (void);
void Edge_Sense2_Irq(void);

char GetEdgeSense1State(void);
char GetEdgeSense2State(void);

bool EdgeSenseInit (void);

bool IsEdgeSensor1Detected (void);
bool IsEdgeSensor2Detected (void);

bool IsPositiveEdgeReached (void);
bool IsNegativeEdgeReached (void);

void EdgeSenseTimeIncrement_ms (void);

bool *GetSetEdgeSensorEnabledState(void);

bool IsEdgeSensorEnabled (void);


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
