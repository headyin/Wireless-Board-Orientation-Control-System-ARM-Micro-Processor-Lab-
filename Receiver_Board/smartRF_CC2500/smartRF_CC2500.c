#include "smartRF_CC2500.h"
#include "stm32f4xx.h"


/* Read mode: the R/W bit set to 1 */
#define READ_MODE              ((uint8_t)0x80) 
/* Burst mode: the burst bit set to 1 */ 
#define BURST_MODE           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)
/*  lower 6 bit for cc2500 address*/
#define CC2500_ADDRESS_MASK        ((uint8_t)0x3f)

__IO uint32_t  CC2500Timeout = CC2500_FLAG_TIMEOUT;
static void CC2500_LowLevel_Init(void);
static uint8_t CC2500_Chip_Select(void);
static uint8_t CC2500_SendByte(uint8_t byte);

/**
  * @brief  Set CC2500 Initialization.
  * @param  LIS302DL_Config_Struct: pointer to a LIS302DL_Config_TypeDef structure 
  *         that contains the configuration setting for the LIS302DL.
  * @retval None
  */
void CC2500_Default_Init(void)
{
	uint8_t buffer[0x2f];
	buffer[CC2500_REG_IOCFG2_ADDR] = SMARTRF_SETTING_IOCFG2;
	buffer[CC2500_REG_IOCFG1_ADDR] = SMARTRF_SETTING_IOCFG1;
	buffer[CC2500_REG_IOCFG0_ADDR] = SMARTRF_SETTING_IOCFG0;
	buffer[CC2500_REG_FIFOTHR_ADDR] = SMARTRF_SETTING_FIFOTHR;
	buffer[CC2500_REG_SYNC1_ADDR] = SMARTRF_SETTING_SYNC1;
	buffer[CC2500_REG_SYNC0_ADDR] = SMARTRF_SETTING_SYNC0;
	buffer[CC2500_REG_PKTLEN_ADDR] = SMARTRF_SETTING_PKTLEN;
	buffer[CC2500_REG_PKTCTRL1_ADDR] = SMARTRF_SETTING_PKTCTRL1;
	buffer[CC2500_REG_PKTCTRL0_ADDR] = SMARTRF_SETTING_PKTCTRL0;
	buffer[CC2500_REG_ADDR_ADDR] = SMARTRF_SETTING_ADDR;
	buffer[CC2500_REG_CHANNR_ADDR] = SMARTRF_SETTING_CHANNR;
	buffer[CC2500_REG_FSCTRL1_ADDR] = SMARTRF_SETTING_FSCTRL1;
	buffer[CC2500_REG_FSCTRL0_ADDR] = SMARTRF_SETTING_FSCTRL0;
	buffer[CC2500_REG_FREQ2_ADDR] = SMARTRF_SETTING_FREQ2;
	buffer[CC2500_REG_FREQ1_ADDR] = SMARTRF_SETTING_FREQ1;
	buffer[CC2500_REG_FREQ0_ADDR] = SMARTRF_SETTING_FREQ0;
	buffer[CC2500_REG_MDMCFG4_ADDR] = SMARTRF_SETTING_MDMCFG4;
	buffer[CC2500_REG_MDMCFG3_ADDR] = SMARTRF_SETTING_MDMCFG3;
	buffer[CC2500_REG_MDMCFG2_ADDR] = SMARTRF_SETTING_MDMCFG2;
	buffer[CC2500_REG_MDMCFG1_ADDR] = SMARTRF_SETTING_MDMCFG1;
	buffer[CC2500_REG_MDMCFG0_ADDR] = SMARTRF_SETTING_MDMCFG0;
	buffer[CC2500_REG_DEVIATN_ADDR] = SMARTRF_SETTING_DEVIATN;
	buffer[CC2500_REG_MCSM2_ADDR] = SMARTRF_SETTING_MCSM2;
	buffer[CC2500_REG_MCSM1_ADDR] = SMARTRF_SETTING_MCSM1;
	buffer[CC2500_REG_MCSM0_ADDR] = SMARTRF_SETTING_MCSM0;
	buffer[CC2500_REG_FOCCFG_ADDR] = SMARTRF_SETTING_FOCCFG;
	buffer[CC2500_REG_BSCFG_ADDR] = SMARTRF_SETTING_BSCFG;
	buffer[CC2500_REG_AGCTRL2_ADDR] = SMARTRF_SETTING_AGCCTRL2;
	buffer[CC2500_REG_AGCTRL1_ADDR] = SMARTRF_SETTING_AGCCTRL1;
	buffer[CC2500_REG_AGCTRL0_ADDR] = SMARTRF_SETTING_AGCCTRL0;
	buffer[CC2500_REG_WOREVT1_ADDR] = SMARTRF_SETTING_WOREVT1;
	buffer[CC2500_REG_WOREVT0_ADDR] = SMARTRF_SETTING_WOREVT0;
	buffer[CC2500_REG_WORCTRL_ADDR] = SMARTRF_SETTING_WORCTRL;
	buffer[CC2500_REG_FREND1_ADDR] = SMARTRF_SETTING_FREND1;
	buffer[CC2500_REG_FREND0_ADDR] = SMARTRF_SETTING_FREND0;
	buffer[CC2500_REG_FSCAL3_ADDR] = SMARTRF_SETTING_FSCAL3;
	buffer[CC2500_REG_FSCAL2_ADDR] = SMARTRF_SETTING_FSCAL2;
	buffer[CC2500_REG_FSCAL1_ADDR] = SMARTRF_SETTING_FSCAL1;
	buffer[CC2500_REG_FSCAL0_ADDR] = SMARTRF_SETTING_FSCAL0;
	buffer[CC2500_REG_RCCTRL1_ADDR] = SMARTRF_SETTING_RCCTRL1;
	buffer[CC2500_REG_RCCTRL0_ADDR] = SMARTRF_SETTING_RCCTRL0;
	buffer[CC2500_REG_FSTEST_ADDR] = SMARTRF_SETTING_FSTEST;
	buffer[CC2500_REG_PTEST_ADDR] = SMARTRF_SETTING_PTEST;
	buffer[CC2500_REG_AGCTEST_ADDR] = SMARTRF_SETTING_AGCTEST;
	buffer[CC2500_REG_TEST2_ADDR] = SMARTRF_SETTING_TEST2;
	buffer[CC2500_REG_TEST1_ADDR] = SMARTRF_SETTING_TEST1;
  buffer[CC2500_REG_TEST0_ADDR] = SMARTRF_SETTING_TEST0;

	
  /* Configure the low level interface ---------------------------------------*/
  CC2500_LowLevel_Init();
  
  /* Configure*/
  CC2500_SRES_CMD();
	
	CC2500_WriteRegister(buffer, CC2500_REG_IOCFG2_ADDR, 0x2f);
	
}

