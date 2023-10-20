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
    ProximityInit();
    CleanerDetectInit(); 
    Variableint();
    flash_ptr_check();
    ZigbeeInit();
    USART0_Read(USART0_Process_buffer, 0);
    GetZigbeeAPIMacAddress();
    TransmitWelcomeMessage(); 
//    unCleanerMAC *getCleanerMAC = GetSetCleanerMAC();
//    SendCommandToCleaner(getCleanerMAC->cleanerMAC_64,SET_AND_START_CYCLE,(char*)"1000,1000,0,0,2");
    USART2_STATE=Transmit_Packet;

//    uint8_t temp_buffer[] = {0x00};
    
    while ( true )
    {   
         OperateLeds();
         ProcessUartData();                   
         RobotOperate();
         RobotFSM(); 
         RelayFSM(); 
         SDcardOperation();
         FlashLogOperation();
         WDT_Clear();
         
//         TWIHS0_Write(0x68, temp_buffer, 1);
         
    }
    return ( EXIT_FAILURE ); /* Execution should not come here during normal operation */
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
    Daycount= 0;
    rowsActuallyCleaned = 0;  
    commandtocleaner = 0;
    trackchangercount = 0;
    Cumulative_distance = 0;
}
