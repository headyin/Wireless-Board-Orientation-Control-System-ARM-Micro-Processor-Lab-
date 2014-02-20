/**
 ******************************************************************************
 * @file    accelerometer.c
 * @author  Xinshang Yin, Chandani Patel
 * @version V1.0.0
 * @date    20-February-2014
 * @brief   This file provides functions to measure the acceleration of the board
 *          and generate interupt from the accelerometer to the processor.         
 */

/* Includes ------------------------------------------------------------------*/
#include "accelerometer.h"

/* Private defines -------------------------------------------------------- */

/* Interrupt Active High Low */
#define LIS302DL_INTERRUPT_ACTIVE_HIGH                    ((uint8_t)0x00)
#define LIS302DL_INTERRUPT_ACTIVE_LOW                     ((uint8_t)0x80)

/* Push-pull/Open Drain selection on the interrupt pad */
#define LIS302DL_INTERRUPT_PPOD_PP                        ((uint8_t)0x00)
#define LIS302DL_INTERRUPT_PPOD_OD                        ((uint8_t)0x40)

/* Interrupt 2 configuration bits */
#define LIS302DL_INTERRUPT_I2CFG_GND                      ((uint8_t)0x00)
#define LIS302DL_INTERRUPT_I2CFG_FFWU1                    ((uint8_t)0x08)
#define LIS302DL_INTERRUPT_I2CFG_FFWU2                    ((uint8_t)0x10)
#define LIS302DL_INTERRUPT_I2CFG_FFWU12                   ((uint8_t)0x18)
#define LIS302DL_INTERRUPT_I2CFG_DATAREADY                ((uint8_t)0x20)
#define LIS302DL_INTERRUPT_I2CFG_NOSIG                    ((uint8_t)0x38)

/* Interrupt 1 configuration bits */
#define LIS302DL_INTERRUPT_I1CFG_GND                      ((uint8_t)0x00)
#define LIS302DL_INTERRUPT_I1CFG_FFWU1                    ((uint8_t)0x01)
#define LIS302DL_INTERRUPT_I1CFG_FFWU2                    ((uint8_t)0x02)
#define LIS302DL_INTERRUPT_I1CFG_FFWU12                   ((uint8_t)0x03)
#define LIS302DL_INTERRUPT_I1CFG_DATAREADY                ((uint8_t)0x04)
#define LIS302DL_INTERRUPT_I1CFG_NOSIG                    ((uint8_t)0x07)


/* Private types ----------------------------------------------------------------*/

typedef struct 
{
  uint8_t Interrupt_Polarity;      /* Interrupt polarity, active high or low */
  uint8_t Interrupt_PP_OD;         /* Push-pull or Open Drain selection on interrupt pad */
  uint8_t INT2_Configuration;      /* Interrupt 2 configuration */
  uint8_t INT1_Configuration;      /* Interrupt 1 configuration */
} LIS302DL_ControlReg3TypeDef;

/* Private variables ------------------------------------------------------------*/

/* strctures used for LIS302DL initialization */
LIS302DL_InitTypeDef  LIS302DL_InitStruct;
LIS302DL_FilterConfigTypeDef LIS302DL_FilterConfigStruct;
LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;
LIS302DL_ControlReg3TypeDef LIS302DL_ControlReg3Struct;

/* Private functions -------------------------------------------------------- */

/**
 * @brief  Initialize the control register 3 for the interrupt type configuration
 * @param  LIS302DL_ControlReg3TypeDef: pointer to a LIS302DL_ControlReg3Struct
 * @retval None
 */
void LIS302DL_CTRLREG3_Init(LIS302DL_ControlReg3TypeDef* LIS302DL_ControlReg3Struct)
{
  uint8_t ctrl = 0x00;
  
  /* configure interupt polarity, push-pull/open drain selection and IN1 and IN2 output */                   
  ctrl = (uint8_t)(LIS302DL_ControlReg3Struct->Interrupt_Polarity | \
                   LIS302DL_ControlReg3Struct->Interrupt_PP_OD | \
                   LIS302DL_ControlReg3Struct->INT2_Configuration | \
                   LIS302DL_ControlReg3Struct->INT1_Configuration);
  
  /* Write value to MEMS CLICK_CFG register */
  LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG3_ADDR, 1);
}



/* Public functions -------------------------------------------------------- */
/**
 * @brief  Initiliaze the accelerometer to start it in active mode at 100Hz
 *         using full scale 2g in XYZ direction and generate an interupt of 
 *         high polarity to INT1 when data ready.
 * @param  None
 * @retval None
 */
void accelerometer_init(void)
{
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
  
  /* configuration of the control register 3 of LIS302DL*/
  /* interrupt polarity high */
  LIS302DL_ControlReg3Struct.Interrupt_Polarity = LIS302DL_INTERRUPT_ACTIVE_HIGH;
  /* interrupt pad push-pull */
  LIS302DL_ControlReg3Struct.Interrupt_PP_OD = LIS302DL_INTERRUPT_PPOD_PP;
  /* INT2 pad is not used */
  LIS302DL_ControlReg3Struct.INT2_Configuration = LIS302DL_INTERRUPT_I2CFG_NOSIG;
  /* INT1 pad send GND signal */
  LIS302DL_ControlReg3Struct.INT1_Configuration = LIS302DL_INTERRUPT_I1CFG_GND;
  LIS302DL_CTRLREG3_Init(&LIS302DL_ControlReg3Struct);
}

