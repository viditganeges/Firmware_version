#include "main.h"

#define DEFAULT_WILD_CARD_DEV_ID          "00000"
#define MAX_JASON_VALUE_LENGTH             200U
#define MAX_NO_OF_RESULT_STRINGS           50U
#define DEFAULT_FLOAT_PRECISION            2U
#define MAX_NUMBER_STR_LENGTH              15U                                        /* This is a maximum length for all int32_t and float number strings  */
#define LOG_STR_LEN                        35U                                         /* [xx,xx] */
#define MAX_KEY_VALUES_ONE_TIME            100U  
#define NO_OF_REQUEST_INPUT_VALUES_DATA    7U
#define JSON_NO_OF_TEST_JIG_VALUES         4U                   
#define MAX_NO_OF_ALERTS                   15U
//#define DEFAULT_RX_MAC_ADDR                  0xFFFFFFFFFFFFFFFF 

typedef struct
{
  char *keyStr;
  char *startStr;
  char *endStr;
  char *valueStr[MAX_KEY_VALUES_ONE_TIME];
  uint16_t noOfValues;
}stKeyValueData;

static char txBuff[MAX_UART_TX_BUFFER_LENGTH];

const char KEY_TYPE_TIME_STAMP[]          =       "TS";
const char KEY_TYPE_ID_NO[]               =       "DID";
const char KEY_TYPE_CMD[]                 =       "CMD";
const char KEY_TYPE_ERROR[]               =       "STATUS";
const char KEY_TYPE_MODE[]                =       "MODE";
const char KEY_TYPE_PWM[]                 =       "PWM";
const char KEY_TYPE_VALUES[]              =       "VALUES";
const char KEY_TYPE_DEVICE_ID[]           =       "DEVID";
const char KEY_TYPE_HARDWARE_VER[]        =       "HVER";
const char KEY_TYPE_FIRMWARE_VER[]        =       "FVER";
const char KEY_TYPE_SERIAL_NO[]           =       "SNO";

static void Error_Handler(void);
static eJsonStatus ConvertJsonAndProcess(const char * jsonString,const char* command);
static eJsonStatus ParseRxdata_config(const char * Json_String, JsonFormatKeys * JsonKeys, JsonFormatValues * JsonValues);
static bool ParseHeaderDataHex(char* tempBuff, uint16_t* headerBufferLength, const char* command, uint64_t MACaddr );
static bool ParseKeyAndData(char * dataBuffer, uint16_t * dataBufferLength, char* key, char* data, char endChar);


static eJsonStatus ConvertJsonAndProcess(const char * jsonString,const char* command)
{
  //Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
  char value[MAX_JASON_VALUE_LENGTH+1];
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
  uint8_t tempCount = 0;
  ParseHeaderDataHex(&txBuff[dataCount],&bufferLength,command,GetZigbeeMacAddress());
//    ParseHeaderDataHex(&txBuff[dataCount],&bufferLength,command,macadd);
    dataCount = dataCount+bufferLength;   
    bufferLength = sizeof(txBuff)-dataCount;
    if(Parse_Json(jsonString, KEY_TYPE_VALUES, value,(sizeof(value)/sizeof(value[0]))-1) == 0)
    {
      if(strlen(value)!=0)
      {
//        Serial_Debug("\nValues");
//        Serial_Debug(value);
        tempCount = snprintf(&txBuff[dataCount],bufferLength,"%s,%d",value,2);
      }
      else
      {
        error_code = JSON_INVALID_DATA;
        ClearTxBuff();
        return error_code;
      }
    }
    else
    {
      tempCount = snprintf(&txBuff[dataCount],bufferLength,"%d",2);
    }
    dataCount = dataCount+tempCount; 
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
//    Serial_Debug("\r\ndataCount->");
//    Serial_Debug_Num(dataCount); 
//    Serial_Debug("\r\nData->");
//    Serial_Debug_Bytes(txBuff,dataCount);
//    Serial_Debug("\r\nEnd");
    ProcessReceivedJsonData((uint8_t * )txBuff,strlen(txBuff));
    return error_code;
}
eJsonStatus ConvertHexAndProcess(char * jsonString,uint16_t* size)
{
  //Serial_Debug(jsonString);
  eJsonStatus error_code = JSON_NO_ERROR;
  uint16_t dataCount=0;
  txBuff[dataCount]=0;
//  char value[MAX_JASON_VALUE_LENGTH+1];
  uint16_t bufferLength = sizeof(txBuff)-dataCount;
//  uint8_t tempCount = 0;
  stMessage message;
  ExtractData((uint8_t*)jsonString,*size,&message);
//  bool status = 
  ParseHeaderDataJson(&txBuff[dataCount], &bufferLength, message.CMD,GetZigbeeMacAddress());
//  ParseHeaderDataJson(&txBuff[dataCount], &bufferLength, message.CMD,macadd );
  dataCount = dataCount+bufferLength;  
  bufferLength = sizeof(txBuff)-dataCount;
  int temp_count = snprintf(&txBuff[dataCount],bufferLength,",");
  dataCount = dataCount+temp_count;  
  char endChar='}';
  bufferLength = sizeof(txBuff)-dataCount;
  //RemoveCRC((char*)jsonString);
  ParseKeyAndData(&txBuff[dataCount], &bufferLength, (char*) KEY_TYPE_VALUES, (char*)message.values, endChar );
  dataCount = dataCount+bufferLength;
  *size = dataCount;
  //Serial_Debug(txBuff);
  strncpy(jsonString,txBuff,strlen(txBuff));
  //Serial_Debug(jsonString);
  return error_code;
}
eJsonStatus ProcessReceivedJsonData_config(char * jsonReceivedData)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  const char * Json_String;
  JsonFormatKeys JsonKeys;
  JsonFormatValues JsonValues;
  char JsonValues_Multi[MAX_JSON_VALUES_ONE_TIME][MAX_JSON_VALUES_ONE_TIME_LENGTH+1];
  JsonValues.Value = &JsonValues_Multi;
  JsonValues.Max_Value_Length = MAX_JSON_VALUES_ONE_TIME_LENGTH;
  Json_String = (const char *)jsonReceivedData;
  error_code = ParseRxdata_config(Json_String, &JsonKeys, &JsonValues);
  //eJsonStatus parseErrorCode = error_code;
  if((error_code == JSON_NO_ERROR) || (error_code == JSON_WILDCARD_DEV_ID))
  {
//    Serial_Debug((char const *)"\nCMD Value: ");
//    Serial_Debug((char const *)(*JsonValues.Value)[SNO_KEY_TYPE_CMD]); 
    //error_code = ProcessJsonCommand(Json_String, (*JsonValues.Value)[SNO_KEY_TYPE_CMD], strtoull((*JsonValues.Value)[SNO_KEY_TYPE_ID_NO],NULL,16));
    error_code = ConvertJsonAndProcess(Json_String,(*JsonValues.Value)[SNO_KEY_TYPE_CMD]);
    if(error_code != JSON_NO_ERROR)
    {
      Error_Handler();
    }
  }
  else if(error_code == JSON_PARSE_ERROR)
  {
     error_code = ConvertJsonAndProcess(Json_String,(*JsonValues.Value)[SNO_KEY_TYPE_CMD]); 
  }
  else
  {
    Error_Handler();
  }
  return error_code;
}

