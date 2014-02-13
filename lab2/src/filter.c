/**
  ******************************************************************************
  * @file    filter.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to get the moving average filter    
	*          Note: this filter is not thread safe
  */  
 
/* Includes ------------------------------------------------------------------*/ 
#include "filter.h"
#include <string.h>

/* Private constants --------------------------------------------------------*/
/* Filter depth size, range from 1 - 65536 */
#define FILTER_SIZE 40

/* Private types ------------------------------------------------------------*/
/**
 * @breif Moving average buffer structure
 */
typedef struct{
	/* Buffer array to store values */
	int16_t buffer[FILTER_SIZE];
	/* the sum of all buffer values */
	int32_t sum;
	/* the average of all buffer values */
	int16_t average;
	/* The index to keep track of the new measurement placement */
	uint16_t index;
}MA_Filter;		

/* Private variables --------------------------------------------------------*/
/* An instance of the filter buffer */
MA_Filter _ma_filter;

/**
  * @brief  Initializes the filter struct.
  * @param  The filter struct
  * @retval None
  */
void filter_init(void)
{
	//Initialize all values to 0
	memset( (void *) &_ma_filter, 0 , sizeof(MA_Filter));
}


/**
  * @brief  add one sample to the filter and 
  *         calculate the average using the moving average method
  * @param  sample that we retrieved, range -32768 to 32767
  * @retval The value of the average
  */
int16_t filter_add(int16_t sample)
{
	/* get index to i */
	uint16_t i = _ma_filter.index;
	/* remove the old value at index i*/
	_ma_filter.sum -= _ma_filter.buffer[i];	
	/* add the sample at index i*/
	_ma_filter.buffer[i] = sample;
	/* add the sample value to sum */
	_ma_filter.sum += sample;
	/* update the average value */
	_ma_filter.average = (int16_t) (_ma_filter.sum / FILTER_SIZE);
	/* move index to point to the next position in the buffer */
	_ma_filter.index = (_ma_filter.index + 1) % FILTER_SIZE;	
	return _ma_filter.average;
}

/**
  * @brief  get the average value from the current filter buffer
  * @retval The value of the average
  */
int16_t filter_average(void)
{
	return _ma_filter.average;
}

/**
  * @brief  get the sum from the current filter buffer
  * @retval The value of the sum
  */
int32_t filter_sum(void)
{
	return _ma_filter.sum;
}

