#include "main.h"

DWORD RECordMemory_size = 10*1000;       //(Multiply by 10 - 1MB,Multiply by 100 - 10MB,Multiply by 1000 - 100MB)
FATFS FatFs;	/* FatFs work area needed for each volume */
FIL Fil;		/* File object needed for each open file */

BYTE SOC_logtime;
uint32_t SOC_SDlogTimer = 0;
uint32_t SOC_SDcurntcount = 0;

bool cleaner_logtime = false;

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


////////////////////////////////////////////SOC Log main Function START////////////////////////////////

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
    ParseHeaderDataSDcard(&txBuff[dataCount],&bufferLength,CMD_SOC_LOG); //301C
    dataCount = dataCount+bufferLength;
    bufferLength = sizeof(txBuff)-dataCount;
     
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );
    uint16_t dataCount1=0;
    
    external_rtc_read(&sys_time);
    dataCount1 = snprintf((char *)&Log_Array[dataCount1],20,"%02d-%02d-%02d %02d:%02d:%02d",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
    strcat(Log_Array,"," );
    
    mppt_bms_calculation();
    
    strcat(Log_Array,mppt_receieve_buffer);
//    dataCount =   strlen(Log_Array);
//    snprintf(&Log_Array[dataCount],200,"%s,%s,%s,%s,%s,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s",pv_vol,pv_curr,battery_volt,batt_charg_curr,Dtemp_str,Soc_Str,GetChargingOrNot(),bat_arr,pv_arr,mpptcurr_arr,mpptout_arr,inttemp_arr,exttemp_arr,batstate_arr,loadout_arr,loadtrip_arr);
    
    uint8_t tempCount = snprintf((char*)&txBuff[dataCount],bufferLength,"%s",Log_Array);  
    dataCount = dataCount+tempCount; 
    txBuff[dataCount] = STOP_CHAR;
    dataCount++;
    Serial_Debug("\r\ndataCount->");
    Serial_Debug_Num(dataCount); 
    Serial_Debug("\r\nData->");
    Serial_Debug_Bytes((char*)txBuff,dataCount);
    Serial_Debug("\r\nEnd");
    
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
   
   if(getInterval->I2 == 0)
   {
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
                    UART0_Write((void *)readBuf, strlen(readBuf));
                    while(UART0_WriteIsBusy());
                }

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

////////////////////////////////////////////SOC Log main Function END////////////////////////////////

////////////////////////////////////////////CLEANER LOG START////////////////////////////////

void cleaner_log_Data(void)
{
    uartflag = 3;
    uint16_t dataCount=0;
    uint8_t txBuff[200];
    memset(txBuff , '\0' , 200 );
    txBuff[dataCount]=0;
    uint16_t bufferLength = sizeof(txBuff)-dataCount;
    ParseHeaderDataSDcard(&txBuff[dataCount],&bufferLength,CMD_CLEANER_LOG); //300E
    dataCount = dataCount+bufferLength;
    bufferLength = sizeof(txBuff)-dataCount;
    
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );
    uint16_t dataCount1=0;
    external_rtc_read(&sys_time);
    snprintf((char *)&Log_Array[dataCount1],20,"%02d-%02d-%02d %02d:%02d:%02d",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
    strcat(Log_Array,"," );
    dataCount1 = strlen(Log_Array);
    snprintf(&Log_Array[dataCount1],100,"%d,%lu",trackchangercount,Cumulative_distance);
    
    
//    int responsedaycount = Daycount;
        
//    stnoofrows* setNoofrows = GetSetNoofrowsLimits();
//    stnoofrows tempsetNoofrows = *setNoofrows;
    
    
//    if(((tempsetNoofrows.Row[responsedaycount]-rowdaycount) == 0)||(trackchangercount == 0)) //
//    {
//        Cumulative_distance = 0;
//    }  //changes
    
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
        SD_Card_Append("cleanerlog.csv",(uint8_t *)Log_Array,strlen(Log_Array));
    }
  
}

void GETLog_Cleaner(void)
{
    char readBuf[250];
    char temp_var[5];
    sDateTime dataTime;
  
    uint32_t utcTime = GetRTCDateTimeSeconds();
    uint32_t hr1_utcTime = utcTime - 3600;

    if(File_open("cleanerlog.csv", SYS_FS_FILE_OPEN_READ))
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
                    UART0_Write((void *)readBuf, strlen(readBuf));
                    while(UART0_WriteIsBusy());
                }

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

void cleaner_log_main(void )
{
  if(cleaner_logtime == true)
  {
      cleaner_logtime = false;
      cleaner_log_Data();
  } 
}
////////////////////////////////////////////CLEANER LOG END////////////////////////////////