static eJsonStatus ParseRxdata_config(const char * Json_String, JsonFormatKeys * JsonKeys, JsonFormatValues * JsonValues)
{
  eJsonStatus error_code = JSON_NO_ERROR;
  //JsonKeys->Key[SNO_KEY_TYPE_TIME_STAMP] = (char*)KEY_TYPE_TIME_STAMP;              
  JsonKeys->Key[SNO_KEY_TYPE_ID_NO]      = (char*)KEY_TYPE_ID_NO;                  
  JsonKeys->Key[SNO_KEY_TYPE_CMD]        = (char*)KEY_TYPE_CMD;   
  JsonKeys->No_of_Keys = SNO_KEY_TYPE_CMD+1;                                  // Assuming this as the MAXimum enum value used in this assignment
  JsonValues->No_of_Values = JsonKeys->No_of_Keys; 
  if(Parse_Json_Multi(Json_String, JsonKeys, JsonValues) == 0)
  {
  }
  else
  {
    error_code = JSON_PARSE_ERROR;
  }  
  return error_code; 
}

static bool ParseHeaderDataHex(char* tempBuff, uint16_t* headerBufferLength, const char* command, uint64_t MACaddr )
{
  union {
    uint16_t MACdata;
    uint16_t StoreMAC[2];
  }unStoreMAC;
  
  unStoreMAC.MACdata = MACaddr;
  uint16_t buffCount = 0; 
  tempBuff[0] = START_CHAR;  
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  //uint16_t deviceID = GetDeviceID();
  uint16_t snData = snprintf(&tempBuff[buffCount],*headerBufferLength,"%02X%X",unStoreMAC.StoreMAC[1],unStoreMAC.StoreMAC[0]);
  buffCount = buffCount+snData;
  *headerBufferLength = *headerBufferLength - buffCount;
  //printf("Data->%s",tempBuff);
  tempBuff[buffCount] = DELIMITER_1;
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  snData = snprintf(&tempBuff[buffCount],*headerBufferLength,"%s",command);
  buffCount = buffCount+snData;
  *headerBufferLength = *headerBufferLength - buffCount;
  tempBuff[buffCount] = DELIMITER_2;
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  *headerBufferLength = buffCount;
  // Serial_Debug_Bytes(tempBuff,buffCount);
  //Serial_Debug(tempBuff);
  //      if(*headerBufferLength < MAX_UART_TX_BUFFER_LENGTH)
  //  {
  //    return false;
  //  }
  //  else
  //  {
  return true;
  //  }
  
}
static void Error_Handler(void)
{
  /* Do Nothing */
}
bool ParseHeaderDataJson(char * headerBuffer, uint16_t * headerBufferLength, uint16_t command,uint64_t macAddr)
{
  
  union {
    uint64_t MACdata;
    uint16_t StoreMAC[4];
  }unStoreMAC;
  unStoreMAC.MACdata = macAddr;
  headerBuffer[0] = '\0';

    uint32_t dataLength = snprintf(headerBuffer, *headerBufferLength, "{\"%s\":\"%X%X%X%X\",\"%s\":\"%04X\"", 
                                 KEY_TYPE_ID_NO,unStoreMAC.StoreMAC[3],unStoreMAC.StoreMAC[2],unStoreMAC.StoreMAC[1],unStoreMAC.StoreMAC[0], KEY_TYPE_CMD, command);
  
  *headerBufferLength = strlen(headerBuffer);
  if(*headerBufferLength < dataLength)
  {
    return false;
  }
  else
  {
    return true;
  }
}
static bool ParseKeyAndData(char * dataBuffer, uint16_t * dataBufferLength, char* key, char* data, char endChar)
{
  uint32_t dataLength = 0;
  uint32_t bufferLength = *dataBufferLength;
  dataLength += snprintf(dataBuffer, bufferLength, "\"%s\":\"%s\"%c", key, data, endChar);
  *dataBufferLength = strlen(dataBuffer);
  if(*dataBufferLength < dataLength)
  {
    return false;
  }
  else
  {
    return true;
  }
}



