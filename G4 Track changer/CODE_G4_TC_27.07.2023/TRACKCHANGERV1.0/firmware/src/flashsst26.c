#include "main.h"
#include <string.h>


/* Application's state machine enum */
//typedef enum
//{
//	APP_STATE_INITIALIZE,
//    APP_STATE_WAIT_MIN_POWER_UP_TIME,
//    APP_STATE_RESET,
//    APP_STATE_GLOBAL_BLK_PROTECTION_UNLOCK,
//    APP_STATE_JEDEC_ID_READ,            
//    APP_STATE_SECTOR_ERASE,
//    APP_STATE_READ_STATUS,    
//    APP_STATE_PAGE_PROGRAM,
//    APP_STATE_MEMORY_READ,
//    APP_STATE_VERIFY,
//    APP_STATE_XFER_SUCCESSFUL,
//    APP_STATE_XFER_ERROR,    
//    APP_STATE_IDLE,  
//    APP_STATE_WRITE_READ_COMPLETE,
//} APP_STATES;
//
///* SST26 Flash Commands */
//#define APP_CMD_ENABLE_RESET                      0x66
//#define APP_CMD_MEMORY_RESET                      0x99
//#define APP_CMD_STATUS_REG_READ                   0x05
//#define APP_CMD_CONFIG_REG_READ                   0x35
//#define APP_CMD_MEMORY_READ                       0x03
//#define APP_CMD_MEMORY_HIGH_SPEED_READ            0x0B
//#define APP_CMD_ENABLE_WRITE                      0x06
//#define APP_CMD_DISABLE_WRITE                     0x04
//#define APP_CMD_4KB_SECTOR_ERASE                  0x20
//#define APP_CMD_BLOCK_ERASE                       0xD8
//#define APP_CMD_CHIP_ERASE                        0xC7
//#define APP_CMD_PAGE_PROGRAM                      0x02
//#define APP_CMD_JEDEC_ID_READ                     0x9F
//#define APP_CMD_GLOBAL_BLOCK_PROTECTION_UNLOCK    0x98
//
//#define APP_STATUS_BIT_WEL                        (0x01 << 1)
//#define APP_STATUS_BIT_BUSY                       (0x01 << 7)
//
//#define APP_PAGE_PROGRAM_SIZE_BYTES               256
//#define APP_CS_ENABLE()                           CHIP_SELECT_Clear()
//#define APP_CS_DISABLE()                          CHIP_SELECT_Set()
//
//#define APP_MEM_ADDR                              0x10000
//#define APP_MEM_ADDR1                              0x0
//#define LED_On()                                  LED_Clear()
//#define LED_Off()                                 LED_Set()
//
//#define Null '\0'
//
//uint8_t status_reg;
//typedef struct
//{
//    APP_STATES    state;
//    APP_STATES    nextState;
//    uint8_t             transmitBuffer[APP_PAGE_PROGRAM_SIZE_BYTES + 5];    
//    uint8_t             manufacturerID;
//    uint16_t            deviceID;
//    uint8_t             isCSDeAssert;
//    volatile bool       isTransferDone;
//}APP_DATA;
//
//APP_DATA          appData;
//uint8_t                 writeDataBuffer[APP_PAGE_PROGRAM_SIZE_BYTES];
//uint8_t                 readDataBuffer[APP_PAGE_PROGRAM_SIZE_BYTES];
//uint8_t                 writeDataBuffer1[APP_PAGE_PROGRAM_SIZE_BYTES];
//uint32_t flash_address;



void APP_Reset(void)
{    
    appData.isTransferDone = false; 
    
    appData.transmitBuffer[0] = APP_CMD_ENABLE_RESET;
    
    APP_CS_ENABLE();
    appData.isCSDeAssert = true;
    QSPI_Write(appData.transmitBuffer, 1);        
    while (appData.isTransferDone == false);  
    
    appData.isTransferDone = false;        
    
    appData.transmitBuffer[0] = APP_CMD_MEMORY_RESET;
    
    APP_CS_ENABLE();
    appData.isCSDeAssert = true;
    QSPI_Write(appData.transmitBuffer, 1); 
    
    while (appData.isTransferDone == false);  
}

