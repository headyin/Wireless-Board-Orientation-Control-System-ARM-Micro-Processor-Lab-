#ifndef LCD16_H
#define LCD16_H

#include "stdint.h"
#include "cmsis_os.h"

#define LINE_LENGTH 16
#define VIRTUAL_LINE_LENGTH 40

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


void lcd_clear_command(void);
void lcd_write_char(char c);
void lcd_display_init(void);
void lcd_gpio_init(void);
void lcd_send_character(uint8_t);
void lcd_send_command(uint8_t command);
void lcd_set_display_angles(float,float);
void lcd_set_display_temperaure(float temp);

void lcd_thread(void const * argument);
osThreadId  lcd_thread_create(void);
void lcd_start(void);

#endif
