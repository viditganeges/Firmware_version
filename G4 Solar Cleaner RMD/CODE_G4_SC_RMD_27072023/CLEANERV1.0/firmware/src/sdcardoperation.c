#include "main.h"

DWORD RECordMemory_size = 10*1000;       //(Multiply by 10 - 1MB,Multiply by 100 - 10MB,Multiply by 1000 - 100MB)
FATFS FatFs;	/* FatFs work area needed for each volume */
FIL Fil;		/* File object needed for each open file */

BYTE Temperature_logtime;
uint32_t Temperature_SDlogTimer = 0;
uint32_t Temperature_SDcurntcount = 0;

BYTE SOC_logtime;
uint32_t SOC_SDlogTimer = 0;
uint32_t SOC_SDcurntcount = 0;

BYTE MOTORCRNT_logtime;
uint32_t MOTORCRNT_SDlogTimer = 0;
uint32_t MOTORCRNT_SDcurntcount = 0;

static bool ParseHeaderDataSDcard(uint8_t* tempBuff, uint16_t* headerBufferLength, uint16_t command );

static bool ParseHeaderDataSDcard(uint8_t* tempBuff, uint16_t* headerBufferLength, uint16_t command )
{
  uint16_t buffCount = 0;
  tempBuff[0] = START_CHAR;
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  uint64_t macAddr = GetZigbeeMacAddress();                                    
  uint16_t maxStrLength = MAC_ID_STR_LEN;
  char macStr[MAC_ID_STR_LEN];
  Int64ToHexString (macAddr, macStr, &maxStrLength);
  uint16_t snData = snprintf((char *)&tempBuff[buffCount],*headerBufferLength,"%02X",DEVICE_ID);
  buffCount = buffCount+snData;
  *headerBufferLength = *headerBufferLength - buffCount;
  tempBuff[buffCount] = DELIMITER_1;
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  snData = snprintf((char *)&tempBuff[buffCount],*headerBufferLength,"%04X",command);
  buffCount = buffCount+snData;
  *headerBufferLength = *headerBufferLength - buffCount;
  tempBuff[buffCount] = DELIMITER_2;
  buffCount++;
  *headerBufferLength = *headerBufferLength - buffCount;
  *headerBufferLength = buffCount;
  Serial_Debug_Bytes((char*)tempBuff,buffCount);
  Serial_Debug((char*)tempBuff); 
  return true; 
}


////////////////////////////////////////////Temperature Log main Function Start////////////////////////////////
void Temperature_log_Data(void)
{
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );
    uint16_t dataCount=0;
    
    external_rtc_read(&sys_time);
    dataCount = snprintf((char *)&Log_Array[dataCount],20,"%02d-%02d-%04d %02d:%02d:%02d",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
    strcat(Log_Array,"," );
    char temp1[10],temp2[10],temp3[10];
    memset(temp1, 0, sizeof(temp1));
    memset(temp2, 0, sizeof(temp2));
    memset(temp3, 0, sizeof(temp3));
    ftoa(temp_read_A(),temp1,2);
    ftoa(temp_read_B(),temp2,2);
    ftoa(temp_read_C(),temp3,2);
  
    dataCount =   strlen(Log_Array);
    snprintf(&Log_Array[dataCount],24 ,"%s,%s,%s",temp1,temp2,temp3);    
    strcat(Log_Array,"\r\n" );
    if (SD_MOUNT_STATUS == 2 ) 
    {  
        SD_Card_Append("Temperature.csv",(uint8_t *)Log_Array,strlen(Log_Array));
    }
  
}
void Temperature_SDlogtimer(void )
{
  Temperature_SDlogTimer++;
}
uint32_t GetTemperature_SDlogTimer(void)
{
  return Temperature_SDlogTimer;
}
void Temperaturelog_main(void)
{
   Temperature_SDcurntcount = GetTemperature_SDlogTimer();
   stInterval* getInterval = GetSetIntervalLimits(); 
   if(getInterval->I1 == 0)
   {
     getInterval->I1 = 1.0; //1.0 - one minutes
   }
  if(Temperature_SDcurntcount >= getInterval->I1 * 1000)//1000- 1sec 59500
  {    
      Temperature_logtime = true;      
  }
  if(Temperature_logtime == true)
  {
      Temperature_logtime = false;
      Temperature_SDlogTimer =0;
      Temperature_log_Data();
  }
}

