#ifndef ___SPI_H___
#define ___SPI_H___
#include "sys.h"
#include "user_config.h"

#ifdef SPI_ENABLE

/************************
 * SPI1_NSS		--	PA4 *
 * SPI1_SCK		--	PA5 *
 * SPI1_MISO	--	PA6 *
 * SPI1_MOSI	--	PA7 *
 ************************/
#define SPI1_RCC_GPIO_CLK_ENABLE() 			__HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_GPIO							GPIOA
#define SPI1_NSS_PIN						GPIO_PIN_4	
#define SPI1_SCK_PIN						GPIO_PIN_5	
#define SPI1_MISO_PIN						GPIO_PIN_6	
#define SPI1_MOSI_PIN						GPIO_PIN_7	

extern SPI_HandleTypeDef spi1_handler;  //SPI¾ä±ú

void SPI1_Init(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_ReadWriteByte(u8 TxData);

#endif
#endif
