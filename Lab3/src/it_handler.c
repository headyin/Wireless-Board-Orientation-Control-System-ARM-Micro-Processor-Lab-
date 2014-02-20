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
#include "stm32f4_discovery_lis302dl.h"
#include "stdio.h"

/* Private variables */
int8_t x_acceleration;
int8_t y_acceleration;
int8_t z_acceleration;
uint8_t buffer;

/* Public functions ------------------------------------*/
/**
 * @brief  This function handles the EXTI0 interrupt
 * @param  None
 * @retval None
 */
void EXTI0_IRQHandler(void)
{
  LIS302DL_Read(&buffer, LIS302DL_OUT_X_ADDR, 1);
  x_acceleration = (int8_t) buffer;
  LIS302DL_Read(&buffer, LIS302DL_OUT_Y_ADDR, 1);
  y_acceleration = (int8_t) buffer;
  LIS302DL_Read(&buffer, LIS302DL_OUT_Z_ADDR, 1);
  z_acceleration = (int8_t) buffer;
  
  printf("%d, %d, %d\n", x_acceleration, y_acceleration, z_acceleration);
}