void APP_WriteEnable(void)
{
    appData.isTransferDone = false;    
    
    appData.transmitBuffer[0] = APP_CMD_ENABLE_WRITE;
    
    APP_CS_ENABLE();
    appData.isCSDeAssert = true;
    QSPI_Write(appData.transmitBuffer, 1);    
    
    while (appData.isTransferDone == false);  
}

void APP_WriteDisable(void)
{
    appData.isTransferDone = false;    
    
    appData.transmitBuffer[0] = APP_CMD_DISABLE_WRITE;
    
    APP_CS_ENABLE();
    appData.isCSDeAssert = true;
    QSPI_Write(appData.transmitBuffer, 1);    
    
    while (appData.isTransferDone == false);  
}

void APP_SectorErase(uint32_t address)
{       
    APP_WriteEnable();
    
    appData.isTransferDone = false;    
    
    /* The address bits from A11:A0 are don't care and must be Vih or Vil */
    address = address & 0xFFFFF000;
    
    appData.transmitBuffer[0] = APP_CMD_4KB_SECTOR_ERASE;
    appData.transmitBuffer[1] = (address >> 16);
    appData.transmitBuffer[2] = (address >> 8);
    appData.transmitBuffer[3] = address;
    
    APP_CS_ENABLE();   
    appData.isCSDeAssert = true;
    QSPI_Write(appData.transmitBuffer, 4);    
    
    while (appData.isTransferDone == false);  
}

void APP_ChipErase(void)
{       
    APP_WriteEnable();
    
    appData.isTransferDone = false;            
    
    appData.transmitBuffer[0] = APP_CMD_CHIP_ERASE;    
    
    APP_CS_ENABLE();  
    appData.isCSDeAssert = true;
    QSPI_Write(appData.transmitBuffer, 1);    
    
    while (appData.isTransferDone == false);  
}

void APP_PageProgram(uint32_t address, uint8_t* pPageData)
{        
    uint32_t i;
    
    APP_WriteEnable();
    
    appData.isTransferDone = false;                   
    
    appData.transmitBuffer[0] = APP_CMD_PAGE_PROGRAM;
    appData.transmitBuffer[1] = (address >> 16);
    appData.transmitBuffer[2] = (address >> 8);
    appData.transmitBuffer[3] = address;
    
    for (i = 0; i < APP_PAGE_PROGRAM_SIZE_BYTES; i++)
    {
        appData.transmitBuffer[4 + i] = pPageData[i];
    }
    
    APP_CS_ENABLE(); 
    appData.isCSDeAssert = true;
    QSPI_Write(appData.transmitBuffer, (4 + APP_PAGE_PROGRAM_SIZE_BYTES));    
        
    while (appData.isTransferDone == false);  
}

void APP_PageProgram_size(uint32_t address, uint8_t* pPageData, int size)
{        
    uint32_t i;
    
    APP_WriteEnable();
    
    appData.isTransferDone = false;                   
    
    appData.transmitBuffer[0] = APP_CMD_PAGE_PROGRAM;
    appData.transmitBuffer[1] = (address >> 16);
    appData.transmitBuffer[2] = (address >> 8);
    appData.transmitBuffer[3] = address;
    
    for (i = 0; i < APP_PAGE_PROGRAM_SIZE_BYTES; i++)
    {
        appData.transmitBuffer[4 + i] = pPageData[i];
    }
    
    APP_CS_ENABLE(); 
    appData.isCSDeAssert = true;
    QSPI_Write(appData.transmitBuffer, (4 + APP_PAGE_PROGRAM_SIZE_BYTES));    
        
    while (appData.isTransferDone == false);  
}

