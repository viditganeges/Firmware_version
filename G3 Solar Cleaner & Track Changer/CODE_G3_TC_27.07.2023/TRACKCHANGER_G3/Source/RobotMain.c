#include "board.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "delay.h"
#include <stdio.h>
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
#include "BmsUart/BmsUart.h"
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
#include "Pwm1.h"
#include "proximity.h"
#include "QuadDecoder.h"
#include "ResponseAPIProcess.h"
#include "cleanerdetectcommon.h"
#include "switch.h"
#include "rfid.h"
#include "DefaultValuesConfig.h"

#include "APIProcessing.h"
#include "SDFAT/ff.h"       
#include "SDFAT/ffconf.h"
#include "SDFAT/diskio.h"
#include "SDFAT/integer.h"
#include "SDFAT/SDcardoperation.h"
#include "MotorControl/Motor1/Motor1Control_hal.h"

#include "NumStrConversion.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/*******************************************************************************
* Definitions
******************************************************************************/

#define Time 1296000  //for 15days
#define Bmstime 10000

uint32_t Initialtime;
uint32_t Timetodelete;
uint32_t currenttime;

int Bmscount = 0;

BYTE SD_MOUNT_STATUS;
void DispDebugInfo (void);
static void ProcessUartData(void);
uint8_t count = 0;
bool rfid_f = 1;

//static void BmstransmitTimeron (uint32_t setBmstransmitTimeCount_ms);
//static inline void bmstransmitStopTimerStop (void);
//static volatile bool bmstransmitStopTimeOver          = true;
//static inline void bmstransmitStopTimerStop (void);
//static int32_t bmstransmitStop_timecount              = 0;
//static int32_t brushMotor_timercount                 = 0;
//void BmstransmitStopTimeIncrement_ms (void);
void Bmsoperate(void);
//unsigned long long hex2dec(char *hex,BYTE len);

extern int trackchangercount;
extern int tc_count;
extern int rowsActuallyCleaned;
extern char Board_ON;
extern char ERROR_OCCURRED;

uint8_t rxDataBuffer[MAX_UART_RX_BUFFER_LENGTH];
//
//unsigned long int bmsvalue;
//char* startbms, endbms;
//char temp_arr[100][10], temp_arr1[100][10],slave_arr[10],fun_arr[10],byte_arr[10],uart_arr[100][10];
//unsigned int Slave_address, function_code,BAT_STATE,BAT_STATE_CNT,LOAD_STATE,ENERGY_HI, REG_5V_EN,REG_5V_TRIP,MODBUS_ADR,REG_PWM,MPPT_CNT,MPPT_STATE,REG_DOMINATE,PV_EN,LOAD_EN,LOAD_COUNT,size,size1, i,slave_address_cmp, function_cmp,p1,p2,p3,size_pkt,byte1,byte2,byte3,byte_value;
//unsigned char Byte_cnt;
//float Vbat,VPV,IMPPT,PWR,VN_bat1,VL5V,VLOAD,VN_bat2,HS_TEMP,EXT_TEMP,VR_bat, BAT_SET,BAT_COMP,ENERGY_LO,VMPPT_APP_SET,REG_SET_PV,REG_SET_VBAT,REG_SET_IMPPT,BEST_PV,BEST_PWR,IMPPT_APP_SET;
//int byte_int;
 
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


///////~~~~~~~~~~~~~~~~~COMBINED~~~~~~~~~~~~~~~~~~~~~~~~///////////
//static void BmstransmitTimeron (uint32_t setBmstransmitTimeCount_ms)
//{
//  if(setBmstransmitTimeCount_ms == 0)
//  {
//    bmstransmitStopTimeOver = true;
//    bmstransmitStopTimerStop();
//  }
//  else
//  {
//    bmstransmitStop_timecount = setBmstransmitTimeCount_ms;
//    bmstransmitStopTimeOver = false;
//  }
//}
//
//static inline void bmstransmitStopTimerStop (void)
//{
//  bmstransmitStop_timecount = 0;
//}
//
//void BmstransmitStopTimeIncrement_ms (void)
//{
//  if(bmstransmitStop_timecount)
//  {
//    if(--bmstransmitStop_timecount <= 0)
//    {
//      bmstransmitStopTimeOver = true;
//      bmstransmitStopTimerStop();
//    }
//    else
//    {
//      bmstransmitStopTimeOver = false;
//    }
//  }
//}

