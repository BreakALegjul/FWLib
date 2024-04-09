#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "stm32f10x.h"


//EEPROM 设备地址（8位）
#define EEPROM_ADDR   0xA0

/** 
* IIC EERPOM的引脚定义
*
*/
#define STM32_I2C_OWN_ADDR     0x5f 





void IIC_EEPROM_Config(void);
void EEPROM_Write_Byte(uint8_t Addr,uint8_t Data);
void EEPROM_Write_Page(uint8_t Addr,uint8_t *Data,uint8_t numByteToWrite);
void EEPROM_Read(uint8_t Addr,uint8_t *Data,uint8_t numByteToRead);
void EEPROM_WaitForReadEnd(void);
#endif /*__BSP_IIC_H*/

