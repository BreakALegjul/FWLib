#include "bsp_dma_mtm.h"


const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};

uint32_t aDST_Buffer[BUFFER_SIZE];

void MTM_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)aDST_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;

	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	DMA_Init(MTM_DMA_CHANNEL, &DMA_InitStructure);
	DMA_ClearFlag(MTM_DMA_FLAG_TC);
	DMA_Cmd(MTM_DMA_CHANNEL, ENABLE);
}
/* ------------------------------- 判断两个数组是否一样 ------------------------------- */
uint8_t Buffercmp(const uint32_t *pBuffer,uint32_t *pBuffer1,uint8_t bufferLength)
{
	while(bufferLength--)
	{
		if(*pBuffer!=*pBuffer1)
			return 0;
		*pBuffer++;
		*pBuffer1++;
	}
	return 1;
}

