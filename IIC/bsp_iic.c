#include "bsp_iic.h"



void IIC_EEPROM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	/* --------------------------------- GPIO初始化 -------------------------------- */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/* --------------------------------- IIC初始化 --------------------------------- */
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = STM32_I2C_OWN_ADDR;
	I2C_Init(I2C1,&I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);
}
/* ------------------------------ 向EEPROM写入一个字节 ----------------------------- */
void EEPROM_Write_Byte(uint8_t Addr,uint8_t Data)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	I2C_SendData(I2C1, Addr);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
	I2C_SendData(I2C1, Data);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
	I2C_GenerateSTOP(I2C1, ENABLE);
}
/* ----------------------------- 向EEPROOM写入多个字节 ----------------------------- */
void EEPROM_Write_Page(uint8_t Addr,uint8_t *Data,uint8_t numByteToWrite)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	I2C_SendData(I2C1, Addr);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
	while(numByteToWrite)
	{
		I2C_SendData(I2C1, *Data);
		while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
		Data++;
		numByteToWrite--;
	}
	I2C_GenerateSTOP(I2C1, ENABLE);
}
/* ----------------------------- 从EEPROM读取一个字节数据 ---------------------------- */
void EEPROM_Read(uint8_t Addr,uint8_t *Data,uint8_t numByteToRead)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	I2C_SendData(I2C1, Addr);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);
	//第二次产生起始信号
	I2C_GenerateSTART(I2C1, ENABLE);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR);
	while(numByteToRead)
	{
		if(numByteToRead==1)
		{
			I2C_AcknowledgeConfig(I2C1, DISABLE);
		}
		while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR);
		*Data=I2C_ReceiveData(I2C1);
		Data++;
		numByteToRead--;
	}
	I2C_GenerateSTOP(I2C1, ENABLE);
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}
/* ----------------------------- 等待EEPROM内部时序完成 ----------------------------- */
void EEPROM_WaitForReadEnd(void)
{
	do
	{
		I2C_GenerateSTART(I2C1, ENABLE);
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_SB) == RESET);
		I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
	} 
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET);
	I2C_GenerateSTOP(I2C1, ENABLE);
}





