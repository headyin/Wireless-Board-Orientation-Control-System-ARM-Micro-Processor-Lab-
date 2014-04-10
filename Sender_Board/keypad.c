#include "keypad.h"

#include <stdio.h>
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "stm32f4xx_conf.h"

GPIO_InitTypeDef GPIO_ROWS;
GPIO_InitTypeDef GPIO_COLUM;

void keypadThread(void const *args);

osThreadId keypadThreadID;
osThreadDef(keypadThread, osPriorityNormal, 1, 0);

void keypad_init()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  GPIO_ROWS.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_ROWS.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_ROWS.GPIO_OType = GPIO_OType_PP; 
  GPIO_ROWS.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_COLUM.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_COLUM.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_COLUM.GPIO_OType = GPIO_OType_PP;
  GPIO_COLUM.GPIO_PuPd = GPIO_PuPd_UP;
}

void setRowMode(void)
{
  GPIO_ROWS.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOE, &GPIO_ROWS);

  GPIO_COLUM.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOE, &GPIO_COLUM);

  GPIO_ResetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
}
void setColumMode(void)
{
  GPIO_ROWS.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOE, &GPIO_ROWS);

  GPIO_COLUM.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOE, &GPIO_COLUM);

	GPIO_ResetBits(GPIOE, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);
}
char decode(int rowNumber, int columNumber)
{
		switch (rowNumber)
		{
				case 1:
					switch(columNumber)
					{
						case 1:
								return '1';
						case 2:
								return '2';
						case 3:
								return '3';
						case 4:
								return 'A';
					}
				case 2:
					switch(columNumber)
					{
						case 1:
								return '4';
						case 2:
								return '5';
						case 3:
								return '6';
						case 4:
								return 'B';
					}
			case 3:
				switch(columNumber)
				{
						case 1:
								return '7';
						case 2:
								return '8';
						case 3:
								return '9';
						case 4:
								return 'C';
				}
			case 4:
				switch(columNumber)
					{
						case 1:
								return '*';
						case 2:
								return '0';
						case 3:
								return '#';
						case 4:
								return 'D';
					}			
		}
    return '#';
}
int getRow(void)
{
  int rowNumber = 0;
  if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10) == 0)
  {
    setColumMode();
    rowNumber = 1;
  }
  else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11) == 0)
  {
    setColumMode();
    rowNumber = 2;
  }
  else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) == 0)
  {
    setColumMode();
    rowNumber = 3;
  }
  else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) == 0)
  {
    setColumMode();
    rowNumber = 4;
  }
  return rowNumber;
}

int getColumn(void)
{
  int columNumber = 0;
  if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
  {
    setRowMode();
    columNumber = 1;
  }
  else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == 0)
  {
    setRowMode();
    columNumber = 2;
  }
	else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 0)
	{
    setRowMode();
    columNumber = 3;
	}
  else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0)
  {
    setRowMode();
    columNumber = 4;
  }
  return columNumber;
}


/**
*@brief Program entry point
*/
void keypadThread(void const *args) 
{
  int rowNumber = 0, columNumber = 0;
  keypad_init();
  while(1)
  {
    osDelay(300);
    rowNumber = getRow();
    if (rowNumber != 0)
    {
      columNumber = getColumn();
    }
    if(rowNumber != 0 && columNumber != 0)
    {
      char c = decode(rowNumber, columNumber);
      printf("%c", c);
      rowNumber = 0;
      columNumber = 0;
    }
  }
}

osThreadId keypad_Thread_Create(void)
{
  keypadThreadID = osThreadCreate(osThread(keypadThread), NULL);
  return keypadThreadID;
}
