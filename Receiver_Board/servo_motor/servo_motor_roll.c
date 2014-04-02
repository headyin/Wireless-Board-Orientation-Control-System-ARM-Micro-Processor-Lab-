/**
  ******************************************************************************
  * @file    servo_motor_roll.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    26-February-2014
  * @brief   This file provides functions to control servo motors
  */
#include "servo_motor_roll.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include <math.h>

#define PULSE_DEGREE_SLOP -3.37f
#define NIGHTY_DEGREE_PULSE 450 + 15

//semaphore
osSemaphoreId servo_motor_roll_semaphore;

//definations for timer, semaphore and thread
osSemaphoreDef(servo_motor_roll_semaphore); 
osThreadDef(servo_motor_roll_Thread, osPriorityNormal, 1, 0);
osThreadId servo_motor_roll_thread_id;

TIM_OCInitTypeDef  TIM_OCInitStructure;

/** 
  * @brief  This function handles TIM3 global interrupt request. 
  * @param  None 
  * @retval None 
  */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
    //add servo_motor_roll_semaphore by 1
    osSemaphoreRelease (servo_motor_roll_semaphore);
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); 
	}
}


/**
  * @brief  Initialize GPIOC pin6 used for PWM output
  * @param  None
  * @retval None
  */
void servo_motor_gpio_init_roll(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  /* GPIOC clock enable*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  /* GPIOC Configuration: TIM3 CH1   (PC6) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  /* GPIO Alternate function Mode */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	/* High speed */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/* Output type: push pull */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* Pull up */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* Connect TIM3 pins to AF2 */  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
}

/**
  * @brief  Initialize TIM3 CH1 as PWM output
  * @param  None
  * @retval None
  */
void servo_motor_tim_init_roll(void)
{
  /*
   * TIM3 counter clock frequency = 315000
   * Prescaler = (TIM3CLK / TIM3 counter clock frequency) - 1
   * with TIM_period = 2100, the PWM output frequency is
   * TIM3 counter clock frequency / TIM_period = 315000 / 6300 = 50Hz
   */
  //TIM3 base is already initialized in pwm_alarm 
	/* idenpendent PWM mode 1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	/* output compare state enabled, ebable the comparasion between counter and puls value*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/* Output polarity high, when counter < puls value, set the output polarity high*/
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/* Set CCR3 to 0 degree at initial*/
	TIM_OCInitStructure.TIM_Pulse = NIGHTY_DEGREE_PULSE;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	/* Enable the prereload of CCR3 register, which controls the duty circly of PWM */
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	/* Enable the prereload of TIM3_ARR register, which defermines the frequency of PWM */
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/* TIM3 enable counter */
	//TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief  Initialize GPIOC pin6 as PWG output (TIM3 CH1)
  * @param  None
  * @retval None
  */
void servo_motor_init_roll(void)
{
	servo_motor_gpio_init_roll();
	servo_motor_tim_init_roll();
}

/**
  * @brief  Update the motor angle with the roll angle of the chip
  * @param  int16_t rollAngle, the roll angle of the chip range from 0 to 180
  * @retval None
  */
void servo_motor_update_roll(float rollAngle)
{
  uint16_t pulse = NIGHTY_DEGREE_PULSE + round((rollAngle) * PULSE_DEGREE_SLOP);
	TIM_OCInitStructure.TIM_Pulse = pulse;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

}

void servo_motor_roll_Start(void)
{
  //start timer
  /* Enable TIM3 */
	TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief  measure the tempeature when the semaohore is ready.
  * @param  None
  * @retval None
  */
void servo_motor_roll_Thread(void const * argument)
{
  
  while (1)
  {
		//Wait for semaphore to be released by timer interrupt
    osSemaphoreWait (servo_motor_roll_semaphore, osWaitForever);

    float rollAngle =45.0;
   
    servo_motor_update_roll(rollAngle);
  }
}

/**
  * @brief  create a thread for temperature measurement
  * @param  None
  * @retval The thread ID for temperature
  */
osThreadId  servo_motor_roll_Thread_Create(void)
{
  //initialize ADC
	servo_motor_init_roll();

  //create semaphore
  servo_motor_roll_semaphore = osSemaphoreCreate(osSemaphore(servo_motor_roll_semaphore), 1);

  //create servo motor thread
  servo_motor_roll_thread_id = osThreadCreate(osThread(servo_motor_roll_Thread), NULL);
  return servo_motor_roll_thread_id;
}
