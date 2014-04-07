/**
  ******************************************************************************
  * @file    servo_motor.h
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    26-February-2014
  * @brief   This file provides functions to control servo motors
  */

#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "stm32f4xx.h"

void servo_motor_init(void);
void servo_motor_update_roll(int16_t rollAngle);
void servo_motor_update_pitch(int16_t pitchAngle);


#endif
