#ifndef SMARTRF_CC2500_H
#define SMARTRF_CC2500_H

#include "stdint.h"

/**
  * @brief  CC2500 SPI Interface pins
  */
#define CC2500_SPI                       SPI2
#define CC2500_SPI_CLK                   RCC_APB1Periph_SPI2

#define CC2500_SPI_SCK_PIN               GPIO_Pin_10                 /* PB.10 */
#define CC2500_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define CC2500_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define CC2500_SPI_SCK_SOURCE            GPIO_PinSource10
#define CC2500_SPI_SCK_AF                GPIO_AF_SPI2

#define CC2500_SPI_MISO_PIN              GPIO_Pin_2                  /* PC.02 */
#define CC2500_SPI_MISO_GPIO_PORT        GPIOC                       /* GPIOC */
#define CC2500_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define CC2500_SPI_MISO_SOURCE           GPIO_PinSource2
#define CC2500_SPI_MISO_AF               GPIO_AF_SPI2

#define CC2500_SPI_MOSI_PIN              GPIO_Pin_3                  /* PC.03 */
#define CC2500_SPI_MOSI_GPIO_PORT        GPIOC                       /* GPIOC */
#define CC2500_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define CC2500_SPI_MOSI_SOURCE           GPIO_PinSource3
#define CC2500_SPI_MOSI_AF               GPIO_AF_SPI2

#define CC2500_SPI_CS_PIN                GPIO_Pin_1                  /* PC.01 */
#define CC2500_SPI_CS_GPIO_PORT          GPIOC                       /* GPIOC */
#define CC2500_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOC

#define CC2500_SPI_INT_PIN              GPIO_Pin_4                  /* PC.04 */
#define CC2500_SPI_INT_GPIO_PORT        GPIOC                       /* GPIOC */
#define CC2500_SPI_INT_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define CC2500_SPI_INT_EXTI_LINE        EXTI_Line4
#define CC2500_SPI_INT_EXTI_PORT_SOURCE EXTI_PortSourceGPIOC
#define CC2500_SPI_INT_EXTI_PIN_SOURCE  EXTI_PinSource4
#define CC2500_SPI_INT_EXTI_IRQn        EXTI4_IRQn 

#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define CC2500_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define CC2500_TIMEOUT_ERROR        ((uint8_t)0x01)
#define CC2500_STATUS_ERROR         ((uint8_t)0x80)


#define CC2500_COMMAND_SRES         ((uint8_t)0x30)
#define CC2500_COMMAND_SCAL         ((uint8_t)0x33)
#define CC2500_COMMAND_SRX          ((uint8_t)0x34)
#define CC2500_COMMAND_STX          ((uint8_t)0x35)
#define CC2500_COMMAND_SIDLE        ((uint8_t)0x36)
#define CC2500_COMMAND_SNOP         ((uint8_t)0x3D)


#define CC2500_FIFO_ADDR            ((uint8_t)0x3F)

