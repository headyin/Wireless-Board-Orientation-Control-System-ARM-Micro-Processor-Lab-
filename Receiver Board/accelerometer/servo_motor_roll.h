/**
  ******************************************************************************
  * @file    servo_motor_roll.h
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    26-February-2014
  * @brief   This file provides functions to control servo motors
  */

#ifndef SERVO_MOTOR_ROLL_H
#define SERVO_MOTOR_ROLL_H

#include "stm32f4xx.h"
#include "cmsis_os.h"

/**
  * @brief  Initialize GPIOD pin14 as PWG output (TIM4 CH4)
  * @param  None
  * @retval None
  */
void servo_motor_init_roll(void);

/**
  * @brief  Update the motor angle with the roll angle of the chip
  * @param  int16_t rollAngle, the roll angle of the chip range from 0 to 180
  * @retval None
  */
void servo_motor_update_roll(float rollAngle);


#endif
