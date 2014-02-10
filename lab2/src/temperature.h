/**
  ******************************************************************************
  * @file    temperature.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to get the temperatures from 
  *          the system built-in tempeature sensor.         
  */ 
	
/* Exported functions --------------------------------------------------------*/  

#ifndef TEMPERATURE_H
#define TEMPERAUTRE_H

void temperature_Init(void);
float temperature_MeasureValue(void);

#endif
