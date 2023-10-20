#include "board.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "delay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LTC4015_Main.h"
#include "adc.h"
#include "spi.h"
#include "i2c_init.h"
#include "PwmDriveSpi.h"
#include "WDTimer.h"
#include "RobotControl.h"       
#include "RotateSenseCommon.h"       
#include "EdgeSenseCommon.h"       
#include "ZigbeeUart.h"       
#include "DebugUart.h"  
#include "ConfigUart.h"      
#include "DefaultValues.h"       
#include "RobotOperation.h"       
#include "PortInterrupt.h"      
#include "Relay.h"               
#include "ZigbeeAPIMode.h"       
#include "MotCurrentFaultCommon.h"       
#include "CommCommon.h"  
#include "store_soc.h"     
#include "FaultProcess.h" 
#include "APIProcessing.h"
#include "ConfigAPIProcess.h"
#include "temp_sensors.h"
#include "eeprommain.h"
#include "Rtc.h"
#include "LPTimer.h"
#include "LedCommon.h"
#include "Serial_Debug.h"
#include "NumStrConversion.h"
#include "Led3.h"
#include "EventLogger.h"
#include "eepromStdtoHalDriver.h"
#include "switch.h"

#include "SDFAT/ff.h"
#include "SDFAT/ffconf.h"
#include "SDFAT/diskio.h"
#include "SDFAT/integer.h"
#include "SDFAT/SDcardoperation.h"

/*******************************************************************************
* Definitions
******************************************************************************/
#define Time 1296000  //for 15days
#define Bmstime 10

uint32_t Initialtime;
uint32_t Timetodelete;
uint32_t currenttime;

BYTE SD_MOUNT_STATUS;
void DispDebugInfo (void);
static void ProcessUartData(void);
 
extern char Board_ON;
extern char ERROR_OCCURRED;

int Bmscount = 0;
static void BmstransmitTimeron (uint32_t setBmstransmitTimeCount_ms);
static inline void bmstransmitStopTimerStop (void);
static volatile bool bmstransmitStopTimeOver          = true;
static inline void bmstransmitStopTimerStop (void);
static int32_t bmstransmitStop_timecount              = 0;
static int32_t brushMotor_timercount                 = 0;
void BmstransmitStopTimeIncrement_ms (void);
void Bmsoperate(void);

unsigned long long hex2dec(char *hex,BYTE len);

uint8_t rxDataBuffer[MAX_UART_RX_BUFFER_LENGTH];


#define MPPT_RX_BUFFER_SIZE    100
char mppt_received_data[MPPT_RX_BUFFER_SIZE];
void mppt_bms_calculation(void);
char controller_status;
char mppt_receieve_buffer[70];

enum { 
    Receive_Wait=0,
    Received_Packet,  
    Transmit_Packet,    
}USART_STATE;

float pv_vol,pv_curr,pv_power,battery_volt,batt_charg_curr,batt_discharg_curr;
int pv_work_status,Batt_SOC,charge_cont_temp,batt_temp,generated_energy;

char pv_vol_arr[10] ={'0'},pv_curr_arr[10]={'0'},pv_power_arr[10]={'0'},battery_volt_arr[10]={'0'},batt_charg_curr_arr[10]={'0'},batt_discharg_curr_arr[10]={'0'};

extern int mpptflag;

static void BmstransmitTimeron (uint32_t setBmstransmitTimeCount_ms)
{
  //int i = 0;
  if(setBmstransmitTimeCount_ms == 0)
  {
    bmstransmitStopTimeOver = true;
    bmstransmitStopTimerStop();
  }
  else
  {
    bmstransmitStop_timecount = setBmstransmitTimeCount_ms;
    bmstransmitStopTimeOver = false;
  }
}

static inline void bmstransmitStopTimerStop (void)
{
  bmstransmitStop_timecount = 0;
}

