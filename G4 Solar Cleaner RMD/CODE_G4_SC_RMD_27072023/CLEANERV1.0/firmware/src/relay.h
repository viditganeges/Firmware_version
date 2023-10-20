#ifndef __RELAY_H_
#define __RELAY_H_

#include <main.h>

#define RELAY_NO_PULSE_TIME_MS        5000

int relaycontrol;

bool RelayInit (void);
void RelayOnPulse (void);
void RelayOffPulse (void);
void RelayNoPulse (void);
void RelayTimeIncrement_ms (void);
void RelayFSM (void);

#endif