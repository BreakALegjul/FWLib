#ifndef __BSP_DMA_MTM_H
#define __BSP_DMA_MTM_H

#include "stm32f10x.h"

#define BUFFER_SIZE 32
#define MTM_DMA_CHANNEL 		DMA1_Channel6
#define MTM_DMA_FLAG_TC				DMA1_FLAG_TC6


extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];
void MTM_DMA_Config(void);
uint8_t Buffercmp(const uint32_t *pBuffer,uint32_t *pBuffer1,uint8_t bufferLength);
#endif  /* __BSP_DMA_MTM_H */

