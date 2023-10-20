#include "main.h"
#include <string.h>

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
    QSPI_Write(appData.transmitBuffer, (4 + size));    
        
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
    
}

//*******************FLASH CODE*******************************************//
void Flash_Task ( void )
{
    /* Check the application's current state. */
        switch (appData.state)
        {
            case FLASH_STATE_INITIALIZE:
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
                SYSTICK_DelayMs(20);                                      
                appData.state = FLASH_STATE_GLOBAL_BLK_PROTECTION_UNLOCK;
                break;
                
            case FLASH_STATE_GLOBAL_BLK_PROTECTION_UNLOCK:
                APP_GlobalWriteProtectionUnlock();
                
                SYSTICK_DelayMs(20);
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
                break;
                
            case FLASH_STATE_JEDEC_ID_READ:
                APP_JEDEC_ID_Read(&appData.manufacturerID, &appData.deviceID);
                
                SYSTICK_DelayMs(20);
                
                appData.state = FLASH_STATE_SECTOR_ERASE;
                break;                                
                
            case FLASH_STATE_SECTOR_ERASE:
                APP_SectorErase(flash_address);
                SYSTICK_DelayMs(50);
                
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
                if(OPERATION == WRITE_OPERATION)
                {
                    APP_PageProgram(flash_address, &writeDataBuffer[0]);
                    SYSTICK_DelayMs(20);
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
                memset(readDataBuffer,Null,sizeof(readDataBuffer));
                APP_MemoryRead(flash_address, readDataBuffer, APP_PAGE_PROGRAM_SIZE_BYTES, false);
                SYSTICK_DelayMs(20);
                appData.state = FLASH_STATE_VERIFY;  
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
                appData.state = FLASH_STATE_IDLE;
                break;

            case FLASH_STATE_XFER_ERROR:
                appData.state = FLASH_STATE_IDLE;
                break;
                
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
    return status;
}



void Read_SST_26_VF_016B_Data(uint32_t Address, uint32_t Read_Buff_Size,uint8_t *read_buffer)
{
    eEepromStatus status = EEPROM_OK;
    appData.state = FLASH_STATE_INITIALIZE;
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
          Serial_Debug("\n Eeprom Write Error :");
          Serial_Debug_Num(status);
        }
}

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

/* *****************************************************************************
 End of File
 */