void GETLog_Temperature(void)
{
    char readBuf[250];
    char temp_var[5];
    sDateTime dataTime;
  
    uint32_t utcTime = GetRTCDateTimeSeconds();
    uint32_t hr1_utcTime = utcTime - 3600;
    
    memset(logReadArray, 0, sizeof(logReadArray));

    if(File_open("Temperature.csv", SYS_FS_FILE_OPEN_READ))
    {
        File_readline(readBuf);
        reading:
        memset(readBuf, 0, sizeof(readBuf));
        if(File_readline(readBuf))
        {
            if((readBuf[0]>=0x30) && (readBuf[0]<=0x39))
            {
                uint32_t data_time = 0;

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf, 2);
                dataTime.date = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+3, 2);
                dataTime.month = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+6, 4);
                dataTime.year = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+11, 2);
                dataTime.hour = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+14, 2);
                dataTime.minute = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+17, 2);
                dataTime.second = atoi(temp_var);

                data_time = ConvertRTCDatetimeToSeconds(&dataTime);

                if((data_time >= hr1_utcTime) && (data_time <= utcTime))
                {
                    if (uartflag == 1)
                    {    
                        UART0_Write((void *)readBuf, strlen(readBuf));
                        while(UART0_WriteIsBusy());
                    }
                }
                
//                WDT_Clear();

                if(data_time >= utcTime)
                {
                    goto exit;
                }
            }

            goto reading;
        }

        exit:
            File_close();
    }
}

//////////////////////////////////////////////Temperature Log main Function END////////////////////////////////
//////////////////////////////////////////////SOC Log main Function START////////////////////////////////

void SOC_SDlogtimer(void )
{
  SOC_SDlogTimer++;
}

uint32_t  GetSOC_SDlogTimer(void)
{
  return SOC_SDlogTimer;
}

void SOC_log_Data(void)
{
    uartflag = 3;
    uint16_t dataCount=0;
    uint8_t txBuff[200];
    memset(txBuff , '\0' , 200 );
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderDataSDcard(&txBuff[dataCount],&bufferLength,CMD_SOC_LOG); //201C
    dataCount = dataCount+bufferLength;
    bufferLength = sizeof(txBuff)-dataCount;   
    
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );
    uint16_t dataCount1=0;
    external_rtc_read(&sys_time);
    dataCount1 = snprintf((char *)&Log_Array[dataCount1],20,"%02d-%02d-%04d %02d:%02d:%02d",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
    strcat(Log_Array,"," );
    mppt_bms_calculation();
    strcat(Log_Array,mppt_receieve_buffer);
    
    uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%s",Log_Array);  
    dataCount = dataCount+tempCount; 
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
//    UartTransmit(txBuff,dataCount);
//    ClearTxBuff();
    
    stInterval* getInterval = GetSetIntervalLimits(); 
    if(getInterval->I6 == 1)
    {
        UartTransmit(txBuff,dataCount);
        ClearTxBuff();
    }
    
    
    strcat(Log_Array,"\r\n" );
    if (SD_MOUNT_STATUS == 2 ) 
    {  
      SD_Card_Append("soclog.csv",(uint8_t *)Log_Array,strlen(Log_Array));
    }
}

void SOClog_main(void )
{
   SOC_SDcurntcount = GetSOC_SDlogTimer();
   stInterval* getInterval = GetSetIntervalLimits(); 
   if(getInterval->I2 == 0){
     getInterval->I2 = 1.0; //1.0 - one minutes
   }
   
  if(SOC_SDcurntcount >= getInterval->I2 * 59500)//1000- 1sec
  {    
      SOC_logtime = true;      
  }
   
  if(SOC_logtime == true)
  {
      SOC_logtime = false;
      SOC_SDlogTimer =0;
      SOC_log_Data();
  }
}

void GETLog_SOC(void)
{
    char readBuf[250];
    char temp_var[5];
    sDateTime dataTime;
    uint32_t utcTime = GetRTCDateTimeSeconds();
    uint32_t hr1_utcTime = utcTime - 3600; 
    memset(logReadArray, 0, sizeof(logReadArray));
    if(File_open("soclog.csv", SYS_FS_FILE_OPEN_READ))
    {
        File_readline(readBuf);
        reading:
        memset(readBuf, 0, sizeof(readBuf));
        if(File_readline(readBuf))
        {
            if((readBuf[0]>=0x30) && (readBuf[0]<=0x39))
            {
                uint32_t data_time = 0;

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf, 2);
                dataTime.date = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+3, 2);
                dataTime.month = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+6, 4);
                dataTime.year = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+11, 2);
                dataTime.hour = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+14, 2);
                dataTime.minute = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+17, 2);
                dataTime.second = atoi(temp_var);

                data_time = ConvertRTCDatetimeToSeconds(&dataTime);

                if((data_time >= hr1_utcTime) && (data_time <= utcTime))
                {
                    if (uartflag == 1)
                    {    
                        UART0_Write((void *)readBuf, strlen(readBuf));
                        while(UART0_WriteIsBusy());
                    }
                }
                