/**
  * @brief  Issue chip reset command.
  * @param  None
  * @retval uint8_t : 0(Ok) or CC2500_TIMEOUT_ERROR
  */
uint8_t CC2500_SRES_CMD(void)
{
  CC2500Timeout = CC2500_FLAG_TIMEOUT;
	CC2500_WriteCommand(CC2500_COMMAND_SRES, 0);
	while (GPIO_ReadInputDataBit(CC2500_SPI_MISO_GPIO_PORT, CC2500_SPI_MISO_PIN)
         == (uint8_t) Bit_SET)
  {
    if((CC2500Timeout--) == 0)
		{
			return CC2500_TIMEOUT_UserCallback();
		}
  }
	return 0;
}

/**
  * @brief  Command: calibrate frequency synthesizer and turn it off.
  * @param  uint8_t ReadWriteFIFOFlag: 1: return RX FIFO available bytes; 0: return TX FIFO available bytes
  * @retval uint8_t : chip status
  */
uint8_t CC2500_SCAL_CMD(uint8_t ReadWriteFIFOFlag)
{
	return CC2500_WriteCommand(CC2500_COMMAND_SCAL, ReadWriteFIFOFlag);
}

/**
  * @brief  Command: Flush the RX FIFO buffer. Only issueSFRXin IDLE or RXFIFO_OVERFLOW states
  * @param  uint8_t ReadWriteFIFOFlag: 1: return RX FIFO available bytes; 0: return TX FIFO available bytes
  * @retval uint8_t : chip status
  */
