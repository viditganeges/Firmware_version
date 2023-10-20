/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"
#include "device.h"



// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
#pragma config TCM_CONFIGURATION = 0
#pragma config SECURITY_BIT = CLEAR
#pragma config BOOT_MODE = SET




// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 4 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART4ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX4];

/* USART transmit/receive transfer objects pool */
static DRV_USART_BUFFER_OBJ drvUSART4BufferObjPool[DRV_USART_QUEUE_SIZE_IDX4];

const DRV_USART_PLIB_INTERFACE drvUsart4PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG)USART0_ReadCallbackRegister,
    .read = (DRV_USART_PLIB_READ)USART0_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY)USART0_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET)USART0_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT)USART0_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG)USART0_WriteCallbackRegister,
    .write = (DRV_USART_PLIB_WRITE)USART0_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY)USART0_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET)USART0_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET)USART0_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP)USART0_SerialSetup
};

const uint32_t drvUsart4remapDataWidth[] = { 0x0, 0x40, 0x80, 0xC0, 0x20000 };
const uint32_t drvUsart4remapParity[] = { 0x800, 0x0, 0x200, 0x600, 0x400, 0xC00 };
const uint32_t drvUsart4remapStopBits[] = { 0x0, 0x1000, 0x2000 };
const uint32_t drvUsart4remapError[] = { 0x20, 0x80, 0x40 };

const DRV_USART_INTERRUPT_SOURCES drvUSART4InterruptSources =
{
    /* Peripheral has single interrupt vector */
    .isSingleIntSrc                        = true,

    /* Peripheral interrupt line */
    .intSources.usartInterrupt             = USART0_IRQn,
};

const DRV_USART_INIT drvUsart4InitData =
{
    .usartPlib = &drvUsart4PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX4,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvUSART4ClientObjPool[0],

    .dmaChannelTransmit = SYS_DMA_CHANNEL_NONE,

    .dmaChannelReceive = SYS_DMA_CHANNEL_NONE,

    /* Combined size of transmit and receive buffer objects */
    .bufferObjPoolSize = DRV_USART_QUEUE_SIZE_IDX4,

    /* USART transmit and receive buffer buffer objects pool */
    .bufferObjPool = (uintptr_t)&drvUSART4BufferObjPool[0],

    .interruptSources = &drvUSART4InterruptSources,

    .remapDataWidth = drvUsart4remapDataWidth,

    .remapParity = drvUsart4remapParity,

    .remapStopBits = drvUsart4remapStopBits,

    .remapError = drvUsart4remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 3 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART3ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX3];

/* USART transmit/receive transfer objects pool */
static DRV_USART_BUFFER_OBJ drvUSART3BufferObjPool[DRV_USART_QUEUE_SIZE_IDX3];

const DRV_USART_PLIB_INTERFACE drvUsart3PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG)USART2_ReadCallbackRegister,
    .read = (DRV_USART_PLIB_READ)USART2_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY)USART2_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET)USART2_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT)USART2_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG)USART2_WriteCallbackRegister,
    .write = (DRV_USART_PLIB_WRITE)USART2_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY)USART2_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET)USART2_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET)USART2_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP)USART2_SerialSetup
};

const uint32_t drvUsart3remapDataWidth[] = { 0x0, 0x40, 0x80, 0xC0, 0x20000 };
const uint32_t drvUsart3remapParity[] = { 0x800, 0x0, 0x200, 0x600, 0x400, 0xC00 };
const uint32_t drvUsart3remapStopBits[] = { 0x0, 0x1000, 0x2000 };
const uint32_t drvUsart3remapError[] = { 0x20, 0x80, 0x40 };

const DRV_USART_INTERRUPT_SOURCES drvUSART3InterruptSources =
{
    /* Peripheral has single interrupt vector */
    .isSingleIntSrc                        = true,

    /* Peripheral interrupt line */
    .intSources.usartInterrupt             = USART2_IRQn,
};

const DRV_USART_INIT drvUsart3InitData =
{
    .usartPlib = &drvUsart3PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX3,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvUSART3ClientObjPool[0],

    .dmaChannelTransmit = SYS_DMA_CHANNEL_NONE,

    .dmaChannelReceive = SYS_DMA_CHANNEL_NONE,

    /* Combined size of transmit and receive buffer objects */
    .bufferObjPoolSize = DRV_USART_QUEUE_SIZE_IDX3,

    /* USART transmit and receive buffer buffer objects pool */
    .bufferObjPool = (uintptr_t)&drvUSART3BufferObjPool[0],

    .interruptSources = &drvUSART3InterruptSources,

    .remapDataWidth = drvUsart3remapDataWidth,

    .remapParity = drvUsart3remapParity,

    .remapStopBits = drvUsart3remapStopBits,

    .remapError = drvUsart3remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 2 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART2ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX2];

