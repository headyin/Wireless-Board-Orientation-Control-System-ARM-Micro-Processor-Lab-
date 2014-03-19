#include "lcd16.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "string.h"
#include "stdio.h"

char line1[LINE_LENGTH];
char line2[LINE_LENGTH];

void lcd_set_data(uint8_t data)
{
  uint16_t pins = 0x00ff & data;
  GPIO_SetBits(LCD_GPIO, pins);
  GPIO_ResetBits(LCD_GPIO, LCD_RW);
}

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
  //function set 2 lines, 8-bit interface
  lcd_send_command(0x38);
  //enable display(0x0c) and cursor display(0x0e)
  lcd_send_command(0x0c);
  //clear
  lcd_send_command(0x01);
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

void lcd_display_angles(float roll, float pitch)
{
  int i;
  lcd_clear_command();
  memset(line1,0,strlen(line1));
  memset(line2,0,strlen(line2));
  snprintf(line1, LINE_LENGTH + 1, "Roll  : %.1f%c", roll, 0xdf);
  snprintf(line2, LINE_LENGTH + 1, "Pitch : %.1f%c", pitch, 0xdf);
  for (i = 0; i < strlen(line1); i++) {
    lcd_write_char(line1[i]);
  }
  for (i = 0; i < VIRTUAL_LINE_LENGTH - strlen(line1); i++) {
    lcd_write_char(' ');
  }
  for (i = 0; i < strlen(line2); i++) {
    lcd_write_char(line2[i]);
  }
}

void lcd_display_temperaure(float temp)
{
  int i;
  lcd_clear_command();
  memset(line1,0,strlen(line1));
  memset(line2,0,strlen(line2));
  snprintf(line1, LINE_LENGTH + 1, "Temperature:");
  snprintf(line2, LINE_LENGTH + 1, "    %.1f%cC", temp, 0xdf);
  for (i = 0; i < strlen(line1); i++) {
    lcd_write_char(line1[i]);
  }
  for (i = 0; i < VIRTUAL_LINE_LENGTH - strlen(line1); i++) {
    lcd_write_char(' ');
  }
  for (i = 0; i < strlen(line2); i++) {
    lcd_write_char(line2[i]);
  }
}


