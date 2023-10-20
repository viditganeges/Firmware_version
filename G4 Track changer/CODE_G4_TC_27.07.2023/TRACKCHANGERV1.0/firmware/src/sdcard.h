#ifndef _SDCARDOPERATION_H
#define _SDCARDOPERATION_H

#include "main.h"

#define BUFF_COUNT 10000+1
#define Data_Read_Cnt  BUFF_COUNT  

DWORD RECordMemory_size;// = 10*1000;   

char data_read_buffer[Data_Read_Cnt];
char data_read_buffer2[Data_Read_Cnt];
  
void SDcardOperation(void);
void SD_Card_Write(const char *filename,uint8_t *Data_Buffer, size_t size); 
void SD_Card_Read(const char *filename,uint8_t *Data_Buffer, size_t size, int offset);
void SD_Card_Append( const char *filename,uint8_t  *Data_Buffer, size_t size);
void SD_Card_Delete( const char *filename);
void SD_Card_Create( const char *filename);
void Delete_SDdata(void);

bool File_open(const char *filename, SYS_FS_FILE_OPEN_ATTRIBUTES mode);
bool File_readline(char *data);
void File_close(void);


char Board_ON_SD;
char Board_ON_FLASH;

#define Log_Array_count 120

#define SOC_HEADING             "DATE&TIME,PV VOLTAGE,PV CURRENT,PV POWER,BATTERY VOLTAGE,BATTERY TEMPERATURE,BATTERY CHARGING CURRENT,BATTERY DISCHARGING CURRENT,PV WORKING STATUS,BATTERY SOC,CHARGE CONTROLLER TEMPERATURE,GENERATED ENERGY,CONTROLLER STATUS\n"
#define CYCLELOG_HEADING       "DATE&TIME,CURRENTMODE,DAYCOUNT,ROWCOUNT,PULSECOUNT,DISTANCE,DIRECTION,STARTTIME,STARTSOC,ENDTIME,ENDSOC,ERRORCOUNT\n"
#define ERRORLOG_HEADING        "DATE&TIME,CURRENTMODE,DIRECTION,PULSECOUNT,DISTANCE(MM),ERRORCODE,ERRORCOUNT\n"
#define CLEANERLOG_HEADING        "DATE&TIME,ROWCOUNT,CLEANERTOTALDISTANCE\n"

int SD_MOUNT_STATUS;
bool cleaner_logtime;

///////////////SOC LOG FUNCTIONS//////////
void SOC_log_Data(void);
void SOC_SDlogtimer(void );
uint32_t GetSOC_SDlogTimer(void);
void GETLog_SOC(void);
void SOClog_main(void);


/////////////CLEANER LOG FUNCTIONS//////////

void cleaner_log_Data(void);
void GETLog_Cleaner(void);
void cleaner_log_main(void);

/////////////ERROR LOG FUNCTIONS//////////

void ERROR_log_Data(void);
void GETLog_ERROR(void);
void ERRORlog_main(void);

///////////// AutoCycle LOG FUNCTIONS///////////

void AutoCycle_main(void);
void AutoCycle_log_Data(void);
void GETLog_AUTOCYCLE(void);

#endif
