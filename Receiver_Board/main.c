#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "servo_motor/servo_motor_pitch.h"
#include "servo_motor/servo_motor_roll.h"

/*!
 @brief Program entry point
 */
int main (void) {
  //initlilization all component
	servo_motor_pitch_Thread_Create();
servo_motor_roll_Thread_Create();
  //start

	servo_motor_pitch_Start();
	servo_motor_roll_Start();


  
  osDelay(osWaitForever);

}


