#ifndef __BSP_DMA_MTP_H
#define __BSP_DMA_MTP_H

#include "stm32f10x.h"
#include <stdio.h>


#define  SendBuffer_Size								5000


#define  USART_TX_DMA_CLK               RCC_AHBPeriph_DMA1
#define  MTM_DMA_CHANNEL								DMA1_Channel4
#define  MTM_DMA_FLAG_TC								DMA2_FLAG_TC4
#define  USART1_DR_ADDRESS							(USART1_BASE+0x04)


#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO Òý½Åºê¶¨Òå
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler
extern uint8_t SendBuffer[SendBuffer_Size];
void USART_DMA_Config(void);
void MTP_DMA_Config(void);

#endif /*	__BSP_DMA_MTP_H	*/

