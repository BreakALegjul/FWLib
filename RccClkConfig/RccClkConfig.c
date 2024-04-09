#include "RccClkConfig.h"

void HSE_SetSysClock(uint32_t RCC_PLLMul)
{
	__IO uint32_t HSEStartUpStatus = 0;
	
	RCC_DeInit();
	//HSE��ʼ��
	RCC_HSEConfig(RCC_HSE_ON);
	//��ȡHSE��ʼ��״̬λ
	HSEStartUpStatus=RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus = SUCCESS)
	{
		//ʹ��FLASH Ԥ�滺����	
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		//����Ƶ	
		FLASH_SetLatency(FLASH_Latency_2);
		//��RCCʱ�ӱ�
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		//������ʹ�õ�ʱ����Դ	
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul);
		//ʹ��PLLʱ��
		RCC_PLLCmd(ENABLE);
		//�ȴ���ʼ���ɹ�
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
			//����PLLʱ����Ϊϵͳʱ��
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
			//�ȴ���ʼ���ɹ�
			while(RCC_GetSYSCLKSource()!=0x08);
		}
	}
	else 
	{
		//�˴�д��ʼ��ʧ����ִ�еĴ���
	}
}
