#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "temperature.h"
#include "filter.h"
#include "pwm_alarm.h"
#include "sseg_display.h"
#include "stm32f4xx.h"

/* temperature sample frequency 25Hz*/
#define TEMPERATURE_SAMPLE_FREQUENCY 25
/* every 25Hz, the display is refreshed 6 / 3 = 2 times, display refresh frequency 50Hz */
#define LOOPS_PER_PERIOD 6
/* The display is updated every 5 temperature measurements */
#define TEMPERATURE_DISPLAY_UPDATE_RATE 5


/* define const variables */
/* the three 7-seg displays */
const uint16_t Display_Digit[3] = {DISPLAY_SELECT_1, DISPLAY_SELECT_2, DISPLAY_SELECT_3};

/* define static variables*/
static volatile uint_fast16_t tick_flag;
float temp;
uint32_t sysTick_Config_Ret;
int16_t sample, moving_average;
uint16_t loops, tempCount, digit[3];


int main()
{
	
	/* initialize temperature measurement */
	temperature_Init();
	/* initialize filter */
	void filter_init();
	/* init alarm */
	pwm_alarm_init();
	/* init the 7-seg display */
	sseg_display_init();
	/* run a quick check for all the displays */
	sseg_display_check();
	
	/* reset counters */
	loops = 0;
	tempCount = 0;
	
	/* configure sys  tick */
	/* reset system tick flag*/
	tick_flag = 0;
	/* Number of ticks between two interrupts */
	sysTick_Config_Ret = SysTick_Config(SystemCoreClock / TEMPERATURE_SAMPLE_FREQUENCY / LOOPS_PER_PERIOD);
	/* return value 0 means function succeeded */
	assert(sysTick_Config_Ret == 0);
	
  while(1) {
		/* refresh display */
		sseg_display(Display_Digit[loops % 3], digit[loops % 3]);
		/* wait for a system timer interrupt */
		while(!tick_flag);		
		/* reset system tick flag*/
		tick_flag = 0;
		/* update counters */
		loops = (loops + 1) % LOOPS_PER_PERIOD;
		if (loops == 0)
		{
			tempCount = (tempCount + 1) % TEMPERATURE_DISPLAY_UPDATE_RATE;
			/* measure temperature */
			temp = temperature_MeasureValue();
			sample = (int16_t) round(temp * 100);
			moving_average = filter_add(sample);
			if (moving_average < 0)
			{
				moving_average = 0;
			}
			pwm_alarm_update(moving_average / 100);
			if (tempCount == 0)
			{
				/* update digits, +10 means the digit is with dot*/
				digit[0] = (moving_average / 1000) % 10;
				digit[1] = 10 + (moving_average / 100) % 10;
				digit[2] = (moving_average / 10) % 10;
			}

		}
		/* clear display */
		sseg_clear();

		printf("%fC\n", moving_average / 100.00);
	}
}

/**
 * @brief Interrupt handler for system tick
 */
void SysTick_Handler(){
	/* set tick flag */
	tick_flag = 1;
}



