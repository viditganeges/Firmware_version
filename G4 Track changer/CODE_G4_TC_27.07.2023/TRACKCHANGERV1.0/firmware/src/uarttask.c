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
             //memset(UART0_Receive_buffer,Null,sizeof(UART0_Receive_buffer));
             UART0_Read(UART0_Receive_buffer,200);
             break;  
        case packet_received:
             memset(Process_Buffer,Null,sizeof(Process_Buffer));
             strncpy(Process_Buffer,Read_buffer,strlen(Read_buffer));
             memset(Read_buffer,Null,sizeof(Read_buffer));
             memset(UART0_Receive_buffer,Null,sizeof(UART0_Receive_buffer));
             //IO_TASK(Process_Buffer);        //uart post task process  
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
             USART0_Read((uint8_t*)&USART0_Receive_buffer,34);//
//             memset(Zigbee_uart_receive,Null,sizeof(Zigbee_uart_receive));
//             memcpy(Zigbee_uart_receive,USART0_Receive_buffer,sizeof(USART0_Receive_buffer));
//             memset(USART0_Receive_buffer,Null,sizeof(USART0_Receive_buffer));
             break;  
        case  PACKET_RECEIVED: 
             memset(Zigbee_uart_receive,Null,sizeof(Zigbee_uart_receive));
             memcpy(Zigbee_uart_receive,USART0_Receive_buffer,sizeof(USART0_Receive_buffer));
             ZIGBEE_UARTTASK(Zigbee_uart_receive);
//            if((strcmp(Zigbee_uart_receive,"{"))&& (strcmp(Zigbee_uart_receive,"}")))
//            {
//                USART0_STATE=WAIT_RECEIVE;
//                //Receiving_packet_processing(Zigbee_uart_receive);
//            }
//             ZIGBEE_PROCESS(Zigbee_uart_receive);   
//             USART0_STATE=WAIT_RECEIVE;            
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

//void ZIGBEE_PROCESS(char *Zigbee_Buffer)
//{
//    BYTE Zigbee_size;
////    char *End;
////    char *Start;
////    int len;
//    //char Temp_Buffer[100];
//    char temp[100];
//   // int i=0,j=0;
//    
//        Zigbee_size=USART0_Receive_buffer[2]+4;//(receivd pac size =length+4)ie 23+4  length is in 3rd index
//        memset(temp,Null,sizeof(temp));
//        memset(USART0_Process_buffer,Null,sizeof(USART0_Process_buffer));
//        memcpy(USART0_Process_buffer,USART0_Receive_buffer,Zigbee_size);
//        //sprintf(temp, "%s", USART0_Process_buffer);
////        for(i=0;i<Zigbee_size;i++)
////        {
////          sprintf(temp, "%x", USART0_Process_buffer[i]);
////          if(temp[1] == 0)
////          {
////             Temp_Buffer[j++]='0';
////             Temp_Buffer[j++]=temp[0];
////          }
////
////          else
////          {
////             Temp_Buffer[j++]=temp[0]; 
////             Temp_Buffer[j++]=temp[1];
////
////          }
////        }
////        Start=strstr(USART0_Process_buffer,"{");
////        Start=Start+strlen("{");
////        End=strstr(Start,"}"); 
////        len=End-Start;
////        memset(Temp_Buffer,Null,sizeof(Temp_Buffer));
////        strncpy(Temp_Buffer,Start,(len));
//      // if((strstr(USART0_Process_buffer, "MOTOR1 ON")))
//            //USART0_Process_buffer
//       if(USART0_Process_buffer[21]==MOTOR_CONTROL)
//       {
//            if(USART0_Process_buffer[22]==MOTOR_1 && USART0_Process_buffer[23]==ON_)
//            {
//                GPIO_PC5_LED1_Set();
////                Motor_Drive_Task(MOTOR_A,ON);//MOTOR ON
//                USART0_STATE=WAIT_RECEIVE;
//            } 
//            else if(USART0_Process_buffer[22]==MOTOR_1 && USART0_Process_buffer[23]==OFF_ )
//            {
////               Motor_Drive_Task(MOTOR_A,OFF);//MOTOR ON
//               GPIO_PC5_LED1_Clear();
//               USART0_STATE=WAIT_RECEIVE;
//            }
//       }
//       
//     
//}

void USART1_TASK(void)
{

    switch( USART1_STATE)
    {
        case RECEIVE_WAIT:
             memset(USART1_Receive_buffer,Null,sizeof(USART1_Receive_buffer));
             USART1_Read(USART1_Receive_buffer,10);
             break;  
        case RECEIVED_PACKET:
           
            strncpy(USART1_Process_buffer,USART1_Receive_buffer,strlen(USART1_Receive_buffer));
            memset(USART1_Receive_buffer,Null,sizeof(USART1_Receive_buffer));
            VerifyAndSetData();   
            USART1_STATE=Transmit_Packet;
            
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
//     char buff[] = {0x00,0x01,0x00,0x00,0x00,0x06,0x01,0x03,0x00,0x01,0x00,0x0A};
    switch( USART2_STATE)
    {
        case Receive_Wait:
//             memset(USART2_Receive_buffer,Null,sizeof(USART2_Receive_buffer));
////             memset(Process_Buffer,Null,sizeof(Process_Buffer));
//             USART2_Read(USART2_Receive_buffer,29);  
//             while(USART2_ReadIsBusy());
             GPIO_PC3_Set();
             USART2_STATE=Received_Packet;
             break;  
        case Received_Packet:
            
            memcpy(mppt_received_data,USART2_Receive_buffer,100);
            strncpy(USART2_Process_buffer,USART2_Receive_buffer,strlen(USART2_Receive_buffer));
            memset(USART2_Receive_buffer,Null,sizeof(USART2_Receive_buffer));
             Bmscount++;
             mppt_bms_calculation();
////////             rfidvalue = HEX_to_DEC(&USART2_Process_buffer[2],8); 
////////            DEC_TEN_ASCII(rfidvalue,&RFID_10dig_String[0],&RFID_10dig_String[1],&RFID_10dig_String[2],&RFID_10dig_String[3],&RFID_10dig_String[4],&RFID_10dig_String[5],&RFID_10dig_String[6],&RFID_10dig_String[7],&RFID_10dig_String[8],&RFID_10dig_String[9]);
////////            
//////////            UartTransmit((uint8_t *)RFID_10dig_String,strlen(RFID_10dig_String));
////////            UART0_Write((void *)RFID_10dig_String,10);
////////                while(UART0_WriteIsBusy());
////////            SYSTICK_DelayMs(500);
////////            memset(RFID_10dig_String,Null,sizeof(RFID_10dig_String));    
            
//            Start=strstr(USART2_Process_buffer,"{");
//            Start=Start+strlen("{");
//            End=strstr(Start,"}"); 
//            len=End-Start;
//            strncpy(USART2_Transmit_buffer,Start,(len));
             break;  
        case Transmit_Packet:    
//             GPIO_PC3_Set();
             GPIO_PC3_Set();
//             USART2_Write(buff,12);
             
             Bmsoperate();
//             while(!USART2_TxComplete());
//             SYSTICK_DelayMs(2);
//             GPIO_PA23_Clear();
             GPIO_PC3_Clear();
             //uart0Obj.rxSize = 0;
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





