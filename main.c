#include "stm32f10x.h"
#include "led.h"
#include "key.h"
#include "exti_key.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_dma_mtm.h"
#include "bsp_dma_mtp.h"
#include "bsp_iic.h"
#include "bsp_Spi_Flash.h"

uint8_t WriteBuff[4096];
uint8_t ReadBuff[4096];


void Delay(uint32_t count)
{
	for(;count!=0;count--);
}	
int main()
{
	uint16_t i;
	uint32_t ID;
	USART_Config();
	printf("这是一个FLASH读取函数测试\r\n");
	SPI_FLASH_Init();
	ID=SPI_FLASH_Read_ID();
	printf("ID=0x%x\r\n",ID);
	
	SPI_FLASH_Sector_Erase(0);
	for(i=0;i<25;i++)
	{
		WriteBuff[i]=i+25;
	}
	
	SPI_FLASH_Write_Data(0,WriteBuff,25);
	
	SPI_FLASH_Read_Data(0,ReadBuff,4096);	
	
	for(i=0;i<4096;i++)
	{
		printf("0x%x ",ReadBuff[i]);
		if(i%10 == 0)
		{
			printf("\r\n");
		}
	}
	SPI_FLASH_Sector_Erase(0);
	while(1)
	{	

	}
}

