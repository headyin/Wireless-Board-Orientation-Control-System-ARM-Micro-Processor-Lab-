#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "cmsis_os.h"

/* Public functions ------------------------------------ */
void accelerometer_init(void);
float getPitch(void);
float getRoll(void);
void measure_accleration(void);
void accelerometer_Thread(void const * argument);
osThreadId  accelerometer_Thread_Create(void);


#endif
