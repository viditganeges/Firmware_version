#ifndef _EXTERNALRTC_H    /* Guard against multiple inclusion */
#define _EXTERNALRTC_H

#define DS1307_SLAVEID    0x68
//bool external_rtc_oscillator_get_state(void);
//void external_rtc_oscillator_set_state(bool state);
//void external_rtc_battery_set_state(bool state);
//void external_rtc_init(void);
bool external_rtc_write(struct tm * Time);
void external_rtc_read(struct tm * Time);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
