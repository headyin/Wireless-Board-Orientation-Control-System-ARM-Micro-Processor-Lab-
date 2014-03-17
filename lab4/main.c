#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "temperature/temperature.h"
#include "accelerometer/accelerometer.h"

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

  
  osDelay(osWaitForever);

}


