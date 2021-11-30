/*
 * MCP9700.h
 *
 *  Created on: Oct 15, 2021
 *      Author: Ekaterina
 */

#ifndef MCP9700_H_
#define MCP9700_H_

#include "stm32f0xx.h"
#include "ADC.h"
#include "USART.h"
#include "global.h"

/* Максимальное напряжение, приходящее с датчика, 1.75 В. Для АЦП это
 * 2172 отсчета, минимальное - 100 мВ и 124 отсчета. Составляя пропорции,
 *  и учитывая,что температура меняется от -40 до 125 градусов, получаем
 * параметры ниже. */



#define MIN_ADC 124
#define ADC_TEMP 0.081 //количество градусов на отсчет АДЦ

void MCP9700_On();
void MCP9700_init();
void MCP9700_get_temp();
void MCP9700_Convert_And_USART();
void MCP9700_Get_temperature(uint32_t data);
void delay(uint32_t tick);

#endif /* MCP9700_H_ */
