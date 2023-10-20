#ifndef _OVERCURRENTFAULT_H    /* Guard against multiple inclusion */
#define _OVERCURRENTFAULT_H

#include "main.h"

#define CURRENT_AADR_A  0x44
#define CURRENT_AADR_B  0x45
#define CURRENT_AADR_C  0x40

float AbsoluteValue(float value);

void INA226_Init(uint8_t slaveID);
uint16_t INA226_read(uint8_t slaveID, uint8_t regPointer);
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
  MOTOR1_DRIVER_FAULT,//           = 0x1000,
  MOTOR2_DRIVER_FAULT,//           = 0x1002,
  MOTOR3_DRIVER_FAULT,//           = 0x1004,
  MOTOR1_FAULT,//                  = 0x1008,
  MOTOR2_FAULT,
  MOTOR3_FAULT,
  MOTOR1_CURRENT_RUNTIME_EXCEEDED_FAULT,
  MOTOR2_CURRENT_RUNTIME_EXCEEDED_FAULT,
  MOTOR3_CURRENT_RUNTIME_EXCEEDED_FAULT,
  MOTOR1_TEMP_FAULT,
  MOTOR2_TEMP_FAULT,
  MOTOR3_TEMP_FAULT,
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
void ClearRoboterror(void);

char Robot_Error_Count;
char Robot_Error_Flag;
char Error_Cleared,Robot_Error_Count1;

void ClearAllFaults (void);
void ClearFaultsOnRobotCommand (void);
bool IsOnlyNonMotorError (void);
void CheckError (void);
eErrorState GetErrorState (void);
void SetErrorState (eErrorState errorSt);

uint32_t GetMotorFaultCode (void);
void SetFault(eFaultFlags faultFlag);
void ClearFault(eFaultFlags faultFlag);
bool IsRTCFault (void);
bool IsAnyFault(void);
void CheckCommunicationError (void);
void IloadMot1MaxTimeIncrement_ms (void);
bool IsMot1OverCurrentFault (void);
void ClearMot1OverCurrentFault (void);
void CheckMot1OverLoad (void);
void ClearMot1MaxOverCurrentCount (void);
void Motor1FaultCheck(void);
void Motor1run(void);
void Motor1CurrentDurationCheck(void);
void Motor2CurrentDurationCheck(void);
void Motor3CurrentDurationCheck(void);

void IloadMot2MaxTimeIncrement_ms (void);
bool IsMot2OverCurrentFault (void);
void ClearMot2OverCurrentFault (void);
void CheckMot2OverLoad (void);
void ClearMot2MaxOverCurrentCount (void);
void Motor2FaultCheck(void);
void Motor2run(void);

void IloadMot3MaxTimeIncrement_ms (void);
bool IsMot3OverCurrentFault (void);
void ClearMot3OverCurrentFault (void);
void CheckMot3OverLoad (void);
void ClearMot3MaxOverCurrentCount (void);
void BrushMotorFaultCheck(void);
void BrushMotorrun(void);

void CheckMotorOverLoad (void);
void CheckMotorDriverFault(void);
void CheckRunFault(void);
void CheckMotorTempFault(void);
void CheckMotorCurrentDurationFault(void);

void CheckMot1Temp (void);
void CheckMot2Temp (void);
void CheckMot3Temp (void);

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
