#include "main.h"

__STATIC_INLINE uint32_t decimaltobcd( uint32_t aDecValue )
{
    uint32_t  decValueDiv10 = aDecValue / 10;

    return( (decValueDiv10 << 4) + ( aDecValue - (decValueDiv10 * 10) ) );
}

__STATIC_INLINE uint32_t bcdtodecimal( uint32_t aBcdValue )
{
    return( (10 * ((aBcdValue & 0xF0) >> 4)) + (aBcdValue & 0x0F) );
}

//bool external_rtc_oscillator_get_state(void)
//{
//    uint8_t temp_buffer[1];
//    memset(temp_buffer, 0, sizeof(temp_buffer));
//    temp_buffer[0] = 0x03;
//    
//    TWIHS2_Write(MCP79410_SLAVEID, temp_buffer, 1);
//    Twihs2_BusywaitandRecover();
//    
//    memset(temp_buffer, 0, sizeof(temp_buffer));
//    
//    TWIHS2_Read(MCP79410_SLAVEID, temp_buffer, 1);
//    Twihs2_BusywaitandRecover();
//    
//    return ((temp_buffer[0]&0x20)>>5);
//}
//
//void external_rtc_oscillator_set_state(bool state)
//{
//    uint8_t temp_buffer[2];
//    memset(temp_buffer, 0, sizeof(temp_buffer));
//    
//    TWIHS2_Write(MCP79410_SLAVEID, temp_buffer, 1);
//    Twihs2_BusywaitandRecover();
//    
//    memset(temp_buffer, 0, sizeof(temp_buffer));
//    
//    TWIHS2_Read(MCP79410_SLAVEID, temp_buffer, 1);
//    Twihs2_BusywaitandRecover();
//    
//    uint8_t temp = (0x80 & (state << 7)) | (0x7F & temp_buffer[0]);
//    
//    memset(temp_buffer, 0, sizeof(temp_buffer));
//    temp_buffer[1] = temp;
//    
//    TWIHS2_Write(MCP79410_SLAVEID, temp_buffer, 2);
//    Twihs2_BusywaitandRecover();
//}
//
//void external_rtc_battery_set_state(bool state)
//{
//    uint8_t temp_buffer[2];
//    memset(temp_buffer, 0, sizeof(temp_buffer));
//    temp_buffer[0] = 0x03;
//    
//    TWIHS2_Write(MCP79410_SLAVEID, temp_buffer, 1);
//    Twihs2_BusywaitandRecover();
//    
//    memset(temp_buffer, 0, sizeof(temp_buffer));
//    
//    TWIHS2_Read(MCP79410_SLAVEID, temp_buffer, 1);
//    Twihs2_BusywaitandRecover();
//    
//    uint8_t temp = (0x08 & (state << 3)) | (0xF7 & temp_buffer[0]);
//    
//    memset(temp_buffer, 0, sizeof(temp_buffer));
//    temp_buffer[0] = 0x03;
//    temp_buffer[1] = temp;
//    
//    TWIHS2_Write(MCP79410_SLAVEID, temp_buffer, 2);
//    Twihs2_BusywaitandRecover();
//}
//
//void external_rtc_init(void)
//{
//    external_rtc_oscillator_set_state(true);
//    external_rtc_battery_set_state(true);
//}

bool external_rtc_write(struct tm * Time)
{
    bool status = false;
    
    uint8_t temp_buffer[8];
    memset(temp_buffer, 0, sizeof(temp_buffer));
    
    temp_buffer[1] = (decimaltobcd(Time->tm_sec) & 0x7F);
    temp_buffer[2] = (decimaltobcd(Time->tm_min) & 0x7F);
    temp_buffer[3] = (decimaltobcd(Time->tm_hour) & 0x3F);
    temp_buffer[4] = (decimaltobcd(Time->tm_wday) & 0x07);
    temp_buffer[5] = (decimaltobcd(Time->tm_mday) & 0x3F);
    temp_buffer[6] = (decimaltobcd(Time->tm_mon) & 0x1F);
    temp_buffer[7] = (decimaltobcd((Time->tm_year)%100) & 0xFF);
    
    status = TWIHS0_Write(DS1307_SLAVEID, temp_buffer, 8);
    while(TWIHS0_IsBusy());
    
//    if(!status)
//    {
//        return status;
//    }
    
    return status;
}

void external_rtc_read(struct tm * Time)
{
    uint8_t temp_buffer[7];
    memset(temp_buffer, 0, sizeof(temp_buffer));
    
    TWIHS0_Write(DS1307_SLAVEID, temp_buffer, 1);
    while(TWIHS0_IsBusy());
    
    TWIHS0_Read(DS1307_SLAVEID, temp_buffer, 7);
    while(TWIHS0_IsBusy());
    
    Time->tm_sec = bcdtodecimal(temp_buffer[0]&0x7F);
    Time->tm_min = bcdtodecimal(temp_buffer[1]&0x7F);
    Time->tm_hour = bcdtodecimal(temp_buffer[2]&0x3F);
    Time->tm_wday = bcdtodecimal(temp_buffer[3]&0x07);
    Time->tm_mday = bcdtodecimal(temp_buffer[4]&0x3F);
    Time->tm_mon = bcdtodecimal(temp_buffer[5]&0x1F);
    Time->tm_year = bcdtodecimal(temp_buffer[6]&0xFF)+2000;
}