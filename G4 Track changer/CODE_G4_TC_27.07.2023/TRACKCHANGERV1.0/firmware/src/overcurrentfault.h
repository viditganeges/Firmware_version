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

#ifndef _OVERCURRENTFAULT_H    /* Guard against multiple inclusion */
#define _OVERCURRENTFAULT_H

#include "main.h"

#define CURRENT_AADR_A  0x68
#define CURRENT_AADR_B  0x6C
#define CURRENT_AADR_C  0x6A

float current_read_A(void);
float current_read_B(void);
float current_read_C(void);

float GetImot1Value (void);
float GetImot2Value (void);
float GetImot3Value (void);


typedef enum eFaultFlags_def
{
  NO_FAULT                       = 0x0000,
  OVER_CURRENT_MOT1_FAULT,
  OVER_CURRENT_MOT2_FAULT,
  OVER_CURRENT_MOT3_FAULT,
  MIN_BAT_VOLTAGE_FAULT, 
  OVER_BTEMP_FAULT,
  BATTERY_FAULT,
  ZIGBEE_FAULT,
  RTC_FAULT,
  OVER_DTEMP_FAULT,                //not used in cleaner
  MOTOR_STALL_FAULT,               //not used in cleaner
  COMMUNICATION_FAULT,
  SPI_FLASH_FAULT,                  //not used in cleaner
  OTAA_FAILURE,                     //not used in cleaner
  EEPROM_FAULT,                     //not used in cleaner
  MAX_BAT_VOLTAGE_FAULT,            //not used in cleaner
  UNKNOWN_FAULT,                    //not used in cleaner
  MAX_FAULT,                         /* This should be the last enum above ALL_FAULT. Its used to keep track maximum no of fault codes */
  ALL_FAULT                     = 0xFFFF   /* This should be the last enum */
}eFaultFlags;

typedef enum eErrorState_def
{
  ER_IDLE,
  ER_NO_ERROR,
  ER_CLEAR_ERROR,
  ER_ERROR
}eErrorState;

typedef struct {
  uint32_t errorCodeTime[MAX_FAULT];
  int8_t errorCodes[MAX_FAULT];
  uint8_t noOffaults;
} stFaultTimeData;

char Error_Count(void);
char ClearRoboterror(void);

char Robot_Error_Count;
char Robot_Error_Flag;
char Error_Cleared,Robot_Error_Count1;


void ClearAllFaults (void);
void ClearFaultsOnRobotCommand (void);
bool IsOnlyNonMotorError (void);
void CheckError (void);
eErrorState GetErrorState (void);
void SetErrorState (eErrorState errorSt);

//stFaultTimeData * GetFaultTimeData(void);
//void ReportErrorOnZigeeStart (void);
//uint8_t * GetSetMinBattSoc (void);
uint32_t GetMotorFaultCode (void);
void SetFault(eFaultFlags faultFlag);
//void ClearFault(eFaultFlags faultFlag);
//void ClearFaultsOnRobotCommand (void);
bool IsRTCFault (void);
bool IsAnyFault(void);
//void CheckCommunicationError (void);
//
//
void IloadMot1MaxTimeIncrement_ms (void);
bool IsMot1OverCurrentFault (void);
void ClearMot1OverCurrentFault (void);
void CheckMot1OverLoad (void);
void ClearMot1MaxOverCurrentCount (void);

void IloadMot2MaxTimeIncrement_ms (void);
bool IsMot2OverCurrentFault (void);
void ClearMot2OverCurrentFault (void);
void CheckMot2OverLoad (void);
void ClearMot2MaxOverCurrentCount (void);

void IloadMot3MaxTimeIncrement_ms (void);
bool IsMot3OverCurrentFault (void);
void ClearMot3OverCurrentFault (void);
void CheckMot3OverLoad (void);
void ClearMot3MaxOverCurrentCount (void);
//
//typedef struct
//{
//  uint32_t maxIloadNoiseTimeValue;
//  uint32_t maxIloadFreqTimeValue; 
//  uint32_t maxIloadRepeatCountValue; 
//  uint32_t motorPauseDelay;
//}stMaxIloadConfigValues;
//
//typedef struct{
//  float Imot1;
//  float Imot2;
//  float Imot3;
//}stMaxMotorCurrent;
//                                       //07D
//typedef struct{
//  float Dia;
//  float Pulse;
//  uint32_t Speed;
//}stWheelDia;
//
//typedef struct{
//  float Cycle;
//}stCycleFrequency;
//
//typedef struct{
//  float I1,I2,I3,I4,I5,I6,I7;
//  float P1,P2,P3,P4,P5,P6,P7;
//}stInterval;
//
//typedef struct{
//  uint32_t Continue1;
//  uint32_t Count1;
//}stContinue;
//
//typedef struct{
//  uint32_t Return1;
//}stReturn;
//
//typedef struct {
//  uint32_t CDistance;
//} stcomdistance;
//
//typedef struct{
//  uint32_t Row[7],TRow;
//}stnoofrows;
//
//stMaxMotorCurrent* GetSetMaxMotorCurrentLimits (void);
//void GetMaxMotorCurrentLimits(stMaxMotorCurrent * motCurLimits);
//
//stMaxIloadConfigValues *GetSetMaxIloadConfigValues (void);
//void GetMaxIloadConfigValues(stMaxIloadConfigValues * configValues);
//
////07D
//stWheelDia* GetSetwheelDiaLimits(void);
//void GetWheelDiaLimits(stWheelDia * wheeldiaLimits);
//
//stCycleFrequency* GetSetcycleFrequencyLimits(void);
//void GetCycleFrequencyLimits(stCycleFrequency * CycleFrequencyLimits);
//
//stInterval* GetSetIntervalLimits(void);
//void GetIntervalLimits(stInterval * IntervalLimits);
//
//stContinue* GetSetContinueLimits(void);
//void GetContinueLimits(stContinue * ContinueLimits);
//
//stReturn* GetSetReturnLimits(void);
//void GetReturnLimits(stReturn * ReturnLimits);
//
//stcomdistance* GetSetComDistanceLimits(void);
//void GetComDistanceLimits(stcomdistance * ComDistanceLimits);
//
//stnoofrows* GetSetNoofrowsLimits(void);
//void GetNoofrowsLimits(stnoofrows * NoofrowsLimits); 
//
void CheckMotorOverLoad (void);
//void CheckADCValues (void);
//void FindAndUpdateImotOffsetValue (void);
bool IsMotorOverCurrentFault (void);
void ClearMotorOverCurrentFault (void);
void IloadMotMaxTimeIncrement_ms (void);

bool IsMotor1OverCurrentFault (void);
bool IsMotor2OverCurrentFault (void);
bool IsMotor3OverCurrentFault (void);

void ClearMotor1OverCurrentFault (void);
void ClearMotor2OverCurrentFault (void);
void ClearMotor3OverCurrentFault (void);


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