void BmstransmitStopTimeIncrement_ms (void)
{
  if(bmstransmitStop_timecount)
  {
    if(--bmstransmitStop_timecount <= 0)
    {
      bmstransmitStopTimeOver = true;
      bmstransmitStopTimerStop();
    }
    else
    {
      bmstransmitStopTimeOver = false;
    }
  }
}


 void main(void)
{  
  BOARD_InitPins();
  BOARD_BootClockRUN();
  WDTimerInit(WDT_RESET_TIME_START);
  LedInit();
  LPTMR_Initialisation();
  DebugUartInit(); 
  ConfigUartInit();        
  TransmitWelcomeMessage(DEBUG_UART);
  TransmitWelcomeMessage(CONFIG_UART); 
  if(RTCInit() == false)
  {
    SetFault(RTC_FAULT);
    Serial_Debug("\n RTC Initialisation ERROR *~*~*~*~*~*");
  }
  else
  {
    Serial_Debug("\n RTC Initialisation OK");  
  } 
  
  //SpiInit();
  //Direct_SPI_Init();//direct pin inits
  ADCInit();
  
  WDTimerRefresh();
  
  if(EEPROM_Init() == false)
  {
    Serial_Debug("\n EEPROM Initialisation Failed");   
  }
  else
  {
    uint8_t status = (uint8_t)InitialiseEepromVariables();   
    //SetLogEvent(EV_LOG_EEPROM, status);  
  }

  EventLogInit(EEPROM);
  DefaultValuesInit();   
  RelayInit();
//  DoInitialRelayOff();  //07S
  RobotInit();          
  RotateSenseInit();    
  EdgeSenseInit();     
  EnablePortInterrupt();
  BatteryChargerInit();
  InitI2C();
  InitSwitch();
  //msdelay(2000);
  
  if(ZigbeeUartInit() == ERROR)
  {
    SetFault(ZIGBEE_FAULT);
    //SetLogEvent(EV_LOG_ZB_UART, (uint8_t)EV_UART_ERROR);
  }
  else
  {
    //SetLogEvent(EV_LOG_ZB_UART, (uint8_t)EV_UART_STARTED); 
  } 
  WDTimerRefresh();
#ifndef ZIGBEE_API_MODE_ENABLE
  TransmitWelcomeMessage(ZIGBEE_UART);
#endif
  FindAndUpdateImotOffsetValue();
  UpdateWDTimerValue(WDT_RESET_TIME);
  RestartCommTimer();
  
    Direct_SPI_Init();
    SD_MOUNT_STATUS = SDcardMount();
  
  ERROR_OCCURRED = 1;
  Board_ON = 1;
  
  Initialtime = GetRTCDateTimeSeconds();
  Timetodelete = Initialtime+Time;
  
  USART_STATE=Transmit_Packet;
  
  while (1)
  {
    WDTimerRefresh();
    OperateLeds();
    ChargerErrorHandler();
    if(USART_STATE==Transmit_Packet)
    {
      Bmsoperate();
    }
    
    ProcessUartData();
    RelayFSM();
    RobotOperate();
    RobotFSM();
    CheckMotorOverLoad();
    ZigbeePoll();
    DispDebugInfo();
    CheckAndStoreQC();
    ControlledCharging();
    SDcardOperation();
//    if(USART_STATE==Transmit_Packet)
//    {
//      Bmsoperate();
//      SDdelay1(10);
//    }   
    currenttime = GetRTCDateTimeSeconds();
    if(currenttime>=Timetodelete)
    {
      Delete_SDdata();
      Timetodelete=0;
      Initialtime = GetRTCDateTimeSeconds();
      Timetodelete = Initialtime+Time;
    }
  }
}

