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
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "accelerometer.h"
#include "atan_LUT.h"
#include "math.h"
#include "maFilter.h"
#include "LIS302DL/stm32f4_discovery_lis302dl.h"
#include "wirelessTransmitter.h"
#include "push_button.h"

#include "stdio.h"

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
#define LIS302DL_INTERRUPT_I2CFG_CLICK                    ((uint8_t)0x38)

/* Interrupt 1 configuration bits */
#define LIS302DL_INTERRUPT_I1CFG_GND                      ((uint8_t)0x00)
#define LIS302DL_INTERRUPT_I1CFG_FFWU1                    ((uint8_t)0x01)
#define LIS302DL_INTERRUPT_I1CFG_FFWU2                    ((uint8_t)0x02)
#define LIS302DL_INTERRUPT_I1CFG_FFWU12                   ((uint8_t)0x03)
#define LIS302DL_INTERRUPT_I1CFG_DATAREADY                ((uint8_t)0x04)
#define LIS302DL_INTERRUPT_I1CFG_CLICK                    ((uint8_t)0x07)

#define PI 3.1415926f

/* Private types ----------------------------------------------------------------*/

typedef struct 
{
  uint8_t Interrupt_Polarity;      /* Interrupt polarity, active high or low */
  uint8_t Interrupt_PP_OD;         /* Push-pull or Open Drain selection on interrupt pad */
  uint8_t INT2_Configuration;      /* Interrupt 2 configuration */
  uint8_t INT1_Configuration;      /* Interrupt 1 configuration */
} LIS302DL_ControlReg3TypeDef;

/* Private variables ------------------------------------------------------------*/
/* 10000 times of the calibration matrix */
int16_t calibration[4][3] = {185, 5  ,  3 ,
                              0 , 179,  0,
                              0 , 2  , 185,
                              0 , 357, -62};

/*variables used for acceleration calibration */
int8_t x_acceleration;
int8_t y_acceleration;
int8_t z_acceleration;
uint8_t buffer;

float roll_degree_MA, pitch_degree_MA;

float acceleration[3];

/* Private variables ------------------------------------------------------------*/

//semaphore
osSemaphoreId accelerometer_semaphore;
//structure used for moving average filter for the roll
MA_Filter accelerometer_roll_filter_struct;
//structure used for moving average filter for the pitch
MA_Filter accelerometer_pitch_filter_struct;
                              
/* Private functions ----------------------------------------------------------- */

 /**
 * @brief  This function handles the EXTI1 interrupt
 * @param  None
 * @retval None
 */
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) == RESET) return;
  //add accelerometer_semaphore by 1, so the accelerometer thread can measure the acceleration once
  osSemaphoreRelease (accelerometer_semaphore);
  /* Clear the EXTI line 1 pending bit */
  EXTI_ClearITPendingBit(EXTI_Line1);
}

//definations for semaphore and thread
osSemaphoreDef(accelerometer_semaphore); 
osThreadDef(accelerometer_Thread, osPriorityNormal, 1, 0);
//define a mutex
osMutexDef (accelerometerRollFilterMutex);
osMutexDef (accelerometerPitchFilterMutex);
osThreadId accelerometer_thread_id;

/* Private functions ----------------------------------- */
/**
 * @brief  calibrate the sensor data
 * @param  None
 * @retval None
 */
void calibrate()
{
	acceleration[0] = (float) (calibration[0][0]*x_acceleration+calibration[0][1]*y_acceleration+calibration[0][2]*z_acceleration+calibration[3][0]) / 10000;
	acceleration[1] = (float) (calibration[1][0]*x_acceleration+calibration[1][1]*y_acceleration+calibration[1][2]*z_acceleration+calibration[3][1]) / 10000;
	acceleration[2] = (float) (calibration[2][0]*x_acceleration+calibration[2][1]*y_acceleration+calibration[2][2]*z_acceleration+calibration[3][2]) / 10000;
}

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

/**
 * @brief  Initiliaze the LIS302DL accelerometer to start it in active mode at 100Hz
 *         using full scale 2g in XYZ direction and generate an interupt of 
 *         high polarity to INT1(PE0 EXTI0) when data ready.
 * @param  None
 * @retval None
 */
void LIS302DL_Sensor_Init(void)
{
	/* strctures used for LIS302DL initialization */
	LIS302DL_InitTypeDef  LIS302DL_InitStruct;
	LIS302DL_FilterConfigTypeDef LIS302DL_FilterConfigStruct;
	LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;
	LIS302DL_ControlReg3TypeDef LIS302DL_ControlReg3Struct;

  /* configuration of LIS302DL */
	/* put sensor in active mode */
  LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	/* sensor output data rate in 100Hz */
  LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
	/* enable the sensors on XYZ directions */
  LIS302DL_InitStruct.Axes_Enable = LIS302DL_XYZ_ENABLE;
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
  /* INT2 pad sends DATAREADY interrupt signal */
  LIS302DL_ControlReg3Struct.INT2_Configuration = LIS302DL_INTERRUPT_I2CFG_DATAREADY;
  /* INT1 pad sends GND signal */
  LIS302DL_ControlReg3Struct.INT1_Configuration = LIS302DL_INTERRUPT_I1CFG_GND;
  LIS302DL_CTRLREG3_Init(&LIS302DL_ControlReg3Struct);

}

