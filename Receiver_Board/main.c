#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "servo_motor/servo_motor.h"
#include "wirelessTransmitter.h"
/*!
 @brief Program entry point
 */
int main (void) {
  //initlilization all component
    
    servo_motor_init();
    wireless_Receiver_Thread_Create();

  osDelay(osWaitForever);

}