static void ProcessUartData(void)
{
  //uint8_t rxDataBuffer[MAX_UART_RX_BUFFER_LENGTH];
  uint64_t rxMacAddress = DEFAULT_RX_MAC_ADDR;
  if(DebugUartReceive(rxDataBuffer, (sizeof(rxDataBuffer)/sizeof rxDataBuffer[0])) == SUCCESS)
  {
    SwitchLed3On();
    SetUartType(DEBUG_UART);
    ProcessReceivedJsonData(rxDataBuffer,strlen(rxDataBuffer));
    SwitchLed3Off();
  }
  if(ZigbeeUartReceive(&rxMacAddress, rxDataBuffer, (sizeof(rxDataBuffer)/sizeof rxDataBuffer[0])) == SUCCESS)
  {
    RestartCommTimer();
    ClearCommTimeOver();
    SwitchLed3On();
    SetUartType(ZIGBEE_UART);
    SetReceivedMACAddress(rxMacAddress);
#ifdef ZIGBEE_JSON_MODE 
    ProcessReceivedJsonData_config(rxDataBuffer);
#else
    ProcessReceivedJsonData(rxDataBuffer,strlen(rxDataBuffer));
#endif
    SetReceivedMACAddress(0);
    SwitchLed3Off();
  }
  if(ConfigUartReceive(rxDataBuffer, (sizeof(rxDataBuffer)/sizeof rxDataBuffer[0])) == SUCCESS)
  {
    SwitchLed3On();
    SetUartType(CONFIG_UART);
    if(GetSwitchState()==AUTO_CONTROL)
    {
//      Bmscalculation();
//      USART_STATE = Received_Packet;
//      Bmscount++;
//      mppt_bms_calculation();
    }
    else if(GetSwitchState() == MANUAL_CONTROL)
    {
      ProcessReceivedJsonData_config(rxDataBuffer);
    }
    RestartCommTimer();
    SDdelay1(10);
  }
  if(mpptflag == 1)
  {
    if(GetSwitchState()==AUTO_CONTROL)
    {
//      Bmscalculation();
      USART_STATE = Received_Packet;
//      Bmscount++;
//      if(Bmscount == 4)
//      {
//        Bmscount = 0;
//      }
      mppt_bms_calculation();
      mpptflag = 0;
    }
  }
}

void DispDebugInfo (void)
{
  char dispstring[200];
  static uint32_t timeValue = 0;
  uint32_t msTimeCount = GetLPTimerMsTicks();
  if((msTimeCount - timeValue) > 2000)
  {
    timeValue = msTimeCount;
    
    snprintf(dispstring, sizeof dispstring, "\nRotateSenseCount : %d, %d, %d, Edge Sensor : %d, %d",
             GetRotateSenseCount(),GetRotateSense1Count(),GetRotateSense2Count(),
             IsEdgeSensor1Detected(), IsEdgeSensor2Detected()); 
    
    Serial_Debug(dispstring);
    float Imot1 = GetImot1Value();
    float Imot2 = GetImot2Value();
    float Imot3 = GetImot3Value();
    char temp1Str[20], temp2Str[30], temp3Str[30];
    ftoa(Imot1, temp1Str, 2);
    ftoa(Imot2, temp2Str, 2);
    ftoa(Imot3, temp3Str, 2);
    snprintf(dispstring, sizeof dispstring, "\nImot Values-> Imot_1 = %s, Imot_2 = %s, Imot_3 = %s",temp1Str,temp2Str, temp3Str); 
    Serial_Debug(dispstring);  
//    Imot1 = GetTemperatureSensorData(TEMPSENSOR_1);
//    Imot2 = GetTemperatureSensorData(TEMPSENSOR_2);
//    ftoa(Imot1, temp1Str, 2);
//    ftoa(Imot2, temp2Str, 2);
//    snprintf(dispstring, sizeof dispstring, "\nTEMPERATURE-> TEMP_SENSE_1 = %s, TEMP_SENSE_2 = %s",temp1Str,temp2Str); 
//    Serial_Debug(dispstring); 
    BatteryInfo();
  }
}


//void Bmsoperate(void)
//{
//  if(GetSwitchState()==AUTO_CONTROL)
//  {
//    if((bmstransmitStopTimeOver == true )&&(Bmscount == 0))
//    {
//      ConfigUartTransmit(":110310000010CC", 15);
//      Bmscount=1;
//      BmstransmitTimeron(Bmstime);
//    }
//    else if((bmstransmitStopTimeOver == true )&&(Bmscount == 1))
//    {
//       ConfigUartTransmit(":11031010000EBE", 15);
//      Bmscount=2;
//      BmstransmitTimeron(Bmstime);
//    }  
//    else if((bmstransmitStopTimeOver == true )&&(Bmscount == 2))
//    {
//      ConfigUartTransmit(":1103101F0006B7", 15);
//      Bmscount=0;
//      BmstransmitTimeron(Bmstime);
//    }   
//  }
//}

