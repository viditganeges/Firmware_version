#include "main.h"

#define CMD_ACK_RESPONSE                0x1111
#define CMD_STATUS_RESPONSE             0x1021

bool isAckReceived = false;
bool isCleanerDone = false;


uint32_t cleanerErrorCode = NO_FAULT;
uint32_t Cumulative_distance_cont = 0;          //CHANGE
uint16_t cleaner_in_comm = 0;

static const stResponseCommand * presentResponseCommandParams = NULL; 

static eJsonStatus ProcessResponseCommand (stMessage message) ;
static eJsonStatus AssignResponseCommand (uint16_t presentCommand_Hex) ;

static eJsonStatus ProcessAcknowledgement (uint64_t macAddress,const char * jsonString);
static eJsonStatus ProcessStatusResponse (uint64_t macAddress,const char * jsonString);
static void Error_Handler(void);


stResponseCommand responseCommandParams[] = 
{
  {CMD_NUM_ACK_RES,     CMD_ACK_RESPONSE,       ProcessAcknowledgement},
  {CMD_NUM_STATUS_RES,  CMD_STATUS_RESPONSE,    ProcessStatusResponse},
};

bool IsAckReceived(void)
{
  return isAckReceived;
}

void ClearIsAckReceived(void)
{
  isAckReceived = false;
}

void SetIsAckReceived(void)
{
  isAckReceived = true;
}

bool IsCleanerDone(void)
{
  return isCleanerDone;
}

void ClearIsCleanerDone(void)
{
  isCleanerDone = false;
}

void SetIsCleanerDone(void)
{
  isCleanerDone = true;
}

void SetErrorFromCleaner(uint32_t errorCode)
{
  cleanerErrorCode = errorCode;
}
uint32_t IsErrorFromCleaner(void)
{
  return cleanerErrorCode;
}

void ClearErrorFromCleaner(void)
{
  cleanerErrorCode = NO_FAULT;
}
static eJsonStatus ProcessAcknowledgement (uint64_t macAddress, const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\nIn ProcessAcknowledgement : ");
  Serial_Debug("\r\n Data Received -->");Serial_Debug(jsonString);
  SetIsAckReceived();
  error_code = atoi(jsonString);     //(eJsonStatus)atoi(jsonString);
  return error_code;
}

uint8_t cleanerStatus;

static eJsonStatus ProcessStatusResponse (uint64_t macAddress, const char * jsonString)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  Serial_Debug("\r\n In ProcessStatusResponse");  
  Serial_Debug("\r\n Data Received -->");
  Serial_Debug(jsonString);
//  int responsedaycount;
  char* statusResponseStr[JSON_NO_OF_STATUS_RESPONSE];
  uint8_t maxNoStrings=(sizeof statusResponseStr)/4;
  uint8_t totalResultStringCount =  SplitString ((char*)jsonString, statusResponseStr, ',',maxNoStrings);
  Serial_Debug("\nStatus Response Received Values :");
  for(uint8_t dispcount = 0; dispcount < totalResultStringCount; dispcount++)
  {
    Serial_Debug(statusResponseStr[dispcount]);
    Serial_Debug(", ");
  }
  if((totalResultStringCount == JSON_NO_OF_STATUS_RESPONSE))
  {
    cleanerStatus = atoi(statusResponseStr[2]);
    uint32_t errorCode = atol(statusResponseStr[5]);
//    Cumulative_distance = atoi(statusResponseStr[6]); 
    Cumulative_distance_cont = atoi(statusResponseStr[6]); 
    cleaner_in_comm = atoi(statusResponseStr[7]);
     
//    responsedaycount = Daycount;
    
//    int responsedaycount = Daycount;
//        
//    stnoofrows* setNoofrows = GetSetNoofrowsLimits();
//    stnoofrows tempsetNoofrows = *setNoofrows;
    
    if(cleanerStatus == CYCLE_COMPLETED)         /*ROBOT_COMPLETED*/ //6
    {
      Cumulative_distance = Cumulative_distance_cont+Cumulative_distance;
      SetIsCleanerDone();  
    }   
//    if(((tempsetNoofrows.Row[responsedaycount-1]-rowdaycount) == 0)||(trackchangercount == 0)) 
//    {
//        Cumulative_distance = 0;
//    }  //changes
    
    if(errorCode!= NO_FAULT)
    {
      SetErrorFromCleaner(errorCode);
    }
  }
  return error_code;
}

static eJsonStatus ProcessResponseCommand (stMessage message) 
{
  eJsonStatus jsonStatus;
  jsonStatus = AssignResponseCommand (message.CMD); 
  if(presentResponseCommandParams != NULL)
  {
    jsonStatus = presentResponseCommandParams->executeCommand(message.deviceID,(char*)message.values);
  }
  
  return jsonStatus;
}

static eJsonStatus AssignResponseCommand (uint16_t presentCommand_Hex) 
{
  uint16_t commandParamsCount = 0;
  uint16_t commandParamsTotalCount = sizeof(responseCommandParams)/sizeof(responseCommandParams[0]);
  for(commandParamsCount =0; commandParamsCount < commandParamsTotalCount; commandParamsCount++)
  {
    if(responseCommandParams[commandParamsCount].command_Hex == presentCommand_Hex)
    {
      presentResponseCommandParams = &responseCommandParams[commandParamsCount];
      break;
    }
  }
  if(commandParamsCount >= commandParamsTotalCount)
  {
    presentResponseCommandParams = NULL;
    return JSON_INVALID_CMD_VALUE;
  }
  else
  {
    return JSON_NO_ERROR;
  }
}


eJsonStatus ProcessResponse (uint64_t macAddress, uint8_t * jsonReceivedData, uint16_t size)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  stMessage message;
  error_code = ExtractDatafromzigbee(jsonReceivedData,size,&message);
  if((error_code == JSON_NO_ERROR) || (error_code == JSON_WILDCARD_DEV_ID))
  {
    Serial_Debug((char const *)"\nCMD Value: ");
    Serial_Debug_Num(message.CMD); 
    message.deviceID = macAddress;
    error_code = ProcessResponseCommand(message);//(*JsonValues.Value)[SNO_KEY_TYPE_CMD]);
    
    if(error_code != JSON_NO_ERROR)
    {
      Error_Handler();
    }
  }
  else
  {
    Error_Handler();
  }
  return error_code;
}

static void Error_Handler(void)
{
  /* Do Nothing */
}
