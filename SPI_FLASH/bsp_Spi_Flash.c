#include "bsp_Spi_Flash.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;     

static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

static void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FLASH_SPI_GPIO_APBxClock_FUN(FLASH_SPI_GPIO_CLK, ENABLE);
	FLASH_SPI_APBxClock_FUN(FLASH_SPI_CLK, ENABLE);
	/* ------------------------------- SPI_SCK时钟配置 ------------------------------ */
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FLASH_SPI_SCK_PORT,&GPIO_InitStructure);
	/* ------------------------------- MOSI主机输出配置 ------------------------------- */
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(FLASH_SPI_MOSI_PORT,&GPIO_InitStructure);
	/* ------------------------------- MISO主机输入配置 ------------------------------- */
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(FLASH_SPI_MISO_PORT,&GPIO_InitStructure);
	/* ---------------------------------- CS配置 ---------------------------------- */
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(FLASH_SPI_CS_PORT,&GPIO_InitStructure);
	FLASH_SPI_CS_HIGH;
}

static void SPI_MODE_Config(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_CRCPolynomial = 0;
	SPI_Init(FLASH_SPIx, &SPI_InitStructure);
	SPI_Cmd(FLASH_SPIx, ENABLE);
}
/* ----------------------------- SPI_FLASH初始化函数 ----------------------------- */
void SPI_FLASH_Init(void)
{
	SPI_GPIO_Config();
	SPI_MODE_Config();
}
	
/* ------------------------------- 发送并接收一个字节数据 ------------------------------ */
uint8_t SPI_FLASH_Send_Byte(uint8_t data)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0)return  SPI_TIMEOUT_UserCallback(0);
	}
	SPI_I2S_SendData(FLASH_SPIx,data);
	SPITimeout = SPIT_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0)return  SPI_TIMEOUT_UserCallback(0);
	}
	return SPI_I2S_ReceiveData(FLASH_SPIx);
}

uint8_t SPI_FLASH_Read_Byte(void)
{
	return SPI_FLASH_Send_Byte(DUMMY);
}
/* ------------------------------- 读取FLASH_ID ------------------------------- */
uint32_t SPI_FLASH_Read_ID(void)
{
	uint32_t FLASH_ID;
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(READ_JEDEC_ID);
	FLASH_ID = SPI_FLASH_Send_Byte(DUMMY);
	FLASH_ID <<= 8;
	FLASH_ID |= SPI_FLASH_Send_Byte(DUMMY);
	FLASH_ID <<= 8;
	FLASH_ID |= SPI_FLASH_Send_Byte(DUMMY);
	FLASH_SPI_CS_HIGH;
	return FLASH_ID;
}
/* ------------------------------- 使能向FLASH内写入 ------------------------------ */
void SPI_FLASH_Write_Enable(void)
{
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(WRITE_ENABLE);
	FLASH_SPI_CS_HIGH;
}


/* ------------------------------- 擦除FLASH指定扇区 ------------------------------ */
void SPI_FLASH_Sector_Erase(uint32_t Erase_addr)
{
	SPI_FLASH_Write_Enable();
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(ERASE_SECTOR);
	SPI_FLASH_Send_Byte((Erase_addr>>16)&0xff);
	SPI_FLASH_Send_Byte((Erase_addr>>8)&0xff);
	SPI_FLASH_Send_Byte(Erase_addr&0xff);
	FLASH_SPI_CS_HIGH;
	SPI_FLASH_WaitForWriteEnd();
}

/* -------------------------------- 读取FLASH内容 ------------------------------- */
void SPI_FLASH_Read_Data(uint32_t Address,uint8_t *ReadBuff,uint32_t numByteToRead)
{
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(READ_DATA);
	SPI_FLASH_Send_Byte((Address>>16)&0xff);
	SPI_FLASH_Send_Byte((Address>>8)&0xff);
	SPI_FLASH_Send_Byte(Address&0xff);
	while(numByteToRead--)
	{
		*ReadBuff = SPI_FLASH_Send_Byte(DUMMY);
		ReadBuff++;
	}
	FLASH_SPI_CS_HIGH;
}

/* ------------------------------- 向FLASH内写入数据 ------------------------------ */
void SPI_FLASH_Write_Data(uint32_t Address,uint8_t *WriteBuff,uint32_t numByteToWrite)
{
	SPI_FLASH_Write_Enable();
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(WRITE_DATA);
	SPI_FLASH_Send_Byte((Address>>16)&0xff);
	SPI_FLASH_Send_Byte((Address>>8)&0xff);
	SPI_FLASH_Send_Byte(Address&0xff);
	while(numByteToWrite--)
	{
		SPI_FLASH_Send_Byte(*WriteBuff);
		WriteBuff++;
	}
	FLASH_SPI_CS_HIGH;
	SPI_FLASH_WaitForWriteEnd();
}
/* ------------------------------- 等待FLASH内部时序完成 ------------------------------ */
void SPI_FLASH_WaitForWriteEnd(void)
{
	uint8_t Status_reg = 0;
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(READ_STATUS);
	do{
		Status_reg = SPI_FLASH_Send_Byte(DUMMY);
	}
	while((	Status_reg & 0x01) == 1);	
	FLASH_SPI_CS_HIGH;
}


/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示SPI读取失败.
  */
static  uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  
  return 0;
}
