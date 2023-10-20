#ifndef _FLASH_H    /* Guard against multiple inclusion */
#define _FLASH_H

typedef enum eEepromStatus_def {
  EEPROM_OK             = 0x00,
  EEPROM_READ_ERROR     = 0x01,
  EEPROM_WRITE_ERROR    = 0x02,
  EEPROM_LOCK_ERROR     = 0x04,
  EEPROM_UNLOCK_ERROR   = 0x08
}eEepromStatus;

typedef enum eepromAccessType_def
{
  EEPROM_READ,
  EEPROM_WRITE  
}eEpromAccessType;

typedef eEepromStatus (*EepromStoreProcess_Ptr)(eEpromAccessType eepromOp);

eEepromStatus InitialiseEepromVariables(void);


eEepromStatus EepromDataNil(eEpromAccessType eepromOp);
eEepromStatus EepromSetMode(eEpromAccessType eepromOp);
eEepromStatus EepromSetMotorPwmManualMode(eEpromAccessType eepromOp);
eEepromStatus EepromSetMotorPwmAutoMode(eEpromAccessType eepromOp);
eEepromStatus EepromSetAutoScheduleTime(eEpromAccessType eepromOp);
eEepromStatus EepromSetEdgeSensorState(eEpromAccessType eepromOp);
eEepromStatus EepromSetPulseCount(eEpromAccessType eepromOp);
eEepromStatus EepromSetLowBatSoC(eEpromAccessType eepromOp);
eEepromStatus EepromZigbeeConfiguration(eEpromAccessType eepromOp);
eEepromStatus EepromDevIDInfo(eEpromAccessType eepromOp);
eEepromStatus EepromStoreTrackInfo(eEpromAccessType eepromOp);
eEepromStatus EepromStoreTrackInfo2(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRowLength(eEpromAccessType eepromOp);
eEepromStatus EepromStoreCleanerMAC(eEpromAccessType eepromOp);
eEepromStatus EepromStoreEncoderPulseTolerance(eEpromAccessType eepromOp);
eEepromStatus EepromSetTcControlMode(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRfidData(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRfidData2(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRfidData3(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRfidState(eEpromAccessType eepromOp);
eEepromStatus EepromHeartbeatConfiguration(eEpromAccessType eepromOp);
eEepromStatus EepromSetWheelDia(eEpromAccessType eepromOp);
eEepromStatus EepromSetCycleFrequency(eEpromAccessType eepromOp);
eEepromStatus EepromSetLogInterval(eEpromAccessType eepromOp);
eEepromStatus EepromSetContinue(eEpromAccessType eepromOp);
eEepromStatus EepromSetReturnState(eEpromAccessType eepromOp);
eEepromStatus EepromSetcomdistance(eEpromAccessType eepromOp);
eEepromStatus EepromSetNoofRowday(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRowLength2(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRowLength3(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRowLength4(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRowLength5(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRowLength6(eEpromAccessType eepromOp);
eEepromStatus EepromStoreRowLength7(eEpromAccessType eepromOp);

eEepromStatus EepromSetrelaytime(eEpromAccessType eepromOp);


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */


