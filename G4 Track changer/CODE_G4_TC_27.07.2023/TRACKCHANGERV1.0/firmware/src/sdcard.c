/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************


#include "sdcard.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define SDCARD_MOUNT_NAME    SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0
#define SDCARD_DEV_NAME      SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0
#define SDCARD_FILE_NAME     "ganges.txt"
#define SDCARD_DIR_NAME      "Dir1"

#define APP_DATA_LEN         512

APP_DATA_FS  FSappData;
APP_DATA_FS  FSappData_;

/* Application data buffer */
uint8_t  dataBuffer[APP_DATA_LEN];

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void APP_SysFSEventHandler(SYS_FS_EVENT event,void* eventData,uintptr_t context)
{
    switch(event)
    {
        /* If the event is mount then check if SDCARD media has been mounted */
        case SYS_FS_EVENT_MOUNT:
            if(strcmp((const char *)eventData, SDCARD_MOUNT_NAME) == 0)
            {
                FSappData.sdCardMountFlag = true;
                SD_MOUNT_STATUS = 1;
                GPIO_PC7_LED4_Set();
                SDcardmountcheckError = false;
            }
            break;

        /* If the event is unmount then check if SDCARD media has been unmount */
        case SYS_FS_EVENT_UNMOUNT:
            if(strcmp((const char *)eventData, SDCARD_MOUNT_NAME) == 0)
            {
                FSappData.sdCardMountFlag = false;

                FSappData.fs_state = APP_MOUNT_WAIT;
                SD_MOUNT_STATUS = 0;
                GPIO_PC7_LED4_Clear();
                SDcardmountchecktimer = 0;
            } 

            break;

        case SYS_FS_EVENT_ERROR:
        default:
            break;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void FSAPP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    FSappData.fs_state = APP_MOUNT_WAIT;

    /* Register the File System Event handler */
    SYS_FS_EventHandlerSet((void const*)APP_SysFSEventHandler,(uintptr_t)NULL);
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks (SYS_FS_FILE_OPEN_ATTRIBUTES mode, int offset)
{
    static int retry_count = 0;

    SYS_FS_Tasks();

   DRV_SDMMC_Tasks(sysObj.drvSDMMC0);
    /* Check the application's current state. */
    switch ( FSappData.fs_state )
    {
        case APP_MOUNT_WAIT:
            /* Wait for SDCARD to be Auto Mounted */
            if(FSappData.sdCardMountFlag == true)
            {
                retry_count = 0;
                FSappData.fs_state = APP_SET_CURRENT_DRIVE;
            }
            else
            {
                if(retry_count == 200)
                {
                    retry_count = 0;
                    FSappData.fs_state = APP_IDLE;
                }
                else
                {
                    retry_count++;
                    SYSTICK_DelayMs(1);
                }
            }
            break;

        case APP_SET_CURRENT_DRIVE:
            if(SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_FAILURE)
            {
                /* Error while setting current drive */
                FSappData.fs_state = APP_ERROR;
            }
            else
            {
                if(mode == SYS_FS_FILE_DELETE)
                {
                    FSappData.fs_state = APP_DELETE_FILE;
                }
                else
                {
                    /* Open a file for reading. */
                    FSappData.fs_state = APP_OPEN_FIRST_FILE;
                }
            }

            break;

        case APP_OPEN_FIRST_FILE:
            //open file in the specified mode
            if(mode == SYS_FS_FILE_CREATE)
            {
                FSappData.fileHandle = SYS_FS_FileOpen(sd_Filename, SYS_FS_FILE_OPEN_WRITE);
            
                if(FSappData.fileHandle == SYS_FS_HANDLE_INVALID)
                {
                    /* Could not open the file. Error out*/
                    FSappData.fs_state = APP_ERROR;
                    break;
                }
                
                FSappData.fs_state = APP_CLOSE_FILE;
            }
            else
            {
                FSappData.fileHandle = SYS_FS_FileOpen(sd_Filename, mode);
            
                if(FSappData.fileHandle == SYS_FS_HANDLE_INVALID)
                {
                    /* Could not open the file. Error out*/
                    FSappData.fs_state = APP_ERROR;
                    break;
                }

                FSappData.fs_state = APP_READ_WRITE_TO_FILE;
            }
            
            break;

        case APP_CREATE_DIRECTORY:
            /* Delete the files under Dir1 directory (if any) */
            SYS_FS_FileDirectoryRemove(SDCARD_DIR_NAME"/"SDCARD_FILE_NAME);

            /* Delete the Dir1 directory if it exists */
            SYS_FS_FileDirectoryRemove(SDCARD_DIR_NAME);

            if(SYS_FS_DirectoryMake(SDCARD_DIR_NAME) == SYS_FS_RES_FAILURE)
            {
                /* Error while setting current drive */
                FSappData.fs_state = APP_ERROR;
            }
            else
            {
                /* Open a second file for writing. */
                FSappData.fs_state = APP_OPEN_SECOND_FILE;
            }
            break;

        case APP_OPEN_SECOND_FILE:
            /* Open a second file inside "Dir1" */
            FSappData.fileHandle1 = SYS_FS_FileOpen(SDCARD_DIR_NAME"/"SDCARD_FILE_NAME, mode);

            if(FSappData.fileHandle1 == SYS_FS_HANDLE_INVALID)
            {
                /* Could not open the file. Error out*/
                FSappData.fs_state = APP_ERROR;
            }
            else
            {
                /* Read from one file and write to another file */
                FSappData.fs_state = APP_READ_WRITE_TO_FILE;
            }
           

        case APP_READ_WRITE_TO_FILE:
            if(OPERATION == WRITE_OPERATION)
            {
                /* Read from one file and write to another file */
                FSappData.fs_state = APP_WRITE_TO_FILE;
            }
            else
            {
                /* Read from one file and write to another file */
                FSappData.fs_state = APP_READ_TO_FILE;
            }
            break;
            
        case APP_WRITE_TO_FILE:
                if(SYS_FS_FileWrite(FSappData.fileHandle, (const void *)sd_buffer, sd_size) == -1)
                {
                    /* Write was not successful. Close the file
                     * and error out.*/
                    SYS_FS_FileClose(FSappData.fileHandle);
                    FSappData.fs_state = APP_ERROR;
                }
                else if(SYS_FS_FileEOF(FSappData.fileHandle) == 1)    /* Test for end of file */
                {
                    /* Continue the read and write process, until the end of file is reached */

                    FSappData.fs_state = APP_CLOSE_FILE;
                }
                break;
                
        case APP_READ_TO_FILE:
            
             FSappData.nBytesRead = SYS_FS_FileRead(FSappData.fileHandle, (void *)dataBuffer, sd_size);
//            
            if (FSappData.nBytesRead == -1)
            {
                /* There was an error while reading the file.
                 * Close the file and error out. */

                SYS_FS_FileClose(FSappData.fileHandle);
                FSappData.fs_state = APP_ERROR;
            }
            else if(SYS_FS_FileEOF(FSappData.fileHandle) == 1)    /* Test for end of file */
             {
                    /* Continue the read and write process, until the end of file is reached */

                    FSappData.fs_state = APP_CLOSE_FILE;
             }
                
                break;
                
        case APP_CLOSE_FILE:
            /* Close both files */
            SYS_FS_FileClose(FSappData.fileHandle);
            SYS_FS_FileClose(FSappData.fileHandle1);

            /* The test was successful. Lets idle. */
            FSappData.fs_state = APP_IDLE;
            break;
            
        case APP_DELETE_FILE:
            /*delete the file*/
            SYS_FS_FileDirectoryRemove(sd_Filename);
            
            /* The test was successful. Lets idle. */
            FSappData.fs_state = APP_IDLE;
            break;

        case APP_IDLE:
            /* The application comes here when the demo has completed successfully.*/
//            LED_ON();
            break;

        case APP_ERROR:
            /* The application comes here when the demo has failed. */
            FSappData.fs_state = APP_IDLE;
            break;

        default:
            break;
    }
}

void SD_Card_Write( const char *filename,uint8_t  *Data_Buffer, size_t size)  
{
   strcpy( (char *)sd_buffer,(char *)Data_Buffer);
   sd_size=size;
   OPERATION=WRITE_OPERATION;
   strcpy( (char *)sd_Filename,(char *)filename);
   
   if(FSappData.fs_state == APP_IDLE)
   {
       FSappData.fs_state = APP_MOUNT_WAIT;
   }
    
    while(FSappData.fs_state != APP_IDLE)        
    {
        /* Maintain Device Drivers */
        APP_Tasks(SYS_FS_FILE_OPEN_WRITE,0);
    } 
}

void SD_Card_Append( const char *filename,uint8_t  *Data_Buffer, size_t size)  
{
    strcpy( (char *)sd_buffer,(char *)Data_Buffer);
    sd_size=size;
    OPERATION=WRITE_OPERATION;
    strcpy( (char *)sd_Filename,(char *)filename);
    
    if(FSappData.fs_state == APP_IDLE)
   {
       FSappData.fs_state = APP_MOUNT_WAIT;
   }
   
    while(FSappData.fs_state != APP_IDLE)        
    {
        /* Maintain Device Drivers */
        APP_Tasks(SYS_FS_FILE_OPEN_APPEND,0);
    } 
}

void SD_Card_Read( const char *filename,uint8_t *Data_Buffer, size_t size, int offset)  
{

    sd_size=size;
    OPERATION=READ_OPERATION;
    strcpy( (char *)sd_Filename,(char *)filename);
    
    if(FSappData.fs_state == APP_IDLE)
   {
       FSappData.fs_state = APP_MOUNT_WAIT;
   }

    while(FSappData.fs_state != APP_IDLE)        
    {
        /* Maintain Device Drivers */
         APP_Tasks(SYS_FS_FILE_OPEN_READ, offset);
    }

    memcpy(Data_Buffer,dataBuffer,size);
}

void SD_Card_Delete( const char *filename)  
{
   strcpy( (char *)sd_Filename,(char *)filename);
    
   if(FSappData.fs_state == APP_IDLE)
   {
       FSappData.fs_state = APP_MOUNT_WAIT;
   }

    while(FSappData.fs_state != APP_IDLE)        
    {
        /* Maintain Device Drivers */
        APP_Tasks(SYS_FS_FILE_DELETE,0);
    }
}

void SD_Card_Create( const char *filename)  
{
    strcpy( (char *)sd_Filename,(char *)filename);
    
    if(FSappData.fs_state == APP_IDLE)
   {
       FSappData.fs_state = APP_MOUNT_WAIT;
   }

    while(FSappData.fs_state != APP_IDLE)        
    {
        /* Maintain Device Drivers */
        APP_Tasks(SYS_FS_FILE_CREATE,0);
    }
}

void SD_Card_Open(const char *filename)
{
    strcpy( (char *)sd_Filename,(char *)filename);
    
    if(FSappData.fs_state == APP_IDLE)
    {
       FSappData.fs_state = APP_MOUNT_WAIT;
    }
    
    while(FSappData.fs_state != APP_IDLE)        
    {
        /* Maintain Device Drivers */
        APP_Tasks(SYS_FS_FILE_OPEN, 0);
    }
}


//bool sdcard_mount_status(void)
//{
//    if(FSappData.sdCardMountFlag == true)
//    {
//        return TRUE;
//    }
//    else
//    {
//        return FALSE;
//    }
//}

//bool AppMountWait(void)
//{
//    return sdcard_mount_status();
//}
//
//bool AppSetCurrentDrive(void)
//{
//    
//}
/*******************************************************************************
 End of File
 */

//--------------------------------------------- extra code added RR -----------------------

bool File_open(const char *filename, SYS_FS_FILE_OPEN_ATTRIBUTES mode)
{
    if(SD_MOUNT_STATUS > 1)
    {
        if(SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_FAILURE)
        {
            return false;
        }
        
        FSappData_.fileHandle = SYS_FS_FileOpen(filename, mode);
        if(FSappData_.fileHandle == SYS_FS_HANDLE_INVALID)
        {
            return false;
        }
        
        return true;
    }
    
    return false;
}

bool File_readline(char *data)
{   
    if(SYS_FS_FileStringGet(FSappData_.fileHandle, data, 1000) == SYS_FS_RES_FAILURE)
    {
        return false;
    }
    
    return true;
}

void File_close(void)
{
    SYS_FS_FileClose(FSappData_.fileHandle);
}