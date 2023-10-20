/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _MAIN_H    /* Guard against multiple inclusion */
#define _MAIN_H

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "math.h"
#include "driver/sdspi/drv_sdspi.h"
#include "sdcard.h"
#include "flashsst26.h"
#include "numtostr.h"
#include "serialdebug.h"
#include "flash.h"
#include "valuesvalidate.h"
#include "apiprocess.h"
#include "defaultvalues.h"
#include "robotoperation.h"
#include "autoschedule.h"
#include "commcommon.h"
#include "configapiProcess.h"
#include "motorcontrol.h"
#include "temperature.h"
#include "rotatesense.h"
#include "edgesense.h"
#include "timer.h"
#include "led.h"
#include "cyclemode.h"
#include "robotcontrol.h"
#include "JsonParser/fun_json.h"
#include "JsonParser/json.h"
#include "bms.h"
#include "rtc.h"
#include "overcurrentfault.h"
#include "processuart.h"

#include "steppermotorcontrol.h"
#include "proximity.h"
#include "rfid.h"
#include "zigbee.h"
#include "relay.h"

#include "flash_log.h"
#include "flashlogoperation.h"

#include "externalrtc.h"

#define Null '\0'
#define WRITE_OPERATION 0
#define READ_OPERATION  1

#define ON 1
#define OFF 2

#define TRUE 1
#define FALSE 0

uint8_t TEST_Array[200];
//void SD_Card_Task(uint8_t Mode);
//void SD_Card_APP_Initialize ( void ); 

 
char sd_buffer[100];
size_t sd_size;
const char sd_Filename[100];
void String_Extraction(char *ptr);
BYTE OPERATION;

/*motor drive declarations*/
//void Motor_Drive_Task(int Motor,int Status);
//void PWM0_CounterEventHandler(uint32_t status, uintptr_t context);
//#define DUTY_INCREMENT (10U)  /* Duty cycle increment value */
//uint16_t period;       /* Save PWM period */
// #define MOTOR_A 1
// #define MOTOR_B 2
// #define MOTOR_C 3


#define uart0 0
#define uart1 1
#define usart0 2
#define usart1 3
#define usart2 4


struct tm sys_time;
//    struct tm alarm_time;
    //15-01-2018 12:00:00 Monday

    
char Read_buffer[200];
char UART0_Receive_buffer[200];
char UART0_Transmit_buffer[200];
//uint8_t Process_Buffer[200];
char Process_Buffer[200];
void IO_TASK(char *Receive_Buffer);

/*TEMP SENSOR TASK*/
#define TEMP_SENSOR_SLAVE_ID_A     0x48//0x49
#define TEMP_SENSOR_SLAVE_ID_B     0x49
#define TEMP_SENSOR_SLAVE_ID_C     0x4A//0x00//0x97//0x4B
#define TEMP_SENSOR_SLAVE_ID_BMS     0x48
#define MCP_SLAVE_ID     0x68
#define MOTOR_CONTROL 0x4D
#define MOTOR_1 0x31
#define ON_ 0x31
#define OFF_ 0x30
#define NEGATIVE     1
#define POSITIVE     0
uint8_t Temp1SIGN_BIT,Temp2SIGN_BIT,Temp3SIGN_BIT,TempBMS_SIGN_BIT;
uint8_t Temp1Hi,Temp1Lo,Temp2Hi,Temp2Lo,Temp3Hi,Temp3Lo,TempBMSHi,TempBMSLo;
float Temperature1_fl,Temperature2_fl,Temperature3_fl,TemperatureBMS_fl;
uint8_t TWI_DATA_OUT[5];
uint8_t TWI_DATA_IN[10];
uint16_t TEMP_DEVID[2];
void delay(uint32_t x);
char Temp1[10],Temp2[10],Temp3[10],TempBMS[10];
void Temp_Sensor_TASK(void);

void UART0_TASK(void);
void UART1_TASK(void);
void USART0_TASK(void);
void USART1_TASK(void);
void USART2_TASK(void);
char UART1_Receive_buffer[100];
char UART1_Transmit_buffer[100];
char USART0_Receive_buffer[200];
char USART0_Transmit_buffer[200];
char USART1_Receive_buffer[100];
char USART1_Transmit_buffer[100];
char USART2_Receive_buffer[100];
char USART2_Transmit_buffer[100];
char UART1_Process_buffer[100];
uint8_t USART0_Process_buffer[200];
char USART1_Process_buffer[100];
char USART2_Process_buffer[100];


void ZIGBEE_PROCESS(uint8_t *Zigbee_Buffer);

enum { 
     wait_receive=0,
     packet_received,
     packet_transmit,    
}UART0_STATE;

enum { 
     Wait_receive=0,
     Packet_received,
     Packet_transmit,    
}UART1_STATE;
enum {  
     WAIT_RECEIVE,
     PACKET_RECEIVED,
     PACKET_TRANSMIT,
     USART0_IDLE,
}USART0_STATE;
enum {  
    RECEIVE_WAIT=0,
    RECEIVED_PACKET,
    TRANSMIT_PACKET,
}USART1_STATE;
enum { 
    Receive_Wait=0,
    Received_Packet,  
    Transmit_Packet,    
}USART2_STATE;

#define MPPT_RX_BUFFER_SIZE    100
char mppt_received_data[MPPT_RX_BUFFER_SIZE];
void mppt_bms_calculation(void);
float pv_vol,pv_curr,pv_power,battery_volt,batt_charg_curr,batt_discharg_curr;
int pv_work_status,Batt_SOC,charge_cont_temp,batt_temp,generated_energy;
char controller_status;
char mppt_receieve_buffer[70];
uint8_t Bmscount;


#define ZIGBEE_RX_BUFFER_SIZE  100
#define ZIGBEE_TX_BUFFER_SIZE  100

char Zigbee_uart_receive[ZIGBEE_RX_BUFFER_SIZE];

void Variableint(void);

bool SD_CARD_WRITE_FLAG;

int commandtocleaner;

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
