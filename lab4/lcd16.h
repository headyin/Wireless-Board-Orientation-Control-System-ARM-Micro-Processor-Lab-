#ifndef LCD16_H
#define LCD16_H

#define LCD_DB0 GPIO_Pin_0
#define LCD_DB1 GPIO_Pin_1
#define LCD_DB2 GPIO_Pin_2
#define LCD_DB3 GPIO_Pin_3
#define LCD_DB4 GPIO_Pin_4
#define LCD_DB5 GPIO_Pin_5
#define LCD_DB6 GPIO_Pin_6
#define LCD_DB7 GPIO_Pin_7
#define LCD_RS GPIO_Pin_8
#define LCD_RW GPIO_Pin_9
#define LCD_EN GPIO_Pin_10
#define LCD_PINS (LCD_DB0 | LCD_DB1 | LCD_DB2 | LCD_DB3 | LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7 | LCD_RS | LCD_RW | LCD_EN)
#define LCD_GPIO GPIOC


void lcd_clear_command(void);
void lcd_write_char(char c);
void lcd_display_init(void);

#endif