void APP_MemoryRead(uint32_t address, uint8_t* pReadBuffer, uint32_t nBytes, bool isHighSpeedRead)
{                        
    uint8_t nTxBytes;
    
    appData.isTransferDone = false;                  
        
    appData.transmitBuffer[1] = (address >> 16);
    appData.transmitBuffer[2] = (address >> 8);
    appData.transmitBuffer[3] = address;        
    
    if (isHighSpeedRead == true)
    {
        appData.transmitBuffer[0] = APP_CMD_MEMORY_HIGH_SPEED_READ;
        /* For high speed read, perform a dummy write */
        appData.transmitBuffer[4] = 0xFF;  
        nTxBytes = 5;
    }
    else
    {
        appData.transmitBuffer[0] = APP_CMD_MEMORY_READ;
        nTxBytes = 4;
    }
    
    APP_CS_ENABLE();  
    appData.isCSDeAssert = false;    
    QSPI_Write(appData.transmitBuffer, nTxBytes);    
    
    while (appData.isTransferDone == false);  
    
    appData.isTransferDone = false;                 
    appData.isCSDeAssert = true;
    QSPI_Read(pReadBuffer, nBytes);    
        
    while (appData.isTransferDone == false);  
}

uint8_t APP_StatusRead(void)
{
    uint8_t status;
    appData.isTransferDone = false;    
    
    appData.transmitBuffer[0] = APP_CMD_STATUS_REG_READ;
    
    APP_CS_ENABLE();        
    appData.isCSDeAssert = true;    
    QSPI_WriteRead(appData.transmitBuffer, 1, appData.transmitBuffer, (1+1));    
        
    while (appData.isTransferDone == false); 
    
    status = appData.transmitBuffer[1];
    
    return status;
}

uint8_t APP_ConfigRegisterRead(void)
{
    uint8_t config_reg;
    appData.isTransferDone = false;    
    
    appData.transmitBuffer[0] = APP_CMD_CONFIG_REG_READ;
    
    APP_CS_ENABLE();  
    appData.isCSDeAssert = true;    
    QSPI_WriteRead(appData.transmitBuffer, 1, appData.transmitBuffer, (1+1));    
        
    while (appData.isTransferDone == false);  
    
    config_reg = appData.transmitBuffer[1];
    
    return config_reg;
}

void APP_JEDEC_ID_Read(uint8_t* manufacturerID, uint16_t* deviceID)
{
    appData.isTransferDone = false;    
    
    appData.transmitBuffer[0] = APP_CMD_JEDEC_ID_READ;
    
    APP_CS_ENABLE();        
    appData.isCSDeAssert = true;      
    QSPI_WriteRead(appData.transmitBuffer, 1, appData.transmitBuffer, (1+3));    
        
    while (appData.isTransferDone == false);  
    
    *manufacturerID = appData.transmitBuffer[1];
    *deviceID = (appData.transmitBuffer[2] << 8UL) | appData.transmitBuffer[3];    
}

void APP_GlobalWriteProtectionUnlock(void)
{
    APP_WriteEnable();
    
    appData.isTransferDone = false;    
    appData.transmitBuffer[0] = APP_CMD_GLOBAL_BLOCK_PROTECTION_UNLOCK;
    
    APP_CS_ENABLE();        
    appData.isCSDeAssert = true;
    QSPI_Write(appData.transmitBuffer, 1);    
        
    while (appData.isTransferDone == false);          
}

void APP_MinPowerOnDelay(void)
{
    uint32_t i;        
    
    /* Cheap delay. 
     * Based on the CPU frequency, ensure the delay is at-least 100 microseconds. 
     */
    for (i = 0; i < 300000; i++)
    {
        asm("NOP");
    }        
}

/* This function will be called by SPI PLIB when transfer is completed */
void APP_SPIEventHandler(uintptr_t context )
{
    uint8_t* isCSDeAssert = (uint8_t*)context;
    
    if (*isCSDeAssert == true)
    {
        /* De-assert the chip select */
        APP_CS_DISABLE();
    }
            
    appData.isTransferDone = true;
}

