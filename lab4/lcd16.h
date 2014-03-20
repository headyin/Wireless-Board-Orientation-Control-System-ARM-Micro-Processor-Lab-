#ifndef LCD16_H
#define LCD16_H

#include "stdint.h"
#include "cmsis_os.h"

//line length
#define LINE_LENGTH 16
#define VIRTUAL_LINE_LENGTH 40

//gpio pins used:
#define LCD_DB0 GPIO_Pin_0
#define LCD_DB1 GPIO_Pin_1
#define LCD_DB2 GPIO_Pin_2
#define LCD_DB3 GPIO_Pin_4
#define LCD_DB4 GPIO_Pin_5
#define LCD_DB5 GPIO_Pin_7
#define LCD_DB6 GPIO_Pin_8
#define LCD_DB7 GPIO_Pin_11
#define LCD_RS GPIO_Pin_12
#define LCD_RW GPIO_Pin_13
#define LCD_EN GPIO_Pin_14
#define LCD_PINS (LCD_DB0 | LCD_DB1 | LCD_DB2 | LCD_DB3 | LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7 | LCD_RS | LCD_RW | LCD_EN)
#define LCD_GPIO GPIOB

/**
 * @brief send a clear command to LCD display
 * @param None
 * @retval None
 */
void lcd_clear_command(void);

/**
 * @brief display a character on LCD display
 * @param char
 * @retval None
 */
void lcd_write_char(char c);

/**
 * @brief Initialize the LCD display
 *        enables LCD using 2-line display and 8-bit interface
 *        cursor auto-right-move without showing
          Clear the LCD screen
 * @param None
 * @retval None
 */
void lcd_display_init(void);

/**
 * @brief Initialize the grio used for LCD
 * @param None
 * @retval None
 */
void lcd_gpio_init(void);

/**
 * @brief display a character on the LCD display
 * @param uint8_t character
 * @retval None
 */
void lcd_send_character(uint8_t);

/**
 * @brief send a command to LCD display
 * @param uint8_t command
 * @retval None
 */
void lcd_send_command(uint8_t command);

/**
 * @brief fill the display buffer with the angle info
 * @param float roll angle
 * @param float pitch angle
 * @retval None
 */
void lcd_set_display_angles(float,float);

/**
 * @brief fill the display buffer with the temperaure info
 * @param float temperature
 * @retval None
 */
void lcd_set_display_temperaure(float temp);

/**
 * @brief Thread function of LCD 
 * @param Not used
 * @retval None
 */
void lcd_thread(void const * argument);


/**
 * @brief Create a new thread for LCD
 * @param None
 * @retval osThreadId the created tread id
 */
osThreadId  lcd_thread_create(void);

/**
 * @brief Start the timer used to controll the lcd thread
 * @param None
 * @retval None
 */
void lcd_start(void);

#endif