//                WDT_Clear();

                if(data_time >= utcTime)
                {
                    goto exit;
                }
            }

            goto reading;
        }

        exit:
            File_close();
    }
}

//////////////////////////////////////////////SOC Log main Function END////////////////////////////////
//////////////////////////////////////////////MOTORCRNT Log main Function START////////////////////////////////

void MOTORCRNT_SDlogtimer(void )
{
  MOTORCRNT_SDlogTimer++;
}

uint32_t  GetMOTORCRNT_SDlogTimer(void)
{
  return MOTORCRNT_SDlogTimer;
}

void MOTORCRNT_log_Data(void)
{
    uartflag = 3;
    uint16_t dataCount=0;
    uint8_t txBuff[200];
    memset(txBuff , '\0' , 200 );
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderDataSDcard(&txBuff[dataCount],&bufferLength,CMD_MOTORCURRENT_LOG); //2011
    dataCount = dataCount+bufferLength;
    bufferLength = sizeof(txBuff)-dataCount;
    
    
    char Log_Array[Log_Array_count];
    int mode_now;
    memset(Log_Array , '\0' , Log_Array_count );
    int Currentmotordistance;
    uint16_t dataCount1 = 0;
    external_rtc_read(&sys_time);
    dataCount1 = snprintf((char *)&Log_Array[dataCount1],20,"%02d-%02d-%04d %02d:%02d:%02d",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
    strcat(Log_Array,"," ); 
    char Imot1[10],Imot2[10],Imot3[10];
    memset(Imot1, 0, sizeof(Imot1));
    memset(Imot2, 0, sizeof(Imot2));
    memset(Imot3, 0, sizeof(Imot3));
    ftoa(current_read_A(),Imot1,2);
    ftoa(current_read_B(),Imot2,2);
    ftoa(current_read_C(),Imot3,2);
    PulseCalculation();
    Currentmotordistance = GetRotateSense1Count() * distance;
    eRobotDirection robotState = GetRobotActualDirection();
    eMode getMode =  GetOperationMode ();
 
    if(getMode == AUTO)
        mode_now =  Get_Completed_Schedule();
    else
         mode_now = 0;
    
     
    snprintf(&Log_Array[strlen(Log_Array)],100,"%d,%d,%s,%s,%s,%d,%d",mode_now,robotState,Imot1,Imot2,Imot3,GetRotateSense1Count(),Currentmotordistance);
    
    uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%s",Log_Array);  
    dataCount = dataCount+tempCount; 
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
//    UartTransmit(txBuff,dataCount);
//    ClearTxBuff();
    stInterval* getInterval = GetSetIntervalLimits(); 
    if(getInterval->I6 == 1)
    {
        UartTransmit(txBuff,dataCount);
        ClearTxBuff();
    }
    
    
    strcat((char *)Log_Array,"\r\n" );
  
    if (SD_MOUNT_STATUS == 2 ) 
    {  
         SD_Card_Append("motorcurrent.csv",(uint8_t *)Log_Array,strlen(Log_Array));
    }  
}

void GETLog_MOTORCRNT(void)
{
    char readBuf[250];
    char temp_var[5];
    sDateTime dataTime;
  
    uint32_t utcTime = GetRTCDateTimeSeconds();
    uint32_t hr1_utcTime = utcTime - 3600;
    
    memset(logReadArray, 0, sizeof(logReadArray));

    if(File_open("motorcurrent.csv", SYS_FS_FILE_OPEN_READ))
    {
        File_readline(readBuf);
        reading:
        memset(readBuf, 0, sizeof(readBuf));
        if(File_readline(readBuf))
        {
            if((readBuf[0]>=0x30) && (readBuf[0]<=0x39))
            {
                uint32_t data_time = 0;

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf, 2);
                dataTime.date = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+3, 2);
                dataTime.month = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+6, 4);
                dataTime.year = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+11, 2);
                dataTime.hour = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+14, 2);
                dataTime.minute = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+17, 2);
                dataTime.second = atoi(temp_var);

                data_time = ConvertRTCDatetimeToSeconds(&dataTime);

                if((data_time > hr1_utcTime) && (data_time <= utcTime))
                {
                    if (uartflag == 1)
                    {    
                        UART0_Write((void *)readBuf, strlen(readBuf));
                        while(UART0_WriteIsBusy());
                    }
                }
                