void APP_Initialize (void)
{
//    uint32_t i;
    
//    APP_CS_DISABLE();
//    LED_Off();
    
//    appData.state = APP_STATE_INITIALIZE;
    
    /* Fill up the test data */
//    for (i = 0; i < APP_PAGE_PROGRAM_SIZE_BYTES; i++)
//    {
//        writeDataBuffer[i] = i;
//    }    
//    memset(writeDataBuffer,Null,sizeof(writeDataBuffer));
//    strncpy((char *)writeDataBuffer,"GANGES_PHASE4",13);
//    memset(writeDataBuffer1,Null,sizeof(writeDataBuffer1));
//    strncpy((char *)writeDataBuffer1,"flashwrite",13);
}
//void delay(uint16_t x)
//{
//    while(x--);
//}
//*******************FLASH CODE*******************************************//
void Flash_Task ( void )
{
    /* Check the application's current state. */
        switch (appData.state)
        {
            case FLASH_STATE_INITIALIZE:
//                APP_Initialize();
                APP_CS_DISABLE();
                /* Register a callback with the SPI PLIB and pass a pointer to the isCSDeAssert variable as the context */
                QSPI_CallbackRegister(APP_SPIEventHandler, (uintptr_t) &appData.isCSDeAssert);
                appData.state = FLASH_STATE_WAIT_MIN_POWER_UP_TIME;
                break;
                
            case FLASH_STATE_WAIT_MIN_POWER_UP_TIME:
                APP_MinPowerOnDelay();
                appData.state = FLASH_STATE_RESET;                                 
                break;
                
            case FLASH_STATE_RESET:
                APP_Reset();
                SYSTICK_DelayMs(20);//delay(10000);
                
                        
                appData.state = FLASH_STATE_GLOBAL_BLK_PROTECTION_UNLOCK;
                break;
                
            case FLASH_STATE_GLOBAL_BLK_PROTECTION_UNLOCK:
                APP_GlobalWriteProtectionUnlock();
                
                SYSTICK_DelayMs(20);//delay(10000);
                if(OPERATION == WRITE_OPERATION)
                {
                  appData.state = FLASH_STATE_SECTOR_ERASE;  
                }
                else if(OPERATION == READ_OPERATION)
                {
                    appData.state = FLASH_STATE_READ_STATUS;
                    appData.nextState = FLASH_STATE_PAGE_PROGRAM;
                }
                else
                {
                    appData.state = FLASH_STATE_JEDEC_ID_READ;
                }
                
//                appData.state = FLASH_STATE_JEDEC_ID_READ;
                break;
                
            case FLASH_STATE_JEDEC_ID_READ:
                APP_JEDEC_ID_Read(&appData.manufacturerID, &appData.deviceID);
                
                SYSTICK_DelayMs(20);//delay(10000);
                
                appData.state = FLASH_STATE_SECTOR_ERASE;
                break;                                
                
            case FLASH_STATE_SECTOR_ERASE:
//                APP_SectorErase(APP_MEM_ADDR);
                APP_SectorErase(flash_address);
                SYSTICK_DelayMs(50);//delay(10000);
                
                appData.state = FLASH_STATE_READ_STATUS;
                appData.nextState = FLASH_STATE_PAGE_PROGRAM;
                break;
                
            case FLASH_STATE_READ_STATUS:
                status_reg = APP_StatusRead();
                if ((status_reg & APP_STATUS_BIT_BUSY) == 0)                                             
                {
                    appData.state = appData.nextState;
                }
                break;
                
            case FLASH_STATE_PAGE_PROGRAM:
//                APP_PageProgram(APP_MEM_ADDR, &writeDataBuffer[0]);
                if(OPERATION == WRITE_OPERATION)
                {
                    APP_PageProgram(flash_address, &writeDataBuffer[0]);
                    SYSTICK_DelayMs(20);//delay(10000);
//                    appData.state = FLASH_STATE_READ_STATUS;
//                    appData.nextState = FLASH_STATE_MEMORY_READ;
                    appData.state = FLASH_STATE_XFER_SUCCESSFUL;
                }
                else if(OPERATION == READ_OPERATION)
                {
                    appData.state = FLASH_STATE_MEMORY_READ;
                }
                else
                {
                    appData.state = FLASH_STATE_READ_STATUS;
                    appData.nextState = FLASH_STATE_MEMORY_READ;
                }
                break;
                
            case FLASH_STATE_MEMORY_READ:
//                memset(readDataBuffer,Null,sizeof(readDataBuffer));
//                APP_MemoryRead(APP_MEM_ADDR, readDataBuffer, APP_PAGE_PROGRAM_SIZE_BYTES, false);
                memset(readDataBuffer,Null,sizeof(readDataBuffer));
                APP_MemoryRead(flash_address, readDataBuffer, APP_PAGE_PROGRAM_SIZE_BYTES, false);
                SYSTICK_DelayMs(20);//delay(10000);
                
                appData.state = FLASH_STATE_VERIFY;  
                //appData.state = FLASH_STATE_XFER_SUCCESSFUL;
                break;
                
            case FLASH_STATE_VERIFY:
                if (memcmp(writeDataBuffer, readDataBuffer, APP_PAGE_PROGRAM_SIZE_BYTES) == 0)
                {
                    appData.state = FLASH_STATE_XFER_SUCCESSFUL;
                }
                else
                {
                    appData.state = FLASH_STATE_XFER_ERROR;
                }
                break;

            case FLASH_STATE_XFER_SUCCESSFUL:
//                LED_On();
                appData.state = FLASH_STATE_IDLE;
                break;

            case FLASH_STATE_XFER_ERROR:
//                LED_Off();
                appData.state = FLASH_STATE_IDLE;
                break;
            
//            case APP_STATE_WRITE_READ_COMPLETE:
//                break;
                
            case FLASH_STATE_IDLE:
                break;
                
            default:
                break;
        }
}

