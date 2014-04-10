#include "arm_math.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "accelerometer.h"
#include "smartRF_CC2500.h"
#include "wirelessTransmitter.h"
#include "push_button.h"
#include "lcd16.h"
#include "keypad.h"

#include "stdio.h"



/*!
 @brief Program entry point
 */
int main (void)
{
  //for receiving
  //wireless_Receiver_Thread_Create();


  //for sending:
  button_init();
  CC2500_Default_Init();
  accelerometer_Thread_Create();
  lcd_thread_create();
  keypad_Thread_Create();

  accelerometer_start();
  lcd_start();

  osDelay(osWaitForever);
}

