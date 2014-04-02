#include "lcd16.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "string.h"
#include "stdio.h"
#include "push_button.h"
#include "accelerometer/accelerometer.h"
#include "temperature/temperature.h"


//buffers of display content
char line1[LINE_LENGTH];
char line2[LINE_LENGTH];



//semaphore
osSemaphoreId lcd_semaphore;
osSemaphoreDef(lcd_semaphore); 
osThreadDef(lcd_thread, osPriorityNormal, 1, 0);
osThreadId lcd_thread_id;


/** 
  * @brief  This function handles TIM3 global interrupt request. 
  * @param  None 
  * @retval None 
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
    osSemaphoreRelease (lcd_semaphore);
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1); 
	}
}

/**
 * @brief write data to the LCD trough the 8 data pins
 * @param uint8_t data
 * @retval None
 */
void lcd_set_data(uint8_t data)
{
  uint16_t pins = 0x0000;
  if (data & 0x01) pins |= LCD_DB0;
  if (data & 0x02) pins |= LCD_DB1;
  if (data & 0x04) pins |= LCD_DB2;
  if (data & 0x08) pins |= LCD_DB3;
  if (data & 0x10) pins |= LCD_DB4;
  if (data & 0x20) pins |= LCD_DB5;
  if (data & 0x40) pins |= LCD_DB6;
  if (data & 0x80) pins |= LCD_DB7;
  GPIO_SetBits(LCD_GPIO, pins);
  GPIO_ResetBits(LCD_GPIO, LCD_RW);
}

/**
 * @brief display a character on the LCD display
 * @param uint8_t character
 * @retval None
 */
void lcd_send_character(uint8_t character)
{
  GPIO_ResetBits(LCD_GPIO, LCD_PINS);
  lcd_set_data(character);
  GPIO_SetBits(LCD_GPIO, LCD_RS);
  GPIO_SetBits(LCD_GPIO, LCD_EN);
  osDelay(1);
  GPIO_ResetBits(LCD_GPIO, LCD_EN);
  osDelay(1);
}

/**
 * @brief send a command to LCD display
 * @param uint8_t command
 * @retval None
 */
void lcd_send_command(uint8_t command)
{
  GPIO_ResetBits(LCD_GPIO, LCD_PINS);
  lcd_set_data(command);
  GPIO_ResetBits(LCD_GPIO, LCD_RS);
  GPIO_SetBits(LCD_GPIO, LCD_EN);
  osDelay(1);
  GPIO_ResetBits(LCD_GPIO, LCD_EN);
  osDelay(2);
}

/**
 * @brief send a clear command to LCD display
 * @param None
 * @retval None
 */
void lcd_clear_command(void)
{
  lcd_send_command(0x01);
}

/**
 * @brief display a character on LCD display
 * @param char
 * @retval None
 */
void lcd_write_char(char c)
{
  lcd_send_character(c);
}

/**
 * @brief Initialize the LCD display
 *        enables LCD using 2-line display and 8-bit interface
 *        cursor auto-right-move without showing
          Clear the LCD screen
 * @param None
 * @retval None
 */
void lcd_display_init(void)
{
  lcd_gpio_init();
  
  //function set 2 lines, 8-bit interface
  lcd_send_command(0x38);
  //enable display(0x0c) and cursor display(0x0e)
  lcd_send_command(0x0c);
  //clear
  lcd_send_command(0x01);
}

/**
 * @brief Initialize the grio used for LCD
 * @param None
 * @retval None
 */
void lcd_gpio_init(void)
{
  GPIO_InitTypeDef  lcd_GPIO_InitStructure;
  /* GPIOC Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* Configure PC0 -PC10 in output pushpull mode */
  lcd_GPIO_InitStructure.GPIO_Pin = LCD_PINS;
	/* output mode */
  lcd_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	/* Output type Push Pull */
  lcd_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* High speed */
  lcd_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/* Do not pull*/
  lcd_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(LCD_GPIO, &lcd_GPIO_InitStructure);
}

/**
 * @brief Initialize the TIM used for LCD
 * @param None
 * @retval None
 */
void lcd_timer_init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t prescaler;

	/* TIM2 pheriph clock enable, TIM3 clock frequency = SystemCoreClock / 2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Enable TIM2 NVIC interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /*
	 * TIM3 counter clock frequency = 60000Hz
	 * Prescaler = (TIM3CLK / TIM3 counter clock frequency) - 1
	 */
	prescaler = (uint16_t) ((SystemCoreClock / 2) / 60000) - 1;
	
	/* Time base configuration */
  /* TIM_Period = 50000, TIM3 Interrupt frequency = TIM2 Counter Frequency / TIM_Period = 1Hz*/
  TIM_TimeBaseStructure.TIM_Period = 60000;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	/* up-counting counter mode, counter counts from 0 to ARR and restarts from 0 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 60000;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  /* Disable TIM2 Channel 1 preload*/
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	/* Enable TIM3_CH1 interrupt */
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

}

/**
 * @brief fill the display buffer with the angle info
 * @param float roll angle
 * @param float pitch angle
 * @retval None
 */
void lcd_set_display_angles(float roll, float pitch)
{
  memset(line1,0,strlen(line1));
  memset(line2,0,strlen(line2));
  snprintf(line1, LINE_LENGTH + 1, "Roll  : %.1f%c", roll, 0xdf);
  snprintf(line2, LINE_LENGTH + 1, "Pitch : %.1f%c", pitch, 0xdf);
}

/**
 * @brief fill the display buffer with the temperaure info
 * @param float temperature
 * @retval None
 */
void lcd_set_display_temperaure(float temp)
{
  memset(line1,0,strlen(line1));
  memset(line2,0,strlen(line2));
  snprintf(line1, LINE_LENGTH + 1, "Temperature:");
  snprintf(line2, LINE_LENGTH + 1, "    %.1f%cC", temp, 0xdf);
}

/**
 * @brief Thread function of LCD 
 * @param Not used
 * @retval None
 */
void lcd_thread(void const * argument)
{
  int i;
  while (1)
  {
    osSemaphoreWait (lcd_semaphore, osWaitForever);
    if (getThreadToRun() == 1)
    {
      //Accelerometer mode
      lcd_set_display_angles(getFilteredRollAngle(),getFilteredPitchAngle());
    } else
    {
      //Temperture mode
      lcd_set_display_temperaure(get_filterd_tempeature());
    }
    //clear the curent content in LCD
    lcd_clear_command();
    //print the first line
    for (i = 0; i < strlen(line1); i++)
    {
      lcd_write_char(line1[i]);
    }
    //move to second line
    for (i = 0; i < VIRTUAL_LINE_LENGTH - strlen(line1); i++)
    {
      lcd_write_char(' ');
    }
    //print the second line
    for (i = 0; i < strlen(line2); i++)
    {
      lcd_write_char(line2[i]);
    }
  }
}

/**
 * @brief Create a new thread for LCD
 * @param None
 * @retval osThreadId the created tread id
 */
osThreadId  lcd_thread_create(void)
{
  lcd_display_init();
  lcd_timer_init();
  lcd_semaphore = osSemaphoreCreate(osSemaphore(lcd_semaphore), 1);
  //create temperature thread
  lcd_thread_id = osThreadCreate(osThread(lcd_thread), NULL);
  return lcd_thread_id;
}

/**
 * @brief Start the timer used to controll the lcd thread
 * @param None
 * @retval None
 */
void lcd_start(void)
{
  TIM_Cmd(TIM2, ENABLE);
}