#define CC2500_REG_IOCFG2_ADDR      ((uint8_t)0x00)
#define CC2500_REG_IOCFG1_ADDR      ((uint8_t)0x01)
#define CC2500_REG_IOCFG0_ADDR      ((uint8_t)0x02)
#define CC2500_REG_FIFOTHR_ADDR     ((uint8_t)0x03)
#define CC2500_REG_SYNC1_ADDR       ((uint8_t)0x04)
#define CC2500_REG_SYNC0_ADDR       ((uint8_t)0x05)
#define CC2500_REG_PKTLEN_ADDR      ((uint8_t)0x06)
#define CC2500_REG_PKTCTRL1_ADDR    ((uint8_t)0x07)
#define CC2500_REG_PKTCTRL0_ADDR    ((uint8_t)0x08)
#define CC2500_REG_ADDR_ADDR        ((uint8_t)0x09)
#define CC2500_REG_CHANNR_ADDR      ((uint8_t)0x0A)
#define CC2500_REG_FSCTRL1_ADDR     ((uint8_t)0x0B)
#define CC2500_REG_FSCTRL0_ADDR     ((uint8_t)0x0C)
#define CC2500_REG_FREQ2_ADDR       ((uint8_t)0x0D)
#define CC2500_REG_FREQ1_ADDR       ((uint8_t)0x0E)
#define CC2500_REG_FREQ0_ADDR       ((uint8_t)0x0F)
#define CC2500_REG_MDMCFG4_ADDR     ((uint8_t)0x10)
#define CC2500_REG_MDMCFG3_ADDR     ((uint8_t)0x11)
#define CC2500_REG_MDMCFG2_ADDR     ((uint8_t)0x12)
#define CC2500_REG_MDMCFG1_ADDR     ((uint8_t)0x13)
#define CC2500_REG_MDMCFG0_ADDR     ((uint8_t)0x14)
#define CC2500_REG_DEVIATN_ADDR     ((uint8_t)0x15)
#define CC2500_REG_MCSM2_ADDR       ((uint8_t)0x16)
#define CC2500_REG_MCSM1_ADDR       ((uint8_t)0x17)
#define CC2500_REG_MCSM0_ADDR       ((uint8_t)0x18)
#define CC2500_REG_FOCCFG_ADDR      ((uint8_t)0x19)
#define CC2500_REG_BSCFG_ADDR       ((uint8_t)0x1A)
#define CC2500_REG_AGCTRL2_ADDR     ((uint8_t)0x1B)
#define CC2500_REG_AGCTRL1_ADDR     ((uint8_t)0x1C)
#define CC2500_REG_AGCTRL0_ADDR     ((uint8_t)0x1D)
#define CC2500_REG_WOREVT1_ADDR     ((uint8_t)0x1E)
#define CC2500_REG_WOREVT0_ADDR     ((uint8_t)0x1F)
#define CC2500_REG_WORCTRL_ADDR     ((uint8_t)0x20)
#define CC2500_REG_FREND1_ADDR      ((uint8_t)0x21)
#define CC2500_REG_FREND0_ADDR      ((uint8_t)0x22)
#define CC2500_REG_FSCAL3_ADDR      ((uint8_t)0x23)
#define CC2500_REG_FSCAL2_ADDR      ((uint8_t)0x24)
#define CC2500_REG_FSCAL1_ADDR      ((uint8_t)0x25)
#define CC2500_REG_FSCAL0_ADDR      ((uint8_t)0x26)
#define CC2500_REG_RCCTRL1_ADDR     ((uint8_t)0x27)
#define CC2500_REG_RCCTRL0_ADDR     ((uint8_t)0x28)
#define CC2500_REG_FSTEST_ADDR      ((uint8_t)0x29)
#define CC2500_REG_PTEST_ADDR       ((uint8_t)0x2A)
#define CC2500_REG_AGCTEST_ADDR     ((uint8_t)0x2B)
#define CC2500_REG_TEST2_ADDR       ((uint8_t)0x2C)
#define CC2500_REG_TEST1_ADDR       ((uint8_t)0x2D)
#define CC2500_REG_TEST0_ADDR       ((uint8_t)0x2E)


