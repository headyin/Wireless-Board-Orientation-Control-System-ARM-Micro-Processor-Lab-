/**
  ******************************************************************************
  * @file    filter.h
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to get the moving average filter    
	*          Note: this filter is using a mutex, so it is thread safe
  */  

#ifndef FILTER_H
#define FILTER_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "cmsis_os.h"


/* Public constants --------------------------------------------------------*/
/* Filter max depth size, range from 1 - 65536 */
#define FILTER_MAX_SIZE 40

/* Public types ------------------------------------------------------------*/
/**
 * @breif Moving average buffer structure
 */
typedef struct{
	/* Buffer array to store values */
	int16_t buffer[FILTER_MAX_SIZE];
	/* the sum of all buffer values */
	int32_t sum;
	/* the average of all buffer values */
	int16_t average;
	/* The index to keep track of the new measurement placement */
	uint16_t index;
  /* The depth (size) of the moving average filter, range from 1 - FILTER_MAX_SIZE */
  uint16_t depth;
  /* the mutex used to lock this resource*/
  osMutexId mutexId;
}MA_Filter;		

/* Exported functions --------------------------------------------------------*/  
void filter_init(MA_Filter* filter_struct, uint16_t depth);

void filter_add(int16_t sample, MA_Filter* filter_struct);

int16_t filter_average(MA_Filter* filter_struct);

int32_t filter_sum(MA_Filter* filter_struct);

#endif
