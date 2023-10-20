#include "flashlogoperation.h"

void AutoCycleFlash_main(void)
{
  if (CYCLE_OCCURED_FLASH == 1)
  {
    AutoCycleFlash_log_Data();
    CYCLE_OCCURED_FLASH = 0;
  }
}

void AutoCycleFlash_log_Data(void)
{

    char man_day;
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );
    int mode_now;
    uint16_t dataCount = 0; 
    external_rtc_read(&sys_time);
    snprintf((char *)&Log_Array[dataCount],20,"%02d-%02d-%02d %02d:%02d:%02d,",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
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
   
    strcat(Log_Array,"\r\n" );
       
    Flash_write_log(Log_Array,CYLCE_LOG);
}


void  ERRORlogFlash_main(void)
{  
   if((SDcardmountchecktimer >= 5000)&&(SDcardmountcheckError == false))
   {
       SDcardmountchecktimer = 0;
       SDcardmountcheckError = true;
       ERROR_OCCURRED_FLASH = 1;
   }
   
   if(ERROR_OCCURRED_FLASH == 1) //5 seconds
   {      
       ERRORFlash_log_Data();
       ERROR_OCCURRED_FLASH = 0;
   }  
}

void ERRORFlash_log_Data(void)
{
    int Card_reset;
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );
    uint16_t dataCount =0;
    int Currentdistance;
    
    external_rtc_read(&sys_time);
    dataCount = snprintf((char *)&Log_Array[dataCount],20,"%02d-%02d-%02d %02d:%02d:%02d",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
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

    if(Board_ON_FLASH == 1)
    {
         Board_ON_FLASH = 0;
         Card_reset = 1;
         SDerrorlog[0] = '0';
    }
    else 
    {
        Card_reset = 0;
    }
   
    snprintf(&Log_Array[strlen(Log_Array)],100,"%d,%d,%d,%d,%s,%d",modeValue,robotState,GetRotateSense1Count(),Currentdistance,SDerrorlog,Card_reset);
    strcat(Log_Array,"\r\n" );
    
    Flash_write_log(Log_Array,ERROR_LOG);
}


void FlashLogOperation(void)
{
    AutoCycleFlash_main();
    ERRORlogFlash_main();
}