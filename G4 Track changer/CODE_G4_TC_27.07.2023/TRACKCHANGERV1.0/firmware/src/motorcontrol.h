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

#ifndef _MOTORCONTROL_H    /* Guard against multiple inclusion */
#define _MOTORCONTROL_H

#include "main.h"

typedef struct
{
  int maxIloadNoiseTimeValue;
  int maxIloadFreqTimeValue; 
  int maxIloadRepeatCountValue; 
  int motorPauseDelay;
}stMaxIloadConfigValues;

typedef struct{
  float Imot1;
  float Imot2;
  float Imot3;
}stMaxMotorCurrent;

typedef struct {
 int maxNegCountForPVsetup;
 int maxPosCountForPVsetup;
} stCountRange;

typedef struct{
  int Dia;
  int Pulse;
  int Speed;
}stWheelDia;

typedef struct{
  int Cycle;
}stCycleFrequency;

typedef struct{
  uint32_t I1,I2,I3,I4,I5,I6,I7;
  uint32_t P1,P2,P3,P4,P5,P6,P7;
}stInterval;

typedef struct{
  int Continue1;
  int Count1;
}stContinue;

typedef struct{
  int Return1;
}stReturn;

typedef struct {
  int CDistance;
} stcomdistance;

typedef struct
{
  int Row[7];
  int TRow;
}stnoofrows;

stnoofrows maxNoofrowsLimits;

typedef struct
{
  int runrfid;
}strunrfid;

typedef struct{
  uint32_t T1;
}strelay;

stMaxIloadConfigValues *GetSetMaxIloadConfigValues (void);
void GetMaxIloadConfigValues(stMaxIloadConfigValues * configValues);

stMaxMotorCurrent* GetSetMaxMotorCurrentLimits (void);
void GetMaxMotorCurrentLimits(stMaxMotorCurrent * motCurLimits);

stWheelDia* GetSetwheelDiaLimits(void);
void GetWheelDiaLimits(stWheelDia * wheeldiaLimits);

bool * GetSetLinearEnabledState (void);
bool IsLinearEnabled (void);

bool * GetSetBrushEnabledState (void);
bool IsBrushEnabled (void);    //static removed

uint8_t * GetSetMinBattSoc (void);

stCycleFrequency* GetSetcycleFrequencyLimits(void);
void GetCycleFrequencyLimits(stCycleFrequency * CycleFrequencyLimits);

stInterval* GetSetIntervalLimits(void);
void GetIntervalLimits(stInterval * IntervalLimits);

stContinue* GetSetContinueLimits(void);
void GetContinueLimits(stContinue * ContinueLimits);

stReturn* GetSetReturnLimits(void);
void GetReturnLimits(stReturn * ReturnLimits);

stcomdistance* GetSetComDistanceLimits(void);
void GetComDistanceLimits(stcomdistance * ComDistanceLimits);

stnoofrows* GetSetNoofrowsLimits(void);
void GetNoofrowsLimits(stnoofrows * NoofrowsLimits);

strunrfid* GetSetrunrfidLimits(void);
void GetrunrfidLimits(strunrfid * runrfidLimits);


stCountRange *GetCountRange(void);
stCountRange *GetSetCountRange(void);

strelay* GetSetrelayLimits(void);
void GetrelayLimits(strelay * relayLimits);

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
