/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _CONFIGUARTJSON_H    /* Guard against multiple inclusion */
#define _CONFIGUARTJSON_H


//#include "json.h"
//#include "apiprocess.h"

#include "main.h"


//eJsonStatus ProcessReceivedJsonData_config(char * jsonReceivedData);
//eJsonStatus ConvertHexAndProcess(char * jsonString,uint16_t* size);
bool ParseHeaderDataJson(char * headerBuffer, uint16_t * headerBufferLength, uint16_t command,uint64_t MACaddr);


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