unsigned long long hex2dec(char *hex,BYTE len)
{
     
   long long decimal = 0, base = 1;
   int i = 0, length;

   length = strlen(hex);
    for(i = length--; i >= 0; i--)
    {
        if(hex[i] >= '0' && hex[i] <= '9')
        {
            decimal += (hex[i] - 48) * base;
            base *= 16;
        }
        else if(hex[i] >= 'A' && hex[i] <= 'F')
        {
            decimal += (hex[i] - 55) * base;
            base *= 16;
        }
        else if(hex[i] >= 'a' && hex[i] <= 'f')
        {
            decimal += (hex[i] - 87) * base;
            base *= 16;
        }
    }
    return decimal;
 }


void Bmsoperate(void)
{
  if(GetSwitchState()==AUTO_CONTROL)
  {
    char buff1[] = {0x00,0x01,0x00,0x00,0x00,0x06,0x01,0x03,0x00,0x01,0x00,0x0A};
    char buff2[] = {0x00,0x01,0x00,0x00,0x00,0x06,0x01,0x03,0x00,0x0A,0x00,0x0A};
    char buff3[] = {0x00,0x01,0x00,0x00,0x00,0x06,0x01,0x03,0x00,0x36,0x00,0x0A};

    if(Bmscount == 0)
    {
      ConfigUartTransmit(buff1, 12);
    }
    else if(Bmscount == 1)
    {
      ConfigUartTransmit(buff2, 12);
    }  
    else if(Bmscount == 2)
    {
      ConfigUartTransmit(buff3, 12);
    } 
    else if(Bmscount == 3)
    {
        Bmscount = 0;
    }
    msdelay(100);
  }
}

uint32_t hextodec(uint8_t *hex, uint8_t len)
{
    uint32_t ans = 0;
    
    for(int l=0;l<len;l++)
    {
        ans = ans<<8;
        ans = ans + hex[l];
    }
    
    return ans;
}

