/**
  ******************************************************************************
  * @file    pwm_alarm.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to generate an alarm through a LED 
  *          by changing its light with PWM.         
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "pwm_alarm.h"

/* Private defines -------------------------------------------------------- */
#define ALARMM_UPPER_BOUND 55
#define ALARM_LOWER_BOUND 55
#define MAX_LED_BRIGHTNESS 2100


/* Private variables ------------------------------------------------------------*/ 
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
GPIO_InitTypeDef GPIO_InitStructure;
uint8_t alarmStatus;
uint16_t led_brightness;


/**
  * @brief  Initialize GPIOA pin0 as PWG output (TIM5 CH1)
  * @param  None
  * @retval None
  */
void pwm_alarm_init(void)
{
	alarm_gpio_init();
	alarm_tim_init();
	alarmStatus = 0;
	led_brightness = 0;

}

void alarm_gpio_init(void)
{
  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOD clock enable using PCLK1 = 42Mhz */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  /* GPIOC Configuration: TIM4 CH3 (PD14) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	/* GPIO Alternate function Mode */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	/* High speed */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/* Output type: push pull */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* Pull up */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 

  /* Connect TIM4 pins to AF2 */  
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
}

void alarm_tim_init(void)
{
	/* Since our main loop has a frequency of 150Hz, which means the alarm status will be updated in 150Hz               */
	/* In oder to make PWM have enough peroids running between every two status updates to have effect on the birghtness */
	/* We want to have at leaset 100 PWM periods between every two status updates. So the output clock frequency is > 150 * 100 = 15KHz */
	/* Here we choose output clock frequency to be 20KHZ, the counter clock frequency 42MHz, so ARR = 42M / 20K = 2100 */
	/* And the prescaler is 168M / 2 / 42M  - 1 = 1   */
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = MAX_LED_BRIGHTNESS;
  TIM_TimeBaseStructure.TIM_Prescaler = 1;
	/* The ration between clock frequency and the sampleing frequency, not used */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	/* upcounting counter mode, counter counts from 0 to ARR and restarts from 0 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* idenpendent PWM mode 1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	/* output compare state enabled, ebable the comparasion between counter and puls value*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/* Output polarity high, when counter < puls value, set the output polarity high*/
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/* Set CCR3 to 0 at initial, so the alarm is off */
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	
	/* Enable the prereload of CCR3 register, which controls the duty circly of PWM */
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	/* Enable the prereload of TIM4_ARR register, which defermines the frequency of PWM */
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);


}

void pwm_alarm_update(int16_t temp)
{
	/* If alarm is off, but temperature is greater than ALARMM_UPPER_BOUND */
	/* then the alarm should be on */
	if ((alarmStatus == 0) && (temp >= ALARMM_UPPER_BOUND))
	{
		/* alarm on */
		alarmStatus = 1;
	}
	/* If the alarm is on and temperature is less than ALARM_LOWER_BOUND */
	/* then the alarm should be off */
	else if ((alarmStatus == 1) && (temp < ALARM_LOWER_BOUND))
	{
		/* alarm off */
		alarmStatus = 0;
	}
	/* start led with alarm on*/
	if (alarmStatus ==1)
	{
		led_brightness = (led_brightness + 30) % MAX_LED_BRIGHTNESS;
	} else {
		led_brightness = 0;
	}
	TIM_OCInitStructure.TIM_Pulse = led_brightness;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);

}

