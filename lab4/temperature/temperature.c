/**
  ******************************************************************************
  * @file    temperature.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to get the temperatures from 
  *          the system built-in tempeature sensor.         
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stm32f4xx.h"
#include "temperature\temperature.h"
#include "cmsis_os.h"
#include "maFilter.h"
#include "math.h"

#include "stdio.h"

/* Private define ------------------------------------------------------------*/ 
/* Vref voltage in V */
#define VREFINT_VOLTAGE_V 1.21
/* temperature sensor voltage in V at 25 degree */
#define TEMPERATURE_V25 0.76
/* the average slope of voltage vs temperature of temperature sensor in mV/C */
#define AVG_SLOPE 2.5

/* Private variables ------------------------------------------------------------*/
//teimer id
osTimerId temperature_timer_id;
//semaphore
osSemaphoreId tempeature_semaphore;
//structure used for moving average filter
MA_Filter temperature_filter_struct;

/* Private functions -------------------------------------------------------------*/

void temperature_Timer_Callback(void const *arg)
{
  //add tempeature_semaphore by 1, so the temperature thead can measure the tempeature once
  osSemaphoreRelease (tempeature_semaphore);
}

//definations for timer, semaphore and thread
osTimerDef (tempeature_Timer, temperature_Timer_Callback); 
osSemaphoreDef(tempeature_semaphore); 
osThreadDef(temperature_Thread, osPriorityNormal, 1, 0);
//define a mutex
osMutexDef (temperatureFilterMutex);


/* Public Functions --------------------------------------------------------------*/
/**
  * @brief  initialize the necessary peripherals needed
  * @param  None
  * @retval None
  */
void temperature_Init(void)
{
  /* ADC common init structure */
  ADC_CommonInitTypeDef adc_common_init_s;
  /* ADC init structure */
  ADC_InitTypeDef adc_init_s;

  
  /* Enable the High Speed APB (APB2) peripheral clock for ADC1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* All ADCs are independent */
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	/* Direct Memory Access mode disabled */
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	/* Frequency of the clock to ADC is  PCLK2 / 2 */
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2;
	/* Delay beteen two sampling phases, not used in independent mode */
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&adc_common_init_s);

	/* 12-bit, need 15 ADC clock cycle */
	adc_init_s.ADC_Resolution = ADC_Resolution_12b;
	/* Single (one channel) mode */
	adc_init_s.ADC_ScanConvMode = DISABLE;
	/* single conversion mode*/
	adc_init_s.ADC_ContinuousConvMode = DISABLE;
	/* trigger detection disabled */
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	/* right data alignment: bit12-15 are 0  */
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right;
	/* number of adc conversions */
	adc_init_s.ADC_NbrOfConversion = 1; 
	ADC_Init(ADC1, &adc_init_s);

  /* Enables ADC1 peripheral */
	ADC_Cmd(ADC1, ENABLE);
	/* Enable the internal connection of Temperature sensor and Vrefint sources with the ADC channels */
	ADC_TempSensorVrefintCmd(ENABLE);
}

/**
  * @brief  start a periodic timer at 25Hz to start measuring the tempeature
  * @param  None
  * @retval None
  */
osStatus temperature_Start(void)
{
  //create semaphore
  tempeature_semaphore = osSemaphoreCreate(osSemaphore(tempeature_semaphore), 1);
  
  //create and start timer
  temperature_timer_id = osTimerCreate (osTimer(tempeature_Timer), osTimerPeriodic, NULL);
  if (temperature_timer_id != NULL)
  {
    //start timer with 40ms delay. 1000ms / 25Hz = 40
    return osTimerStart (temperature_timer_id, 40);
  }
  //cannot create timer
  return osErrorResource;
}

/**
  * @brief  Initializes the temperature sensor and its related ADC.
  * @param  None
  * @retval the float value of temperature measured in Celsius.
  */
float temperature_MeasureValue(void)
{	
	/* Raw value of temperature sensor voltage converted from ADC1_IN16 */
	uint16_t v_refint;
	/* Raw value of VREFINT converted from ADC1_INT17 */
	uint16_t v_sensor;
	
	/* select ADC1_IN16 to sample sensor voltage value*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_28Cycles);
	/* start one ADC conversion */
	ADC_SoftwareStartConv(ADC1);
	/*  wait unitl ECO bit is set, sample finished */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	/* Read the value from ADC_DR*/
	v_sensor = ADC_GetConversionValue(ADC1);

	/* select ADC1_IN16 to sample reference voltage value*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_28Cycles);
	/* start one ADC conversion */
	ADC_SoftwareStartConv(ADC1);
	/*  wait unitl ECO bit is set, sample finished */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	/* Read the value from ADC_DR*/
	v_refint = ADC_GetConversionValue(ADC1);

  /*
	 * measured_sensor_voltage = actual_reference_voltage * sampled_sensor_voltage / sampled_reference_voltage_value
	 * temperature = (measured_sensor_voltage - sensor_voltage_at_25) / AVG_SLOPE + 25
	 */
	return (VREFINT_VOLTAGE_V / v_refint * v_sensor - TEMPERATURE_V25) * 1000 / AVG_SLOPE + 25;
}



/**
  * @brief  measure the tempeature when the semaohore is ready.
  * @param  None
  * @retval None
  */
void temperature_Thread(void const * argument)
{
  float temp;
  int16_t sample;
  temperature_Init();
  filter_init(&temperature_filter_struct, 20);
  temperature_filter_struct.mutexId = osMutexCreate(osMutex (temperatureFilterMutex));
  
  while (1)
  {
    osSemaphoreWait (tempeature_semaphore, osWaitForever);
    temp = temperature_MeasureValue();
    filter_add((int16_t) round(temp * 100), &temperature_filter_struct);
    sample = filter_average(&temperature_filter_struct);
    printf("%f\n", sample / 100.0);
  }
}

/**
  * @brief  create a thread for temperature measurement
  * @param  None
  * @retval None
  */
osThreadId  temperature_Thread_Create(void)
{
  //start temperature thread
  return osThreadCreate(osThread(temperature_Thread), NULL);
}

