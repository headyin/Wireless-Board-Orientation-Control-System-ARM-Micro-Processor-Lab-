/**
  ******************************************************************************
  * @file    temperature.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to get the temperatures from 
  *          the system built-in tempeature sensor.         
  */ 
	

#ifndef TEMPERATURE_H
#define TEMPERAUTRE_H

#include "cmsis_os.h"

/* Exported functions --------------------------------------------------------*/ 

float get_filterd_tempeature(void);

/**
  * @brief  initialize the necessary peripherals needed
  * @param  None
  * @retval None
  */
void temperature_Init(void);

/*!
 @brief do one tempeature ADC conversion and return measured temperature value
 @return temperature value in float
 */
float temperature_MeasureValue(void);

/**
  * @brief  start a periodic timer at 25Hz to start measuring the tempeature
  * @param  None
  * @retval None
  */
void temperature_Start(void);

/**
  * @brief  create a thread for temperature measurement
  * @param  None
  * @retval None
  */
osThreadId  temperature_Thread_Create(void);

/**
  * @brief  measure the tempeature when the semaohore is ready.
  * @param  None
  * @retval None
  */
void temperature_Thread(void const * argument);

#endif
