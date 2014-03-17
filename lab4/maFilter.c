/**
  ******************************************************************************
  * @file    filter.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to get the moving average filter    
	*          Note: this filter is using a mutex, so it is thread safe
  */  
 
/* Includes ------------------------------------------------------------------*/ 
#include "maFilter.h"
#include <string.h>

#include "stdio.h"

/* Private variables --------------------------------------------------------*/


/**
  * @brief  Initializes the filter struct.
  * @param  The filter struct
  * @retval void
  */
void filter_init(MA_Filter* filter_struct, uint16_t depth)
{

	//Initialize all values to 0
	memset( (void *) filter_struct, 0 , sizeof(MA_Filter));
  filter_struct->depth = depth;
}


/**
  * @brief  add one sample to the filter and 
  *         calculate the average using the moving average method
  * @param  sample that we retrieved, range -32768 to 32767
  * @retval NONE
  */
void filter_add(int16_t sample, MA_Filter* filter_struct)
{
  //lock
  osStatus status  = osMutexWait(filter_struct->mutexId, osWaitForever);
  if (status != osOK)
  {
    printf("error occurs in filter_add when trying to lock: %d\n", status);
    return;
  }
  
	/* get index to i */
	uint16_t i = filter_struct->index;
	/* remove the old value at index i*/
	filter_struct->sum -= filter_struct->buffer[i];	
	/* add the sample at index i*/
	filter_struct->buffer[i] = sample;
	/* add the sample value to sum */
	filter_struct->sum += sample;
	/* update the average value */
	filter_struct->average = (int16_t) (filter_struct->sum / filter_struct->depth);
	/* move index to point to the next position in the buffer */
	filter_struct->index = (filter_struct->index + 1) % filter_struct->depth;
  
  //unlock
  status = osMutexRelease(filter_struct->mutexId);
  if (status != osOK)
  {
    printf("error occurs in filter_add when trying to unlock: %d\n", status);
    return;
  }
}

/**
  * @brief  get the average value from the current filter buffer
  * @retval The value of the average
  */
int16_t filter_average(MA_Filter* filter_struct)
{
  int16_t avg;
  //lock
  osStatus status  = osMutexWait(filter_struct->mutexId, osWaitForever);
  if (status != osOK)
  {
    printf("error occurs in filter_average when trying to lock: %d\n", status);
    return 0 ;
  }
  
	avg = filter_struct->average;
  //unlick
  status = osMutexRelease(filter_struct->mutexId);
  if (status != osOK)
  {
    printf("error occurs in filter_average when trying to unlock: %d\n", status);
    return avg;
  }
  return avg;
}


/**
  * @brief  get the sum from the current filter buffer
  * @retval The value of the sum
  */
int32_t filter_sum(MA_Filter* filter_struct)
{
  int32_t sum;
  //lock
  osStatus status  = osMutexWait(filter_struct->mutexId, osWaitForever);
  if (status != osOK)
  {
    printf("error occurs in filter_sum when trying to lock: %d\n", status);
    return 0 ;
  }
	sum = filter_struct->sum;
  //unlick
  status = osMutexRelease(filter_struct->mutexId);
  if (status != osOK)
  {
    printf("error occurs in filter_sum when trying to unlock: %d\n", status);
    return sum;
  }
  return sum;
}