void mppt_bms_calculation(void)
{
    uint8_t bms_string[2];
    
//    memset(mppt_received_data,NULL,sizeof(mppt_received_data));
    memset(mppt_receieve_buffer,NULL,sizeof(mppt_receieve_buffer));
//    memcpy(mppt_received_data,rxDataBuffer,sizeof(rxDataBuffer));
    
    if(USART_STATE == Received_Packet)
    { 
        if(Bmscount == 0)
        {
          Bmscount = 1;
            //PV_VOLTAGE
           bms_string[0] = mppt_received_data[9]; 
           bms_string[1] = mppt_received_data[10]; 
           
           pv_vol =  hextodec(bms_string,2);
           pv_vol *= 0.01;
           
           ftoa(pv_vol,pv_vol_arr, 2);
           
           //PV_CUURENT
           bms_string[0] = mppt_received_data[11]; 
           bms_string[1] = mppt_received_data[12]; 
           
           pv_curr =  hextodec(bms_string,2);
           pv_curr *= 0.01;
           ftoa(pv_curr,pv_curr_arr, 2);
           
           //PV_POWER
           bms_string[0] = mppt_received_data[13]; 
           bms_string[1] = mppt_received_data[14]; 
           
           pv_power =  hextodec(bms_string,2);
           pv_power *= 0.01;
           ftoa(pv_power,pv_power_arr, 2);
           
           //PV_WORK_STATUS
           bms_string[0] = mppt_received_data[15]; 
           bms_string[1] = mppt_received_data[16]; 

           pv_work_status =  hextodec(bms_string,2);                          //
//           ftoa(pv_work_status,pv_work_status_arr, 2);
           
           //BATTERY_VOLTAGE
           bms_string[0] = mppt_received_data[17]; 
           bms_string[1] = mppt_received_data[18]; 
           
           battery_volt =  hextodec(bms_string,2);
           battery_volt *= 0.01;
           ftoa(battery_volt,battery_volt_arr, 2);
           
           //BATTERY_TEMPERATURE
           bms_string[0] = mppt_received_data[19]; 
           bms_string[1] = mppt_received_data[20]; 
           
           batt_temp =  hextodec(bms_string,2);
//           ftoa(pv_curr,pv_curr_arr, 2);
           
           //BATTERY_CHARGING_CURRENT
           bms_string[0] = mppt_received_data[21]; 
           bms_string[1] = mppt_received_data[22]; 
           
           batt_charg_curr =  hextodec(bms_string,2);
           batt_charg_curr *= 0.01;
           ftoa(batt_charg_curr,batt_charg_curr_arr, 2);
           
           //BATTERY_DISCHARGING_CURRENT
           bms_string[0] = mppt_received_data[23]; 
           bms_string[1] = mppt_received_data[24]; 
           
           batt_discharg_curr =  hextodec(bms_string,2);
           batt_discharg_curr *= 0.01;
           ftoa(batt_discharg_curr,batt_discharg_curr_arr, 2);
           
//           snprintf(mppt_receieve_buffer,70,"%s,%s,%s,%s,%d,%s,%s,%d,%d,%d,%d,%d",pv_vol_arr,pv_curr_arr,pv_power_arr,battery_volt_arr,batt_temp,batt_charg_curr_arr,batt_discharg_curr_arr,pv_work_status,Batt_SOC,charge_cont_temp,generated_energy,controller_status);
           
        }
        else if(Bmscount == 1)
        {
          Bmscount = 2;
           //SOC
           bms_string[0] = mppt_received_data[9]; 
           bms_string[1] = mppt_received_data[10]; 
           
           Batt_SOC =  hextodec(bms_string,2);
//           ftoa(pv_curr,pv_curr_arr, 2);
           
           //charge_control_temp
           bms_string[0] = mppt_received_data[13]; 
           bms_string[1] = mppt_received_data[14]; 
           
           charge_cont_temp =  hextodec(bms_string,2);
//           ftoa(pv_curr,pv_curr_arr, 2);

           //generated energy 
           bms_string[0] = mppt_received_data[15]; 
           bms_string[1] = mppt_received_data[16]; 
           
           generated_energy =  hextodec(bms_string,2);
           ftoa(pv_curr,pv_curr_arr, 2);
           
//          snprintf(mppt_receieve_buffer,70,"%s,%s,%s,%s,%d,%s,%s,%d,%d,%d,%d,%d",pv_vol_arr,pv_curr_arr,pv_power_arr,battery_volt_arr,batt_temp,batt_charg_curr_arr,batt_discharg_curr_arr,pv_work_status,Batt_SOC,charge_cont_temp,generated_energy,controller_status);
           
        }
        else if(Bmscount == 2)
        {
          Bmscount = 3;
           //controller working status 
           bms_string[0] = mppt_received_data[9]; 
           bms_string[1] = mppt_received_data[10]; 
           
           controller_status =  hextodec(bms_string,2);
           
//         snprintf(mppt_receieve_buffer,70,"%s,%s,%s,%s,%d,%s,%s,%d,%d,%d,%d,%d",pv_vol_arr,pv_curr_arr,pv_power_arr,battery_volt_arr,batt_temp,batt_charg_curr_arr,batt_discharg_curr_arr,pv_work_status,Batt_SOC,charge_cont_temp,generated_energy,controller_status);
//           msdelay(500);
//           ftoa(pv_curr,pv_curr_arr, 2);
        }   
        
        USART_STATE=Transmit_Packet;
        memset(mppt_receieve_buffer,NULL,sizeof(mppt_receieve_buffer));      
        snprintf(mppt_receieve_buffer,70,"%s,%s,%s,%s,%d,%s,%s,%d,%d,%d,%d,%d",pv_vol_arr,pv_curr_arr,pv_power_arr,battery_volt_arr,batt_temp,batt_charg_curr_arr,batt_discharg_curr_arr,pv_work_status,Batt_SOC,charge_cont_temp,generated_energy,controller_status);
       
    }


  
    
}
