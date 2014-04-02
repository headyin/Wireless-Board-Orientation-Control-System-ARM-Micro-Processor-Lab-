#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "temperature/temperature.h"
#include "accelerometer/accelerometer.h"
#include "lcd16.h"
#include "push_button.h"

/*!
 @brief Program entry point
 */
int main (void) {
  //initlilization all component
  temperature_Thread_Create();
  accelerometer_Thread_Create();
  button_init();
  lcd_thread_create();

  //start
  lcd_start();
  temperature_Start();
  accelerometer_start();


  
  osDelay(osWaitForever);

}


