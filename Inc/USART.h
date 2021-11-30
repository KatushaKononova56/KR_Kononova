#ifndef _USART_
#define _USART_

#include "stm32f0xx.h"
#include "DMA_Init.h"

/*Для  USART   TX - PA9,  RX - PA10 */

void USART1_DMA_Init();
void USART1_GPIO_Init();
void usart_start();
void send_temp(float temperature);
void send_data_start(float dist);
void send_dist_4(float dist, float dist_temp);
void send_dist(float dist);

#endif
