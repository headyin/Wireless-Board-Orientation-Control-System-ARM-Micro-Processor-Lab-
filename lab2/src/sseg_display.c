/**
  ******************************************************************************
  * @file    sseg_display.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to show a 3-digit number through 3 7-segment displays 
  */ 
	
/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "sseg_display.h"
#include "stm32f4xx.h"

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
	assert(number < 20);
	assert(display <= (DISPLAY_SELECT_1 | DISPLAY_SELECT_2 | DISPLAY_SELECT_3));
	assert(display > 0);
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
}
