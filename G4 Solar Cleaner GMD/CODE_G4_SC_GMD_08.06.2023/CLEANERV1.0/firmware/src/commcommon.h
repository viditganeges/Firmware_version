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

#ifndef _COMMCOMMON_H    /* Guard against multiple inclusion */
#define _COMMCOMMON_H

#include "main.h"

typedef struct{
  bool enable;
  int interval_ms;
  int noOfMessages;
  int maxReconnectTime;
}stHeartbeatConfig;

void CommTimeIncrement_ms (void);
void CommTimerStop (void);
bool IsCommTimeOver (void);
 
ErrorStatus UartTransmit (uint8_t * txBuffer, uint16_t txBufferLength);
stHeartbeatConfig* GetSetHeartbeatConfig(void);
void RestartCommTimer (void);
void ClearCommTimeOver (void);

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
