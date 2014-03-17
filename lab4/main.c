#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "temperature\temperature.h"


/*!
 @brief Program entry point
 */
int main (void) {
  //initlilization all component
  temperature_Thread_Create();
  
  
  //start
  temperature_Start();

  
  osDelay(osWaitForever);

}


