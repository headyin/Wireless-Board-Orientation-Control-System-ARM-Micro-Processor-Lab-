/**
  ******************************************************************************
  * @file    filter.c
  * @author  Xinshang Yin, Chandani Patel
  * @version V1.0.0
  * @date    06-February-2014
  * @brief   This file provides functions to get the moving average filter    
  */  
 
/* Includes ------------------------------------------------------------------*/ 
#include "stdint.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "filter.h"

/* Private define ------------------------------------------------------------*/ 
#define D=6;
struct MA_Filter;

/**
  * @brief  Initializes the filter struct.
  * @param  The filter struct
  * @retval None
  */
void initialize_filter(MA_Filter *filter_struct){
	//Initialize all values to 0
	filter_struct->sum = 0;
	filter_struct->average = 0;
	filter_struct->index = 0;
	int i;
	for(i=0; i<D; i++){
		filter_struct->buffer[i] = 0;
	}
}

/**
  * @brief  Calculates the average using the moving average method
  * @param  The filter struct and the float sample that we retrieved
  * @retval The float value of the average
  */
float calculate_average(MA_Filter *filter_struct, float sample){
	
	//Check if buffer is full, if it is then set index back to 0 else increase the index
	if (moving_average->index == D-1) {
		moving_average->index = 0;																																								//If so, set to 0
	} else {
		moving_average->index++;																																									//Otherwise, increment like normal
	}
	//Add sample to buffer at the correct index
	filter_struct->buffer[filter_struct->index] = sample;
	
	int i;
	//Sum of all values in buffer
	for(i=0; i<D; i++){
		filter_struct->sum += filter_struct->buffer[i];
	}
	//Calculate average by dividing sum by depth size
	filter_struct->average =  filter_struct->sum/D;
	
	return filter_struct->average;
}