#include "cleanerdetectcommon.h"

bool CleanerDetectInit(void)
{
  bool status = true;
  status&=CleanerDetect1Init();
  status&=CleanerDetect2Init();
  return status;
}

bool IsCleanerPresent(void)
{
return (GetCleanerDetect1PinState()&&GetCleanerDetect2PinState());
}