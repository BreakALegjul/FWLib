#include "led.h"

void GPIO_LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(LED1_GPIO_CLK|LED2_GPIO_CLK|LED3_GPIO_CLK, ENABLE);
	/* --------------------------------- 配置Red灯 --------------------------------- */
	GPIO_InitStruct.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
	/* --------------------------------- 配置Blue灯 -------------------------------- */
	GPIO_InitStruct.GPIO_Pin = LED3_GPIO_PIN;
	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);
	/* -------------------------------- 配置green灯 -------------------------------- */
	GPIO_InitStruct.GPIO_Pin = LED2_GPIO_PIN;
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
	/* --------------------------------- 关闭RGB灯 --------------------------------- */
	GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
	GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
	GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}	




