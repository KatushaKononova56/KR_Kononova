/*
 * ADC.c
 *
 *  Created on: Oct 16, 2021
 *      Author: Ekaterina
 */


#include "ADC.h"

extern void delay(uint32_t tick);
extern uint8_t status_get_data;

uint16_t data_adc_DMA[Size_adc];

// настройка выбор РС0 на аналоговый вход
void GPIOC_init_adc(){
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER0;
}

// настройка таймера для АЦП
void TIM15_init_TRG0() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
	TIM15->CR1 |= TIM_CR1_ARPE;
	TIM15->ARR = 80-1;
	TIM15->PSC = 10-1;
	TIM15->CR2 |= TIM_CR2_MMS_1;
	TIM15->CR1 |= TIM_CR1_CEN;
}

void ADC_init(){
	GPIOC_init_adc();
	TIM15_init_TRG0();
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	// выбираем тактирование АЦП от генератора на 14 МГц
	RCC->CR2 |= RCC_CR2_HSI14ON;
	while ((RCC->CR2 & RCC_CR2_HSI14RDY) != RCC_CR2_HSI14RDY);
	ADC1->SMPR |= ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;
	// выбор канала
	ADC1->CHSELR = ADC_CHSELR_CHSEL10;
	ADC1->CFGR1|=ADC_CFGR1_EXTEN_0; // rising edge
	ADC1->CFGR1 |= ADC_CFGR1_EXTSEL_2;//TIM15_TRGO
	//подключение к DMA
	ADC1->CFGR1 |= ADC_CFGR1_DMACFG;
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN;
	DMA_ADC(&data_adc_DMA[0],Size_adc);
	ADC1->CR |= ADC_CR_ADEN;
	ADC1->CR |= ADC_CR_ADSTART;
}

void start_convert(){
	//  заново записываем данные с ДМА, и запускаем
	DMA1_Channel1->CCR &= ~DMA_CCR_EN;
	DMA1_Channel1->CMAR = (uint32_t)(data_adc_DMA);
	DMA1_Channel1->CNDTR = Size_adc;
	DMA1_Channel1->CCR |= DMA_CCR_EN;
	ADC1->CR |= ADC_CR_ADEN;
	ADC1->CR |= ADC_CR_ADSTART;
	status_get_data=not_ready_data;
}


// возвращает среднее значение пришедших данных с АЦП
uint16_t ADC_middle_value(){
	// считаем среднее значение
	uint32_t sum=0;
	for (int i=0; i<Size_adc; i++){
		sum+=data_adc_DMA[i];
		delay(10);
	}
	sum=sum/Size_adc;

	return (uint16_t)sum;
}
