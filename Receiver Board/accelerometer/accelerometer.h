#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "cmsis_os.h"

/* Public functions ------------------------------------ */

/**
 * @brief  Initiliaze the LIS302DL accelerometer and the NVIC external interrupt 
 *         to start it in active mode at 100Hz using full scale 2g in XYZ direction 
  *        and generate an interupt of high polarity to INT1(PE0 EXTI0) when data ready.
 * @param  None
 * @retval None
 */
void accelerometer_init(void);

/**
 * @brief  calculate pitch angle
 * @param  None
 * @retval roll angle
 */
float getPitch(void);

/**
 * @brief  calculate roll angle
 * @param  None
 * @retval roll angle
 */
float getRoll(void);

/**
 * @brief  measure and calibrate the acceleration value;
 * @param  None
 * @retval None
 */
void measure_accleration(void);

/**
  * @brief  measure the angles when the semaphore is ready.
  * @param  None
  * @retval None
  */
void accelerometer_Thread(void const * argument);

	/**
  * @brief  create a thread for acceleration measurement
  * @param  None
  * @retval The thread ID for accelerometer
  */
osThreadId  accelerometer_Thread_Create(void);

/**
  * @brief  start interrrupt triggering to get acceleration measurement
  * @param  None
  * @retval None
  */
void accelerometer_start(void);

/**
  * @brief  Get the filtered pitch angle from filter struct
  * @param  None
  * @retval The filtered roll angle
  */
float getFilteredRollAngle(void);

/**
  * @brief  Get the filtered pitch angle from filter struct
  * @param  None
  * @retval The filtered pitch angle
  */
float getFilteredPitchAngle(void);


#endif
