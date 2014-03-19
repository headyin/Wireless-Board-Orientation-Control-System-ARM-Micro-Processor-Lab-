#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "temperature/temperature.h"
#include "accelerometer/accelerometer.h"
#include "lcd16.h"

/*!
 @brief Program entry point
 */
int main (void) {
  //initlilization all component
  temperature_Thread_Create();
  accelerometer_Thread_Create();
  
  
  //start
  temperature_Start();
  accelerometer_start();
  
  //lcd_clear_command();
  //lcd_write_char('a');
  lcd_display_init();
  
  osDelay(osWaitForever);

}


