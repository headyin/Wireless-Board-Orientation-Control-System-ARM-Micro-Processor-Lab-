#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "accelerometer/servo_motor_pitch.h"

/*!
 @brief Program entry point
 */
int main (void) {
  //initlilization all component
	servo_motor_pitch_Thread_Create();

  //start

	servo_motor_pitch_Start();


  
  osDelay(osWaitForever);

}


