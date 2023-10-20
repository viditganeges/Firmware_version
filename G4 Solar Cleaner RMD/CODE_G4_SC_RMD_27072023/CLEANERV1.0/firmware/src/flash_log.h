#ifndef _FLASH_LOG_H    /* Guard against multiple inclusion */
#define _FLASH_LOG_H

#include "main.h"

#define CYCLE_LOG_ADDR      510
#define ERROR_LOG_ADDR      511

#define CYCLE_LOG_START_ADDR      100
#define ERROR_LOG_START_ADDR      200

#define CYCLE_LOG_END_ADDR      CYCLE_LOG_START_ADDR+3
#define ERROR_LOG_END_ADDR      ERROR_LOG_START_ADDR+3

#define CYLCE_LOG           0
#define ERROR_LOG           1

#define PAGE_SPLIT          2
#define PAGE_SIZE           256

#define SECTOR_SIZE         4096

void pointer_check(uint8_t log_type);
int pointer_inc(int val, uint8_t log_type);
uint32_t pointercount_to_addr(uint32_t pointer_count, uint8_t log_type);

int log_pointer_read(uint8_t log_type);
void log_pointer_write(int addr, uint8_t log_type);

void Flash_write_log(char *data, uint8_t log_type);
void Flash_read_log(int read_pointer_count, char *data, uint8_t log_type);
void Flash_readall_log(uint8_t log_type);

void Flash_clearlog(uint8_t log_type);

void flash_ptr_check(void);


#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
