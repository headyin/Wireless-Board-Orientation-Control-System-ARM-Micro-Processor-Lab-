#ifndef O_MODE_TRANS_H
#define O_MODE_TRANS_H

#include "stdint.h"
#include "cmsis_os.h"

void req_tran_timed_angels(uint32_t n, int32_t* rolls, int32_t* pitches, int32_t* deltas);
osThreadId oModeTrans_Thread_Create(void);

#endif
