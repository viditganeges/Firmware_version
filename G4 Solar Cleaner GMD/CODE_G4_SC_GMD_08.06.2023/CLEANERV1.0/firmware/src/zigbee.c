#include "main.h"

void ZigbeeInit(void)
{
    frameID = 1;
    USART0_STATE = USART0_IDLE;
    zigbeeInitStatus = true;
    ZigbeeState = IDLESTATE;
}

uint8_t ZigbeeFrameID(void)
{
    frameID++;
    if(frameID == 0)
    {
        frameID++;
    }
    
    return frameID;
}

uint8_t zigbee_checksum(uint8_t *data,int length)
{
    uint8_t calculated_checksum = 0x00;
    
    for(int n=0;n<length;n++)
    {
        calculated_checksum += data[n];
    }
    
    calculated_checksum = 0xFF - calculated_checksum;
    
    return calculated_checksum;
}

bool zigbee_uartsend(uint8_t *data, int length)
{
    bool status = false;
    status = USART0_Write(data, length);
    while(USART0_WriteIsBusy());
    return status;
}

bool ZigbeeTxPacket_Gateway(uint64_t macaddress, uint8_t *data, uint16_t length)
{
    if(ZigbeeState == GENERALRXWAIT || ZigbeeState == IDLESTATE)
    {        
        memset(ZigbeeTxBuffer, 0, sizeof(ZigbeeTxBuffer));
        ZigbeeTxBuffer[0] = 0x7E;                           //start delimiter
        ZigbeeTxBuffer[1] = ((length+14)>>8) & 0xFF;             //length of data (16 bit)
        ZigbeeTxBuffer[2] = (length+14) & 0xFF;
        ZigbeeTxBuffer[3] = ZIGBEETRANSMITREQUEST;
        ZigbeeTxBuffer[4] = 0x01;//ZigbeeFrameID();                
        ZigbeeTxBuffer[5] = (macaddress>>56) & 0xFF;        //64 bit source address
        ZigbeeTxBuffer[6] = (macaddress>>48) & 0xFF;
        ZigbeeTxBuffer[7] = (macaddress>>40) & 0xFF;
        ZigbeeTxBuffer[8] = (macaddress>>32) & 0xFF;
        ZigbeeTxBuffer[9] = (macaddress>>24) & 0xFF;
        ZigbeeTxBuffer[10] = (macaddress>>16) & 0xFF;
        ZigbeeTxBuffer[11] = (macaddress>>8) & 0xFF;
        ZigbeeTxBuffer[12] = macaddress & 0xFF;
        ZigbeeTxBuffer[13] = 0x00;                          //16 bit source address
        ZigbeeTxBuffer[14] = 0x00;
        ZigbeeTxBuffer[15] = 0x00;                          //broadcast radius
        ZigbeeTxBuffer[16] = 0x00;                          //options
        memcpy(&ZigbeeTxBuffer[17], data, length);
        ZigbeeTxBuffer[17+length] = zigbee_checksum(&ZigbeeTxBuffer[3] , length+14);         
        ZigbeeTxSize = 17+length+1;
        USART0_ReadAbort();
        ZigbeeState = TRANSMITPACKET;
        ZigbeeControl();       
        return true;
    }
    return false;
}

bool ZigbeeTxPacket_Zigbee(uint64_t macaddress, uint8_t *data, uint16_t length)
{
    if(ZigbeeState == GENERALRXWAIT || ZigbeeState == IDLESTATE)
    {        
        memset(ZigbeeTxBuffer, 0, sizeof(ZigbeeTxBuffer));
        ZigbeeTxBuffer[0] = 0x7E;                           //start delimiter
        ZigbeeTxBuffer[1] = ((length+14)>>8) & 0xFF;             //length of data (16 bit)
        ZigbeeTxBuffer[2] = (length+14) & 0xFF;
        ZigbeeTxBuffer[3] = ZIGBEETRANSMITREQUEST;
        ZigbeeTxBuffer[4] = 0x01;//ZigbeeFrameID();                
        ZigbeeTxBuffer[5] = (macaddress>>56) & 0xFF;        //64 bit source address
        ZigbeeTxBuffer[6] = (macaddress>>48) & 0xFF;
        ZigbeeTxBuffer[7] = (macaddress>>40) & 0xFF;
        ZigbeeTxBuffer[8] = (macaddress>>32) & 0xFF;
        ZigbeeTxBuffer[9] = (macaddress>>24) & 0xFF;
        ZigbeeTxBuffer[10] = (macaddress>>16) & 0xFF;
        ZigbeeTxBuffer[11] = (macaddress>>8) & 0xFF;
        ZigbeeTxBuffer[12] = macaddress & 0xFF;
        ZigbeeTxBuffer[13] = 0xFF;                          //16 bit source address
        ZigbeeTxBuffer[14] = 0xFE;
        ZigbeeTxBuffer[15] = 0x00;                          //broadcast radius
        ZigbeeTxBuffer[16] = 0x00;                          //options
        memcpy(&ZigbeeTxBuffer[17], data, length);
        ZigbeeTxBuffer[17+length] = zigbee_checksum(&ZigbeeTxBuffer[3] , length+14);   
        ZigbeeTxSize = 17+length+1;
        USART0_ReadAbort();
        ZigbeeState = TRANSMITPACKET;
        ZigbeeControl();
        return true;
    }
    return false;
}

void ZigbeeTx_TC(uint8_t *data, uint16_t length)
{
    unChangerMAC *_ChangerMAC = GetSetChangerMAC();
    ZigbeeTxPacket_Zigbee(_ChangerMAC->changerMAC_64, data, length);
}