void main(void)
{
  BOARD_InitPins();
  BOARD_BootClockRUN();
  WDTimerInit(WDT_RESET_TIME_START);
  LedInit();
  LPTMR_Initialisation();
  DebugUartInit(); 
  ConfigUartInit(); 
  BmsUartInit();
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
  
  // SpiInit();
  //ADCInit();
  
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
  
  //EventLogInit(EEPROM);
  DefaultValuesInit();   
  #if (HARDWARE_VERSION == HVER_1_0)
  RelayInit();                                //not used 
  #endif
  RobotInit();          
  RotateSenseInit();    
  EdgeSenseInit();     
  EnablePortInterrupt();
  BatteryChargerInit();
  InitI2C();
  //DoInitialRelayOn();
  InitSwitch();
  RfidInit();
  
//  MotorDriveOn();
  
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
  //  WDTimerRefresh();
#ifndef ZIGBEE_API_MODE_ENABLE
  TransmitWelcomeMessage(ZIGBEE_UART);
#endif
  //  FindAndUpdateImotOffsetValue();
  UpdateWDTimerValue(WDT_RESET_TIME);
  ProximityInit();
  CleanerDetectInit();
  
  
  Direct_SPI_Init();     
  
  SD_MOUNT_STATUS = SDcardMount(); 
   
  ERROR_OCCURRED = 1;
  Board_ON = 1;
  
 rowsActuallyCleaned = trackchangercount;
 tc_count = trackchangercount;
 
    Initialtime = GetRTCDateTimeSeconds();
    Timetodelete = Initialtime+Time;

  while (1)
  {
    WDTimerRefresh();
    OperateLeds();    
//    ChargerErrorHandler();    MPPT UPDATE
    if(USART_STATE==Transmit_Packet)
    {
      Bmsoperate();
    }
    ProcessUartData();
    RelayFSM();
    RobotOperate();
    RobotFSM();
    //    CheckMotorOverLoad();
    ZigbeePoll();
    DispDebugInfo();
//    CheckAndStoreQC();            MPPT UPDATE
//    ControlledCharging();         MPPT UPDATE 
    SDcardOperation();     
//    Bmsoperate();
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
 // uint8_t rxDataBuffer[MAX_UART_RX_BUFFER_LENGTH];
  uint64_t rxMacAddress = DEFAULT_RX_MAC_ADDR;
 // memset(rxDataBuffer,NULL,sizeof(rxDataBuffer));

  if(DebugUartReceive(rxDataBuffer, (sizeof(rxDataBuffer)/sizeof rxDataBuffer[0])) == SUCCESS)
  {
    SwitchLed3On();
    SetUartType(DEBUG_UART);
    memset(rxDataBuffer,NULL,sizeof(rxDataBuffer));
   // ProcessReceivedJsonData(rxDataBuffer,strlen(rxDataBuffer));
    SwitchLed3Off();
  }
  if(ZigbeeUartReceive(&rxMacAddress, rxDataBuffer, (sizeof(rxDataBuffer)/sizeof rxDataBuffer[0])) == SUCCESS)
  {
    //count++;
    SwitchLed3On();
    SetUartType(ZIGBEE_UART);
    //    Serial_Debug(rxDataBuffer);
    //    Serial_Debug("\r\nCount->");
    //    Serial_Debug_Num(count);
    if(rxMacAddress == getCurrentCleanerMac())
    {
      ProcessResponse(rxMacAddress,rxDataBuffer,strlen(rxDataBuffer));
    }
    else
    {
      SetReceivedMACAddress(rxMacAddress);
#ifdef ZIGBEE_JSON_MODE 
      ProcessReceivedJsonData_config(rxDataBuffer);
#else
      ProcessReceivedJsonData(rxDataBuffer,strlen(rxDataBuffer));
#endif     
    }
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
    }
    else if(GetSwitchState() == MANUAL_CONTROL)
    {
      ProcessReceivedJsonData_config(rxDataBuffer);
    }
    SwitchLed3Off();
  }
    if(mpptflag == 1)
  {
    if(GetSwitchState()==AUTO_CONTROL)
    {
      USART_STATE = Received_Packet;
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
    Serial_Debug("\r\nProximity Count-->");
    Serial_Debug_Num(GetProximityCount());
    snprintf(dispstring, sizeof dispstring, "\nRotateSenseCount : %d, %d, %d, Edge Sensor : %d, %d",
             GetRotateSenseCount(),GetRotateSense1Count(),GetRotateSense2Count(),
             IsEdgeSensor1Detected(), IsEdgeSensor2Detected()); 
    //    
    Serial_Debug(dispstring);
    Serial_Debug("\r\nCleaner presence ->");
    Serial_Debug_Num(IsCleanerPresent());
    //    float Imot1 = GetImot1Value();
    //    float Imot2 = GetImot2Value();
    //    float Imot3 = GetImot3Value();
    //    char temp1Str[20], temp2Str[30], temp3Str[30];
    //    ftoa(Imot1, temp1Str, 2);
    //    ftoa(Imot2, temp2Str, 2);
    //    ftoa(Imot3, temp3Str, 2);
    //    snprintf(dispstring, sizeof dispstring, "\nImot Values-> Imot_1 = %s, Imot_2 = %s, Imot_3 = %s",temp1Str,temp2Str, temp3Str); 
    //    Serial_Debug(dispstring);  
    //    Imot1 = GetTemperatureSensorData(TEMPSENSOR_1);
    //    Imot2 = GetTemperatureSensorData(TEMPSENSOR_2);
    //    ftoa(Imot1, temp1Str, 2);
    //    ftoa(Imot2, temp2Str, 2);
    //    snprintf(dispstring, sizeof dispstring, "\nTEMPERATURE-> TEMP_SENSE_1 = %s, TEMP_SENSE_2 = %s",temp1Str,temp2Str); 
    //    Serial_Debug(dispstring); 
    //    BatteryInfo();
    //    uint8_t data[30];
    //  data[0] = 0x01;
    //  data[1] = 'F';
    //  data[2] = 0x02;
    //  data[3] = '1';
    //  data[4] = '0';
    //  data[5] = '0';
    //  data[6] = 'E';
    //  data[7] = 0x03;
    //  data[8] = '2';
    //  data[9] = 0x04;
    // ZigbeeApiTxData(0x13A20041A7A9FC,data,10);
    // count = 0;
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

//unsigned long long hex2dec(char *hex,BYTE len)
//{
//     
//   long long decimal = 0, base = 1;
//   int i = 0, length;
//
//   length = strlen(hex);
//    for(i = length--; i >= 0; i--)
//    {
//        if(hex[i] >= '0' && hex[i] <= '9')
//        {
//            decimal += (hex[i] - 48) * base;
//            base *= 16;
//        }
//        else if(hex[i] >= 'A' && hex[i] <= 'F')
//        {
//            decimal += (hex[i] - 55) * base;
//            base *= 16;
//        }
//        else if(hex[i] >= 'a' && hex[i] <= 'f')
//        {
//            decimal += (hex[i] - 87) * base;
//            base *= 16;
//        }
//    }
//    return decimal;
// }

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
    memset(mppt_receieve_buffer,NULL,sizeof(mppt_receieve_buffer));
    
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

           pv_work_status =  hextodec(bms_string,2);                       

           
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
           
        }
        else if(Bmscount == 1)
        {
          Bmscount = 2;
           //SOC
           bms_string[0] = mppt_received_data[9]; 
           bms_string[1] = mppt_received_data[10]; 
           
           Batt_SOC =  hextodec(bms_string,2);
           
           //charge_control_temp
           bms_string[0] = mppt_received_data[13]; 
           bms_string[1] = mppt_received_data[14]; 
           
           charge_cont_temp =  hextodec(bms_string,2);

           //generated energy 
           bms_string[0] = mppt_received_data[15]; 
           bms_string[1] = mppt_received_data[16]; 
           
           generated_energy =  hextodec(bms_string,2);
           ftoa(pv_curr,pv_curr_arr, 2);          
           
        }
        else if(Bmscount == 2)
        {
           Bmscount = 3;
           //controller working status 
           bms_string[0] = mppt_received_data[9]; 
           bms_string[1] = mppt_received_data[10]; 
           
           controller_status =  hextodec(bms_string,2);
        }   
        
        USART_STATE=Transmit_Packet;
        memset(mppt_receieve_buffer,NULL,sizeof(mppt_receieve_buffer));      
        snprintf(mppt_receieve_buffer,70,"%s,%s,%s,%s,%d,%s,%s,%d,%d,%d,%d,%d",pv_vol_arr,pv_curr_arr,pv_power_arr,battery_volt_arr,batt_temp,batt_charg_curr_arr,batt_discharg_curr_arr,pv_work_status,Batt_SOC,charge_cont_temp,generated_energy,controller_status);
    }   
}