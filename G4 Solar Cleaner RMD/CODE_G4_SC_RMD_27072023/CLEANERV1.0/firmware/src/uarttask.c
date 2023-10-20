#include "main.h"

void delay(uint32_t x)
{
    while(x--);
}

void UART0_TASK(void)
{
    switch( UART0_STATE)
    {
        case wait_receive:            
             UART0_Read(UART0_Receive_buffer,200);
             break;  
        case packet_received:
             memset(Process_Buffer,Null,sizeof(Process_Buffer));
             strncpy(Process_Buffer,Read_buffer,strlen(Read_buffer));
             memset(Read_buffer,Null,sizeof(Read_buffer));
             memset(UART0_Receive_buffer,Null,sizeof(UART0_Receive_buffer)); 
             DEBUG_UARTTASK(Process_Buffer);
             break;  
        case packet_transmit:  
             UART0_Write(UART0_Transmit_buffer,strlen(UART0_Transmit_buffer));
             while(UART0_WriteIsBusy());
             memset(Process_Buffer,Null,sizeof(Process_Buffer));
             memset(UART0_Transmit_buffer,Null,sizeof(UART0_Transmit_buffer));
             memset(UART0_Receive_buffer,Null,sizeof(UART0_Receive_buffer));
             UART0_STATE=wait_receive;
             break;        
           
        default:
              UART0_STATE=wait_receive;
            break;
    }
  
}

void UART1_TASK(void)
{
    char *End;
    char *Start;
    int len;
    switch(UART1_STATE)
    {
        case Wait_receive:
            
             UART1_Read(UART1_Receive_buffer,200);
             break;  
        case Packet_received:
             
            strncpy(UART1_Process_buffer,UART1_Receive_buffer,strlen(UART1_Receive_buffer));
            Start=strstr(UART1_Process_buffer,"{");
            Start=Start+strlen("{");
            End=strstr(Start,"}"); 
            len=End-Start;
            strncpy(UART1_Transmit_buffer,Start,(len));
            UART1_STATE=Packet_transmit;
            break;  
        case Packet_transmit:    
             
             UART1_Write(UART1_Transmit_buffer,strlen(UART1_Transmit_buffer));
             while(UART1_WriteIsBusy());
             memset(UART1_Process_buffer,Null,sizeof(UART1_Process_buffer));
             memset(UART1_Transmit_buffer,Null,sizeof(UART1_Transmit_buffer));
             memset(UART1_Receive_buffer,Null,sizeof(UART1_Receive_buffer));
             UART1_STATE=Wait_receive;
             break;        
           
        default:
            break;
    }
  
}


 void USART0_TASK(void)
{
    switch( USART0_STATE)
    {
        case  WAIT_RECEIVE:
             USART0_Read((uint8_t*)&USART0_Receive_buffer,100);
             memcpy(Zigbee_uart_receive,USART0_Receive_buffer,sizeof(USART0_Receive_buffer));
             break;  
        case  PACKET_RECEIVED: 
             memset(Zigbee_uart_receive,Null,sizeof(Zigbee_uart_receive));
             memcpy(Zigbee_uart_receive,USART0_Receive_buffer,sizeof(USART0_Receive_buffer));
             USART0_STATE=WAIT_RECEIVE;
             ZIGBEE_PROCESS(Zigbee_uart_receive);             
             break;  
        case PACKET_TRANSMIT:    
             
             USART0_Write((uint8_t*)&USART0_Transmit_buffer,strlen(USART0_Transmit_buffer));
             while(USART0_WriteIsBusy());
             memset(USART0_Process_buffer,Null,sizeof(USART0_Process_buffer));
             memset(USART0_Transmit_buffer,Null,sizeof(USART0_Transmit_buffer));
             memset(USART0_Receive_buffer,Null,sizeof(USART0_Receive_buffer));
              USART0_STATE=WAIT_RECEIVE;
             break;        
           
        default:
            break;
    }
  
}

