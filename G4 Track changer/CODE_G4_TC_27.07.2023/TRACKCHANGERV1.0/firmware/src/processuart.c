#include "apiprocess.h"
#include "processuart.h"
#include "main.h"


void ProcessUartData(void)
{
    UART0_TASK();      //Debug UART
    UART1_TASK();      //Config UART
    if((USART0_STATE == PACKET_RECEIVED) && (ZigbeeState == GENERALRXWAIT))
    {
        ZIGBEE_PROCESS(USART0_Process_buffer);
    }
    USART1_TASK();     //RFID USART
    USART2_TASK();     //  
}

void DEBUG_UARTTASK(char *Receive_Buffer)
{
    memset(UART0_Transmit_buffer,Null,sizeof(UART0_Transmit_buffer));
    if(UART0_STATE == packet_received)
    {  
        SwitchLed3On();
        uartflag = 1;
        ProcessReceivedJsonData_config(Process_Buffer);
        UART0_STATE=packet_transmit;
        SwitchLed3Off();
    }
    
}
    
void ZIGBEE_UARTTASK(char *Zigreceive_Buffer)
{
    memset(USART0_Transmit_buffer,Null,sizeof(USART0_Transmit_buffer));
    if(USART0_STATE == PACKET_RECEIVED)
    { 
        SwitchLed3On();
        uartflag = 2;
//        if(rxMacAddress == getCurrentCleanerMac())
        if(commandtocleaner == 1)
        {
          ProcessResponse(getCurrentCleanerMac(),USART0_Process_buffer,sizeof(USART0_Process_buffer));
          commandtocleaner = 0;
        }
        else 
        {
          ProcessReceivedJsonData((uint8_t*)USART0_Process_buffer,sizeof(USART0_Process_buffer));
        }
        USART0_STATE=PACKET_TRANSMIT;
        SwitchLed3Off();
    }
}


