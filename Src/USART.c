#include "USART.h"

uint32_t data_for_usart[16];
uint8_t status_dma_usart=ready_transmit;

void USART1_GPIO_Init(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;
	GPIOA->AFR[1] |= (0x01 << GPIO_AFRH_AFSEL9_Pos) | (0x01 << GPIO_AFRH_AFSEL10_Pos); /* (4) */
}

void USART1_DMA_Init(){
	USART1_GPIO_Init();

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	USART1->BRR |= SystemCoreClock/BAUD; //выбираем скорость

	USART1->CR3 |= USART_CR3_OVRDIS;
	USART1->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;

	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE; //разрешаем отправку и прием
	while ((USART1->ISR & USART_ISR_TC) != USART_ISR_TC);
	USART1->ICR |= USART_ICR_TCCF; //clear TC flag

	USART1->CR1 |= USART_CR1_UE;
	DMA_USART(&data_for_usart[0], 2);
}

// на вход температура
void send_temp(float temperature){ // для отправки целой и дробной части отдельно
	if(status_dma_usart == not_ready_transmit){
		while(status_dma_usart == not_ready_transmit);
	}
	uint32_t data = temperature*100;
	data_for_usart[0]=(uint32_t)temperature;
	data_for_usart[1]=(uint32_t)(data%100);
	DMA_USART_EN(&data_for_usart[0], 2);
}
//на вход дистанция
void send_dist(float dist){ // для отправки 2 байтов
	if(status_dma_usart == not_ready_transmit){
		while(status_dma_usart == not_ready_transmit);
	}
	uint32_t data = dist*100;
	data_for_usart[0]=(uint32_t)((data >> 8) & 0xFF);
	data_for_usart[1]=(uint32_t)(data & 0xFF);
	DMA_USART_EN(&data_for_usart[0], 2);
}
