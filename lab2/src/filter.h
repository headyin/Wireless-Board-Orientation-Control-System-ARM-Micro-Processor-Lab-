/**
  ******************************************************************************
  * @file    filter.h
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to get the moving average filter    
	*          Note: this filter is not thread safe
  */  

#ifndef FILTER_H
#define FILTER_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported functions --------------------------------------------------------*/  
void filter_init(void);
int16_t filter_add(int16_t sample);
int16_t filter_average(void);
int32_t filter_sum(void);

#endif
