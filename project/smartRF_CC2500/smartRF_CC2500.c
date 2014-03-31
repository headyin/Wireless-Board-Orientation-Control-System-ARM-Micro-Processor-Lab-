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
void LIS302DL_Init()
{  
  /* Configure the low level interface ---------------------------------------*/
  CC2500_LowLevel_Init();
  
  /* Configure*/

  
  /* Write value to CC2500 CTRL_REG1 regsister */
}

/**
  * @brief  Reads a block of data from the CC2500 registers.
  * @param  pBuffer : pointer to the buffer that receives the data read from the CC2500.
  * @param  ReadAddr : CC2500's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the CC2500 registers.
  * @retval None
  */
void LIS302DL_ReadRegister(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
  //set the first bit of header to 1 (read)
  //if multiple bytes to read, set the secodn bit of header to 1 (burst)
  if(NumByteToRead > 0x01)
  {
    ReadAddr = (uint8_t) ((ReadAddr & CC2500_ADDRESS_MASK) | (READ_MODE | BURST_MODE));
  }
  else
  {
    ReadAddr = (uint8_t) ((ReadAddr & CC2500_ADDRESS_MASK) | (READ_MODE));
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
  * @brief  Writes several byte to the CC2500 registers.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the CC2500 registers.
  * @param  WriteAddr : CC2500's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void LIS302DL_WriteRegister(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  //set the first bit of header to 1 (read)
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

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval uint8_t 1 timeout
  */
uint8_t CC2500_TIMEOUT_UserCallback(void)
{
	return CC2500_TIMEOUT_ERROR;
}
