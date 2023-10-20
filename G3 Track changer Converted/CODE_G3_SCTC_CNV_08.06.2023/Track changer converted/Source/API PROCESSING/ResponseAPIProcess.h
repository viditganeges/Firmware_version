#ifndef _RESPONSE_API_PROCESS_H_
#define _RESPONSE_API_PROCESS_H_

#include "APIProcessing.h"
#include <stdint.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <stdio.h>

#define JSON_NO_OF_STATUS_RESPONSE      8       
#define SET_AND_START_CYCLE             0x001D
#define GET_CLEANER_STATUS              0x1021
#define STOP_CLEANER                    0x0003
#define CYCLE_RESUME                    0x0003
#define GET_CLEANER_MODE                0x1001
#define HOME_RETURN                     0x0033

typedef eJsonStatus (*RxResponseProcess_Ptr)(uint64_t macAddress, const char * jsonString);

typedef enum eResponseCommand_def 
{
  CMD_NUM_ACK_RES,
  CMD_NUM_STATUS_RES,
  CMD_NUM_INVALID_RES
}eResponseCommand;

typedef struct stResponseCommand_def {
  eResponseCommand command;
  const uint16_t command_Hex;
  RxResponseProcess_Ptr executeCommand;
}stResponseCommand;

bool IsAckReceived(void);
void ClearIsAckReceived(void);
void SetIsAckReceived(void);
bool IsCleanerDone(void);
void ClearIsCleanerDone(void);
uint32_t IsErrorFromCleaner(void);
void ClearErrorFromCleaner(void);
void SetIsCleanerDone(void);

static eJsonStatus ProcessAcknowledgement (uint64_t macAddress,const char * jsonString);
static eJsonStatus ProcessStatusResponse (uint64_t macAddress,const char * jsonString);

ErrorStatus SendCommandToCleaner(uint64_t macAddress,uint16_t command,char* values);

eJsonStatus ProcessResponse (uint64_t macAddress, uint8_t * jsonReceivedData, uint16_t size);
static void Error_Handler(void);
#endif