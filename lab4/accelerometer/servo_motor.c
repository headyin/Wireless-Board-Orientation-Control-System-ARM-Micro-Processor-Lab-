/** 
  ****************************************************************************** 
  * @file    servo_motor.c 
  * @author  Xinshang Yin, Chandani Patel 
  * @version V1.0.0 
  * @date    26-February-2014 
  * @brief   This file provides functions to control servo motors 
  */
  
#include "stm32f4xx_gpio.h" 
#include "stm32f4xx_rcc.h" 
#include "stm32f4xx_tim.h" 
#include "servo_motor.h" 
#include <math.h> 
  
#define PULSE_DEGREE_SLOP -3.37f 
#define NIGHTY_DEGREE_PULSE 450 + 15 
  
TIM_OCInitTypeDef  TIM_OCInitStructure; 
  
  
/** 
  * @brief  Initialize GPIOA pin1 used for PWM output 
  * @param  None 
  * @retval None 
  */
void servo_motor_gpio_init(void) 
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 
    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 
      
    /* GPIOA clock enable*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
      
    /* GPIOA Configuration: TIM5 CH4 (PA1) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
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
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); 
} 
  
/** 
  * @brief  Initialize TIM5 CH4 as PWM output 
  * @param  None 
  * @retval None 
  */
void servo_motor_tim_init(void) 
{ 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
    uint16_t prescaler, period; 
  
    /* 
     * TIM3 counter clock frequency = 315000 
     * Prescaler = (TIM3CLK / TIM3 counter clock frequency) - 1 
     * with TIM_period = 2100, the PWM output frequency is 
     * TIM3 counter clock frequency / TIM_period = 315000 / 6300 = 50Hz 
     */
    prescaler = (uint16_t) ((SystemCoreClock / 2) / 315000) - 1; 
    period = 6300; 
  
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = period; 
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler; 
    /* The ration between clock frequency and the sampleing frequency, not used */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    /* upcounting counter mode, counter counts from 0 to ARR and restarts from 0 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
      
    /* idenpendent PWM mode 1*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    /* output compare state enabled, ebable the comparasion between counter and puls value*/
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    /* Output polarity high, when counter < puls value, set the output polarity high*/
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    /* Set CCR3 to 0 degree at initial*/
    TIM_OCInitStructure.TIM_Pulse = NIGHTY_DEGREE_PULSE; 
    TIM_OC2Init(TIM5, &TIM_OCInitStructure); 
      
    /* Enable the prereload of CCR3 register, which controls the duty circly of PWM */
    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); 
    /* Enable the prereload of TIM5_ARR register, which defermines the frequency of PWM */
    TIM_ARRPreloadConfig(TIM5, ENABLE); 
    /* TIM5 enable counter */
    TIM_Cmd(TIM5, ENABLE); 
} 
  
/** 
  * @brief  Initialize GPIOA pin1 as PWG output (TIM5 CH4) 
  * @param  None 
  * @retval None 
  */
void servo_motor_init(void) 
{ 
    servo_motor_gpio_init(); 
    servo_motor_tim_init(); 
} 
  
/** 
  * @brief  Update the motor angle with the roll angle of the chip 
  * @param  int16_t rollAngle, the roll angle of the chip range from 0 to 180 
  * @retval None 
  */
void servo_motor_update(float rollAngle) 
{ 
    uint16_t pulse = NIGHTY_DEGREE_PULSE + round((rollAngle) * PULSE_DEGREE_SLOP); 
    TIM_OCInitStructure.TIM_Pulse = pulse; 
    TIM_OC2Init(TIM5, &TIM_OCInitStructure); 
  
} 