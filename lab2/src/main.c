#include <stdio.h>
#include "temperature.h"

//#include "gpio_example.h"


int main()
{
	float temp;
	
	temperature_Init();
	while(1) {
		int i;
		for (i = 0; i < 65535; i++);
		temp = temperature_MeasureValue();
		printf("The temperature is %fC.\n", temp);
	}
}




