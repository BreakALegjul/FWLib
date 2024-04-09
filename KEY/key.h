#ifndef __KEY_H_
#define __KEY_H_

#include "stm32f10x.h"
#define KEY_GPIOA_CLK			RCC_APB2Periph_GPIOA
#define GPIO_KEY1_PORT 		GPIOA
#define GPIO_KEY1_PIN 		GPIO_Pin_0
#define KEY_GPIOC_CLK			RCC_APB2Periph_GPIOC
#define GPIO_KEY2_PORT 		GPIOC
#define GPIO_KEY2_PIN 		GPIO_Pin_13


#define KEY_ON     1
#define KEY_OFF    0

void GPIO_KEY_Config(void);
uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN);

#endif

