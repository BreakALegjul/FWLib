#include "RccClkConfig.h"

void HSE_SetSysClock(uint32_t RCC_PLLMul)
{
	__IO uint32_t HSEStartUpStatus = 0;
	
	RCC_DeInit();
	//HSE初始化
	RCC_HSEConfig(RCC_HSE_ON);
	//读取HSE初始化状态位
	HSEStartUpStatus=RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus = SUCCESS)
	{
		//使能FLASH 预存缓冲区	
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		//二分频	
		FLASH_SetLatency(FLASH_Latency_2);
		//看RCC时钟表
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		//设置所使用的时钟来源	
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul);
		//使能PLL时钟
		RCC_PLLCmd(ENABLE);
		//等待初始化成功
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
			//配置PLL时钟作为系统时钟
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
			//等待初始化成功
			while(RCC_GetSYSCLKSource()!=0x08);
		}
	}
	else 
	{
		//此处写初始化失败所执行的代码
	}
}
