#ifndef _GPIO_lib_
#define _GPIO_lib_

#include "stm32f0xx.h"
#include "global.h"
#include "struct.h"


void GPIOC_SR_PIN(int pin_num, int pin_state); //set or reset pin
void TIM6_For_Button_init();
void Button_init();
void Button_GPIOA_Init();
void GPIOC_DIODES_Init();



#endif