#define SMARTRF_RADIO_CC2500
#define SMARTRF_SETTING_IOCFG2      ((uint8_t)0x07) //0x29
#define SMARTRF_SETTING_IOCFG1      ((uint8_t)0x2E)
#define SMARTRF_SETTING_IOCFG0      ((uint8_t)0x06) //0x06
#define SMARTRF_SETTING_FIFOTHR     ((uint8_t)0x07) //0x07
#define SMARTRF_SETTING_SYNC1       ((uint8_t)0xD3)
#define SMARTRF_SETTING_SYNC0       ((uint8_t)0x91)
#define SMARTRF_SETTING_PKTLEN      ((uint8_t)0x0A) //0x0A // Packet Length of 10bytes (0xFF)
#define SMARTRF_SETTING_PKTCTRL1    ((uint8_t)0x04) //0x04
#define SMARTRF_SETTING_PKTCTRL0    ((uint8_t)0x0C) //0x05 // Fixed Packet Length (0x05)
#define SMARTRF_SETTING_ADDR        ((uint8_t)0x00) //0x00 // Global Broadcast Address
#define SMARTRF_SETTING_CHANNR      ((uint8_t)0x00) //0x00 // Channel 0
#define SMARTRF_SETTING_FSCTRL1     ((uint8_t)0x12) //0x0C//0x12 //Frequency offset = 457kHz
#define SMARTRF_SETTING_FSCTRL0     ((uint8_t)0x00) //0x00
#define SMARTRF_SETTING_FREQ2       ((uint8_t)0x5D) //0x5D //Carrier Frequency is 2.433GHz + 6*8KHz
#define SMARTRF_SETTING_FREQ1       ((uint8_t)0x94) //0x93  
#define SMARTRF_SETTING_FREQ0       ((uint8_t)0x2A) //0xB1
#define SMARTRF_SETTING_MDMCFG4     ((uint8_t)0x2C) //deault:0x8C //0x0E //0x2D // BW of channel = 541.666kHz
#define SMARTRF_SETTING_MDMCFG3     ((uint8_t)0x3B) //defaut:0x22 // Baud Rate = 125kb
#define SMARTRF_SETTING_MDMCFG2     ((uint8_t)0x72) //0x73 //before demodulator, MSK modulation, 16/16 sync word bits detected
#define SMARTRF_SETTING_MDMCFG1     ((uint8_t)0x42) //0x42 //mimnum 8 preamble bytes
#define SMARTRF_SETTING_MDMCFG0     ((uint8_t)0xF8) //0xF8 // Default Channel Spacing of 200kHz
#define SMARTRF_SETTING_DEVIATN     ((uint8_t)0x01) //0x00 //0x01 // 1785kHz
#define SMARTRF_SETTING_MCSM2       ((uint8_t)0x07)
#define SMARTRF_SETTING_MCSM1       ((uint8_t)0x30)
#define SMARTRF_SETTING_MCSM0       ((uint8_t)0x18) //default 0x04 //0x18 // Automatically calibrate When going from IDLE to RX or TX (or FSTXON) check CC2500 datasheet
#define SMARTRF_SETTING_FOCCFG      ((uint8_t)0x36) //0x1D // check datasheet
#define SMARTRF_SETTING_BSCFG       ((uint8_t)0x1C) //default 0x6A //0x1c
#define SMARTRF_SETTING_AGCCTRL2    ((uint8_t)0xc7) //default 0x03 //0xC7
#define SMARTRF_SETTING_AGCCTRL1    ((uint8_t)0x40) //0x00
#define SMARTRF_SETTING_AGCCTRL0    ((uint8_t)0xB0) //default 0x91
#define SMARTRF_SETTING_WOREVT1     ((uint8_t)0x87)
#define SMARTRF_SETTING_WOREVT0     ((uint8_t)0x6B)
#define SMARTRF_SETTING_WORCTRL     ((uint8_t)0xF8)
#define SMARTRF_SETTING_FREND1      ((uint8_t)0xB6) //default 0x56
#define SMARTRF_SETTING_FREND0      ((uint8_t)0x10) //0x10
#define SMARTRF_SETTING_FSCAL3      ((uint8_t)0xEA) //default 0xA9
#define SMARTRF_SETTING_FSCAL2      ((uint8_t)0x0A)
#define SMARTRF_SETTING_FSCAL1      ((uint8_t)0x00) //default 0x20
#define SMARTRF_SETTING_FSCAL0      ((uint8_t)0x19) //default 0x0D //0x11
#define SMARTRF_SETTING_RCCTRL1     ((uint8_t)0x41)
#define SMARTRF_SETTING_RCCTRL0     ((uint8_t)0x00)
#define SMARTRF_SETTING_FSTEST      ((uint8_t)0x59) //0x59
#define SMARTRF_SETTING_PTEST       ((uint8_t)0x7F)
#define SMARTRF_SETTING_AGCTEST     ((uint8_t)0x3F)
#define SMARTRF_SETTING_TEST2       ((uint8_t)0x88) //0x88
#define SMARTRF_SETTING_TEST1       ((uint8_t)0x31) //0x31
#define SMARTRF_SETTING_TEST0       ((uint8_t)0x0B) //0x0B


uint8_t CC2500_TIMEOUT_UserCallback(void);
void CC2500_Default_Init(void);
void CC2500_ReadRegister(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void CC2500_WriteRegister(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
uint8_t CC2500_WriteCommand(uint8_t WriteCommand, uint8_t ReadWriteFIFOFlag);
uint8_t CC2500_SRES_CMD(void);
uint8_t CC2500_SCAL_CMD(uint8_t ReadWriteFIFOFlag);
uint8_t CC2500_SRX_CMD(uint8_t ReadWriteFIFOFlag);
uint8_t CC2500_STX_CMD(uint8_t ReadWriteFIFOFlag);
uint8_t CC2500_SIDLE_CMD(uint8_t ReadWriteFIFOFlag);
uint8_t CC2500_SNOP_CMD(uint8_t ReadWriteFIFOFlag);
void CC2500_Write_TXFIFO(uint8_t* pbuffer, uint16_t NumByteToWrite);
void CC2500_Read_RXFIFO(uint8_t* pBuffer, uint16_t NumByteToRead);



#endif 
