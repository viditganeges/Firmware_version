
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWM1_CONFIG_H
#define __PWM1_CONFIG_H

#define SERIAL_DEBUG_PWM1

#define STOP_PWM_VALUE          0

#if (HARDWARE_VERSION == HVER_1_0)
#define PWM1_FTM_BASEADDR       FTM2                       //FTM2
#else
#define PWM1_FTM_BASEADDR       FTM1
#endif

#define PWM1_CHANNEL1           0U
#define PWM1_CHANNEL2           7U

#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)


#endif