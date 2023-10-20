#ifndef _BMSUART_H_
#define _BMSUART_H_

#include <stdint.h>
#include "BmsUart_Config.h"
#include "Customtypedefs.h"

//#include "NumStrConversion.h"
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
////#define  BYTE   unsigned char
//uint64_t hex2dec(char *hex,uint8_t len);

//int intToStr(int x, char str[], int d);
//void ftoa(float n, char* res, int afterpoint);
//void reverse(char* str, int len);

// char pkt[100] = ":1103200480033700030003048002BC04840480001DFFF104BE0043000205B300000002EC"; //75
// char pkt[100] = ":11030C000000000011000000010002CC/n"; //35
//char pkt[100]=":11031A125C05B303E70000004C0000000000000000000003E7000100008B/n";//63

//unsigned long int val;
//char* start, end,slave_ptr,fun_ptr,byte_ptr1,byte_ptr2,byte_ptr3;
//char temp_arr[100][100], temp_arr1[100][100],slave_arr[10],fun_arr[10],byte_arr[10],uart_arr[300][300],temp[300][300];
//unsigned int Slave_address, function_code,BAT_STATE,BAT_STATE_CNT,LOAD_STATE,ENERGY_HI, REG_5V_EN,REG_5V_TRIP,MODBUS_ADR,REG_PWM,MPPT_CNT,MPPT_STATE,REG_DOMINATE,PV_EN,LOAD_EN,LOAD_COUNT,size,size1, i,slave_address_cmp, function_cmp,p1,p2,p3,size_pkt,byte1,byte2,byte3,byte_value;
//unsigned char Byte_cnt;
//float Vbat,VPV,IMPPT,PWR,VN_bat1,VL5V,VLOAD,VN_bat2,HS_TEMP,EXT_TEMP,VR_bat, BAT_SET,BAT_COMP,ENERGY_LO,VMPPT_APP_SET,REG_SET_PV,REG_SET_VBAT,REG_SET_IMPPT,BEST_PV,BEST_PWR,IMPPT_APP_SET;
//int byte_int;
//void Bmscalculation(void);

#ifdef __cplusplus
extern "C" {
#endif
  
ErrorStatus BmsUartInit (void);
ErrorStatus BmsUartDeInit (void);
ErrorStatus BmsUartTransmit (uint8_t * txBuffer, uint16_t txBufferLength);
ErrorStatus BmsUartReceive (uint8_t * rxBuffer, uint16_t rxBufferLength);
static void BmsUartReceive_Callback (uint8_t rxData);
static void BmsUartTransmit_Callback (void);
static void BmsUartError_Callback (void);
void BmsRxDataReceive (uint8_t * rxBuffer, uint16_t rxBufferLength);
static void CombineBmsUartReceiveData (uint8_t rxData);
void DisableBmsUartRxIT(void);
void EnableBmsUartRxIT(void); 
ITStatus * GetBmsUartTxCompleteFlagPtr (void);

#ifdef __cplusplus
}
#endif

#endif