/* USART transmit/receive transfer objects pool */
static DRV_USART_BUFFER_OBJ drvUSART2BufferObjPool[DRV_USART_QUEUE_SIZE_IDX2];

const DRV_USART_PLIB_INTERFACE drvUsart2PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG)USART1_ReadCallbackRegister,
    .read = (DRV_USART_PLIB_READ)USART1_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY)USART1_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET)USART1_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT)USART1_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG)USART1_WriteCallbackRegister,
    .write = (DRV_USART_PLIB_WRITE)USART1_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY)USART1_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET)USART1_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET)USART1_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP)USART1_SerialSetup
};

const uint32_t drvUsart2remapDataWidth[] = { 0x0, 0x40, 0x80, 0xC0, 0x20000 };
const uint32_t drvUsart2remapParity[] = { 0x800, 0x0, 0x200, 0x600, 0x400, 0xC00 };
const uint32_t drvUsart2remapStopBits[] = { 0x0, 0x1000, 0x2000 };
const uint32_t drvUsart2remapError[] = { 0x20, 0x80, 0x40 };

const DRV_USART_INTERRUPT_SOURCES drvUSART2InterruptSources =
{
    /* Peripheral has single interrupt vector */
    .isSingleIntSrc                        = true,

    /* Peripheral interrupt line */
    .intSources.usartInterrupt             = USART1_IRQn,
};

const DRV_USART_INIT drvUsart2InitData =
{
    .usartPlib = &drvUsart2PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX2,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvUSART2ClientObjPool[0],

    .dmaChannelTransmit = SYS_DMA_CHANNEL_NONE,

    .dmaChannelReceive = SYS_DMA_CHANNEL_NONE,

    /* Combined size of transmit and receive buffer objects */
    .bufferObjPoolSize = DRV_USART_QUEUE_SIZE_IDX2,

    /* USART transmit and receive buffer buffer objects pool */
    .bufferObjPool = (uintptr_t)&drvUSART2BufferObjPool[0],

    .interruptSources = &drvUSART2InterruptSources,

    .remapDataWidth = drvUsart2remapDataWidth,

    .remapParity = drvUsart2remapParity,

    .remapStopBits = drvUsart2remapStopBits,

    .remapError = drvUsart2remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 1 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART1ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX1];

/* USART transmit/receive transfer objects pool */
static DRV_USART_BUFFER_OBJ drvUSART1BufferObjPool[DRV_USART_QUEUE_SIZE_IDX1];

const DRV_USART_PLIB_INTERFACE drvUsart1PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG)UART1_ReadCallbackRegister,
    .read = (DRV_USART_PLIB_READ)UART1_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY)UART1_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET)UART1_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT)UART1_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG)UART1_WriteCallbackRegister,
    .write = (DRV_USART_PLIB_WRITE)UART1_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY)UART1_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET)UART1_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET)UART1_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP)UART1_SerialSetup
};

const uint32_t drvUsart1remapDataWidth[] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0, 0xFFFFFFFF };
const uint32_t drvUsart1remapParity[] = { 0x800, 0x0, 0x200, 0x600, 0x400, 0xFFFFFFFF };
const uint32_t drvUsart1remapStopBits[] = { 0x0, 0xFFFFFFFF, 0xFFFFFFFF };
const uint32_t drvUsart1remapError[] = { 0x20, 0x80, 0x40 };

const DRV_USART_INTERRUPT_SOURCES drvUSART1InterruptSources =
{
    /* Peripheral has single interrupt vector */
    .isSingleIntSrc                        = true,

    /* Peripheral interrupt line */
    .intSources.usartInterrupt             = UART1_IRQn,
};

