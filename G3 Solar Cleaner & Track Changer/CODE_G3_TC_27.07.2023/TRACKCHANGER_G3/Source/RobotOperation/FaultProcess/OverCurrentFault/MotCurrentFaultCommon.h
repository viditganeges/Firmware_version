#ifndef _MOT_CURRENT_FAULT_COMMON_H_
#define _MOT_CURRENT_FAULT_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  uint32_t maxIloadNoiseTimeValue;
  uint32_t maxIloadFreqTimeValue; 
  uint32_t maxIloadRepeatCountValue; 
  uint32_t motorPauseDelay;
}stMaxIloadConfigValues;

typedef struct{
  float Imot1;
  float Imot2;
  float Imot3;
}stMaxMotorCurrent;

typedef struct{
  float Dia;
  uint32_t Pulse;
 // uint32_t Speed;
}stWheelDia;

typedef struct{
  uint32_t Cycle;
}stCycleFrequency;

typedef struct{
  uint32_t I1,I2,I3,I4,I5,I6,I7;
  uint32_t P1,P2,P3,P4,P5,P6,P7;
}stInterval;

typedef struct{
  uint32_t Continue1;
  uint32_t Count1;
}stContinue;

typedef struct{
  uint32_t Return1;
}stReturn;

stWheelDia* GetSetwheelDiaLimits(void);
void GetWheelDiaLimits(stWheelDia * wheeldiaLimits);

typedef struct{
 // uint32_t Row[10];//
  //uint32_t R1,R2,R3,R4,R5,R6,R7;
  uint32_t Row[7],TRow;
  //uint32_t count1,Rcount2,Rcount3,Rcount4,Rcount5,Rcount6,Rcount7;
 // uint32_t P1,P2,P3,P4,P5,P6,P7;
}stnoofrows;

typedef struct{
  uint32_t runrfid;
}strunrfid;

stCycleFrequency* GetSetcycleFrequencyLimits(void);
void GetCycleFrequencyLimits(stCycleFrequency * CycleFrequencyLimits);

stInterval* GetSetIntervalLimits(void);
void GetIntervalLimits(stInterval * IntervalLimits);

stContinue* GetSetContinueLimits(void);
void GetContinueLimits(stContinue * ContinueLimits);

stReturn* GetSetReturnLimits(void);
void GetReturnLimits(stReturn * ReturnLimits);

stnoofrows* GetSetNoofrowsLimits(void);
void GetNoofrowsLimits(stnoofrows * NoofrowsLimits);

strunrfid* GetSetrunrfidLimits(void);
void GetrunrfidLimits(strunrfid * runrfidLimits);

stMaxMotorCurrent* GetSetMaxMotorCurrentLimits (void);
void GetMaxMotorCurrentLimits(stMaxMotorCurrent * motCurLimits);
stMaxIloadConfigValues *GetSetMaxIloadConfigValues (void);
void GetMaxIloadConfigValues(stMaxIloadConfigValues * configValues);

void CheckMotorOverLoad (void);
void CheckADCValues (void);
void FindAndUpdateImotOffsetValue (void);
bool IsMotorOverCurrentFault (void);
void ClearMotorOverCurrentFault (void);
void IloadMotMaxTimeIncrement_ms (void);

bool IsMotor1OverCurrentFault (void);
bool IsMotor2OverCurrentFault (void);
bool IsMotor3OverCurrentFault (void);

void ClearMotor1OverCurrentFault (void);
void ClearMotor2OverCurrentFault (void);
void ClearMotor3OverCurrentFault (void);
#endif