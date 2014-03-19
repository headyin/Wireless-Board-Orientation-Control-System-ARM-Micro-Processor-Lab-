#include "lcd16.h"
#include "stm32f4xx.h"



void lcd_set_data(uint8_t data)
{
  uint16_t pins = 0x00ff & data;
  GPIO_SetBits(LCD_GPIO, pins);
  GPIO_ResetBits(LCD_GPIO, LCD_RW);
}

void lcd_send_character(uint8_t character)
{
  int i;
  GPIO_SetBits(LCD_GPIO, LCD_EN);
  lcd_set_data(character);
  GPIO_SetBits(LCD_GPIO, LCD_RS);
  for (i = 0; i < 100000; i++);
  GPIO_ResetBits(LCD_GPIO, LCD_EN);
}

void lcd_send_command(uint8_t command)
{
  int i;
  GPIO_SetBits(LCD_GPIO, LCD_EN);
  lcd_set_data(command);
  GPIO_ResetBits(LCD_GPIO, LCD_RS);
  for (i = 0; i < 100000; i++);
  GPIO_ResetBits(LCD_GPIO, LCD_EN);
  for (i = 0; i < 1000000; i++);

}

void lcd_clear_command(void)
{
  lcd_send_command(0x01);
}

void lcd_write_char(char c)
{
  lcd_send_character(c);
}

void lcd_display_init(void)
{
  lcd_send_command(0x01);
  lcd_send_command(0x02);
  lcd_send_command(0x07);
  lcd_send_command(0x0f);  
}

void lcd_gpio_init(void)
{
  GPIO_InitTypeDef  lcd_GPIO_InitStructure;
  /* GPIOC Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

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



