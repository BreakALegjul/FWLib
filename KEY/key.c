#include "key.h"

void GPIO_KEY_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(KEY_GPIOA_CLK | KEY_GPIOC_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_KEY1_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIO_KEY1_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_KEY2_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIO_KEY2_PORT,&GPIO_InitStruct);
}	

uint8_t KEY_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN)
{
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_PIN) == KEY_ON)
	{
		while(GPIO_ReadInputDataBit(GPIOx, GPIO_PIN) == KEY_ON);
		return KEY_ON;
	}
	else return KEY_OFF;
}




