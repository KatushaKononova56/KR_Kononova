/*
 * Buzzer.c
 *
 *  Created on: 23 окт. 2021 г.
 *      Author: Ekaterina
 */
#include "DMA_Init.h"

extern uint8_t status_get_data;
extern uint8_t status_dma_usart;
extern uint8_t status_convertion;


void DMA1_CH1_IRQHandler(){
	DMA1->IFCR |= DMA_IFCR_CTCIF1;
	status_get_data=ready_data;// устанавливаем готовность данных
	status_convertion = convertion_no; // говорим, что конвертация завершена
}

void DMA1_CH2_3_DMA2_CH1_2_IRQHandler(){
	DMA1->IFCR |= DMA_IFCR_CTCIF2;
	status_dma_usart = ready_transmit; // переделись все биты
}

// на вход подается массив, из которого отправлять информацию, и его размер
void DMA_USART(uint32_t *data, uint8_t size){
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel2->CPAR = (uint32_t)(&(USART1->TDR));
	DMA1_Channel2->CMAR = (uint32_t)(&data[0]);
	DMA1_Channel2->CNDTR = size;
	DMA1_Channel2->CCR |= DMA_CCR_PL_0;
	DMA1_Channel2->CCR |= DMA_CCR_MSIZE_1;
	DMA1_Channel2->CCR |= DMA_CCR_PSIZE_1;
	DMA1_Channel2->CCR |= DMA_CCR_MINC;
	DMA1_Channel2->CCR |= DMA_CCR_DIR;//read from memory

	SYSCFG->CFGR1 &= (~SYSCFG_CFGR1_USART1TX_DMA_RMP);
	DMA1_Channel2->CCR |= DMA_CCR_TCIE;
	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 5);
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
}

// включение передачт по усрат, на вход также массив и его размер
void DMA_USART_EN(uint32_t *data, uint8_t size){
	DMA1_Channel2->CCR &=~DMA_CCR_EN;
	DMA1_Channel2->CMAR = (uint32_t)(&data[0]);
	DMA1_Channel2->CNDTR = size;
	DMA1_Channel2->CCR |=  DMA_CCR_EN;
	status_dma_usart = not_ready_transmit;
}


// для адц, на вход подается массив, куда записывать данные с ацп, и его размер
void DMA_ADC(uint16_t *array_adc, uint8_t size){
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel1->CPAR = (uint32_t)(&ADC1->DR);
	DMA1_Channel1->CMAR = (uint32_t)(array_adc);
	DMA1_Channel1->CNDTR = size;
	DMA1_Channel1->CCR |= DMA_CCR_PL_1; //priority = high
	DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR_MINC;
	SYSCFG->CFGR1 &= ~SYSCFG_CFGR1_ADC_DMA_RMP; //needed for ADC requests
	DMA1_Channel1->CCR |= DMA_CCR_TCIE;
	NVIC_SetPriority(DMA1_Channel1_IRQn, 5);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

