#include "main.h"

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
    int Currentdistance;
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );  
    uint16_t dataCount = 0;       
    external_rtc_read(&sys_time);
    dataCount = snprintf((char *)&Log_Array[dataCount],20,"%02d-%02d-%04d %02d:%02d:%02d,",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
    strcat(Log_Array,"," );
    int mode_now;
    PulseCalculation();
    Currentdistance = GetRotateSense1Count() * distance;
    eMode getMode =  GetOperationMode ();  
    if(getMode == AUTO)
        mode_now =  Get_Completed_Schedule();
    else
         mode_now = 0;
    
    if(schedule_BYTE_flashlog == 0)
    {
     snprintf(&Log_Array[dataCount],105,"%d,%d,%d,%s,%d,%d,%d",mode_now,GetRotateSense1Count(),Currentdistance,schedule_log,track_no,day_count,Cumulative_distance);
    }
    else
    {
     memset(Log_Array , '\0' , Log_Array_count );
     dataCount =   strlen(Log_Array);
     snprintf(&Log_Array[dataCount],105,"%s, Total Distance: %d",schedule_log_21,Cumulative_distance);
     schedule_BYTE_flashlog = 0;
    }  
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
   
   if(ERROR_OCCURRED_FLASH == 1) //10 seconds
   {      
       ERRORFlash_log_Data();
       ERROR_OCCURRED_FLASH = 0;
   }  
}

void ERRORFlash_log_Data(void)
{
    int Card_reset;
    int SD_mount_error;
    char Log_Array[Log_Array_count];
    memset(Log_Array , '\0' , Log_Array_count );
    uint16_t dataCount =0;
    int Currentdistance,mode_now;
    external_rtc_read(&sys_time);
    dataCount = snprintf((char *)&Log_Array[dataCount],20,"%02d-%02d-%04d %02d:%02d:%02d",sys_time.tm_mday,sys_time.tm_mon,sys_time.tm_year,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);  
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
    
    if(SDcardmountcheckError == 0)
    {
        SD_mount_error = 0;
    }
    else
    {
        SD_mount_error = 1;
    }
    
    snprintf(&Log_Array[strlen(Log_Array)],100,"%d,%d,%d,%d,%d,%s,%d,%d",modeValue,robotState,mode_now,GetRotateSense1Count(),Currentdistance,SDerrorlog,Card_reset,SD_mount_error);
    strcat(Log_Array,"\r\n" );
    Flash_write_log(Log_Array,ERROR_LOG);
}

void FlashLogOperation(void)
{
    AutoCycleFlash_main();
    ERRORlogFlash_main();
}