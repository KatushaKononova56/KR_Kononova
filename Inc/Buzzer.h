/*
 * Buzzer.h
 *
 *  Created on: 23 окт. 2021 г.
 *      Author: Ekaterina
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "stm32f0xx.h"
#include "HC_SR04.h"
#include "global.h"

//PB2 - Buzzer


#define Perif_Buzzer GPIOB
#define RCC_Buzzer_ON() RCC->AHBENR |= RCC_AHBENR_GPIOBEN
#define Buzzer_output GPIO_MODER_MODER2_0
#define Buzzer_pull_down GPIO_PUPDR_PUPDR2_1




void Buzzer_init();
void GPIOB_Buzzer_init();
void BuzzerON_delay_OFF(uint32_t time);
void Buzzer_Work_On();

#endif /* BUZZER_H_ */
