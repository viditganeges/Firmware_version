#include "main.h"

int main (void)
{
    SYS_Initialize ( NULL );   /* Initialize all modules */     
    TC0_CH0_TimerStart();   
    LedInit();
    InitialiseEepromVariables();   
    DefaultValuesInit(); 
    RobotInit();
    RotateSenseInit(); 
    EdgeSenseInit(); 
    Variableint();
    flash_ptr_check();
    ZigbeeInit();
    USART0_Read(USART0_Process_buffer, 0);
    GetZigbeeAPIMacAddress();
    TransmitWelcomeMessage();
    RelayInit();
     
    USART2_STATE=Transmit_Packet;
    
    while ( true )
    {   
        OperateLeds();
        ProcessUartData();                    
        RobotOperate();
        RobotFSM(); 
        RelayFSM();
        CheckMotorOverLoad();                          //Overload error
        CheckMotorTempFault();                         //Temperature error
        CheckMotorDriverFault();                       //Driver error
        CheckRunFault();                               //Motorrun error
//        CheckMotorCurrentDurationFault();              //Duration error    
        SDcardOperation();
        FlashLogOperation();
        WDT_Clear();
        if(rotatesensorflag == 1)
        {
            bool * rotateSensorState = GetSetRotateSensorEnabledState();
            * rotateSensorState = true;
        }
              
    }
    return ( EXIT_FAILURE ); /* Execution should not come here during normal operation */
}


