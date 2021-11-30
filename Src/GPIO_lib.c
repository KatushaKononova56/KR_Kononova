/*
 * Buzzer.c
 *
 *  Created on: 23 окт. 2021 г.
 *      Author: Ekaterina
 */
#include "GPIO_lib.h"

extern Buffer Buf_Distance_Vert;
//режим работы сенсоров
extern uint8_t mode_HC_SR04_Vert;
extern uint8_t mode_HC_SR04_Horiz;
extern uint8_t modeButton;
extern uint8_t modeMCP9700;
extern uint8_t modeBuzzer;
extern uint8_t Work_Allowed; //разрешение работы ультразвуковых датчиков

void EXTI0_1_IRQHandler(){
	EXTI->PR |= EXTI_PR_PIF0;
	TIM6->CR1 |= TIM_CR1_CEN; // включение таймера
}


//прерывание по таймеру
void TIM6_DAC_IRQHandler(){
	TIM6->SR &= ~TIM_SR_UIF;
	static int ColvoClick=0;
	ColvoClick+=1;
	if(ColvoClick==1){ // если кнопка нажата 1й раз, то включаем прибор
		modeButton=Button_ON;
		modeMCP9700=MCP9700_ON;
		TIM6->CR1 &= ~TIM_CR1_CEN;
	}
	else { // если второй, то все отключаем
		BUZZER_OFF();
		modeButton=Button_OFF;
		Work_Allowed=Work_OFF;
		modeMCP9700=MCP9700_OFF;
		mode_HC_SR04_Vert=HC_SR04_Vert_OFF;
		mode_HC_SR04_Horiz=HC_SR04_Horiz_OFF;
		Buffer_flush(&Buf_Distance_Vert);
		ColvoClick=0;
		TIM6->CR1 &= ~TIM_CR1_CEN;
	}
}
 // инциализация кнопки
 void Button_init(){
	TIM6_For_Button_init();
	Button_GPIOA_Init();
 }

 /*Для исключения дребезга контактов по нажатию кнопки*/
 void TIM6_For_Button_init()
 {
 	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
 	TIM6->CR1 |= TIM_CR1_OPM; //one pulse mode
 	TIM6->PSC = 800-1;
 	TIM6->ARR = 100-1;

 	TIM6->DIER |= TIM_DIER_UIE;
 	NVIC_SetPriority(TIM6_DAC_IRQn, 3);
 	NVIC_EnableIRQ(TIM6_DAC_IRQn);
 }

void Button_GPIOA_Init(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &=~GPIO_MODER_MODER0;
	GPIOA->PUPDR|=GPIO_PUPDR_PUPDR0_1;
	SYSCFG->EXTICR[1]&=~(0x0F<<0); //external interrupt configuration register 1

	EXTI->IMR|= EXTI_IMR_IM0; // Interrupt mask register - turn off mask for interrupts
	EXTI->EMR|= EXTI_EMR_EM0; // Event mask register

	EXTI->RTSR|= EXTI_RTSR_RT0; // RTSR - rising trigger selection register(front front)

	NVIC_SetPriority(EXTI0_1_IRQn, 1); // setting the priority
	//самый высокий приоритет, чтобы по нажатию кнопки выключить и включить считывание пространства
	NVIC_EnableIRQ(EXTI0_1_IRQn);//Enabling an interrupt in the interrupt controller
}
