/**
 ******************************************************************************
 * @file    accelerometer.c
 * @author  Xinshang Yin, Chandani Patel
 * @version V1.0.0
 * @date    20-February-2014
 * @brief   This file provides all the interrupt handler routines used in Lab3     
 */

/* Includes ----------------------------------------------*/
#include "it_handler.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_tim.h"

/* Private constants --------------------------------------*/


/* Private variables -------------------------------------*/

/* flag used to indicate accelerastion sensor interrupt */
volatile uint_fast16_t acceleration_data_ready;
/* flag used to indicate display interrupt */
volatile uint_fast16_t display_ready;


/* Public functions ------------------------------------*/
/**
 * @brief  This function handles the EXTI0 interrupt
 * @param  None
 * @retval None
 */
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) == RESET) return;
    acceleration_data_ready = 1;
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
}

/**
 * @brief Interrupt handler for system tick
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
}

/** 
  * @brief  This function handles TIM3 global interrupt request. 
  * @param  None 
  * @retval None 
  */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
        display_ready = 1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); 
	}

}
