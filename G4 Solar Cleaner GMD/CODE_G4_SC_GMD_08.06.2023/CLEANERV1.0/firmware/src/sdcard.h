#ifndef _SDCARDOPERATION_H
#define _SDCARDOPERATION_H

#include "main.h"

#define BUFF_COUNT 10000+1
#define Data_Read_Cnt  BUFF_COUNT   

#define Log_Array_count 120

#define SOC_HEADING             "DATE&TIME,PV VOLTAGE,PV CURRENT,PV POWER,BATTERY VOLTAGE,BATTERY TEMPERATURE,BATTERY CHARGING CURRENT,BATTERY DISCHARGING CURRENT,PV WORKING STATUS,BATTERY SOC,CHARGE CONTROLLER TEMPERATURE,GENERATED ENERGY,CONTROLLER STATUS\n"
#define TEMPERATURE_HEADING     "DATE&TIME,MOTOR1 TEMPERATURE,MOTOR2 TEMPERATURE,MOTOR3 TEMPERATURE\n"
#define MOTORCURRENT_HEADING    "DATE&TIME,CYCLE NUMBER,DIRECTION,MOTOR1 CURRENT,MOTOR2 CURRENT,MOTOR3 CURRENT,PULSE COUNT,DISTANCE\n"
#define AUTOCYCLE_HEADING       "DATE&TIME,CYCLE NUMBER,PULSE COUNT,DISTANCE,DIRECTION,START TIME,START SOC,END TIME,END SOC,RESTART COUNT,TRACK NUMBER,DAY COUNT,CUMULATIVE DISTANCE\n"
#define ERRORLOG_HEADING        "DATE&TIME,CURRENT MODE,DIRECTION,CYCLE NUMBER,PULSE COUNT,DISTANCE(MM),ERROR CODE,POWER-ON STATUS\n"

//char Board_ON;

char Board_ON_SD;
char Board_ON_FLASH;

char  data_read_buffer[Data_Read_Cnt];
char  data_read_buffer2[Data_Read_Cnt];
  
void SDcardOperation(void);
BYTE SDcardMount(void);

void SD_Card_Write(const char *filename,uint8_t *Data_Buffer, size_t size); 
void SD_Card_Read(const char *filename,uint8_t *Data_Buffer, size_t size, int offset);
void SD_Card_Append( const char *filename,uint8_t  *Data_Buffer, size_t size);
void SD_Card_Delete( const char *filename);
void SD_Card_Create( const char *filename);

void Delete_SDdata(void);

bool File_open(const char *filename, SYS_FS_FILE_OPEN_ATTRIBUTES mode);
bool File_readline(char *data);
void File_close(void);

int SD_MOUNT_STATUS;

///////////////TEMPERATURE LOG FUNCTIONS//////////
void Temperature_log_Data(void);
void Temperature_SDlogtimer(void );
uint32_t GetTemperature_SDlogTimer(void);
void GETLog_Temperature(void);
void Temperaturelog_main(void);

///////////////SOC LOG FUNCTIONS//////////
void SOC_log_Data(void);
void SOC_SDlogtimer(void );
uint32_t GetSOC_SDlogTimer(void);
void GETLog_SOC(void);
void SOClog_main(void);

///////////////MOTOR LOG FUNCTIONS//////////
void MOTORCRNT_log_Data(void);
void MOTORCRNT_SDlogtimer(void );
uint32_t GetMOTORCRNT_SDlogTimer(void);
void GETLog_MOTORCRNT(void);
void MOTORCRNTlog_main(void);

///////////////ERROR LOG FUNCTIONS//////////
void ERROR_log_Data(void);
void GETLog_ERROR(void);
void ERRORlog_main(void);

/////////////// AutoCycle LOG FUNCTIONS///////////
void AutoCycle_main(void);
void AutoCycle_log_Data(void);
void GETLog_AUTOCYCLE(void);


#endif