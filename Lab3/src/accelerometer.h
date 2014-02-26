#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "stm32f4_discovery_lis302dl.h"

/* Public functions ------------------------------------ */
void accelerometer_init(void);
float getPitch(void);
float getRoll(void);
void measure_accleration(void);

#endif
