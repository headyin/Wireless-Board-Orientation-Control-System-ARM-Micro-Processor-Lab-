#include "keypad.h"

#include <stdio.h>
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "push_button.h"
#include "lcd16.h"
#include "stm32f4xx_conf.h"
#include "oModeTrans.h"


#define MAX_INPUT_ANGLES 50
#define MAX_VALUE 10000000

GPIO_InitTypeDef GPIO_ROWS;
GPIO_InitTypeDef GPIO_COLUM;
uint8_t startMode2 = 0;
uint8_t startPitch = 0;
uint8_t finishRoll = 0;
uint8_t finishPitch = 0;

int32_t rollAngle;
int32_t pitchAngle;
int32_t delta;

uint32_t totalAngles;
int32_t rolls[MAX_INPUT_ANGLES];
int32_t pitches[MAX_INPUT_ANGLES];
int32_t deltas[MAX_INPUT_ANGLES];

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
    return ' ';
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

uint8_t setValue(int32_t *value, char c)
{
  if (MAX_VALUE < *value)
  {
    return 0;
  }
  *value = (*value) * 10 + c -'0';
  return 1;
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
    osDelay(200);
    rowNumber = getRow();
    if (rowNumber != 0)
    {
      columNumber = getColumn();
    }
    if(rowNumber != 0 && columNumber != 0)
    {
      char c = decode(rowNumber, columNumber);
      //printf("%c", c);
      switch (c)
      {
        case 'A': set_next_mode(MODE_1); break;
        case 'B': set_next_mode(MODE_2); break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': 
        {
          if (getThreadToRun() == MODE_2)
          {
            if (finishRoll == 0)
            {
              if (setValue(&rollAngle,c))
              {
                lcd_write_char(c);
              }
            }
            else if (finishPitch == 0)
            {
              if (setValue(&pitchAngle,c))
              {
                lcd_write_char(c);
              }
            }
            else
            {
              if (setValue(&delta,c))
              {
                lcd_write_char(c);
              }
            }
          }
          break;
        }
        case '#': //enter
        {
          if (getThreadToRun() == MODE_2)
          {
            if (finishRoll == 0)
            {
              finishRoll = 1;
              lcd_display_string("Pitch Angle:", "");
            }
            else if (finishPitch == 0)
            {
              finishPitch = 1;
              lcd_display_string("Delta Time (s):", "");
            }
            else 
            {
              rollAngle = rollAngle % 181 - 90;
              pitchAngle = pitchAngle % 181 - 90;
              rolls[totalAngles] = rollAngle;
              pitches[totalAngles] = pitchAngle;
              deltas[totalAngles] = delta;
              totalAngles ++;
              startMode2 = 0;
            }
          }
          break;          
        }
        case '*': //reset
        {
          if (getThreadToRun() == MODE_2)
          {
            if (finishRoll == 0)
            {
              lcd_display_string("Roll Angle:", "");
              rollAngle = 0;
            }
            else if (finishPitch == 0)
            {
              lcd_display_string("Pitch Angle:", "");
              pitchAngle = 0;
            }
            else
            {
              lcd_display_string("Delta Time (s):", "");
              delta = 0;
            }
          }
          break;
        }
        case 'D':
        {
          if (getThreadToRun() == MODE_2)
          {
            req_tran_timed_angels(totalAngles, rolls, pitches, deltas);
            totalAngles = 0;
          }
          break;
        }
        default: break;
      }
      rowNumber = 0;
      columNumber = 0;
    }
    if (getThreadToRun() == MODE_2)
    {
      if (startMode2 == 0)
      {
        startMode2 = 1;
        finishRoll = 0;
        finishPitch = 0;
        lcd_display_string("Roll Angle:", "");
        rollAngle = 0;
        pitchAngle = 0;
        delta = 0;
      }
    }
    else
    {
      totalAngles = 0;
    }
  }
}

osThreadId keypad_Thread_Create(void)
{
  keypadThreadID = osThreadCreate(osThread(keypadThread), NULL);
  return keypadThreadID;
}