uint8_t CC2500_SFRX_CMD(uint8_t ReadWriteFIFOFlag)
{
	return CC2500_WriteCommand(CC2500_COMMAND_SFRX, ReadWriteFIFOFlag);
}


/**
  * @brief  Enable RX.
  * @param  uint8_t ReadWriteFIFOFlag: 1: return RX FIFO available bytes; 0: return TX FIFO available bytes
  * @retval uint8_t : chip status
  */
uint8_t CC2500_SRX_CMD(uint8_t ReadWriteFIFOFlag)
{
	return CC2500_WriteCommand(CC2500_COMMAND_SRX, ReadWriteFIFOFlag);
}

/**
  * @brief  Enable TX.
  * @param  uint8_t ReadWriteFIFOFlag: 1: return RX FIFO available bytes; 0: return TX FIFO available bytes
  * @retval uint8_t : chip status
  */
uint8_t CC2500_STX_CMD(uint8_t ReadWriteFIFOFlag)
{
	return CC2500_WriteCommand(CC2500_COMMAND_STX, ReadWriteFIFOFlag);
}

/**
  * @brief  Exit RX/TX.
  * @param  uint8_t ReadWriteFIFOFlag: 1: return RX FIFO available bytes; 0: return TX FIFO available bytes
  * @retval uint8_t : chip status
  */
uint8_t CC2500_SIDLE_CMD(uint8_t ReadWriteFIFOFlag)
{
	return CC2500_WriteCommand(CC2500_COMMAND_SIDLE, ReadWriteFIFOFlag);
}

/**
  * @brief  no operation, used to get chip status.
  * @param  uint8_t ReadWriteFIFOFlag: 1: return RX FIFO available bytes; 0: return TX FIFO available bytes
  * @retval uint8_t : chip status
  */
uint8_t CC2500_SNOP_CMD(uint8_t ReadWriteFIFOFlag)
{
	return CC2500_WriteCommand(CC2500_COMMAND_SNOP, ReadWriteFIFOFlag);
}


/**
  * @brief  Reads a block of data from the CC2500 registers.
  * @param  pBuffer : pointer to the buffer that receives the data read from the CC2500.
  * @param  ReadAddr : CC2500's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the CC2500 registers.
  * @retval None
  */
void CC2500_ReadRegister(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
	ReadAddr = (uint8_t) (ReadAddr & CC2500_ADDRESS_MASK);
  //set the first bit of header to 1 (read)
  //if multiple bytes to read or read status registers, set the second bit of header to 1 (burst)
  if((NumByteToRead > 0x01) || ((ReadAddr >= 0x30) && (ReadAddr <=0x3D)))
  {
    ReadAddr |= (uint8_t) (READ_MODE | BURST_MODE);
  }
  else
  {
    ReadAddr |= (uint8_t) READ_MODE;
  }
  /* Set chip select Low at the start of the transmission */
  if (CC2500_Chip_Select() == CC2500_TIMEOUT_ERROR)
  {
    //terminate the transaction and return
    CC2500_CS_HIGH();
    return;
  }
  
  /* Send the Address of the indexed register */
  CC2500_SendByte(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to LIS302DL (Slave device) */
    *pBuffer = CC2500_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
}


/**
  * @brief  Writes several byte to the CC2500 TX FIFO.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the CC2500 TX FIFO.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void CC2500_Write_TXFIFO(uint8_t* pbuffer, uint16_t NumByteToWrite)
{
  CC2500_WriteRegister(pbuffer, CC2500_FIFO_ADDR, NumByteToWrite);
}

/**
  * @brief  Read several byte from the CC2500 RX FIFO.
  * @param  pBuffer : pointer to the buffer  containing the data to be read from the CC2500 RX FIFO.
  * @param  NumByteToWrite: Number of bytes to read.
  * @retval None
  */
void CC2500_Read_RXFIFO(uint8_t* pBuffer, uint16_t NumByteToRead)
{
  CC2500_ReadRegister(pBuffer, CC2500_FIFO_ADDR, NumByteToRead);
}


/**
  * @brief  Writes several byte to the CC2500 registers.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the CC2500 registers.
  * @param  WriteAddr : CC2500's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void CC2500_WriteRegister(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  //set the first bit of header to 0 (write)
  //if multiple bytes to read, set the secodn bit of header to 1 (burst)
  if(NumByteToWrite > 0x01)
  {
    WriteAddr = (uint8_t) ((WriteAddr & CC2500_ADDRESS_MASK) | BURST_MODE);
  }
  else
  {
    WriteAddr = (uint8_t) (WriteAddr & CC2500_ADDRESS_MASK);
  }

  /* Set chip select Low at the start of the transmission */
  if (CC2500_Chip_Select() == CC2500_TIMEOUT_ERROR)
  {
    //terminate the transaction and return
    CC2500_CS_HIGH();
    return;
  }
  
  /* Send the Address of the indexed register */
  CC2500_SendByte(WriteAddr);

  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    CC2500_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
}

