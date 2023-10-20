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

#ifndef _CYCLEMODE_H    /* Guard against multiple inclusion */
#define _CYCLEMODE_H

//#define SERIAL_DEBUG_CYCLEMODE

typedef enum eCycleModeState_def
{
  CYCLE_IDLE,
  CYCLE_START,
  CYCLE_POS_START,
  CYCLE_WAIT_TILL_POS_END,
  CYCLE_NEG_START,
  CYCLE_WAIT_TILL_NEG_END,
  CYCLE_COMPLETED
}eCycleModeState;

eCycleModeState GetCycleModeState (void);

stRobotPwmParam * GetSetCycleManualPwmParameters (void);
stRobotPwmParam * GetSetCycleAutoPwmParameters (void);

void SetCycleModeState (eCycleModeState cycleModeSt);
void CycleModeFSM (void);

bool IsCycleModeContinution (void);

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
