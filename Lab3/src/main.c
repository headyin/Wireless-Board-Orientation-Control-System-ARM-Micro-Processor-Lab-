#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "math.h"
#include "atan_LUT.h"
#include "accelerometer.h"
#include "it_handler.h"
#include "sseg_display.h"
#include "servo_motor.h"
#include "filter.h"
#include "stdio.h"


/* variables used for seven-seg display and angle*/
const uint16_t Display_Select[3] = {DISPLAY_SELECT_1, DISPLAY_SELECT_2, DISPLAY_SELECT_3};
uint16_t display_digit[3];
uint16_t displayNumber = 0;
float rollAngle;
int16_t degree_MA;

extern volatile uint_fast16_t acceleration_data_ready;
extern volatile uint_fast16_t display_ready;


int main()
{
    acceleration_data_ready = 0;
    display_ready = 0;
    
    accelerometer_init();
	sseg_display_init();
    servo_motor_init();

    while (1)
    {
        /* wait for an interrupt */
        while (!(acceleration_data_ready || display_ready));
        /* acceleromter interrupt */
        if (acceleration_data_ready)
        {
            measure_accleration();
            rollAngle = getRoll() + 90;
            degree_MA = filter_add(round(rollAngle));
            servo_motor_update(degree_MA);

           	/* digit to be displayed in seven-seg */
            display_digit[0] = (uint16_t) degree_MA / 100;
            display_digit[1] = (uint16_t) degree_MA / 10 % 10;
            /* display a dot as well */
            display_digit[2] = (uint16_t) degree_MA % 10 + 10;
            
            printf("%d\n", degree_MA);
            
            /* reset the flag */
            acceleration_data_ready = 0;
        }
        if (display_ready)
        {
            /*select one of the three displays each time */
            displayNumber = (displayNumber + 1) % 3;
            sseg_display(Display_Select[displayNumber], display_digit[displayNumber]);
            /* reset the flag */
            display_ready = 0;
        }
    }
}