/**
  * @brief  Writes one CC2500 Command.
  * @param  WriteCommand : CC2500 Command.
  * @param  ReadWriteFIFOFlag : 1: return RX FIFO available bytes; 0: return TX FIFO available bytes
  * @retval uint8_t status
  */
uint8_t CC2500_WriteCommand(uint8_t WriteCommand, uint8_t ReadWriteFIFOFlag)
{
	uint8_t status;
	WriteCommand &= (uint8_t) CC2500_ADDRESS_MASK;
	if ((WriteCommand < 0x30) || (WriteCommand > 0x3D))
	{
		return CC2500_STATUS_ERROR;
  }
	if (ReadWriteFIFOFlag)
	{
		WriteCommand |= (uint8_t) 0x80;
	}

  /* Set chip select Low at the start of the transmission */
  if (CC2500_Chip_Select() == CC2500_TIMEOUT_ERROR)
  {
    //terminate the transaction and return
    CC2500_CS_HIGH();
    return CC2500_STATUS_ERROR;
  }
  
  /* Send the Address of the indexed register */
  status = CC2500_SendByte(WriteCommand);
  
  /* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
	
	return status;
}

/**
  * @brief  Set the CS pin to low and wait until SO pin goes low
  * @param  None
  * @retval None
  */
static uint8_t CC2500_Chip_Select(void)
{
  CC2500Timeout = CC2500_FLAG_TIMEOUT;
  CC2500_CS_LOW();
  while (GPIO_ReadInputDataBit(CC2500_SPI_MISO_GPIO_PORT, CC2500_SPI_MISO_PIN)
         == (uint8_t) Bit_SET)
  {
    if((CC2500Timeout--) == 0) return CC2500_TIMEOUT_UserCallback();
  }
  return 0;
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
static uint8_t CC2500_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  CC2500Timeout = CC2500Timeout;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((CC2500Timeout--) == 0) return CC2500_TIMEOUT_UserCallback();
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(CC2500_SPI, byte);
  
  /* Wait to receive a Byte */
  CC2500Timeout = CC2500Timeout;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((CC2500Timeout--) == 0) return CC2500_TIMEOUT_UserCallback();
  }
  
  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(CC2500_SPI);
}

/**
  * @brief  Initializes the low level interface used to drive the CC2500
  * @param  None
  * @retval None
  */