bool zigbee_get_parameter(enum ZIGBEE_COMMAND_ID cmdid)//, uint8_t *rxbuff)
{
    memset(ZigbeeTxBuffer, 0, sizeof(ZigbeeTxBuffer));
    ZigbeeTxBuffer[0] = ZIGBEE_FRAME_START_DELIMITER;
    ZigbeeTxBuffer[1] = 0x00;
    ZigbeeTxBuffer[2] = 0x04;
    ZigbeeTxBuffer[3] = ATCOMMAND;
    ZigbeeTxBuffer[4] = ZigbeeFrameID();
    ZigbeeTxBuffer[5] = (cmdid >> 8) & 0xFF;
    ZigbeeTxBuffer[6] = cmdid & 0xFF;
    ZigbeeTxBuffer[7] = zigbee_checksum(&ZigbeeTxBuffer[3], 4);  
    ZigbeeTxSize = 8;  
    USART0_ReadAbort();
    ZigbeeState = TRANSMITPACKET;
    ZigbeeControl();  
    return true;
}

void zigbee_get_mac_L(void)
{
    zigbee_get_parameter(SL);
}

void GetZigbeeAPIMacAddress(void)
{
    zigbee_get_parameter(SL);
    zigbee_get_parameter(SH);
}

uint64_t GetZigbeeMacAddress (void)
{
  if(zigbeeInitStatus == true)
  {
    return DeviceMAC.MAC_64; 
  }
  else
  {
    return DEFAULT_RX_MAC_ADDR;
  }
}

void zigbeeparse_SL(uint8_t *data)
{
    for(int i=0;i<4;i++)
    {
        DeviceMAC.MAC_16[3-i] = data[i];
    }
}

void zigbeeparse_SH(uint8_t *data)
{
    for(int i=0;i<4;i++)
    {
        DeviceMAC.MAC_16[7-i] = data[i];
    }
}


void Zigbeeparse_ATCommand(uint8_t *data)
{
    uint16_t atcommand = 0;
    
    atcommand = data[5];
    atcommand = atcommand << 8;
    atcommand += data[6];
    
    switch(atcommand)
    {
        case SL:
            zigbeeparse_SL(&data[8]);
            break;

        case SH:
            zigbeeparse_SH(&data[8]);
            break;
    }
}

void ZIGBEE_PROCESS(uint8_t *Zigbee_Buffer)
{   
    uint16_t data_len = 0;
    uint8_t frame_type = 0;
    uint8_t calc_checksum = 0;
    
    data_len = Zigbee_Buffer[1];
    data_len = data_len << 8;
    data_len += Zigbee_Buffer[2];
    
    if(data_len)
    {
        calc_checksum = zigbee_checksum(&Zigbee_Buffer[3], data_len);
        
        if(calc_checksum == Zigbee_Buffer[data_len+4-1])
        {
            frame_type = Zigbee_Buffer[3];
            
            switch (frame_type)
            {
                case ATCOMMANDRESPONSE:
                    Zigbeeparse_ATCommand(Zigbee_Buffer);
                    ZigbeeState = GENERALRXWAIT;
                    USART0_STATE = WAIT_RECEIVE;
                    break;
                    
                case ZIGBEERECEIVEPACKET:
                    ZIGBEE_UARTTASK((char*)Zigbee_Buffer);
                    ZigbeeState = GENERALRXWAIT;
                    USART0_STATE = WAIT_RECEIVE;
                    break;    
     
                default:
                    ZigbeeState = GENERALRXWAIT;
                    USART0_STATE = WAIT_RECEIVE;
                    break;
            }
        }        
    }
}



void ZigbeeControl(void)
{
    if(zigbeeInitStatus == true)
    {
        ZigbeeRxTimer = 0;
        bool looprun = true;
        
        while(looprun)
        {
            switch(ZigbeeState)
            {
                case TRANSMITPACKET:
                    USART0_STATE = PACKET_TRANSMIT;                  
                    if(zigbee_uartsend(ZigbeeTxBuffer, ZigbeeTxSize))
                    {
                        USART0_Read(USART0_Process_buffer, 0);
                        USART0_STATE = WAIT_RECEIVE;
                        ZigbeeState = TXRECEIVEWAIT;
                    }
                    else
                    {
                        ZigbeeState = IDLESTATE;
                    }

                    break;

                case TXRECEIVEWAIT:
                    if(USART0_STATE == PACKET_RECEIVED)
                    {
                        ZIGBEE_PROCESS(USART0_Process_buffer);
                        USART0_STATE = WAIT_RECEIVE;
                        ZigbeeState = IDLESTATE;
                    }
                    else if(ZigbeeRxTimer >= 500)
                    {
                        USART0_STATE = WAIT_RECEIVE;
                        ZigbeeState = IDLESTATE;
                    }
                    break;

                case GENERALRXWAIT:
                    if(USART0_STATE == PACKET_RECEIVED)
                    {
                        ZIGBEE_PROCESS(USART0_Process_buffer);
                        USART0_STATE = WAIT_RECEIVE;
                    }
                    break;

                case IDLESTATE:
                    USART0_Read(USART0_Process_buffer, 0);
                    ZigbeeState = GENERALRXWAIT;
                    USART0_STATE = WAIT_RECEIVE;
                    looprun = false;
                    break;

                default:
                    ZigbeeState = IDLESTATE;
                    break;
            }
        }
        
    }
}