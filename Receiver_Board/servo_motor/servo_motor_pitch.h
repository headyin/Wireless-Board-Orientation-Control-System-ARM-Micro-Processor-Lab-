/**
  ******************************************************************************
  * @file    servo_motor_pitch.h
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    26-February-2014
  * @brief   This file provides functions to control servo motors
  */

#ifndef SERVO_MOTOR_PITCH_H
#define SERVO_MOTOR_PITCH_H

#include "stm32f4xx.h"
#include "cmsis_os.h"

/**
  * @brief  Initialize GPIOD pin15 as PWG output (TIM4 CH4)
  * @param  None
  * @retval None
  */
void servo_motor_init_pitch(void);

/**
  * @brief  Update the motor angle with the pitch angle of the chip
  * @param  int16_t pitchAngle, the roll angle of the chip range from 0 to 180
  * @retval None
  */
void servo_motor_update_pitch(float pitchAngle);
void servo_motor_pitch_Thread(void const * argument);
void servo_motor_pitch_Start(void);
osThreadId  servo_motor_pitch_Thread_Create(void);



#endif
