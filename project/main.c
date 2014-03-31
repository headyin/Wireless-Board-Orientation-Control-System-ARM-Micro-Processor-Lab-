#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "smartRF_CC2500.h"
#include "stdio.h"



/*!
 @brief Program entry point
 */
int main (void)
{
	uint8_t writebuffer[3] = {66, 77, 88};
  uint8_t readbuffer[3] = {0, 0, 0};
  uint8_t status;
	CC2500_Init();
	while (1)
	{
		osDelay(1000);
		status = CC2500_SRES_CMD();
		printf("%d\n", (uint16_t) status);
		osDelay(1000);
		CC2500_WriteRegister(writebuffer, 0x2c, 3);
		CC2500_ReadRegister(readbuffer, 0x2c, 3);
		printf("%d, %d, %d\n", (uint16_t) readbuffer[0], (uint16_t) readbuffer[1], (uint16_t) readbuffer[2]);
	}
}

