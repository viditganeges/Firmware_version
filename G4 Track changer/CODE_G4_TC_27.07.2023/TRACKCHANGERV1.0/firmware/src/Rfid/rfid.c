#include "main.h"

uint32_t rfidTime = 0U;
bool rfidDataReceived = 0;
uint8_t rfidUartReceive[RFID_UART_RX_BUFF_SIZE];

volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */
uint32_t rfidTimer_ms = 0U;
eRfidResetState RfidResetState = R_RESET_START;
eRfidState RfidState = RFID_ENABLE;

void RfidTimerStop(void);
bool VerifyCheckSumMod256(uint8_t* data, uint16_t length);
void ClearBuffer(void);
//void TurnOnRfidReader(void);
//void TurnOffRfidReader(void);

void SetDefaultRfidState(void)
{
RfidState = RFID_ENABLE;
}

void SetRfidState(eRfidState state)
{
RfidState = state;
}

eRfidState GetRfidState(void)
{
return RfidState;
}

void RfidTimerIncrement_ms(void)
{
  rfidTimer_ms++;
  if(rfidTime)
  {
    if(--rfidTime <=0)
    {
      ClearBuffer();
      RfidTimerStop();
    }
  }
}

void CombineData(uint8_t rxChar)
{
  
  RfidTimerOn(2);
  rfidUartReceive[rxIndex++] = rxChar;
  
  if(rxIndex>=RFID_TAG_DATA_SIZE)
  {
    VerifyAndSetData();
  }
}

void DEC_TEN_ASCII(int rfid,char *ascii_1,char *ascii_2,char *ascii_3,char *ascii_4,char *ascii_5,char *ascii_6,char *ascii_7,char *ascii_8,char *ascii_9, char *ascii_10 )
{
    char d1=0x00,d2=0x00,d3=0x00,d4=0x00,d5=0x00;
    char d6=0x00,d7=0x00,d8=0x00,d9=0x00,d10=0x00;
    
    d1 =  rfid%10 +0x30;
    d2 = (rfid/10)%10 +0x30;
    d3 = (rfid/100)%10 +0x30;
    d4 = (rfid/1000)%10 +0x30;
    d5 = (rfid/10000)%10 +0x30;
    d6 = (rfid/100000)%10 +0x30;
    d7 = (rfid/1000000)%10 +0x30;
    d8 = (rfid/10000000)%10 +0x30;
    d9 = (rfid/100000000)%10 +0x30;
    d10= (rfid/1000000000)%10 +0x30;
    
    ascii_1[0] = d10;   
    ascii_2[0] = d9;
    ascii_3[0] = d8;
    ascii_4[0] = d7;
    ascii_5[0] = d6;
    ascii_6[0] = d5;   
    ascii_7[0] = d4;
    ascii_8[0] = d3;
    ascii_9[0] = d2;
    ascii_10[0]= d1;
    
}

uint64_t HEX_to_DEC(char *hex,char len)//hex_to decimal vale
{
    uint64_t decimal = 0;   
    int i,j,k,l;
    char BUFFER[9],TEMP[9],DATA[9];
    memset(BUFFER,0,9);
    strncpy(BUFFER,hex,8);
    memset(TEMP,0,9);
    for(i=0;i<len;i++)
    {
        if(  ((BUFFER[i])>='0')  && ((BUFFER[i])<='9')  )
        {
            TEMP[i]=BUFFER[i]-48;
        }
        else if(  ((BUFFER[i])>='A')  && ((BUFFER[i])<='F')  ) 
        {
            TEMP[i]=BUFFER[i]-55;
        }    
    }
    
    memset(DATA,0,9);
    j=1;
    
    SYSTICK_DelayMs(2);
    
    for(k=0;k<len;k++)
    {
        if(k==0 || k==2 || k==4 ||k==6)
        {
            DATA[k/2]=(char)(TEMP[k]<<4);
        }
        else if(k==1 || k==3 || k==5 ||k==7)
        {
            DATA[k-j]=(char)(DATA[k-j] | TEMP[k]);
            j++;
        }
    }
    
    SYSTICK_DelayMs(2);
    
    for(l=0;l<4;l++)
    {
        decimal<<=8;
        decimal |=DATA[l];
    }
    
    SYSTICK_DelayMs(2);

    return decimal;
}



