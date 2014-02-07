#include <stdio.h>
#include <assert.h>
#include "temperature.h"
#include "stm32f4xx.h"

/* temperature sample frequency 25Hz*/
#define TEMPERATURE_SAMPLE_FREQUENCY 25

static volatile uint_fast16_t tick_flag;


int main()
{
	float temp;
	uint32_t sysTick_Config_Ret;
	
	/* initialize temperature measurement */
	temperature_Init();
	/* reset system tick flag*/
	tick_flag = 0;
	/* Number of ticks between two interrupts */
	sysTick_Config_Ret = SysTick_Config(SystemCoreClock / TEMPERATURE_SAMPLE_FREQUENCY);
	/* return value 0 means function succeeded */
	assert(sysTick_Config_Ret == 0);
	
	while(1) {

		/* wait for a system timer interrupt */
		while(!tick_flag);		
		/* reset system tick flag*/
		tick_flag = 0;				
		/* measure temperature */
		temp = temperature_MeasureValue();
		printf("The temperature is %fC.\n", temp);
	}
}

/**
 * @brief Interrupt handler for system tick
 */
void SysTick_Handler(){
	/* set tick flag */
	tick_flag = 1;
}