const DRV_USART_INIT drvUsart1InitData =
{
    .usartPlib = &drvUsart1PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX1,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvUSART1ClientObjPool[0],

    .dmaChannelTransmit = SYS_DMA_CHANNEL_NONE,

    .dmaChannelReceive = SYS_DMA_CHANNEL_NONE,

    /* Combined size of transmit and receive buffer objects */
    .bufferObjPoolSize = DRV_USART_QUEUE_SIZE_IDX1,

    /* USART transmit and receive buffer buffer objects pool */
    .bufferObjPool = (uintptr_t)&drvUSART1BufferObjPool[0],

    .interruptSources = &drvUSART1InterruptSources,

    .remapDataWidth = drvUsart1remapDataWidth,

    .remapParity = drvUsart1remapParity,

    .remapStopBits = drvUsart1remapStopBits,

    .remapError = drvUsart1remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Instance 0 Initialization Data">

static DRV_USART_CLIENT_OBJ drvUSART0ClientObjPool[DRV_USART_CLIENTS_NUMBER_IDX0];

/* USART transmit/receive transfer objects pool */
static DRV_USART_BUFFER_OBJ drvUSART0BufferObjPool[DRV_USART_QUEUE_SIZE_IDX0];

const DRV_USART_PLIB_INTERFACE drvUsart0PlibAPI = {
    .readCallbackRegister = (DRV_USART_PLIB_READ_CALLBACK_REG)UART0_ReadCallbackRegister,
    .read = (DRV_USART_PLIB_READ)UART0_Read,
    .readIsBusy = (DRV_USART_PLIB_READ_IS_BUSY)UART0_ReadIsBusy,
    .readCountGet = (DRV_USART_PLIB_READ_COUNT_GET)UART0_ReadCountGet,
    .readAbort = (DRV_USART_PLIB_READ_ABORT)UART0_ReadAbort,
    .writeCallbackRegister = (DRV_USART_PLIB_WRITE_CALLBACK_REG)UART0_WriteCallbackRegister,
    .write = (DRV_USART_PLIB_WRITE)UART0_Write,
    .writeIsBusy = (DRV_USART_PLIB_WRITE_IS_BUSY)UART0_WriteIsBusy,
    .writeCountGet = (DRV_USART_PLIB_WRITE_COUNT_GET)UART0_WriteCountGet,
    .errorGet = (DRV_USART_PLIB_ERROR_GET)UART0_ErrorGet,
    .serialSetup = (DRV_USART_PLIB_SERIAL_SETUP)UART0_SerialSetup
};

const uint32_t drvUsart0remapDataWidth[] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0, 0xFFFFFFFF };
const uint32_t drvUsart0remapParity[] = { 0x800, 0x0, 0x200, 0x600, 0x400, 0xFFFFFFFF };
const uint32_t drvUsart0remapStopBits[] = { 0x0, 0xFFFFFFFF, 0xFFFFFFFF };
const uint32_t drvUsart0remapError[] = { 0x20, 0x80, 0x40 };

const DRV_USART_INTERRUPT_SOURCES drvUSART0InterruptSources =
{
    /* Peripheral has single interrupt vector */
    .isSingleIntSrc                        = true,

    /* Peripheral interrupt line */
    .intSources.usartInterrupt             = UART0_IRQn,
};

const DRV_USART_INIT drvUsart0InitData =
{
    .usartPlib = &drvUsart0PlibAPI,

    /* USART Number of clients */
    .numClients = DRV_USART_CLIENTS_NUMBER_IDX0,

    /* USART Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvUSART0ClientObjPool[0],

    .dmaChannelTransmit = SYS_DMA_CHANNEL_NONE,

    .dmaChannelReceive = SYS_DMA_CHANNEL_NONE,

    /* Combined size of transmit and receive buffer objects */
    .bufferObjPoolSize = DRV_USART_QUEUE_SIZE_IDX0,

    /* USART transmit and receive buffer buffer objects pool */
    .bufferObjPool = (uintptr_t)&drvUSART0BufferObjPool[0],

    .interruptSources = &drvUSART0InterruptSources,

    .remapDataWidth = drvUsart0remapDataWidth,

    .remapParity = drvUsart0remapParity,

    .remapStopBits = drvUsart0remapStopBits,

    .remapError = drvUsart0remapError,

    .dataWidth = DRV_USART_DATA_8_BIT,
};

// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_SDMMC Instance 0 Initialization Data">

/* SDMMC Client Objects Pool */
static DRV_SDMMC_CLIENT_OBJ drvSDMMC0ClientObjPool[DRV_SDMMC_CLIENTS_NUMBER_IDX0];

/* SDMMC Transfer Objects Pool */
static DRV_SDMMC_BUFFER_OBJ drvSDMMC0BufferObjPool[DRV_SDMMC_QUEUE_SIZE_IDX0];


