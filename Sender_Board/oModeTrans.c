#include "oModeTrans.h"
#include "wirelessTransmitter.h"
#include "push_button.h"
#include "string.h"
#include "cmsis_os.h"
#include "stdio.h"

#define MAX_INPUT_NUM 200
#define SIG_START 0x05

int32_t _rolls[MAX_INPUT_NUM];
int32_t _pitches[MAX_INPUT_NUM];
int32_t _deltas[MAX_INPUT_NUM];
uint32_t totalNum;

void oModeTransThread(void const *args);

osThreadId oModeTransThreadID;
osThreadDef(oModeTransThread, osPriorityNormal, 1, 0);

void send_block_angles(uint32_t n, int32_t* rolls, int32_t* pitches, int32_t* deltas)
{
  int i,j;
  for (i = 0; i < n; i++)
  {
    int32_t delay = deltas[i] * 1000;
    if (delay < 1) delay = 1000;
    osDelay(delay - 999);
    for (j = 0; j < 100; j++)
    {
      wireless_send(MODE_2, rolls[i], pitches[i]);
      osDelay(10);
    }
  }
}

void oModeTransThread(void const *args)
{
  while (1)
  {
    osSignalWait(SIG_START, osWaitForever);
    send_block_angles(totalNum, _rolls, _pitches, _deltas);
  }  
}

void req_tran_timed_angels(uint32_t n, int32_t* rolls, int32_t* pitches, int32_t* deltas)
{
  int i;
  if (n > MAX_INPUT_NUM) return;
  for (i = 0; i < n; i++)
  {
    _rolls[i] = rolls[i];
    _pitches[i] = pitches[i];
    _deltas[i] = deltas[i];
  }

  totalNum = n;
  osSignalSet(oModeTransThreadID, SIG_START);  
}

osThreadId oModeTrans_Thread_Create(void)
{
  oModeTransThreadID = osThreadCreate(osThread(oModeTransThread), NULL);
  return oModeTransThreadID;
}