static void CC2500_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Enable the SPI periph */
  RCC_APB1PeriphClockCmd(CC2500_SPI_CLK, ENABLE);
  
  /* Enable SCK, MOSI, MISO, CS and INT GPIO clocks for CC2500*/
  RCC_AHB1PeriphClockCmd(CC2500_SPI_SCK_GPIO_CLK | CC2500_SPI_MOSI_GPIO_CLK |
                         CC2500_SPI_MISO_GPIO_CLK | CC2500_SPI_CS_GPIO_CLK |
                         CC2500_SPI_INT_GPIO_CLK, ENABLE);

  GPIO_PinAFConfig(CC2500_SPI_SCK_GPIO_PORT, CC2500_SPI_SCK_SOURCE, CC2500_SPI_SCK_AF);
  GPIO_PinAFConfig(CC2500_SPI_MISO_GPIO_PORT, CC2500_SPI_MISO_SOURCE, CC2500_SPI_MISO_AF);
  GPIO_PinAFConfig(CC2500_SPI_MOSI_GPIO_PORT, CC2500_SPI_MOSI_SOURCE, CC2500_SPI_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_SCK_PIN;
  GPIO_Init(CC2500_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  CC2500_SPI_MOSI_PIN;
  GPIO_Init(CC2500_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_MISO_PIN;
  GPIO_Init(CC2500_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(CC2500_SPI);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //SCK pin: low-level idle state
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //first edge (rising) is the MSBit capture strobe
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //not use here
  /* 
   pclk1 = hclk / 4 = 168MHz / 4 = 42MHz
   baud rate = pclk 1 / 8 = 5.25 MHZ
   */
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7; //reset value
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(CC2500_SPI, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(CC2500_SPI, ENABLE);

  /* Configure GPIO PIN for CC2500 Chip select */
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CC2500_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN);

  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_INT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(CC2500_SPI_INT_GPIO_PORT, &GPIO_InitStructure);
}

void CC2500_EXTI_Init(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI Line4 to PC04 pin */
  SYSCFG_EXTILineConfig(CC2500_SPI_INT_EXTI_PORT_SOURCE, CC2500_SPI_INT_EXTI_PIN_SOURCE);

  /* Configure EXTI Line4 */
  /* Pc04 is mapped to EXTI4 */
  EXTI_InitStructure.EXTI_Line = CC2500_SPI_INT_EXTI_LINE;
  /* Interupt mode */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  /* Rrigger interrupt on the rising edge*/
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* Enable EXTI Line1 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI Line0 Interrupt and the priority */
  NVIC_InitStructure.NVIC_IRQChannel = CC2500_SPI_INT_EXTI_IRQn;
  /* Group Priority 1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  /* Sub-priority 1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  /* Enable NVIC_IRQChannel */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval uint8_t 1 timeout
  */
uint8_t CC2500_TIMEOUT_UserCallback(void)
{
	return CC2500_TIMEOUT_ERROR;
}

/**
  * @brief  Get the RX FIFO size
  * @param  None.
  * @retval uint8_t RX FIFO size (bytes availabe to read)
  */
uint8_t getRxBufferSize(void)
{
  return CC2500_SNOP_CMD(1) & 0x0F;
}

/**
  * @brief  Get the TX FIFO size
  * @param  None.
  * @retval uint8_t TX FIFO size (bytes availabe to write)
  */
uint8_t getTxBufferSize(void)
{
  return CC2500_SNOP_CMD(0) & 0x0F;
}

/**
  * @brief  Check if the mode is RX or not
  * @param  None.
  * @retval uint8_t 1 RX, 0 not RX
  */
uint8_t isRxMode(void)
{
  return ((CC2500_SNOP_CMD(1) & 0x70) == 0x10);
}

/**
  * @brief  Check if the mode is RX or not
  * @param  None.
  * @retval uint8_t 1 RX, 0 not RX
  */
uint8_t isRx(uint8_t mode)
{
  return (mode == 0x10);
}

/**
  * @brief  Check if the mode is TX or not
  * @param  None.
  * @retval uint8_t 1 TX, 0 not TX
  */
uint8_t isTxMode(void)
{
  return ((CC2500_SNOP_CMD(0) & 0x70) == 0x20);
}

/**
  * @brief  Check if the mode is IDLE or not
  * @param  None.
  * @retval uint8_t 1 TX, 0 not TX
  */
uint8_t isIdleMode(void)
{
  return ((CC2500_SNOP_CMD(0) & 0x70) == 0x00);
}

/**
  * @brief  Check if the mode is IDLE or not
  * @param  None.
  * @retval uint8_t 1 TX, 0 not TX
  */
uint8_t isIdle(uint8_t mode)
{
  return (mode == 0x00);
}

/**
  * @brief  Check if the mode is RX buffer overflow or not
  * @param  None.
  * @retval uint8_t 1 TX, 0 not TX
  */
uint8_t isRXOFMode(void)
{
  return ((CC2500_SNOP_CMD(0) & 0x70) == 0x60);
}

/**
  * @brief  Check if the mode is RX buffer overflow or not
  * @param  None.
  * @retval uint8_t 1 TX, 0 not TX
  */
uint8_t isRXOF(uint8_t mode)
{
  return (mode == 0x60);
}
