#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "smartRF_CC2500.h"
#include "stdio.h"

void test_receive(void)
{
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
}

void test_send(void)
{
	uint8_t status,size, buffer[10] = {0,1,2,3,4,5,6,7,8,9};
  status = CC2500_STX_CMD(0);
  printf("%d\n", (uint16_t)status);
	while (1)
	{
		osDelay(1000);
		status = CC2500_SNOP_CMD(0);
		size = status & 0x0F;
		status &= 0xF0;
		printf("status = %d, size = %d\n", (uint16_t)status, (uint16_t)size);
		if (size == 0x0F)
		{
			CC2500_Write_TXFIFO(buffer, 10);
		}
		
		//printf("%d, %d, %d\n", (uint16_t) readbuffer[0], (uint16_t) readbuffer[1], (uint16_t) readbuffer[2]);
	}
}


/*!
 @brief Program entry point
 */
int main (void)
{
	//uint8_t writebuffer[3] = {66, 77, 88};
  //uint8_t readbuffer[3] = {0, 0, 0};
	CC2500_Default_Init();
	test_receive();
	//test_send();

}

