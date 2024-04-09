#include "bsp_usart.h"
/* -------------------------------- 嵌套中断配置函数 -------------------------------- */
//static void NVIC_Configeration(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}
/* --------------------------------- 串口配置函数 --------------------------------- */
void USART_Config(void)
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
	
	//NVIC_Configeration();
	//USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	USART_Cmd(DEBUG_USARTx,ENABLE);
}
/* -------------------------------- 发送一个字节数据 -------------------------------- */
void Usart_SendByte(USART_TypeDef* pUSARTx,uint8_t data)
{
	USART_SendData(pUSARTx,data);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET);
}
/* -------------------------------- 发送两个字节数据 -------------------------------- */
void Usart_SendHalfWord(USART_TypeDef* pUSARTx,uint16_t data)
{
	uint8_t Data_H,Data_L;
	Data_H = (data & 0xff00) >> 8;
	Data_L = data & 0xff;
	USART_SendData(pUSARTx,Data_H);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET);
	USART_SendData(pUSARTx,Data_L);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET);
}
/* -------------------------------- 发送8位数据的数组 ------------------------------- */
void Usart_SendArray(USART_TypeDef * pUSARTx,uint8_t * arr,uint8_t num)
{
	uint8_t i;
	for ( i = 0; i < num; i++)
	{
		Usart_SendByte(pUSARTx,arr[i]);
	}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET);
}
/* ---------------------------------- 发送字符串 --------------------------------- */
void Usart_SendStr(USART_TypeDef * pUSARTx,uint8_t * str)
{
	uint8_t i=0;
	do
	{
		Usart_SendByte(pUSARTx,*(str+i));
		i++;
	}while (*(str+i) != '\0');
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET);
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

