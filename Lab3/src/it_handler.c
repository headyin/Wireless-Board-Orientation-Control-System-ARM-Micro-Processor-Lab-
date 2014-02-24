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
#include "stm32f4xx_exti.h"
#include "atan_LUT.h"
#include "math.h"
#include "stdio.h"

#define PI 3.1415926f

/* Private constants --------------------------------------*/
/* 10000 times of the calibration matrix */
int16_t calibration[4][3] = {185, 5  ,  3 ,
                              0 , 179,  0,
                              0 , 2  , 185,
                              0 , 357, -62};

/* Private variables -------------------------------------*/
int8_t x_acceleration;
int8_t y_acceleration;
int8_t z_acceleration;
uint8_t buffer;
float rollAngle;
float acceleration[3];

/*
//used for calibration
int16_t n = 0;
int16_t sumx = 0;
int16_t sumy = 0;
int16_t sumz = 0;
*/

/* Private functions ----------------------------------- */
void calibrate()
{
	acceleration[0] = (float) (calibration[0][0]*x_acceleration+calibration[0][1]*y_acceleration+calibration[0][2]*z_acceleration+calibration[3][0]) / 10000;
	acceleration[1] = (float) (calibration[1][0]*x_acceleration+calibration[1][1]*y_acceleration+calibration[1][2]*z_acceleration+calibration[3][1]) / 10000;
	acceleration[2] = (float) (calibration[2][0]*x_acceleration+calibration[2][1]*y_acceleration+calibration[2][2]*z_acceleration+calibration[3][2]) / 10000;
}

float getPitch()
{
	return  atan_table(acceleration[0]/ sqrt((acceleration[1]*acceleration[1])+(acceleration[2]*acceleration[2])));
}

float getRoll()
{
	return  atan_table(acceleration[1]/ sqrt((acceleration[0]*acceleration[0])+(acceleration[2]*acceleration[2])));
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
  LIS302DL_Read(&buffer, LIS302DL_OUT_X_ADDR, 1);
  x_acceleration = (int8_t) buffer;
  LIS302DL_Read(&buffer, LIS302DL_OUT_Y_ADDR, 1);
  y_acceleration = (int8_t) buffer;
  LIS302DL_Read(&buffer, LIS302DL_OUT_Z_ADDR, 1);
  z_acceleration = (int8_t) buffer;
  
	/* used for calibration
	n++;
	if (n > 100 && n <= 600)
	{
		sumx += x_acceleration;
		sumy += y_acceleration;
		sumz += z_acceleration;
	}
	if (n == 600)
	{
		printf("%d, %d, %d\n", sumx / 500, sumy / 500, sumz / 500);
		n = 0;
	}*/
	
	calibrate();
	rollAngle = getRoll();
  //printf("%f, %f %f\n", acceleration[0], acceleration[1], acceleration[2]);
  printf("%f\n", rollAngle);
  
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


