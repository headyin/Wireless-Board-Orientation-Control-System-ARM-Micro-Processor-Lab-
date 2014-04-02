/**
  ******************************************************************************
  * @file    servo_motor_pitch.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    26-February-2014
  * @brief   This file provides functions to control servo motors
  */
#include "servo_motor_pitch.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "cmsis_os.h"
#include "stdint.h"
#include "stm32f4xx.h"
#include <math.h>

#define PULSE_DEGREE_SLOP -3.37f
#define NIGHTY_DEGREE_PULSE 450 + 15

//semaphore
osSemaphoreId servo_motor_pitch_semaphore;

//definations for timer, semaphore and thread
osSemaphoreDef(servo_motor_pitch_semaphore); 
osThreadDef(servo_motor_pitch_Thread, osPriorityNormal, 1, 0);
osThreadId servo_motor_pitch_thread_id;

TIM_OCInitTypeDef  TIM_OCInitStructure_;

/** 
  * @brief  This function handles TIM3 global interrupt request. 
  * @param  None 
  * @retval None 
  */
void TIM4_IRQHandler_Pitch(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
  {
    //add tempeature_semaphore by 1, so the temperature thead can measure the tempeature once
    osSemaphoreRelease (servo_motor_pitch_semaphore);
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC4); 
	}
}

/**
  * @brief  Initialize GPIOD pin14 used for PWM output
  * @param  None
  * @retval None
  */
void servo_motor_gpio_init_pitch(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  /* GPIOD clock enable*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  /* GPIOD Configuration: TIM4 CH4   (PD15) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
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
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
}

/**
  * @brief  Initialize TIM4 CH4 as PWM output
  * @param  None
  * @retval None
  */
void servo_motor_tim_init_pitch(void)
{
  /*
   * TIM4 counter clock frequency = 315000
   * Prescaler = (TIM3CLK / TIM3 counter clock frequency) - 1
   * with TIM_period = 2100, the PWM output frequency is
   * TIM4 counter clock frequency / TIM_period = 315000 / 6300 = 50Hz
   */
  //TIM4 base is already initialized in pwm_alarm 
	/* idenpendent PWM mode 1*/
	TIM_OCInitStructure_.TIM_OCMode = TIM_OCMode_PWM1;
	/* output compare state enabled, ebable the comparasion between counter and puls value*/
  TIM_OCInitStructure_.TIM_OutputState = TIM_OutputState_Enable;
	/* Output polarity high, when counter < puls value, set the output polarity high*/
  TIM_OCInitStructure_.TIM_OCPolarity = TIM_OCPolarity_High;
	/* Set CCR3 to 0 degree at initial*/
	TIM_OCInitStructure_.TIM_Pulse = NIGHTY_DEGREE_PULSE;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure_);

	/* Enable the prereload of CCR3 register, which controls the duty circly of PWM */
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	/* Enable the prereload of TIM4_ARR register, which defermines the frequency of PWM */
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);
}

/**
  * @brief  Initialize GPIOD pin0 as PWG output (TIM4 CH3)
  * @param  None
  * @retval None
  */
void servo_motor_init_pitch(void)
{
	servo_motor_gpio_init_pitch();
	servo_motor_tim_init_pitch();
}

/**
  * @brief  Update the motor angle with the roll angle of the chip
  * @param  int16_t rollAngle, the roll angle of the chip range from 0 to 180
  * @retval None
  */
void servo_motor_update_pitch(float pitchAngle)
{
  uint16_t pulse = NIGHTY_DEGREE_PULSE + round((pitchAngle) * PULSE_DEGREE_SLOP);
	TIM_OCInitStructure_.TIM_Pulse = pulse;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure_);

}
void servo_motor_pitch_Start(void)
{
  //start timer
  /* Enable TIM4 */
	TIM_Cmd(TIM4, ENABLE);
}
/**
  * @brief  measure the tempeature when the semaohore is ready.
  * @param  None
  * @retval None
  */
void servo_motor_pitch_Thread(void const * argument)
{
  
  while (1)
  {
		//Wait for semaphore to be released by timer interrupt
    osSemaphoreWait (servo_motor_pitch_semaphore, osWaitForever);

    float pitchAngle;
    pitchAngle = 45.0;
    servo_motor_update_pitch(pitchAngle);
  }
}

/**
  * @brief  create a thread for temperature measurement
  * @param  None
  * @retval The thread ID for temperature
  */
osThreadId  servo_motor_pitch_Thread_Create(void)
{
  //initialize ADC
	servo_motor_init_pitch();

  //create semaphore
  servo_motor_pitch_semaphore = osSemaphoreCreate(osSemaphore(servo_motor_pitch_semaphore), 1);

  //create temperature thread
  servo_motor_pitch_thread_id = osThreadCreate(osThread(servo_motor_pitch_Thread), NULL);
  return servo_motor_pitch_thread_id;
}
