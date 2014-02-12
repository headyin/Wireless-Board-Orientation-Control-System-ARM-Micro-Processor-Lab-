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
#include "pwm_alarm.h"
#include "stdint.h"
#include "stm32f4xx.h"


/* Private variables ------------------------------------------------------------*/ 
TIM_OCInitTypeDef  TIM_OCInitStructure;


/**
  * @brief  Initialize GPIOA pin0 as PWG output (TIM5 CH1)
  * @param  None
  * @retval None
  */
void pwm_alarm_init(void)
{
	alarm_gpio_init();
	alarm_tim_init();

}

void alarm_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM5 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

  /* GPIOA clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  /* GPIOC Configuration: TIM5 CH1 (PA0) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	/* GPIO Alternate function Mode */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	/* High speed */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/* Output type: push pull */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* Pull up */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  /* Connect TIM5 pins to AF2 */  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
}

void alarm_tim_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 100;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 50;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM5, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	/* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
	
	
}

