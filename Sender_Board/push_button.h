#ifndef PUSH_BUTTON_H
#define PUSH_BUTTON_H
#include "stdint.h"

#define MODE_1 ((uint8_t)0)
#define MODE_2 ((uint8_t)1)

uint_fast8_t getThreadToRun(void);
void button_init(void);

void set_next_mode(uint8_t mode);

#endif
