#ifndef WIRELESS_TRANSMITTER_H
#define WIRELESS_TRANSMITTER_H

#include "cmsis_os.h"

#define WT_PACKET_RECEIVED_SIG    0x00000001

osThreadId  wireless_Receiver_Thread_Create(void);
void wireless_send(uint8_t mode, float rollAngle, float pitchAngle);



#endif
