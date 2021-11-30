/*
 * ADC.h
 *
 *  Created on: Oct 16, 2021
 *      Author: Ekaterina
 */

#ifndef SRC_ADC_H_
#define SRC_ADC_H_

#include "stm32f0xx.h"
#include "DMA_Init.h"

#define Size_adc 16

void GPIOC_init_adc();
void ADC_init();
void TIM15_init_TRG0();
void start_convert();
uint16_t ADC_middle_value();

#endif /* SRC_ADC_H_ */
