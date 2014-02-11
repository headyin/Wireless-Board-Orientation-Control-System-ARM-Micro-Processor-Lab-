#ifndef FILTER_HEADER
#define FILTER_HEADER
extern const uint32_t D;

typedef struct{
	float buffer[D];			//Buffer array to store values
	float sum;					//The sum of 
	float average;				//The moving average
	int index;					//The index to keep track of the new measurement placement
}MA_Filter;			


void initialize_filter(MA_Filter *Filter_Struct);
float calculate_average(MA_Filter *filter_struct, float sample);

#endif