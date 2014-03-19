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
  //temperature_Thread_Create();
  //accelerometer_Thread_Create();
  lcd_gpio_init();
  
  lcd_display_init();
  lcd_display_angles(90.5231, 179.1234);
  //lcd_display_temperaure(26.89);
  //start
  //temperature_Start();
  //accelerometer_start();


  
  osDelay(osWaitForever);

}


