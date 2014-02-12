#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "temperature.h"
#include "filter.h"
#include "pwm_alarm.h"
#include "stm32f4xx.h"

/* temperature sample frequency 25Hz*/
#define TEMPERATURE_SAMPLE_FREQUENCY 25

static volatile uint_fast16_t tick_flag;


int main()
{
	float temp;
	uint32_t sysTick_Config_Ret;
	int16_t sample, moving_average;
	
	/* initialize temperature measurement */
	temperature_Init();
	/* initialize filter */
	void filter_init();
	/* reset system tick flag*/
	tick_flag = 0;
	/* Number of ticks between two interrupts */
	sysTick_Config_Ret = SysTick_Config(SystemCoreClock / TEMPERATURE_SAMPLE_FREQUENCY);
	/* return value 0 means function succeeded */
	assert(sysTick_Config_Ret == 0);
	/* init alarm */
	pwm_alarm_init();
	
	
	while(1) {
		
		/* wait for a system timer interrupt */
		while(!tick_flag);		
		/* reset system tick flag*/
		tick_flag = 0;				
		/* measure temperature */
		temp = temperature_MeasureValue();
		sample = (int16_t) round(temp * 100);
		moving_average = filter_add(sample);
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



