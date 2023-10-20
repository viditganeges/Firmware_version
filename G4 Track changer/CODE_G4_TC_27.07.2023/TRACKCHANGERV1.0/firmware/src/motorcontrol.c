#include "main.h"

static stMaxIloadConfigValues maxIloadConfigValues;
static stMaxMotorCurrent maxMotorCurrentLimits;
static stCountRange countRange;
static stWheelDia maxWheelDiaLimits;   
static stCycleFrequency maxCycleFrequencyLimits; 
static stInterval maxIntervalLimits;
static stContinue maxContinueLimits;
static stReturn maxReturnLimits;
static stcomdistance maxComDistanceLimits;
//static stnoofrows maxNoofrowsLimits;
static strunrfid  maxrunrfidLimits;

static bool brushEnabledState                   = true;
static bool LinearEnabledState                  = true;

static strelay maxrelayLimits;

static uint8_t setBatMinSOC;

stMaxIloadConfigValues* GetSetMaxIloadConfigValues(void)
{
  return(&maxIloadConfigValues);  
}

void GetMaxIloadConfigValues(stMaxIloadConfigValues * configValues)
{
  *configValues = maxIloadConfigValues;  
}


stMaxMotorCurrent* GetSetMaxMotorCurrentLimits(void)
{
  return(&maxMotorCurrentLimits);
}

void GetMaxMotorCurrentLimits(stMaxMotorCurrent * motCurLimits)
{
  *motCurLimits = maxMotorCurrentLimits;
}

stCountRange *GetSetCountRange(void)
{
  return(&countRange);
}

stCountRange *GetCountRange(void)
{
  return(&countRange);
}

stWheelDia* GetSetwheelDiaLimits(void)
{
  return(&maxWheelDiaLimits);
}

void GetWheelDiaLimits(stWheelDia * wheeldiaLimits)
{
  *wheeldiaLimits = maxWheelDiaLimits;
}

stCycleFrequency* GetSetcycleFrequencyLimits(void)
{
  return(&maxCycleFrequencyLimits);
}

void GetCycleFrequencyLimits(stCycleFrequency * CycleFrequencyLimits)
{
  *CycleFrequencyLimits = maxCycleFrequencyLimits;
}


stInterval* GetSetIntervalLimits(void)
{
  return(&maxIntervalLimits);
}

void GetIntervalLimits(stInterval * IntervalLimits)
{
  *IntervalLimits = maxIntervalLimits;
}


stContinue* GetSetContinueLimits(void)
{
  return(&maxContinueLimits);
}

void GetContinueLimits(stContinue * ContinueLimits)
{
  *ContinueLimits = maxContinueLimits;
}

stReturn* GetSetReturnLimits(void)
{
  return(&maxReturnLimits);
}

void GetReturnLimits(stReturn * ReturnLimits)
{
  *ReturnLimits = maxReturnLimits;
}

stcomdistance* GetSetComDistanceLimits(void)
{
  return(&maxComDistanceLimits);
}

void GetComDistanceLimits(stcomdistance * ComDistanceLimits)
{
  *ComDistanceLimits = maxComDistanceLimits;
}

stnoofrows* GetSetNoofrowsLimits(void)
{
  return(&maxNoofrowsLimits);
}

void GetNoofrowsLimits(stnoofrows * NoofrowsLimits)
{
  *NoofrowsLimits = maxNoofrowsLimits;
}

strunrfid* GetSetrunrfidLimits(void)
{
  return(&maxrunrfidLimits);
}

void GetrunrfidLimits(strunrfid * runrfidLimits)
{
  *runrfidLimits = maxrunrfidLimits;
}

bool * GetSetBrushEnabledState (void)
{
  return(&brushEnabledState);
}

bool IsBrushEnabled (void)
{
  return brushEnabledState;
}

bool * GetSetLinearEnabledState (void)
{
  return(&LinearEnabledState);
}

bool IsLinearEnabled (void)
{
  return LinearEnabledState;
}

uint8_t * GetSetMinBattSoc (void)
{
  return &setBatMinSOC;  
}

strelay* GetSetrelayLimits(void)
{
  return(&maxrelayLimits);
}

void GetrelayLimits(strelay * relayLimits)
{
  *relayLimits = maxrelayLimits;
}