/**
 * @brief  Enable GPIOE port 1 and connect it EXTI1
 *         and set EXTI1 interrupt to highest prioroty
 * @param  None
 * @retval None
 */
void EXTI1_INIT(void)
{
	/*structures used for the initialization of related GPIOs (GPIOE) and EXTI */
	GPIO_InitTypeDef   GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOE clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure PE1 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  /* Connect EXTI Line1 to PE1 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);
  
  /* Configure EXTI Line1 */
  /* PE0 is mapped to EXTI1 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  /* Interupt mode */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  /* Rrigger interrupt on the rising edge*/
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* Enable EXTI Line1 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set EXTI Line0 Interrupt and the priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  /* Group Priority 0 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  /* Sub-priority 0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  /* Enable NVIC_IRQChannel */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/* Public functions -------------------------------------------------------- */
/**
 * @brief  Initiliaze the LIS302DL accelerometer and the NVIC external interrupt 
 *         to start it in active mode at 100Hz using full scale 2g in XYZ direction 
  *        and generate an interupt of high polarity to INT1(PE0 EXTI0) when data ready.
 * @param  None
 * @retval None
 */
void accelerometer_init(void)
{
  /* enable the LIS302DL sensor to generate interrupts */
  LIS302DL_Sensor_Init();
  /* enable the interrupt on PE0 EXTI0 */
  EXTI1_INIT();

}

/**
  * @brief  start interrrupt triggering to get acceleration measurement
  * @param  None
  * @retval None
  */
void accelerometer_start(void)
{
  uint8_t buffer[6];
  /* Trigger one data reading */
  LIS302DL_Read(buffer, LIS302DL_OUT_X_ADDR, 6);
}

/**
 * @brief  measure and calibrate the acceleration value;
 * @param  None
 * @retval None
 */
void measure_accleration(void)
{
  LIS302DL_Read(&buffer, LIS302DL_OUT_X_ADDR, 1);
  x_acceleration = (int8_t) buffer;
  LIS302DL_Read(&buffer, LIS302DL_OUT_Y_ADDR, 1);
  y_acceleration = (int8_t) buffer;
  LIS302DL_Read(&buffer, LIS302DL_OUT_Z_ADDR, 1);
  z_acceleration = (int8_t) buffer;

  calibrate();
}

/**
 * @brief  calculate pitch angle
 * @param  None
 * @retval pitch angle
 */
float getPitch(void)
{
	return  atan_table(acceleration[0]/ sqrt((acceleration[1]*acceleration[1])+(acceleration[2]*acceleration[2])));
}
/**
 * @brief  calculate roll angle
 * @param  None
 * @retval roll angle
 */
float getRoll(void)
{
	return  atan_table(acceleration[1]/ sqrt((acceleration[0]*acceleration[0])+(acceleration[2]*acceleration[2])));
}


/**
  * @brief  measure the angles when the semaphore is ready.
  * @param  None
  * @retval None
  */
void accelerometer_Thread(void const * argument)
{
  float rollAngle, pitchAngle;

  
  while (1)
  {
		//Wait for semaphore to be released by interrupt
    osSemaphoreWait (accelerometer_semaphore, osWaitForever);   
		//Get measurement and calculate angles
    measure_accleration();
    rollAngle = getRoll();
    pitchAngle = getPitch();
		//Filter the angles
    filter_add((int16_t) round(rollAngle * 100), &accelerometer_roll_filter_struct);
    filter_add((int16_t) round(pitchAngle * 100), &accelerometer_pitch_filter_struct);
    roll_degree_MA = filter_average(&accelerometer_roll_filter_struct) / 100.0;
    pitch_degree_MA = filter_average(&accelerometer_pitch_filter_struct) / 100.0;
    if (getThreadToRun() == 0)
    {
      wireless_send(0, roll_degree_MA, pitch_degree_MA);
    }
    //printf("%f, %f\n",roll_degree_MA,pitch_degree_MA);
  }
}

/**
  * @brief  Get the filtered roll angle from filter struct
  * @param  None
  * @retval None
  */
float getFilteredRollAngle(void)
{
  return  roll_degree_MA;
}

/**
  * @brief  Get the filtered pitch angle from filter struct
  * @param  None
  * @retval None
  */
float getFilteredPitchAngle(void)
{
  return pitch_degree_MA; 
}

/**
  * @brief  create a thread for acceleration measurement
  * @param  None
  * @retval The thread ID for accelerometer
  */
osThreadId  accelerometer_Thread_Create(void)
{
  //Initialize the peripherals for the accelerometer
  accelerometer_init();
	//Initialize the peripherals for the servo moter
	//Initialize the filters
  filter_init(&accelerometer_roll_filter_struct, 30);
  filter_init(&accelerometer_pitch_filter_struct,30);
	//Initialize the mutex for the filters
  accelerometer_roll_filter_struct.mutexId = osMutexCreate(osMutex (accelerometerRollFilterMutex));
  accelerometer_pitch_filter_struct.mutexId = osMutexCreate(osMutex (accelerometerPitchFilterMutex));
  //create the semaphore
  accelerometer_semaphore = osSemaphoreCreate(osSemaphore(accelerometer_semaphore), 1);
  
  //start accelerometer thread
  accelerometer_thread_id = osThreadCreate(osThread(accelerometer_Thread), NULL);
  return accelerometer_thread_id;
}