const DRV_SDMMC_PLIB_API drvSDMMC0PlibAPI = {
    .sdhostCallbackRegister = (DRV_SDMMC_PLIB_CALLBACK_REGISTER)HSMCI_CallbackRegister,
    .sdhostInitModule = (DRV_SDMMC_PLIB_INIT_MODULE)HSMCI_ModuleInit,
    .sdhostSetClock  = (DRV_SDMMC_PLIB_SET_CLOCK)HSMCI_ClockSet,
    .sdhostIsCmdLineBusy = (DRV_SDMMC_PLIB_IS_CMD_LINE_BUSY)HSMCI_IsCmdLineBusy,
    .sdhostIsDatLineBusy = (DRV_SDMMC_PLIB_IS_DATA_LINE_BUSY)HSMCI_IsDatLineBusy,
    .sdhostSendCommand = (DRV_SDMMC_PLIB_SEND_COMMAND)HSMCI_CommandSend,
    .sdhostReadResponse = (DRV_SDMMC_PLIB_READ_RESPONSE)HSMCI_ResponseRead,
    .sdhostSetBlockCount = (DRV_SDMMC_PLIB_SET_BLOCK_COUNT)HSMCI_BlockCountSet,
    .sdhostSetBlockSize = (DRV_SDMMC_PLIB_SET_BLOCK_SIZE)HSMCI_BlockSizeSet,
    .sdhostSetBusWidth = (DRV_SDMMC_PLIB_SET_BUS_WIDTH)HSMCI_BusWidthSet,
    .sdhostSetSpeedMode = (DRV_SDMMC_PLIB_SET_SPEED_MODE)HSMCI_SpeedModeSet,
    .sdhostSetupDma = (DRV_SDMMC_PLIB_SETUP_DMA)HSMCI_DmaSetup,
    .sdhostGetCommandError = (DRV_SDMMC_PLIB_GET_COMMAND_ERROR)HSMCI_CommandErrorGet,
    .sdhostGetDataError = (DRV_SDMMC_PLIB_GET_DATA_ERROR)HSMCI_DataErrorGet,
    .sdhostClockEnable = (DRV_SDMMC_PLIB_CLOCK_ENABLE)NULL,
    .sdhostResetError = (DRV_SDMMC_PLIB_RESET_ERROR)NULL,
    .sdhostIsCardAttached = (DRV_SDMMC_PLIB_IS_CARD_ATTACHED)NULL,
    .sdhostIsWriteProtected = (DRV_SDMMC_PLIB_IS_WRITE_PROTECTED)NULL,
};

/*** SDMMC Driver Initialization Data ***/
const DRV_SDMMC_INIT drvSDMMC0InitData =
{
    .sdmmcPlib                      = &drvSDMMC0PlibAPI,
    .bufferObjPool                  = (uintptr_t)&drvSDMMC0BufferObjPool[0],
    .bufferObjPoolSize              = DRV_SDMMC_QUEUE_SIZE_IDX0,
    .clientObjPool                  = (uintptr_t)&drvSDMMC0ClientObjPool[0],
    .numClients                     = DRV_SDMMC_CLIENTS_NUMBER_IDX0,
    .protocol                       = DRV_SDMMC_PROTOCOL_SUPPORT_IDX0,
    .cardDetectionMethod            = DRV_SDMMC_CARD_DETECTION_METHOD_IDX0,
    .cardDetectionPollingIntervalMs = 100,
    .isWriteProtectCheckEnabled     = false,
    .speedMode                      = DRV_SDMMC_CONFIG_SPEED_MODE_IDX0,
    .busWidth                       = DRV_SDMMC_CONFIG_BUS_WIDTH_IDX0,
	.sleepWhenIdle 					= false,
    .isFsEnabled                    = true,
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************
/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="File System Initialization Data">

const SYS_FS_MEDIA_MOUNT_DATA sysfsMountTable[SYS_FS_VOLUME_NUMBER] =
{
    {
        .mountName = SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0,
        .devName   = SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0,
        .mediaType = SYS_FS_MEDIA_TYPE_IDX0,
        .fsType   = SYS_FS_TYPE_IDX0
    },
};


const SYS_FS_FUNCTIONS FatFsFunctions =
{
    .mount             = FATFS_mount,
    .unmount           = FATFS_unmount,
    .open              = FATFS_open,
    .read              = FATFS_read,
    .close             = FATFS_close,
    .seek              = FATFS_lseek,
    .fstat             = FATFS_stat,
    .getlabel          = FATFS_getlabel,
    .currWD            = FATFS_getcwd,
    .getstrn           = FATFS_gets,
    .openDir           = FATFS_opendir,
    .readDir           = FATFS_readdir,
    .closeDir          = FATFS_closedir,
    .chdir             = FATFS_chdir,
    .chdrive           = FATFS_chdrive,
    .write             = FATFS_write,
    .tell              = FATFS_tell,
    .eof               = FATFS_eof,
    .size              = FATFS_size,
    .mkdir             = FATFS_mkdir,
    .remove            = FATFS_unlink,
    .setlabel          = FATFS_setlabel,
    .truncate          = FATFS_truncate,
    .chmode            = FATFS_chmod,
    .chtime            = FATFS_utime,
    .rename            = FATFS_rename,
    .sync              = FATFS_sync,
    .putchr            = FATFS_putc,
    .putstrn           = FATFS_puts,
    .formattedprint    = FATFS_printf,
    .testerror         = FATFS_error,
    .formatDisk        = (FORMAT_DISK)FATFS_mkfs,
    .partitionDisk     = FATFS_fdisk,
    .getCluster        = FATFS_getclusters
};


const SYS_FS_REGISTRATION_TABLE sysFSInit [ SYS_FS_MAX_FILE_SYSTEM_TYPE ] =
{
    {
        .nativeFileSystemType = FAT,
        .nativeFileSystemFunctions = &FatFsFunctions
    }
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)SYSTICK_TimerCallbackSet,
    .timerStart = (SYS_TIME_PLIB_START)SYSTICK_TimerStart,
    .timerStop = (SYS_TIME_PLIB_STOP)SYSTICK_TimerStop,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)SYSTICK_TimerFrequencyGet,
    .timerInterruptEnable = (SYS_TIME_PLIB_INTERRUPT_ENABLE)SYSTICK_TimerInterruptEnable,
    .timerInterruptDisable = (SYS_TIME_PLIB_INTERRUPT_DISABLE)SYSTICK_TimerInterruptDisable,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET)SYSTICK_TimerPeriodSet,
};