//                WDT_Clear();

                if(data_time >= utcTime)
                {
                    goto exit;
                }
            }

            goto reading;
        }

        exit:
            File_close();
    }
}

void MOTORCRNTlog_main(void)
{
   MOTORCRNT_SDcurntcount = GetMOTORCRNT_SDlogTimer();
   stInterval* getInterval = GetSetIntervalLimits(); 
   
   if(getInterval->I3 == 0)
   {
     getInterval->I3 = 1.0; //1.0 - one minutes
   }

  if(MOTORCRNT_SDcurntcount >= getInterval->I3 * 1000)//1000- 1sec
  {    
      MOTORCRNT_logtime = true;      
  } 
  eRobotDirection robotState = GetRobotActualDirection();
  if((MOTORCRNT_logtime == true)&&(robotState == 1 || robotState == 0))
  {
      MOTORCRNT_logtime = false;
      MOTORCRNT_SDlogTimer =0;
      MOTORCRNT_log_Data();
  }
  
}

//////////////////////////////////////////////MOTORCRNT Log main Function END////////////////////////////////
//////////////////////////////////////////////Error Log main Function START//////////////////////////////

void  ERRORlog_main(void)
{  
   if(ERROR_OCCURRED == 1) 
   {
       ERROR_log_Data();
       ERROR_OCCURRED = 0;
   }  
}

void ERROR_log_Data(void)
{
    uartflag = 3;
    uint16_t dataCount=0;
    uint8_t txBuff[200];
    memset(txBuff , '\0' , 200 );
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderDataSDcard(&txBuff[dataCount],&bufferLength,CMD_ERROR_LOG); //2018
    dataCount = dataCount+bufferLength;
    bufferLength = sizeof(txBuff)-dataCount;
    
    
    int Card_reset;
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );
    uint16_t dataCount1 =0;
    int Currentdistance,mode_now;   
    external_rtc_read(&sys_time);
    dataCount1 = snprintf((char *)&Log_Array[dataCount1],20,"%02d-%02d-%04d %02d:%02d:%02d",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
    strcat(Log_Array,"," );
    PulseCalculation();
    Currentdistance = GetRotateSense1Count() * distance;
    eRobotDirection robotState = GetRobotActualDirection();   
    eMode getMode =  GetOperationMode ();
    uint8_t modeValue = 0;
    switch(getMode)
    {
    case AUTO: 
      modeValue = 0;
      mode_now =  Get_Completed_Schedule();
      break;
    case MANUAL:
      modeValue = 1;
      mode_now = 0;
      break;
    default : 
      modeValue = 0;
      mode_now =  Get_Completed_Schedule();
      break;
    }

    if(Board_ON_SD == 1)
    {
         Board_ON_SD = 0;
         Card_reset = 1;
         SDerrorlog[0] = '0';
    }
    else 
    {
        Card_reset = 0;
    }
    
    snprintf(&Log_Array[strlen(Log_Array)],100,"%d,%d,%d,%d,%d,%s,%d",modeValue,robotState,mode_now,GetRotateSense1Count(),Currentdistance,SDerrorlog,Card_reset);
    
    uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%s",Log_Array);  
    dataCount = dataCount+tempCount; 
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
//    UartTransmit(txBuff,dataCount);
//    ClearTxBuff();
    stInterval* getInterval = GetSetIntervalLimits(); 
    if(getInterval->I6 == 1)
    {
        UartTransmit(txBuff,dataCount);
        ClearTxBuff();
    }

    strcat(Log_Array,"\r\n" );
    
    if (SD_MOUNT_STATUS == 2 ) 
    {  
          SD_Card_Append("errorlog.csv",(uint8_t *)Log_Array,strlen(Log_Array));
    } 
}

