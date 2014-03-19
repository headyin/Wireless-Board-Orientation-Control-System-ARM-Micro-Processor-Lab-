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
#define ALARMM_UPPER_BOUND 35
#define ALARM_LOWER_BOUND 35
#define MAX_LED_BRIGHTNESS 6300


/* Private variables ------------------------------------------------------------*/ 
TIM_OCInitTypeDef  alarm_TIM_OCInitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
GPIO_InitTypeDef GPIO_InitStructure;
uint8_t alarmStatus;
uint16_t led_brightness;


/**
  * @brief  Initialize GPIOD pin0 as PWG output (TIM4 CH3)
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
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  uint16_t prescaler, period;
  /*
  * TIM4 counter clock frequency = 315000
  * Prescaler = (TIM4CLK / TIM3 counter clock frequency) - 1
  * with TIM_period = 2100, the PWM output frequency is
  * TIM4 counter clock frequency / TIM_period = 315000 / 6300 = 50Hz
  */
	prescaler = (uint16_t) ((SystemCoreClock / 2) / 315000) - 1;
  period = MAX_LED_BRIGHTNESS;
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  /* The ration between clock frequency and the sampleing frequency, not used */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	/* upcounting counter mode, counter counts from 0 to ARR and restarts from 0 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* idenpendent PWM mode 1*/
	alarm_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	/* output compare state enabled, ebable the comparasion between counter and puls value*/
  alarm_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/* Output polarity high, when counter < puls value, set the output polarity high*/
  alarm_TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/* Set CCR3 to 0 degree at initial*/
	alarm_TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM4, &alarm_TIM_OCInitStructure);
	
	/* Enable the prereload of CCR3 register, which controls the duty circly of PWM */
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	/* Enable the prereload of TIM4_ARR register, which defermines the frequency of PWM */
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);
}

/**
  * @brief  Update the alarm with the temperature value
  * @param  int16_t temp, the value of temperatuer
  * @retval None
  */
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
		led_brightness = (led_brightness + 90) % MAX_LED_BRIGHTNESS;
	} else {
		led_brightness = 0;
	}
	alarm_TIM_OCInitStructure.TIM_Pulse = led_brightness;
	TIM_OC3Init(TIM4, &alarm_TIM_OCInitStructure);

}

