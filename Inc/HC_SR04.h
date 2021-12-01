/*
 * HC_SR04.h
 *
 *  Created on: 30 сент. 2021 г.
 *      Author: Ekaterina
 */

#ifndef HC_SR04_H_
#define HC_SR04_H_

#include "stm32f0xx.h"
#include "global.h"
#include "struct.h"
#include "TIM.h"

/*для ультрозвуковых датчиков 4 пина:
	PA2 - Trig горизонтальный
	PA1 - Echo горизонтальный
	PA5 - Trig вертикальный
	PA6 -  Echo вертикальный*/

#define Perif_HC_SR04_for_trig GPIOA
#define Perif_HC_SR04_ON_for_trig() RCC->AHBENR|= RCC_AHBENR_GPIOAEN;

#define Perif_HC_SR04_for_echo GPIOA
#define Perif_HC_SR04_ON_for_echo() RCC->AHBENR|= RCC_AHBENR_GPIOAEN;


#define Pin_Trig_Horizontal 2
#define Pin_Echo_Horizontal 1
#define Pin_Trig_Vertical 5
#define Pin_Echo_Vertical 6

#define Trig_Horizontal_Init_Output (GPIO_MODER_MODER2_0)
#define Echo_Horizontal_Init_AF (GPIO_MODER_MODER1_1)
#define Trig_Vertical_Init_Output (GPIO_MODER_MODER5_0)
#define Echo_Vertical_Init_AF (GPIO_MODER_MODER6_1)

#define Echo_Vert_PULL_down (GPIO_PUPDR_PUPDR1_1)
#define Echo_Horiz_PULL_down (GPIO_PUPDR_PUPDR6_1)

#define AF_init_echo() Perif_HC_SR04_for_echo->AFR[0] |= 1<<5 | 1<<24;

#define ODR_Trig_Horizontal GPIO_ODR_2
#define ODR_Trig_Vertical GPIO_ODR_5

#define pinTRIG_Horiz_LOW() Perif_HC_SR04_for_trig->ODR&=~ODR_Trig_Horizontal
#define pinTRIG_Horiz_HIGH() Perif_HC_SR04_for_trig->ODR|=ODR_Trig_Horizontal

#define pinTRIG_Vert_LOW() Perif_HC_SR04_for_trig->ODR&=~ODR_Trig_Vertical
#define pinTRIG_Vert_HIGH() Perif_HC_SR04_for_trig->ODR|=ODR_Trig_Vertical

#define TIM2_for_Horiz_ON() TIM2->CR1 |= TIM_CR1_CEN
#define TIM3_for_Vert_ON() TIM3->CR1 |= TIM_CR1_CEN

#define TIM2_CH1_EN() TIM2->CCER |= TIM_CCER_CC1E
#define TIM2_CH2_EN() TIM2->CCER |= TIM_CCER_CC2E

#define TIM3_CH1_EN() TIM3->CCER |= TIM_CCER_CC1E
#define TIM3_CH2_EN() TIM3->CCER |= TIM_CCER_CC2E

void HC_SR04_init();
void HC_SR04_GPIOC_Init();
void HC_SR04_Horiz_Work_ON();
void HC_SR04_Vert_Work_ON();
void HC_SR04_ON();
void Struct_For_HC_SR04_Init();
void CHECK_HC_SR04_ON();


#endif /* HC_SR04_H_ */
