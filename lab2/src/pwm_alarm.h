#ifndef PWM_ALARM_H
#define PWM_ALARM_H

#include "stdint.h"

void alarm_tim_init(void);
void alarm_gpio_init(void);
void pwm_alarm_init(void);
void pwm_alarm_update(int16_t temp);

#endif
