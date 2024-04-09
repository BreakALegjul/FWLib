#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H

#include "stm32f10x.h"

//���ʹ�ðԵ������壬�Ѹú����ó�1 ��ָ�������ó�0
#define USE_BD			1

/**************************SPI��������********************************/
#define             FLASH_SPIx                                SPI1
#define             FLASH_SPI_APBxClock_FUN                  RCC_APB2PeriphClockCmd
#define             FLASH_SPI_CLK                             RCC_APB2Periph_SPI1
#define             FLASH_SPI_GPIO_APBxClock_FUN            	RCC_APB2PeriphClockCmd



#define             FLASH_SPI_SCK_PORT                        GPIOA   
#define             FLASH_SPI_SCK_PIN                         GPIO_Pin_5

#define             FLASH_SPI_MOSI_PORT                        GPIOA 
#define             FLASH_SPI_MOSI_PIN                         GPIO_Pin_7

#define             FLASH_SPI_MISO_PORT                        GPIOA 
#define             FLASH_SPI_MISO_PIN                         GPIO_Pin_6

#if USE_BD
	#define             FLASH_SPI_GPIO_CLK                        RCC_APB2Periph_GPIOA
	
	#define             FLASH_SPI_CS_PORT                        GPIOA 
	#define             FLASH_SPI_CS_PIN                         GPIO_Pin_4
#else
	#define             FLASH_SPI_GPIO_CLK                        (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC)
	
	#define             FLASH_SPI_CS_PORT                        GPIOC
	#define             FLASH_SPI_CS_PIN                         GPIO_Pin_0
#endif


//CS��������
#define FLASH_SPI_CS_HIGH  		GPIO_SetBits(FLASH_SPI_CS_PORT,FLASH_SPI_CS_PIN);
#define FLASH_SPI_CS_LOW 		  GPIO_ResetBits(FLASH_SPI_CS_PORT,FLASH_SPI_CS_PIN);

/*�ȴ���ʱʱ��*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


/*��Ϣ���*/
#define FLASH_DEBUG_ON         0

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define DUMMY 						0x00	
#define READ_JEDEC_ID     0x9f																					
#define ERASE_SECTOR			0x20																			
#define READ_STATUS				0x05
#define READ_DATA					0x03		
#define WRITE_ENABLE      0x06																					
#define WRITE_DATA				0x02	

																					
void SPI_FLASH_Init(void);
uint8_t SPI_FLASH_Send_Byte(uint8_t data);
uint32_t SPI_FLASH_Read_ID(void);																					
uint32_t SPI_Read_ID(void);																			
void SPI_FLASH_Sector_Erase(uint32_t Erase_addr);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_Read_Data(uint32_t Address,uint8_t *ReadBuff,uint32_t numByteToRead);
void SPI_FLASH_Write_Enable(void);
void SPI_FLASH_Write_Data(uint32_t Address,uint8_t *WriteBuff,uint32_t numByteToWrite);	

#endif /* __BSP_SPI_FLASH_H */


