#include "RotateSense1.h"
#include "RotateSense2.h"
#include "QuadDecoder.h"
#include "RobotOperation.h"
#include <math.h>
#include <stdlib.h>

#define COUNTSENSOR     ROTATESENSOR_1          // ROTATESENSOR_2

bool RotateSenseInit (void)
{
  bool status = true;
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
    status &= RotateSense1Init();
    status &= RotateSense2Init();
  }
  else
  {
    status &=QuadDecoderInit();
  }
  return status;  
}

void SetRotateSenseCount (uint32_t count)
{
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
    SetRotateSense1Count(count);
    SetRotateSense2Count(count);  
  }
  else
  {
    SetQuadDecoderCount(count);
    
  }
}

void ClearRotateSenseCount (void)
{
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
    ClearRotateSense1Count();
    ClearRotateSense2Count();
  }
  else
  {
    ClearQuadDecoderCount();
  }
}

void StartRotateSenseCount (void)
{
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
    StartRotateSense1Count();
    StartRotateSense2Count();
  }
  else
  {
    StartQuadDecoderCount();
  }
}

void ResumeRotateSenseCount (void)
{
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
    ResumeRotateSense1Count();
    ResumeRotateSense2Count();
  }
}

void StopRotateSenseCount (void)
{
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
    StopRotateSense1Count();
    StopRotateSense2Count();
  }
}

int32_t GetRotateSenseCount (void)
{
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
#if COUNTSENSOR == ROTATESENSOR_1
    return GetRotateSense1CountValue();  
#else
    return GetRotateSense1CountValue(); 
#endif
  }
  else
  {
    return abs(GetQuadDecoderCountValue());
  }
}

int32_t GetRotateSense1Count (void)
{
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
    return GetRotateSense1CountValue();  
  }
  else
  {
    return abs(GetQuadDecoderCountValue());
  }
}

int32_t GetRotateSense2Count (void)
{
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
    return GetRotateSense2CountValue();
  }
  else
  {
    return 0;
  }
}

void RotateSenseTimeIncrement_ms (void)
{
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
    RotateSense1TimeIncrement_ms();
    RotateSense2TimeIncrement_ms();
  }
  else
  {
    QuadDecoderTimer_ms();
  }
}
void DisableRotateSense(void)
{
  if(GetTcControlMode()==PROXIMITY_MODE)
  {
    DisableRotateSense_1();
    DisableRotateSense_2();
  }
  else
  {
    DisableQuadDecoder();
  }
}