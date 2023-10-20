
#ifndef _BMS_H    /* Guard against multiple inclusion */
#define _BMS_H

bool GetChargingOrNot(void);

void Bmsoperate(void);
void BmstransmitStopTimeIncrement_ms (void);

uint32_t hextodec(uint8_t *hex, uint8_t len);

#endif /* _EXAMPLE_FILE_NAME_H */


