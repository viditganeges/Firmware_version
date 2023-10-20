#include "main.h"

bool GetChargingOrNot(void)
{
//  return((bool)GetRegisterValue(LTC4015_CC_CV_CHARGE_BF)); 
    return 0;
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
void Bmsoperate(void)
{
    char buff1[] = {0x00,0x01,0x00,0x00,0x00,0x06,0x01,0x03,0x00,0x01,0x00,0x0A};
    char buff2[] = {0x00,0x01,0x00,0x00,0x00,0x06,0x01,0x03,0x00,0x0A,0x00,0x0A};
    char buff3[] = {0x00,0x01,0x00,0x00,0x00,0x06,0x01,0x03,0x00,0x36,0x00,0x0A};
//    if((bmstransmitStopTimeOver == true )&&(Bmscount == 0))
    if(Bmscount >= 3){
        Bmscount = 0;
    }
    if(Bmscount == 0)
    {
      USART2_Write(buff1, 12);
      while(!USART2_TxComplete());
//      while(USART2_WriteIsBusy());
//      Bmscount=1;
//      BmstransmitTimeron(Bmstime);
//      USART2_STATE=Receive_Wait;
    }
//    else if((bmstransmitStopTimeOver == true )&&(Bmscount == 1))
    if(Bmscount == 1)
    {
      USART2_Write(buff2, 12);
      while(!USART2_TxComplete());
//      while(USART2_WriteIsBusy());
//      Bmscount=2;
//      BmstransmitTimeron(Bmstime);
//      USART2_STATE=Receive_Wait;
    }  
//    else if((bmstransmitStopTimeOver == true )&&(Bmscount == 2))
    if(Bmscount == 2)
    {
      USART2_Write(buff3, 12);
      while(!USART2_TxComplete());
//      while(USART2_WriteIsBusy());
//      Bmscount=0;
//      BmstransmitTimeron(Bmstime);
//      USART2_STATE=Receive_Wait;
    }   
}


void mppt_bms_calculation(void)
{
    uint8_t bms_string[2];
    
    if(USART2_STATE == Received_Packet)
    {
        if(Bmscount == 1)
        {
            //PV_VOLTAGE
           bms_string[0] = mppt_received_data[9]; 
           bms_string[1] = mppt_received_data[10]; 
           
           pv_vol =  hextodec(bms_string,2);
           pv_vol *= 0.01;
           
           //PV_CUURENT
           bms_string[0] = mppt_received_data[11]; 
           bms_string[1] = mppt_received_data[12]; 
           
           pv_curr =  hextodec(bms_string,2);
           pv_curr *= 0.01;
           
           //PV_POWER
           bms_string[0] = mppt_received_data[13]; 
           bms_string[1] = mppt_received_data[14]; 
           
           pv_power =  hextodec(bms_string,2);
           pv_power *= 0.01;
           
           //PV_WORK_STATUS
           bms_string[0] = mppt_received_data[15]; 
           bms_string[1] = mppt_received_data[16]; 
           
           pv_work_status =  hextodec(bms_string,2);
//           pv_work_status *= 0.01;
           
           //BATTERY_VOLTAGE
           bms_string[0] = mppt_received_data[17]; 
           bms_string[1] = mppt_received_data[18]; 
           
           battery_volt =  hextodec(bms_string,2);
           battery_volt *= 0.01;
           
           //BATTERY_TEMPERATURE
           bms_string[0] = mppt_received_data[19]; 
           bms_string[1] = mppt_received_data[20]; 
           
           batt_temp =  hextodec(bms_string,2);
           
           //BATTERY_CHARGING_CURRENT
           bms_string[0] = mppt_received_data[21]; 
           bms_string[1] = mppt_received_data[22]; 
           
           batt_charg_curr =  hextodec(bms_string,2);
           batt_charg_curr *= 0.01;
           
           //BATTERY_DISCHARGING_CURRENT
           bms_string[0] = mppt_received_data[23]; 
           bms_string[1] = mppt_received_data[24]; 
           
           batt_discharg_curr =  hextodec(bms_string,2);
           batt_discharg_curr *= 0.01;
        }
        if(Bmscount == 2)
        {    
           //SOC
           bms_string[0] = mppt_received_data[9]; 
           bms_string[1] = mppt_received_data[10]; 
           
           Batt_SOC =  hextodec(bms_string,2);
//           Batt_SOC *= 0.01;
           
           //charge_control_temp
           bms_string[0] = mppt_received_data[13]; 
           bms_string[1] = mppt_received_data[14]; 
           
           charge_cont_temp =  hextodec(bms_string,2);
//           charge_cont_temp *= 0.01;
           
           //generated energy 
           bms_string[0] = mppt_received_data[15]; 
           bms_string[1] = mppt_received_data[16]; 
           
           generated_energy =  hextodec(bms_string,2);
//           charge_cont_temp *= 0.01;
        }
        if(Bmscount == 3)
        {
           //controller working status 
           bms_string[0] = mppt_received_data[9]; 
           bms_string[1] = mppt_received_data[10]; 
           
           controller_status =  hextodec(bms_string,2);
//           charge_cont_temp *= 0.01;
        }
            USART2_STATE=Transmit_Packet;
    }

    snprintf(mppt_receieve_buffer,70,"%0.2f,%0.2f,%0.2f,%0.2f,%d,%0.2f,%0.2f,%d,%d,%d,%d,%d",pv_vol,pv_curr,pv_power,battery_volt,batt_temp,batt_charg_curr,batt_discharg_curr,pv_work_status,Batt_SOC,charge_cont_temp,generated_energy,controller_status);
}
