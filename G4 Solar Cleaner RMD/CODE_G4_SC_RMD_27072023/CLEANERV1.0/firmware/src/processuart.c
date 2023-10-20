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
    USART1_TASK();     
    USART2_TASK();       
}

void DEBUG_UARTTASK(char *Receive_Buffer)
{
    memset(UART0_Transmit_buffer,Null,sizeof(UART0_Transmit_buffer));
    if(UART0_STATE == packet_received)
    {  
        uartflag = 1;
        
        ProcessReceivedJsonData_config(Process_Buffer);
        
        RestartCommTimer();
        UART0_STATE=packet_transmit;
    }
    
}
    
void ZIGBEE_UARTTASK(char *Zigreceive_Buffer)
{
    memset(USART0_Transmit_buffer,Null,sizeof(USART0_Transmit_buffer));
    if(USART0_STATE == PACKET_RECEIVED)
    { 
        uartflag = 2;
        RestartCommTimer();
        ClearCommTimeOver();
        ProcessReceivedJsonData((uint8_t*)USART0_Process_buffer,sizeof(USART0_Process_buffer));
        USART0_STATE=PACKET_TRANSMIT;
    }
}


