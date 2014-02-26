#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "math.h"
#include "atan_LUT.h"
#include "accelerometer.h"
#include "it_handler.h"
#include "sseg_display.h"
#include "servo_motor.h"


int main()
{
    accelerometer_init();
	sseg_display_init();
    servo_motor_init();

  
  while (1)
  {
  }
	
}







