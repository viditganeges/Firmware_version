#include "main.h"

/* Application's state machine enum */
typedef enum                                 
{
	FLASH_STATE_INITIALIZE,
    FLASH_STATE_WAIT_MIN_POWER_UP_TIME,
    FLASH_STATE_RESET,
    FLASH_STATE_GLOBAL_BLK_PROTECTION_UNLOCK,
    FLASH_STATE_JEDEC_ID_READ,            
    FLASH_STATE_SECTOR_ERASE,
    FLASH_STATE_READ_STATUS,    
    FLASH_STATE_PAGE_PROGRAM,
    FLASH_STATE_MEMORY_READ,
    FLASH_STATE_VERIFY,
    FLASH_STATE_XFER_SUCCESSFUL,
    FLASH_STATE_XFER_ERROR,    
    FLASH_STATE_IDLE,  
} FLASH_STATES;

/* SST26 Flash Commands */
#define APP_CMD_ENABLE_RESET                      0x66
#define APP_CMD_MEMORY_RESET                      0x99
#define APP_CMD_STATUS_REG_READ                   0x05
#define APP_CMD_CONFIG_REG_READ                   0x35
#define APP_CMD_MEMORY_READ                       0x03
#define APP_CMD_MEMORY_HIGH_SPEED_READ            0x0B
#define APP_CMD_ENABLE_WRITE                      0x06
#define APP_CMD_DISABLE_WRITE                     0x04
#define APP_CMD_4KB_SECTOR_ERASE                  0x20
#define APP_CMD_BLOCK_ERASE                       0xD8
#define APP_CMD_CHIP_ERASE                        0xC7
#define APP_CMD_PAGE_PROGRAM                      0x02
#define APP_CMD_JEDEC_ID_READ                     0x9F
#define APP_CMD_GLOBAL_BLOCK_PROTECTION_UNLOCK    0x98

#define APP_STATUS_BIT_WEL                        (0x01 << 1)
#define APP_STATUS_BIT_BUSY                       (0x01 << 7)

#define APP_PAGE_PROGRAM_SIZE_BYTES               256
#define APP_CS_ENABLE()                           CHIP_SELECT_Clear()
#define APP_CS_DISABLE()                          CHIP_SELECT_Set()

#define APP_MEM_ADDR                              0x0
#define APP_MEM_ADDR1                             APP_MEM_ADDR+1

#define Null '\0'

uint8_t status_reg;

typedef struct
{
    FLASH_STATES    state;
    FLASH_STATES    nextState;
    uint8_t             transmitBuffer[APP_PAGE_PROGRAM_SIZE_BYTES + 5];    
    uint8_t             manufacturerID;
    uint16_t            deviceID;
    uint8_t             isCSDeAssert;
    volatile bool       isTransferDone;
}FLASH_DATA;

FLASH_DATA appData;

uint8_t writeDataBuffer[APP_PAGE_PROGRAM_SIZE_BYTES];
uint8_t readDataBuffer[APP_PAGE_PROGRAM_SIZE_BYTES];
uint8_t writeDataBuffer1[APP_PAGE_PROGRAM_SIZE_BYTES];
uint32_t flash_address;

void APP_Reset(void);
void APP_WriteEnable(void);
void APP_WriteDisable(void);
void APP_SectorErase(uint32_t address);
void APP_ChipErase(void);
void APP_PageProgram(uint32_t address, uint8_t* pPageData);
void APP_PageProgram_size(uint32_t address, uint8_t* pPageData, int size);
void APP_MemoryRead(uint32_t address, uint8_t* pReadBuffer, uint32_t nBytes, bool isHighSpeedRead);
uint8_t APP_StatusRead(void);
uint8_t APP_ConfigRegisterRead(void);
void APP_JEDEC_ID_Read(uint8_t* manufacturerID, uint16_t* deviceID);
void APP_GlobalWriteProtectionUnlock(void);
void APP_MinPowerOnDelay(void);
void APP_SPIEventHandler(uintptr_t context );
void Flash_Task ( void );
uint8_t Write_SST_26_VF_016B_Data(uint32_t Address, uint8_t *Write_Buff, uint32_t Write_Buff_Size_);
void Read_SST_26_VF_016B_Data(uint32_t Address, uint32_t Read_Buff_Size,uint8_t *read_buffer);
bool SST26_IsBusy(void);                


