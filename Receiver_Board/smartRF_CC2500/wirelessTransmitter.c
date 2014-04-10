#include "wirelessTransmitter.h"
#include "smartRF_CC2500.h"
#include "stm32f4xx_exti.h"
#include "cmsis_os.h"
#include "string.h"

#include "stdio.h"
#include "servo_motor/servo_motor.h"

#define PACKET_LENGTH 10  //10 bytes
#define RECEIVER_WAIT_TIMEOUT ((uint32_t) 10000)

void wireless_Receiver_Thread(void const * argument);

//wireless threa definition
osThreadDef(wireless_Receiver_Thread, osPriorityHigh, 1, 0);
osThreadId wireless_Receiver_thread_id;

uint32_t failedCount = 0;


/**
  * @brief  Clear Rx FIFO
  * @param  None
  * @retval None
  */
void clearRxBuffer(void)
{
  uint8_t buffer[15];
  uint8_t rxBufferSize = getRxBufferSize();
  while (rxBufferSize)
  {
    CC2500_Read_RXFIFO(buffer, rxBufferSize);
		rxBufferSize = getRxBufferSize();
  }
}


/**
  * @brief  create a thread for wireless transimssion (receiving only)
  * @param  None
  * @retval The thread ID for accelerometer
  */
osThreadId  wireless_Receiver_Thread_Create(void)
{
  wireless_Receiver_thread_id = osThreadCreate(osThread(wireless_Receiver_Thread), NULL);
  return wireless_Receiver_thread_id;
}

/**
  * @brief  wireless receiver thread main routine
  * @param  None
  * @retval None
  */
void wireless_Receiver_Thread(void const * argument)
{
  uint8_t buffer[PACKET_LENGTH];
  float rollAngle, pitchAngle;
  uint8_t mode;
  uint8_t state = 0;
Restart:  
  //init wireless transimitter
  CC2500_Default_Init();
  //CC2500_EXTI_Init();
  //clear RX FIFO
  clearRxBuffer();
  //enable RX mode
  CC2500_SRX_CMD(1);
  
  while (1)
  {
    switch (state)
    {
      case 0:
      {
        if (getRxBufferSize() >= 1)
        {
          CC2500_Read_RXFIFO(&mode, 1);
          if (mode == 0xFF)
          {
            state = 1;
          }
        }
        break;
      }
      case 1:
      {
        if (getRxBufferSize() >= 1)
        {
          CC2500_Read_RXFIFO(&mode, 1);
          if (mode == 0xFF)
          {
            state = 2;
          }
          else
          {
            state = 0;
          }
        }
        break;
      }
      case 2:
      {
        if (getRxBufferSize() >= PACKET_LENGTH)
        {
          CC2500_Read_RXFIFO(buffer, PACKET_LENGTH);
          memcpy(&rollAngle, buffer, 4);
          memcpy(&pitchAngle, buffer + 4, 4);
          if((rollAngle <= 90) && (rollAngle >= -90))
          {
            servo_motor_update_roll(rollAngle);
          }
          if((pitchAngle <= 90) && (pitchAngle >= -90))
          {
            servo_motor_update_pitch(pitchAngle);
          }
          state = 0;
        }
      }
      break;
      default: break;
    }
    mode = (CC2500_SNOP_CMD(0) & 0x70);
    if (!isRx(mode))
    {
      printf("Not in RX mode: status = %d\n", (uint16_t) CC2500_SNOP_CMD(1));
      if (isRXOF(mode))
      {
        CC2500_SFRX_CMD(1);
      }
      else if (isIdle(mode))
      {
        CC2500_SRX_CMD(1);
      }
      else
      {
        failedCount++;
        if (failedCount == RECEIVER_WAIT_TIMEOUT)
        {
          failedCount = 0;
          goto Restart;
        }
      }
    }
  }
}

 /**
 * @brief  Send mode and angles data through wireless transmitter
 * @param  uint8_t mode
 * @param  float rollAngle
 * @param  float pitchAngle
 * @retval None
 */
void wireless_send(uint8_t mode, float rollAngle, float pitchAngle)
{
  uint8_t buffer[PACKET_LENGTH];
  uint16_t timeOut = 500;
  buffer[0] = 0;
  buffer[1] = mode;
  memcpy(buffer + 2, &rollAngle, 4);
  memcpy(buffer + 6, &pitchAngle, 4);

  //enable TX mode
  CC2500_STX_CMD(0);
  
  while ((!isTxMode()) && (timeOut))
  {
    timeOut --;
  }
  if (isTxMode())
  {
    CC2500_Write_TXFIFO(buffer, PACKET_LENGTH);
    printf("send data\n");
  }
  else
  {
    printf("Not in TX mode: status = %d\n", (uint16_t) CC2500_SNOP_CMD(1));
  }
}

 /**
 * @brief  This function handles the EXTI4 interrupt (packet reveived)
 * @param  None
 * @retval None
 */
void EXTI4_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line4) == RESET) return;
  //osSignalSet (wireless_Receiver_thread_id, WT_PACKET_RECEIVED_SIG);
  /* Clear the EXTI line 4 pending bit */
  EXTI_ClearITPendingBit(EXTI_Line4);
}
