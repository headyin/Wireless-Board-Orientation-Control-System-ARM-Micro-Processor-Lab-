/**
  ******************************************************************************
  * @file    sseg_display.h
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to show a 3-digit number through 3 7-segement displays 
  */ 

#ifndef SSEG_DISPLAY_H
#define SSEG_DISPLAY_H
#include "stm32f4xx_gpio.h"

/* Includes ------------------------------------------------------------------*/

/* Exported defines -------------------------------------------------------- */
/* GPIO Pin 0, 1, 2, 4, 5, 7, 8, 11, 12, 13, 14 */
#define GPIO_Pins_For_Display (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14)

/* GPIO Pin used for select the first 7-segment display */
#define DISPLAY_SELECT_1 GPIO_Pin_0
/* GPIO Pin used for select the second 7-segment display */
#define DISPLAY_SELECT_2 GPIO_Pin_1
/* GPIO Pin used for select the third 7-segment display */
#define DISPLAY_SELECT_3 GPIO_Pin_2

/* define the GPIO pin used for each segment */
#define GPIO_PIN_SEG_1 GPIO_Pin_4
#define GPIO_PIN_SEG_2 GPIO_Pin_5
#define GPIO_PIN_SEG_3 GPIO_Pin_7
#define GPIO_PIN_SEG_4 GPIO_Pin_8
#define GPIO_PIN_SEG_5 GPIO_Pin_11
#define GPIO_PIN_SEG_6 GPIO_Pin_12
#define GPIO_PIN_SEG_7 GPIO_Pin_13
#define GPIO_PIN_SEG_DOT GPIO_Pin_14

/* define the Segments needed for each number */
#define DISPLAY_NUMBER_0 GPIO_PIN_SEG_1 | GPIO_PIN_SEG_2 | GPIO_PIN_SEG_3 | GPIO_PIN_SEG_5 | GPIO_PIN_SEG_6 | GPIO_PIN_SEG_7
#define DISPLAY_NUMBER_1 GPIO_PIN_SEG_1 | GPIO_PIN_SEG_5
#define DISPLAY_NUMBER_2 GPIO_PIN_SEG_2 | GPIO_PIN_SEG_3 | GPIO_PIN_SEG_4 | GPIO_PIN_SEG_5 | GPIO_PIN_SEG_7
#define DISPLAY_NUMBER_3 GPIO_PIN_SEG_2 | GPIO_PIN_SEG_3 | GPIO_PIN_SEG_4 | GPIO_PIN_SEG_6 | GPIO_PIN_SEG_7
#define DISPLAY_NUMBER_4 GPIO_PIN_SEG_1 | GPIO_PIN_SEG_3 | GPIO_PIN_SEG_4 | GPIO_PIN_SEG_6
#define DISPLAY_NUMBER_5 GPIO_PIN_SEG_1 | GPIO_PIN_SEG_2 | GPIO_PIN_SEG_4 | GPIO_PIN_SEG_6 | GPIO_PIN_SEG_7
#define DISPLAY_NUMBER_6 GPIO_PIN_SEG_1 | GPIO_PIN_SEG_2 | GPIO_PIN_SEG_4 | GPIO_PIN_SEG_5 | GPIO_PIN_SEG_6 | GPIO_PIN_SEG_7
#define DISPLAY_NUMBER_7 GPIO_PIN_SEG_2 | GPIO_PIN_SEG_3 | GPIO_PIN_SEG_6
#define DISPLAY_NUMBER_8 GPIO_PIN_SEG_1 | GPIO_PIN_SEG_2 | GPIO_PIN_SEG_3 | GPIO_PIN_SEG_4 | GPIO_PIN_SEG_5 | GPIO_PIN_SEG_6 | GPIO_PIN_SEG_7
#define DISPLAY_NUMBER_9 GPIO_PIN_SEG_1 | GPIO_PIN_SEG_2 | GPIO_PIN_SEG_3 | GPIO_PIN_SEG_4 | GPIO_PIN_SEG_6 | GPIO_PIN_SEG_7
#define DISPLAY_DOT GPIO_Pin_14


/* Exported functions --------------------------------------------------------------*/
void sseg_display(uint16_t display, uint16_t number);
void sseg_display_init(void);
void sseg_display_check(void);
void sseg_clear(void);

#endif