////////////////////////////////////////////ERROR LOG START//////////////////////////////
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
    ParseHeaderDataSDcard(&txBuff[dataCount],&bufferLength,CMD_ERROR_LOG); //3018
    dataCount = dataCount+bufferLength;
    bufferLength = sizeof(txBuff)-dataCount;
    
    int Card_reset;
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );
    uint16_t dataCount1 =0;
    int Currentdistance;
    
    external_rtc_read(&sys_time);
    dataCount1 = snprintf((char *)&Log_Array[dataCount1],20,"%02d-%02d-%02d %02d:%02d:%02d",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
    strcat(Log_Array,"," );

   Currentdistance = GetRotateSense1Count() * distance;
   eRobotDirection robotState = GetRobotActualDirection();
    
    eMode getMode =  GetOperationMode ();
    uint8_t modeValue = 0;
    switch(getMode)
    {
    case AUTO: 
      modeValue = 0;
      break;
    case MANUAL:
      modeValue = 1;
      break;
    default : 
      modeValue = 0;
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
   
    snprintf(&Log_Array[strlen(Log_Array)],100,"%d,%d,%d,%d,%s,%d",modeValue,robotState,GetRotateSense1Count(),Currentdistance,SDerrorlog,Card_reset);
    
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
                    UART0_Write((void *)readBuf, strlen(readBuf));
                    while(UART0_WriteIsBusy());
                }

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

////////////////////////////////////////////ERROR LOG END////////////////////////////////

////////////////////////////////////////////CYCLE LOG START/////////////////////////////

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
    ParseHeaderDataSDcard(&txBuff[dataCount],&bufferLength,CMD_AUTO_CYCLE_LOG); //300F
    dataCount = dataCount+bufferLength;
    bufferLength = sizeof(txBuff)-dataCount;
    
    char man_day;
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );
    int mode_now;
    uint16_t dataCount1 = 0; 
    external_rtc_read(&sys_time);
    snprintf((char *)&Log_Array[dataCount1],20,"%02d-%02d-%02d %02d:%02d:%02d,",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
    strcat(Log_Array,"," );
    
    eMode getMode =  GetOperationMode();
   
    if(getMode == AUTO)
    {
      mode_now =  0;
      man_day = Daycount;
    }
    else
    {
      mode_now = 1;
      man_day = 1;
    }

    stnoofrows* setNoofrows = GetSetNoofrowsLimits();
    stnoofrows tempsetNoofrows = *setNoofrows;
  
    snprintf(&Log_Array[strlen(Log_Array)],105,"%d,%d,%d,%d,%d,%s,%d,%d",mode_now,man_day,trackchangercount+1,GetRotateSense1Count(),GetRotateSense1Count()*Distancevalue,schedule_log,rowsActuallyCleaned,tempsetNoofrows.TRow);
   
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
         SD_Card_Append("cyclelog.csv",(uint8_t *)Log_Array,strlen(Log_Array));
    }   

}


void GETLog_AUTOCYCLE(void)
{
    char readBuf[250];
    char temp_var[5];
    sDateTime dataTime;
  
    uint32_t utcTime = GetRTCDateTimeSeconds();
    uint32_t hr1_utcTime = utcTime - 3600;

    if(File_open("cyclelog.csv", SYS_FS_FILE_OPEN_READ))
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
                    UART0_Write((void *)readBuf, strlen(readBuf));
                    while(UART0_WriteIsBusy());
                }

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
////////////////////////////////////////////CYCLE LOG END////////////////////////////////

void Delete_SDdata(void)
{
     SD_Card_Delete("cleanerlog.csv");
     SD_Card_Delete("cyclelog.csv");
     SD_Card_Delete("errorlog.csv");   
}

void SDcardOperation(void)
{
    if(SD_MOUNT_STATUS == 0)
    {
        SOClog_main();
        ERRORlog_main();
        AutoCycle_main();
        cleaner_log_main();
        
        for(int i=0;i<10;i++)
        {
            SYS_FS_Tasks();
            DRV_SDMMC_Tasks(sysObj.drvSDMMC0);

            SYSTICK_DelayMs(1);
        }
    }
    if(SD_MOUNT_STATUS == 1)
    {  
        static uint8_t temp_bufff[250];
             
        memset(temp_bufff, 0, sizeof(temp_bufff));
        SD_Card_Read("soclog.csv", (uint8_t*)temp_bufff, strlen(SOC_HEADING), 1);
        if(strncmp(SOC_HEADING, (char*)temp_bufff, strlen(SOC_HEADING)))
        {
            SD_Card_Write("soclog.csv",(uint8_t*)SOC_HEADING,strlen(SOC_HEADING));
        }
        
        
        memset(temp_bufff, 0, sizeof(temp_bufff));
        SD_Card_Read("cyclelog.csv", temp_bufff, strlen(CYCLELOG_HEADING), 1);
        if(strncmp(CYCLELOG_HEADING, (char*)temp_bufff, strlen(CYCLELOG_HEADING)))
        {
            SD_Card_Write("autocycle.csv",(uint8_t*)CYCLELOG_HEADING,strlen(CYCLELOG_HEADING));
        }
        
        memset(temp_bufff, 0, sizeof(temp_bufff));
        SD_Card_Read("errorlog.csv", temp_bufff, strlen(ERRORLOG_HEADING), 1);
        if(strncmp(ERRORLOG_HEADING, (char*)temp_bufff, strlen(ERRORLOG_HEADING)))
        {
            SD_Card_Write("errorlog.csv",(uint8_t*)ERRORLOG_HEADING,strlen(ERRORLOG_HEADING));
        }
        
        memset(temp_bufff, 0, sizeof(temp_bufff));
        SD_Card_Read("cleanerlog.csv", temp_bufff, strlen(CLEANERLOG_HEADING), 1);
        if(strncmp(CLEANERLOG_HEADING, (char*)temp_bufff, strlen(CLEANERLOG_HEADING)))
        {
            SD_Card_Write("cleanerlog.csv",(uint8_t*)CLEANERLOG_HEADING,strlen(CLEANERLOG_HEADING));
        }
        SD_MOUNT_STATUS = 2;
    }
    if(SD_MOUNT_STATUS == 2)
    {
        SYS_FS_Tasks();
        DRV_SDMMC_Tasks(sysObj.drvSDMMC0);
        //       Temperaturelog_main();
        SOClog_main();
        //       MOTORCRNTlog_main();
        ERRORlog_main();
        AutoCycle_main();
        cleaner_log_main();
    }
    if(SD_MOUNT_STATUS > 2)
    {
        SD_MOUNT_STATUS = 1;
    }
}
