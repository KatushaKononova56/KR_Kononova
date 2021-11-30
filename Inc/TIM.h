/*
 * TIM.h
 *
 *  Created on: 16 нояб. 2021 г.
 *      Author: Ekaterina
 */

#ifndef TIM_H_
#define TIM_H_


#include "stm32f0xx.h"
#include "global.h"
#include "struct.h"
#include "math.h"
#include "USART.h"

#define Three_metr 17400
#define Three_cm 174

void TIM2_IRQHandler();
void TIM2_For_HC_SR04_Horiz_init();
void TIM3_IRQHandler();
void TIM3_For_HC_SR04_Vert_init();
float Get_Distance(uint16_t time);
uint8_t Pit_YES_or_NO(float data);
float Get_Distance_without_temp(uint16_t time);

#endif /* TIM_H_ */