void VerifyAndSetData(void)
{
  uint64_t RFID_DEC = 0;
  uint8_t actualDataLen = RFID_TAG_DATA_SIZE;
  RFID_DEC = HEX_to_DEC(&USART1_Process_buffer[2],8); 
  DEC_TEN_ASCII(RFID_DEC,&RFID_10dig_String[0],&RFID_10dig_String[1],&RFID_10dig_String[2],&RFID_10dig_String[3],&RFID_10dig_String[4],&RFID_10dig_String[5],&RFID_10dig_String[6],&RFID_10dig_String[7],&RFID_10dig_String[8],&RFID_10dig_String[9]);

  if(strlen(USART1_Process_buffer)>actualDataLen)
  {
    rfidDataReceived = 0;  
  }
  else
  {
   if(strlen(USART1_Process_buffer)==10)   
    {
      ClearLastReadRfidData(); 
      memcpy(lastReadRfidData1,&RFID_10dig_String[0],strlen(RFID_10dig_String));
      int j=0,i=0;  
      while(i <10)
        {
            if(lastReadRfidData1[i]=='0')
            {
                i++;
                continue;  
            } 
            else
             break;
        }
        while(i<10)
//        if(i<10)
        {
            lastReadRfidData[j++]=lastReadRfidData1[i++];      
        }
          lastReadRfidData[actualDataLen-1] = 0;
          ClearBuffer();
          rfidDataReceived = 1; 
    }
    else
    {
      rfidDataReceived = 0;
      rxIndex = 0;
      ClearBuffer();
    }
  }
}

void ClearBuffer(void)
{
  rxIndex = 0;
  memset(USART1_Process_buffer,0,RFID_TAG_DATA_SIZE);
  memset(RFID_10dig_String,0,sizeof(RFID_10dig_String)); 
}

void RfidTimerOn(uint32_t timerOn_ms)
{
  if(timerOn_ms == 0)
  {
    RfidTimerStop();
  }
  else
  {
    rfidTime = timerOn_ms;
  }
}
void RfidTimerStop(void)
{
  rfidTime = 0;
}
//data with checksum last value is checksum
bool VerifyCheckSumMod256(uint8_t* data, uint16_t length)
{
  uint32_t check_sum = 0;
  for(uint16_t i = 0; i<(length-1);i++)
  {
    check_sum+=data[i];
  }
  if((check_sum%256)==data[length-1])
  {
    return 1;
  }
  return 0;
}

bool RfidDataReceive(uint8_t* rfidBuffer,uint16_t rfidBufferLength)
{
  if(rfidDataReceived)
  {
    if(rfidBufferLength<RFID_TAG_DATA_SIZE)
    {
      return 0;
    }
    else
    {
      memcpy(rfidBuffer,lastReadRfidData,strlen((char *)lastReadRfidData)+1);
      //rfidDataReceived = 0;
      return 1;
    }
  }
  return 0;
}

uint32_t GetLastReadRfidData(void)
{
  if(rfidDataReceived)
  { 
    return(strtoul((char*)&lastReadRfidData[2],NULL,16));
  }
  return 0;
}

void ClearLastReadRfidData(void)
{
  rfidDataReceived = 0;
  memset(lastReadRfidData,0,RFID_TAG_DATA_SIZE);
}


void SetRfidResetState(eRfidResetState state)
{
  RfidResetState = state;
}
eRfidResetState GetRfidResetState(void)
{
  return(RfidResetState);
}

eRfidResetStatus ResetRfidFSM(void)
{
  eRfidResetState currentState = GetRfidResetState(); 
  static eRfidResetStatus currentStatus_r = R_RESET_IN_PROGRESS;
  static uint32_t resetOffTime = 0;
  switch(currentState)
  {
  case R_RESET_START : 
    ClearBuffer();
    ClearLastReadRfidData();
    currentStatus_r = R_RESET_IN_PROGRESS;
    Serial_Debug("\r\nReset RFID reader\r\n");
    SetRfidResetState(R_RESET_OFF);
  case R_RESET_OFF : 
//    TurnOffRfidReader();
    SetRfidResetState(R_RESET_WAIT);
    resetOffTime = rfidTimer_ms;
    break;
  case R_RESET_WAIT : 
    if((rfidTimer_ms-resetOffTime)>= RESET_OFF_TIME_MS)
    {
      SetRfidResetState(R_RESET_ON);
    }
    break;
  case R_RESET_ON : 
//    TurnOnRfidReader ();
    SetRfidResetState(R_RESET_WAIT_FOR_ON);
    resetOffTime = rfidTimer_ms;
  case R_RESET_WAIT_FOR_ON :
    if((rfidTimer_ms-resetOffTime)>= RESET_ON_TIME_MS)
    {
      currentStatus_r = R_RESET_DONE;
    }
    break;
  }
  return currentStatus_r;
}