uint8_t Write_SST_26_VF_016B_Data(uint32_t Address, uint8_t *Write_Buff, uint32_t Write_Buff_Size_)
{
    appData.state = FLASH_STATE_INITIALIZE;
    eEepromStatus status = EEPROM_OK;
    OPERATION=WRITE_OPERATION;
    memset(writeDataBuffer,0,sizeof(writeDataBuffer));
    memcpy(writeDataBuffer,Write_Buff,Write_Buff_Size_);
    flash_address=Address*0x1000;
    
    while (appData.state!=FLASH_STATE_IDLE)
    {
         Flash_Task();
    }
    
   //return 0;
    return status;
}



void Read_SST_26_VF_016B_Data(uint32_t Address, uint32_t Read_Buff_Size,uint8_t *read_buffer)
{
    eEepromStatus status = EEPROM_OK;
    appData.state = FLASH_STATE_INITIALIZE;//testing
//    appData.state = FLASH_STATE_MEMORY_READ;
    OPERATION=READ_OPERATION;
      
       uint32_t i;
       memset(readDataBuffer,0,sizeof(readDataBuffer));
       flash_address=Address*0x1000;
       while (appData.state!=FLASH_STATE_IDLE)
        {
             Flash_Task();
        }   
        for(i=0;i<=Read_Buff_Size;i++)
        {
            *read_buffer++ = readDataBuffer[i];

        } 
       
       if(status != EEPROM_OK)
        {
          //SetLogEvent(EV_LOG_EEPROM, (uint8_t)EV_MEM_WRITE_ERROR); 
          Serial_Debug("\n Eeprom Write Error :");
          Serial_Debug_Num(status);
        }
}

/* *****************************************************************************
 End of File
 */

bool SST26_IsBusy(void)                     //RR
{
    int current_status = APP_StatusRead();
    
    if((current_status&0x01) == 0x01)
    {
        return true;
    }
    else
    {
        return false;
    }
}