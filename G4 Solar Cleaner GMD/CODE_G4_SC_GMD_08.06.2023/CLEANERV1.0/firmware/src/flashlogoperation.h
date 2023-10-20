
#ifndef _FLASHLOGOPERATION_H    /* Guard against multiple inclusion */
#define _FLASHLOGOPERATION_H


#include "main.h"

uint32_t SDcardmountchecktimer;
bool SDcardmountcheckError;

void AutoCycleFlash_main(void);
void AutoCycleFlash_log_Data(void);

void  ERRORlogFlash_main(void);
void ERRORFlash_log_Data(void);

void FlashLogOperation(void);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
