/**
  ******************************************************************************
  * @file    sseg_display.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to show a 3-digit number through 3 7-segment displays 
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "sseg_display.h"
#include "misc.h"

/* Private defines -------------------------------------------------------- */

/* Private Const------------------------------------------------------------------ */
/* a const table used for looking up the pins for differnt number to display*/
const uint16_t display_number[20] = 
    {DISPLAY_NUMBER_0, DISPLAY_NUMBER_1, DISPLAY_NUMBER_2, DISPLAY_NUMBER_3, DISPLAY_NUMBER_4,
		 DISPLAY_NUMBER_5, DISPLAY_NUMBER_6, DISPLAY_NUMBER_7, DISPLAY_NUMBER_8, DISPLAY_NUMBER_9,
		 DISPLAY_DOT | DISPLAY_NUMBER_0, DISPLAY_DOT | DISPLAY_NUMBER_1, DISPLAY_DOT | DISPLAY_NUMBER_2,
		 DISPLAY_DOT | DISPLAY_NUMBER_3, DISPLAY_DOT | DISPLAY_NUMBER_4, DISPLAY_DOT | DISPLAY_NUMBER_5,
		 DISPLAY_DOT | DISPLAY_NUMBER_6, DISPLAY_DOT | DISPLAY_NUMBER_7, DISPLAY_DOT | DISPLAY_NUMBER_8,
		 DISPLAY_DOT | DISPLAY_NUMBER_9};


/* Private variables ------------------------------------------------------------*/ 
/* Structure for GPIO init */
GPIO_InitTypeDef  Sseg_Display_GPIO_InitStructure;


/* Private Functions ------------------------------------------------------------*/

/**
  * @brief  implement a software delay
  * @param  None
  * @retval None
  */
void sseg_wait(uint32_t delay)
{
	while (delay > 0)
		delay --;
}

/**
  * @brief  initialize TIM3 channel 1 to generate an interrunt at a freqency of 150Hz
  * @param  None
  * @retval None
  */
void sseg_TIM_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t prescaler;
	
	/* TIM3 pheriph clock enable, TIM3 clock frequency = SystemCoreClock / 2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* Enable TIM3 NVIC interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/*
	 * TIM3 counter clock frequency = 30MHz
	 * Prescaler = (TIM3CLK / TIM3 counter clock frequency) - 1
	 */
	prescaler = (uint16_t) ((SystemCoreClock / 2) / 30000000) - 1;
	
	/* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	/* up-counting counter mode, counter counts from 0 to ARR and restarts from 0 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* Output Compare Timing Mode configuration: Channel1 */
	/* TIM_Pulse = 20000, TIM3 Interrupt frequency = TIM3 Counter Frequency / TIM_Pulse = 1500Hz*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 20000;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	
	/* Disable TIM3 Channel 1 preload*/
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	/* Enable TIM3_CH1 interrupt */
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	/* Enable TIM3 */
	TIM_Cmd(TIM3, ENABLE);
}

/* Exported Functions --------------------------------------------------------------*/
/**
  * @brief  Display the number on a specified 7-seg display
  * @param  display, specifies a 7-seg display, which could be DISPLAY_SELECT_x, x = 1, 2, 3 and their combination
	* @param  number, specifies the number to be displayed, range 0 - 19
	*         if number is (10 + x) (x is 0 to 9), then x will be displayed with dot
  * @retval None
  */
void sseg_display(uint16_t display, uint16_t number)
{
	if (number > 20) return;
	if (display > (DISPLAY_SELECT_1 | DISPLAY_SELECT_2 | DISPLAY_SELECT_3)) return;
	sseg_clear();
	GPIO_SetBits(GPIOB, display | display_number[number]);
}

/**
  * @brief  Check the correctness of the Seven Segement Display
  * @param  None
  * @retval None
  */
void sseg_display_check(void)
{
	uint16_t i;
	for (i = 0; i < 20; i++)
	{
		sseg_display(DISPLAY_SELECT_1 | DISPLAY_SELECT_2 | DISPLAY_SELECT_3, i);
		sseg_wait(0x7FFFFF);
		sseg_clear();
		sseg_wait(0xFFFFF);
	}
}

/**
  * @brief  clear the display
  * @param  None
  * @retval None
  */
void sseg_clear(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pins_For_Display);
}

/**
  * @brief  Initialize the Seven Segement Display
  * @param  None
  * @retval None
  */
void sseg_display_init(void)
{
	 /* GPIOB Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* Configure PB0,1,2,4,5,7,8,11,12,13,14 in output pushpull mode */
  Sseg_Display_GPIO_InitStructure.GPIO_Pin = GPIO_Pins_For_Display;
	/* output mode */
  Sseg_Display_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	/* Output type Push Pull */
  Sseg_Display_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* High speed */
  Sseg_Display_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/* Do not pull*/
  Sseg_Display_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &Sseg_Display_GPIO_InitStructure);
	
	/* enable TIM3_CH1 interrupt */
	sseg_TIM_init();
}