const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = SysTick_IRQn,
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************



/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{
    /* MISRAC 2012 deviation block start */
    /* MISRA C-2012 Rule 2.2 deviated in this file.  Deviation record ID -  H3_MISRAC_2012_R_2_2_DR_1 */


    EFC_Initialize();
  
    CLOCK_Initialize();
	PIO_Initialize();



	RTC_Initialize();

  

 
    TC0_CH0_TimerInitialize(); 
     
    
	UART1_Initialize();

	UART0_Initialize();

	TWIHS0_Initialize();

    USART1_Initialize();

    USART0_Initialize();

    USART2_Initialize();

	SYSTICK_TimerInitialize();
    XDMAC_Initialize();

	RSWDT_REGS->RSWDT_MR = RSWDT_MR_WDDIS_Msk;	// Disable RSWDT 

	WDT_Initialize();
//    WDT_REGS->WDT_MR = WDT_MR_WDDIS_Msk;                 // Disable WDT 

    PWM0_Initialize();
    PWM1_Initialize();

    QSPI_Initialize();

	HSMCI_Initialize();


    sysObj.drvUsart4 = DRV_USART_Initialize(DRV_USART_INDEX_4, (SYS_MODULE_INIT *)&drvUsart4InitData);

    sysObj.drvUsart3 = DRV_USART_Initialize(DRV_USART_INDEX_3, (SYS_MODULE_INIT *)&drvUsart3InitData);

    sysObj.drvUsart2 = DRV_USART_Initialize(DRV_USART_INDEX_2, (SYS_MODULE_INIT *)&drvUsart2InitData);

    sysObj.drvUsart1 = DRV_USART_Initialize(DRV_USART_INDEX_1, (SYS_MODULE_INIT *)&drvUsart1InitData);

    sysObj.drvUsart0 = DRV_USART_Initialize(DRV_USART_INDEX_0, (SYS_MODULE_INIT *)&drvUsart0InitData);


    sysObj.drvSDMMC0 = DRV_SDMMC_Initialize(DRV_SDMMC_INDEX_0,(SYS_MODULE_INIT *)&drvSDMMC0InitData);


    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);

    /*** File System Service Initialization Code ***/
    SYS_FS_Initialize( (const void *) sysFSInit );


//    APP_Initialize();
    
   FSAPP_Initialize ();//SDCARD FILE SYSTEM INTIALIZE

    NVIC_Initialize();
    
	TWIHS2_Initialize();


}


/*******************************************************************************
 End of File
*/
