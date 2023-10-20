#include "main.h"

void pointer_check(uint8_t log_type)
{
    int next_pointer = 0;
    
    int val = log_pointer_read(log_type);
    
    if(log_type == CYLCE_LOG)
    {
        next_pointer = val+1;
        if(next_pointer >= (((CYCLE_LOG_END_ADDR - CYCLE_LOG_START_ADDR)*SECTOR_SIZE)/(PAGE_SIZE/PAGE_SPLIT)))
        {
            next_pointer = 0;
        }
    }
    else if(log_type == ERROR_LOG)
    {
        next_pointer = val+1;
        if(next_pointer >= (((ERROR_LOG_END_ADDR - ERROR_LOG_START_ADDR)*SECTOR_SIZE)/(PAGE_SIZE/PAGE_SPLIT)))
        {
            next_pointer = 0;
        }
    }
    
    log_pointer_write(next_pointer,log_type);
}

int pointer_inc(int val, uint8_t log_type)
{
    int next_pointer = 0;
    
    if(log_type == CYLCE_LOG)
    {
        next_pointer = val+1;
        if(next_pointer >= (((CYCLE_LOG_END_ADDR - CYCLE_LOG_START_ADDR)*SECTOR_SIZE)/(PAGE_SIZE/PAGE_SPLIT)))
        {
            next_pointer = 0;
        }
    }
    else if(log_type == ERROR_LOG)
    {
        next_pointer = val+1;
        if(next_pointer >= (((ERROR_LOG_END_ADDR - ERROR_LOG_START_ADDR)*SECTOR_SIZE)/(PAGE_SIZE/PAGE_SPLIT)))
        {
            next_pointer = 0;
        }
    }
    
    return next_pointer;
}

uint32_t pointercount_to_addr(uint32_t pointer_count, uint8_t log_type)
{
    if(log_type == CYLCE_LOG)
    {
        return ((pointer_count*(PAGE_SIZE/PAGE_SPLIT))+(CYCLE_LOG_START_ADDR*SECTOR_SIZE));
    }
    else 
    {
        return ((pointer_count*(PAGE_SIZE/PAGE_SPLIT))+(ERROR_LOG_START_ADDR*SECTOR_SIZE));
    }
}

int log_pointer_read(uint8_t log_type)
{
    char pointer_count_string[10];
    memset(pointer_count_string, 0, sizeof(pointer_count_string));
    
    if(log_type == CYLCE_LOG)
    {
        Read_SST_26_VF_016B_Data(CYCLE_LOG_ADDR, 10, (uint8_t*)pointer_count_string);
    }
    else if(log_type == ERROR_LOG)
    {
        Read_SST_26_VF_016B_Data(ERROR_LOG_ADDR, 10, (uint8_t*)pointer_count_string);
    }
    
    return atoi(pointer_count_string);
}

void log_pointer_write(int pointer_count, uint8_t log_type)
{
    char pointer_count_string[10];
    memset(pointer_count_string, 0, sizeof(pointer_count_string));
    
    itoa(pointer_count, pointer_count_string, 10);
    
    if(log_type == CYLCE_LOG)
    {
        Write_SST_26_VF_016B_Data(CYCLE_LOG_ADDR, (uint8_t*)pointer_count_string, strlen(pointer_count_string)+1);
    }
    else if(log_type == ERROR_LOG)
    {
        Write_SST_26_VF_016B_Data(ERROR_LOG_ADDR, (uint8_t*)pointer_count_string, strlen(pointer_count_string)+1);
    }
}

void Flash_write_log(char *data, uint8_t log_type)
{
    int current_pointer_count = log_pointer_read(log_type);
    int current_pointer_addr = pointercount_to_addr(current_pointer_count, log_type);
    
    if((current_pointer_addr%SECTOR_SIZE) == 0)
    {
        APP_SectorErase(current_pointer_addr);
    }
    
    SYSTICK_DelayMs(50);
    
    APP_CS_ENABLE();
    APP_PageProgram_size(current_pointer_addr, (uint8_t*)data, strlen(data)+1);
    APP_CS_DISABLE();
    
    log_pointer_write(pointer_inc(current_pointer_count, log_type), log_type);
}

void Flash_read_log(int read_pointer_count, char *data, uint8_t log_type)
{
    int read_pointer_addr = pointercount_to_addr(read_pointer_count, log_type);
    
    APP_CS_ENABLE();
    APP_MemoryRead(read_pointer_addr, (uint8_t*)data, (PAGE_SIZE/PAGE_SPLIT), false);
    APP_CS_DISABLE();
}

void Flash_readall_log(uint8_t log_type)
{
    int current_pointer_count = log_pointer_read(log_type);
    int dummy_pointer_count = pointer_inc(current_pointer_count,log_type);
    char read_buff[PAGE_SIZE/PAGE_SPLIT];    
    
    memset(logReadArray, 0, sizeof(logReadArray));
    
    while(dummy_pointer_count != current_pointer_count)
    {
        memset(read_buff, 0, sizeof(read_buff));
        Flash_read_log(dummy_pointer_count, read_buff, log_type);
        
        dummy_pointer_count = pointer_inc(dummy_pointer_count, log_type);
        
        if(strlen(read_buff)<(PAGE_SIZE/PAGE_SPLIT))
        {
            strcat(logReadArray,read_buff);
            UART0_Write((void *)read_buff, strlen(read_buff));
            while(UART0_WriteIsBusy());
        }
    }
}

void Flash_clearlog(uint8_t log_type)
{
    int sector_count = 0;
    uint32_t sector_addr = 0;
    
    if(log_type == CYLCE_LOG)
    {
        sector_count = CYCLE_LOG_END_ADDR-CYCLE_LOG_START_ADDR;
    }
    else if(log_type == ERROR_LOG)
    {
        sector_count = ERROR_LOG_END_ADDR-ERROR_LOG_START_ADDR;
    }
    
    for(int n=0;n<sector_count;n++)
    {
        sector_addr = pointercount_to_addr(n*(SECTOR_SIZE/(PAGE_SIZE/PAGE_SPLIT)), log_type);
        APP_SectorErase(sector_addr);
        while(SST26_IsBusy());
    }
}

void flash_ptr_check(void)
{
    pointer_check(CYLCE_LOG);
    pointer_check(ERROR_LOG);
}