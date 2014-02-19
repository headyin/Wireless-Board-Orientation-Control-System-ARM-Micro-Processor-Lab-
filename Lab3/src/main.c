#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "math.h"
#include "atan_LUT.h"

#include "stm32f4_discovery_lis302dl.h"

int main()
{
	LIS302DL_InitTypeDef  LIS302DL_InitStruct;
	LIS302DL_FilterConfigTypeDef LIS302DL_FilterConfigStruct;
  LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;

  /* configuration of LIS302DL */
	/* put sensor in active mode */
  LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	/* sensor output data rate in 100Hz */
  LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
	/* enable the sensors on XYZ directions */
  LIS302DL_InitStruct.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE | LIS302DL_Z_ENABLE;
	/* set full-scale to +-2g, so the LSB corresponds to 18mg */
  LIS302DL_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
	/* no self test */
  LIS302DL_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
  LIS302DL_Init(&LIS302DL_InitStruct);

	/* options of using high pass filter */
	/* bypass the high pass filter */
	LIS302DL_FilterConfigStruct.HighPassFilter_Data_Selection = LIS302DL_FILTEREDDATASELECTION_BYPASSED;
	/* High pass filter cut off frequency, not used */
	LIS302DL_FilterConfigStruct.HighPassFilter_CutOff_Frequency = LIS302DL_HIGHPASSFILTER_LEVEL_0;
	/* bypass FFWU1 and FFWU2 filter */
	LIS302DL_FilterConfigStruct.HighPassFilter_Interrupt = LIS302DL_HIGHPASSFILTERINTERRUPT_OFF;
	LIS302DL_FilterConfig(&LIS302DL_FilterConfigStruct);

	/* configuration of the interrupt options of LIS302DL*/
	/* interrupt request not latched */
  LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_NOTLATCHED;
	/* disable interrupt request on single click event on XYZ axises */
  LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_XYZ_DISABLE;
	/* disable interrupt request on single click event on XYZ axises */
  LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_XYZ_DISABLE;
  LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);
	
}







