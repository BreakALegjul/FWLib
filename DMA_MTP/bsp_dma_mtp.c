#include "bsp_dma_mtp.h"

uint8_t SendBuffer[SendBuffer_Size];

void USART_DMA_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_Init(DEBUG_USARTx,&USART_InitStructure);
	
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	USART_Cmd(DEBUG_USARTx,ENABLE);
}

/* ------------------------------- 重定向printf函数 ------------------------------ */
int fputc(int ch,FILE *f)
{
	USART_SendData(DEBUG_USARTx,(uint8_t) ch);
	while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_TXE) == RESET);
	return (ch);
}
/* ------------------------------- 重定向scanf函数 ------------------------------- */
int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(DEBUG_USARTx);
}



void MTP_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(USART_TX_DMA_CLK, ENABLE);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART1_DR_ADDRESS;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SendBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	
	DMA_InitStructure.DMA_BufferSize = SendBuffer_Size;

	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(MTM_DMA_CHANNEL, &DMA_InitStructure);
	DMA_ClearFlag(MTM_DMA_FLAG_TC);
	DMA_Cmd(MTM_DMA_CHANNEL, ENABLE);
}
