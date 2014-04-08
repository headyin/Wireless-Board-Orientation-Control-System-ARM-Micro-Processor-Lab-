#include "arm_math.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "smartRF_CC2500.h"
#include "wirelessTransmitter.h"
#include "stdio.h"

void test_receive(void)
{
/*
	uint8_t status,size, buffer;
  status = CC2500_SRX_CMD(1);
  printf("%d\n", (uint16_t)status);
	while (1)
	{
		osDelay(100);
		status = CC2500_SNOP_CMD(1);
		size = status & 0x0F;
		status &= 0xF0;
		if (size)
		{
			CC2500_Read_RXFIFO(&buffer, 1);
			printf("value = %d\n", (uint16_t)buffer);
		}
		printf("status = %d, size = %d\n", (uint16_t)status, (uint16_t)size);
		//printf("%d, %d, %d\n", (uint16_t) readbuffer[0], (uint16_t) readbuffer[1], (uint16_t) readbuffer[2]);
	}
*/
}

void test_send(void)
{
  uint8_t mode = 0;
  float roll = -90.5;
  while (1)
  {
    wireless_send(mode, roll, roll);
    mode = (mode + 1) % 5;
    roll = roll + 10;
    if (roll > 100) roll = - 90.5;
    osDelay(10);
  }
}


/*!
 @brief Program entry point
 */
int main (void)
{
  //for receiving
  wireless_Receiver_Thread_Create();


  //for sending:
  //CC2500_Default_Init();
  //CC2500_EXTI_Init();
  //test_send();
  
 // test_receive();
  osDelay(osWaitForever);
}