void GETLog_ERROR(void)
{
    char readBuf[250];
    char temp_var[5];
    sDateTime dataTime; 
    uint32_t utcTime = GetRTCDateTimeSeconds();
    uint32_t hr1_utcTime = utcTime - 3600;    
    memset(logReadArray, 0, sizeof(logReadArray));
    if(File_open("errorlog.csv", SYS_FS_FILE_OPEN_READ))
    {
        File_readline(readBuf);
        reading:
        memset(readBuf, 0, sizeof(readBuf));
        if(File_readline(readBuf))
        {
            if((readBuf[0]>=0x30) && (readBuf[0]<=0x39))
            {
                uint32_t data_time = 0;

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf, 2);
                dataTime.date = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+3, 2);
                dataTime.month = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+6, 4);
                dataTime.year = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+11, 2);
                dataTime.hour = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+14, 2);
                dataTime.minute = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+17, 2);
                dataTime.second = atoi(temp_var);

                data_time = ConvertRTCDatetimeToSeconds(&dataTime);

                if((data_time >= hr1_utcTime) && (data_time <= utcTime))
                {
                    if (uartflag == 1)
                    {    
                        UART0_Write((void *)readBuf, strlen(readBuf));
                        while(UART0_WriteIsBusy());
                    }
                }
                
//                WDT_Clear();

                if(data_time >= utcTime)
                {
                    goto exit;
                }
            }

            goto reading;
        }

        exit:
            File_close();
    }
}

//////////////////////////////////////////////Error Log main Function END////////////////////////////////
//////////////////////////////////////////////Autocycle Log main Function END/////////////////////////////

void AutoCycle_main(void)
{
  if (CYCLE_OCCURRED == 1)
  {
    AutoCycle_log_Data();
    CYCLE_OCCURRED = 0;
  }
}

void AutoCycle_log_Data(void)
{
    uartflag = 3;
    uint16_t dataCount=0;
    uint8_t txBuff[200];
    memset(txBuff , '\0' , 200 );
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderDataSDcard(&txBuff[dataCount],&bufferLength,CMD_AUTO_CYCLE_LOG); //200F
    dataCount = dataCount+bufferLength;
    bufferLength = sizeof(txBuff)-dataCount;
    
    int Currentdistance; 
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );    
    uint16_t dataCount1 = 0;    
    external_rtc_read(&sys_time);
    dataCount1 = snprintf((char *)&Log_Array[dataCount1],20,"%02d-%02d-%04d %02d:%02d:%02d,",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
    strcat(Log_Array,"," );
    int mode_now;
    PulseCalculation();
    Currentdistance = GetRotateSense1Count() * distance;
    eMode getMode =  GetOperationMode ();
    if(getMode == AUTO)
        mode_now =  Get_Completed_Schedule();
    else
         mode_now = 0;

    if(schedule_BYTE == 0)
    {
     snprintf(&Log_Array[dataCount],105,"%d,%d,%d,%s,%d,%d,%d",mode_now,GetRotateSense1Count(),Currentdistance,schedule_log,track_no,day_count,Cumulative_distance);
    }
    else
    {
     memset(Log_Array , '\0' , Log_Array_count );
     dataCount =   strlen(Log_Array);
     snprintf(&Log_Array[dataCount],105,"%s, Total Distance: %d",schedule_log_21,Cumulative_distance);
     schedule_BYTE = 0;
    }  
    
    uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%s",Log_Array);  
    dataCount = dataCount+tempCount; 
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
//    UartTransmit(txBuff,dataCount);
//    ClearTxBuff();
    stInterval* getInterval = GetSetIntervalLimits(); 
    if(getInterval->I6 == 1)
    {
        UartTransmit(txBuff,dataCount);
        ClearTxBuff();
    } 
    
    strcat(Log_Array,"\r\n" );   
    if (SD_MOUNT_STATUS == 2 ) 
    {  
         SD_Card_Append("autocycle.csv",(uint8_t *)Log_Array,strlen(Log_Array));
    }  

}

void GETLog_AUTOCYCLE(void)
{ 
    char readBuf[250];
    char temp_var[5];
    sDateTime dataTime;
  
    uint32_t utcTime = GetRTCDateTimeSeconds();
    uint32_t hr1_utcTime = utcTime - 3600;
    
    memset(logReadArray, 0, sizeof(logReadArray));

    if(File_open("autocycle.csv", SYS_FS_FILE_OPEN_READ))
    {
        File_readline(readBuf);
        reading:
        memset(readBuf, 0, sizeof(readBuf));
        if(File_readline(readBuf))
        {
            if((readBuf[0]>=0x30) && (readBuf[0]<=0x39))
            {
                uint32_t data_time = 0;

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf, 2);
                dataTime.date = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+3, 2);
                dataTime.month = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+6, 4);
                dataTime.year = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+11, 2);
                dataTime.hour = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+14, 2);
                dataTime.minute = atoi(temp_var);

                memset(temp_var, 0, sizeof(temp_var));
                strncpy(temp_var, readBuf+17, 2);
                dataTime.second = atoi(temp_var);

                data_time = ConvertRTCDatetimeToSeconds(&dataTime);

                if((data_time >= hr1_utcTime) && (data_time <= utcTime))
                {
                    if (uartflag == 1)
                    {    
                        UART0_Write((void *)readBuf, strlen(readBuf));
                        while(UART0_WriteIsBusy());
                    }
                }
                
