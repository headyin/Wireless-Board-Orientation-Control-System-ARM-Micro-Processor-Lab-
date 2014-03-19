/**
 ******************************************************************************
 * @file    push_button.c
 * @author  Xinshang Yin, Chandani Patel
 * @version V1.0.0
 * @date    17-March-2014
 * @brief   This file provides functions to configure the push button
 */

/* Includes ------------------------------------------------------------------*/
#include "push_button.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

/* flag used to indicate accelerastion sensor interrupt */
volatile uint_fast16_t thread_ready;


uint_fast16_t getThreadToRun(void)
{
  return thread_ready;
}

/**
 * @brief  Enable GPIOA port 0 and connect it EXTI0 
 *         and set EXTI0 interrupt to highest prioroty
 * @param  None
 * @retval None
 */
void button_init(void)
{
  thread_ready = 0;
	/*structures used for the initialization of related GPIOs (GPIOE) and EXTI */
	GPIO_InitTypeDef   GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Connect EXTI Line0 to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
  
  /* Configure EXTI Line0 */
  /* PE0 is mapped to EXTI0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  /* Interupt mode */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  /* Rrigger interrupt on the rising edge*/
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* Enable EXTI Line0 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set EXTI Line0 Interrupt and the priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  /* Group Priority 0 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  /* Sub-priority 0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  /* Enable NVIC_IRQChannel */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/* Public functions ------------------------------------*/
/**
 * @brief  This function handles the EXTI0 interrupt
 * @param  None
 * @retval None
 */
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) == RESET) return;
    thread_ready = 1 - thread_ready; 
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
}
