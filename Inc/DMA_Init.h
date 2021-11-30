#ifndef _DMA_Init_
#define _DMA_Init_

#include "stm32f0xx.h"
#include "global.h"

void DMA_USART(uint32_t *data, uint8_t size);
void DMA_USART_EN(uint32_t *data, uint8_t size);
void DMA_ADC(uint16_t *array_adc, uint8_t size);

#endif