void USART1_TASK(void)
{
    char *End;
    char *Start;
    int len;
    switch( USART1_STATE)
    {
        case RECEIVE_WAIT:
             USART1_Read(USART1_Receive_buffer,200);
             break;  
        case RECEIVED_PACKET:
             
            strncpy(USART1_Process_buffer,USART1_Receive_buffer,strlen(USART1_Receive_buffer));
            Start=strstr(USART1_Process_buffer,"{");
            Start=Start+strlen("{");
            End=strstr(Start,"}"); 
            len=End-Start;
            strncpy(USART1_Transmit_buffer,Start,(len));
            USART1_STATE=TRANSMIT_PACKET;
             break;  
        case TRANSMIT_PACKET:    
             
             USART1_Write(USART1_Transmit_buffer,strlen(USART1_Transmit_buffer));
             while(USART1_WriteIsBusy());
             memset(USART1_Process_buffer,Null,sizeof(USART1_Process_buffer));
             memset(USART1_Transmit_buffer,Null,sizeof(USART1_Transmit_buffer));
             memset(USART1_Receive_buffer,Null,sizeof(USART1_Receive_buffer));
             USART1_STATE=RECEIVE_WAIT;
             break;        
           
        default:
            break;
    }  
}

void USART2_TASK(void)
{
    switch( USART2_STATE)
    {
        case Receive_Wait:
             GPIO_PC3_Set();
             USART2_STATE=Received_Packet;
             break;  
        case Received_Packet:
            memset(mppt_received_data,Null,sizeof(mppt_received_data));
            memcpy(mppt_received_data,USART2_Receive_buffer,100);
            memset(USART2_Receive_buffer,Null,sizeof(USART2_Receive_buffer));
             Bmscount++;
             mppt_bms_calculation();
             break;  
        case Transmit_Packet:    
             GPIO_PC3_Set();
             Bmsoperate();
             GPIO_PC3_Clear();
             memset(USART2_Process_buffer,Null,sizeof(USART2_Process_buffer));
             memset(USART2_Transmit_buffer,Null,sizeof(USART2_Transmit_buffer));
             memset(USART2_Receive_buffer,Null,sizeof(USART2_Receive_buffer));
             Usart2_ReadTimeout = 0;
             USART2_Read(USART2_Receive_buffer,29);  
             while(USART2_ReadIsBusy() && Usart2_ReadTimeout<75);
             USART2_STATE=Receive_Wait;
             break;        
           
        default:
            break;
    } 
}

void Variableint(void)
{
    SD_MOUNT_STATUS = 0;
    SDcardmountchecktimer = 0;
    SDcardmountcheckError = false;
    Board_ON_SD = 1;
    Board_ON_FLASH = 1;
    ERROR_OCCURRED = 1;
    ERROR_OCCURRED_FLASH = 1;
    CYCLE_OCCURRED = 0;
    CYCLE_OCCURED_FLASH = 0;
    uartflag = 0;
    relaycontrol = 0;
    schedule_BYTE = 0;
    schedule_BYTE_flashlog = 0;
    motor1A=0;
    motor2A=0;
    motor3A=0;
    Motor1runFault=0;
    Motor2runFault=0;
    BrushMotorrunFault=0;
    zigbeetcflag = 0;
    rotatesensorflag = 1;  //added
    prev_rotateCount = 0;
     
    motor1_lvl_12_flag = false;
    motor1_lvl_23_flag = false;
    motor1_lvl_34_flag = false;
    motor1_lvl_40_flag = false;
    
    motor2_lvl_12_flag = false;
    motor2_lvl_23_flag = false;
    motor2_lvl_34_flag = false;
    motor2_lvl_40_flag = false;
    
    motor3_lvl_12_flag = false;
    motor3_lvl_23_flag = false;
    motor3_lvl_34_flag = false;
    motor3_lvl_40_flag = false;
    
    Motor1RuntimeCurrentTimer = false;
    Motor2RuntimeCurrentTimer = false;
    Motor3RuntimeCurrentTimer = false;
    
    Motor1CurrrentTimerError = false;
    Motor2CurrrentTimerError = false;
    Motor3CurrrentTimerError = false;
}




