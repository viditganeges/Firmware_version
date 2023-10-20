#ifndef _ZIGBEE_H    /* Guard against multiple inclusion */
#define _ZIGBEE_H

#include "main.h"

#define DEFAULT_RX_MAC_ADDR             0
#define ZIGBEE_FRAME_START_DELIMITER    0x7E

enum ZIGBEE_FRAME_TYPE
{
    ATCOMMAND = 0X08,
    ATCOMMAND_QUEUEPARAMETERVALUE = 0x09,
    ZIGBEETRANSMITREQUEST = 0x10,
    EXPLICITADDRESSINGZIGBEECOMMANDFRAME = 0x11,
    REMOTECOMMANDREQUEST = 0x17,
    CREATESOURCEROUTE = 0x21,
    ATCOMMANDRESPONSE = 0x88,
    MODEMSTATUS = 0x8A,
    ZIGBEETRANSMITSTATUS = 0x8B,
    ZIGBEERECEIVEPACKET = 0x90,
    ZIGBEEEXPLICITRXINDICATOR = 0x91,
    ZIGBEEIODATASAMPLERXINDICATION = 0x92,
    XBEESENSORREADINDICATOR = 0x94,
    NODEIDENTICATIONINDICATOR = 0x95,
    REMOTECOMMANDRESPONSE = 0x97,
    EXTENDEDMODEMSTATUS = 0x98,
    OVERTHEAIRFIRMWAREUPDATESTATUS = 0xA0,
    ROUTERECORDINDICATOR = 0xA1,
    MANYTOONEROUTEREQUESTINDICATOR = 0xA3
};

enum ZIGBEE_COMMAND_ID
{
    SL = 0x534C,
    SH = 0x5348
};

typedef union 
{
uint64_t MAC_64;
uint16_t MAC_32[4];
uint8_t MAC_16[8];
}ZigbeeMAC;

ZigbeeMAC DeviceMAC;

bool zigbeeInitStatus;

uint8_t frameID;
uint8_t ZigbeeTxBuffer[256];

uint32_t ZigbeeTxSize;
uint32_t ZigbeeRxTimer;
typedef enum
{
    IDLESTATE,
    TRANSMITPACKET,
    TXRECEIVEWAIT,
    GENERALRXWAIT,
    ACK_RESPONSE
}ZState;

ZState ZigbeeState;

void ZigbeeInit(void);
uint8_t ZigbeeFrameID(void);
uint8_t zigbee_checksum(uint8_t *data,int length);

uint64_t GetZigbeeMacAddress (void);

bool zigbee_uartsend(uint8_t *data, int length);
bool ZigbeeTxPacket_Gateway(uint64_t macaddress, uint8_t *data, uint16_t length);
bool zigbee_get_parameter(enum ZIGBEE_COMMAND_ID cmdid);

void ZigbeeTx_TC(uint8_t *data, uint16_t length);
void zigbee_get_mac_L(void);
void GetZigbeeAPIMacAddress(void);
void zigbeeparse_SL(uint8_t *data);
void zigbeeparse_SH(uint8_t *data);
void Zigbeeparse_ATCommand(uint8_t *data);
void ZigbeeControl(void);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
