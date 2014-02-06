#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

//#include "gpio_example.h"


int main()
{
	ADC_CommonInitTypeDef adc_common_init_s;
	ADC_InitTypeDef adc_init_s;
	uint16_t v_refint;
	uint16_t v_sensor;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); /* Enable the High Speed APB (APB2) peripheral clock for ADC1 */

	adc_common_init_s.ADC_Mode = ADC_Mode_Independent; /* All ADCs are independent */
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; /* Direct Memory Access mode disabled */
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2; /* Frequency of the clock to ADC is  PCLK2 / 2 */
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; /* Delay beteen two sampling phases, not used in independent mode */
	ADC_CommonInit(&adc_common_init_s);
	
	adc_init_s.ADC_Resolution = ADC_Resolution_12b; /* 12-bit, need 15 ADC clock cycle */
	adc_init_s.ADC_ScanConvMode = DISABLE; /* Single (one channel) mode disabled */
	adc_init_s.ADC_ContinuousConvMode = DISABLE; /* single conversion mode*/
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; /* trigger detection disabled */
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right; /* right data alignment: bit12-15 are 0  */
	adc_init_s.ADC_NbrOfConversion = 1; /* number of adc conversions */
	ADC_Init(ADC1, &adc_init_s);
	
	ADC_Cmd(ADC1, ENABLE); /* Enables ADC1 peripheral */
	ADC_TempSensorVrefintCmd(ENABLE); /* Enable the internal connection of Temperature sensor and Vrefint sources with the ADC channels */
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);
  ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	v_sensor = ADC_GetConversionValue(ADC1);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_480Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	v_refint = ADC_GetConversionValue(ADC1);

	printf("%d, %d, %f\n", v_sensor, v_refint, (1.21 / v_refint * v_sensor - 0.76) * 1000 / 2.5 + 25);

	
	while(1) {
		ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);
  	ADC_SoftwareStartConv(ADC1);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
		v_sensor = ADC_GetConversionValue(ADC1);
	
		ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_480Cycles);
		ADC_SoftwareStartConv(ADC1);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
		v_refint = ADC_GetConversionValue(ADC1);

		printf("%f\n", (1.21 / v_refint * v_sensor - 0.76) * 1000 / 2.5 + 25);
	}
}