//                WDT_Clear();

                if(data_time >= utcTime)
                {
                    goto exit;
                }
            }

            goto reading;
        }

        exit:
            File_close();
    }
}
//////////////////////////////////////////////Autocycle Log main Function END////////////////////////////////

void Delete_SDdata(void)
{
     SD_Card_Delete("Temperature.csv");
     SD_Card_Delete("soclog.csv");
     SD_Card_Delete("motorcurrent.csv");
     SD_Card_Delete("autocycle.csv");
     SD_Card_Delete("errorlog.csv");
}

void SDcardOperation(void)
{
    
    if(SD_MOUNT_STATUS == 0)
    { 
        SOClog_main();
        MOTORCRNTlog_main();
        ERRORlog_main();
        AutoCycle_main();
        
        for(int i=0;i<25;i++)
        {
            SYS_FS_Tasks();
            DRV_SDMMC_Tasks(sysObj.drvSDMMC0);

            SYSTICK_DelayUs(500);
        }
    }
    if(SD_MOUNT_STATUS == 1)
    {        
        static uint8_t temp_bufff[250];       
        memset(temp_bufff, 0, sizeof(temp_bufff));
        SD_Card_Read("Temperature.csv", temp_bufff, strlen(TEMPERATURE_HEADING), 1);
        if(strncmp(TEMPERATURE_HEADING, (char*)temp_bufff, strlen(TEMPERATURE_HEADING)))
        {
            SD_Card_Write("Temperature.csv",(uint8_t*)TEMPERATURE_HEADING,strlen(TEMPERATURE_HEADING));
        }
        
        memset(temp_bufff, 0, sizeof(temp_bufff));
        SD_Card_Read("soclog.csv", (uint8_t*)temp_bufff, strlen(SOC_HEADING), 1);
        if(strncmp(SOC_HEADING, (char*)temp_bufff, strlen(SOC_HEADING)))
        {
            SD_Card_Write("soclog.csv",(uint8_t*)SOC_HEADING,strlen(SOC_HEADING));
        }
        
        memset(temp_bufff, 0, sizeof(temp_bufff));
        SD_Card_Read("motorcurrent.csv", temp_bufff, strlen(MOTORCURRENT_HEADING), 1);
        if(strncmp(MOTORCURRENT_HEADING, (char*)temp_bufff, strlen(MOTORCURRENT_HEADING)))
        {
            SD_Card_Write("motorcurrent.csv",(uint8_t*)MOTORCURRENT_HEADING,strlen(MOTORCURRENT_HEADING));
        }
        
        memset(temp_bufff, 0, sizeof(temp_bufff));
        SD_Card_Read("autocycle.csv", temp_bufff, strlen(AUTOCYCLE_HEADING), 1);
        if(strncmp(AUTOCYCLE_HEADING, (char*)temp_bufff, strlen(AUTOCYCLE_HEADING)))
        {
            SD_Card_Write("autocycle.csv",(uint8_t*)AUTOCYCLE_HEADING,strlen(AUTOCYCLE_HEADING));
        }
        
        memset(temp_bufff, 0, sizeof(temp_bufff));
        SD_Card_Read("errorlog.csv", temp_bufff, strlen(ERRORLOG_HEADING), 1);
        if(strncmp(ERRORLOG_HEADING, (char*)temp_bufff, strlen(ERRORLOG_HEADING)))
        {
            SD_Card_Write("errorlog.csv",(uint8_t*)ERRORLOG_HEADING,strlen(ERRORLOG_HEADING));
        }
        SD_MOUNT_STATUS = 2;
    }
    if(SD_MOUNT_STATUS == 2)
    {
        SYS_FS_Tasks();
        DRV_SDMMC_Tasks(sysObj.drvSDMMC0);
            
        Temperaturelog_main();
        SOClog_main();
        MOTORCRNTlog_main();
        ERRORlog_main();
        AutoCycle_main();       
    } 
    if(SD_MOUNT_STATUS > 2)
    {
        SD_MOUNT_STATUS = 1;
    }
}