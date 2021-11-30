/*
 * global.h
 *
 *  Created on: 12 нояб. 2021 г.
 *      Author: Ekaterina
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

/* Датчик температуры: PC0
 *
 *
 * Ультрозвуковые датчики
 * горизонтальный:
 * 				Echo - PA1
 * 				Trig - PA2
 * вертикальный:
 *  			Echo - PA6
 * 				Trig - PA5
 *
 *
 * Зуммер: PB2
 *
 *
 * USART: PA9 - TX, PA10 -RX
 * */

#define CHECK_HC_SR04 //для выбора режима работы CHECK_MCP9700, CHECK_HC_SR04 or WORK
//#define with_temp // no_temp, with_temp  - для выбора, какие данные отправлять



#define SYSCFG_Clock() RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN

// размер температуры и расстояния, для отправки по усарт (1 байт - целое число, 2 - дробное)
#define SIZE_for_usart 2

//для кнопки

#define Button_ON 1
#define Button_OFF 0


//наличие ямы/ступени
#define Pit_YES 1
#define Pit_NO 0

//статус зуммера
#define BuzzerON_PIT 2
#define BuzzerON 1
#define BuzzerOFF 0


// для датчика температуры
#define MCP9700_ON 1
#define MCP9700_OFF 0

//для ацп
#define ready_data 1
#define not_ready_data 0

#define convertion_yes 1
#define convertion_no 0

//для разрешения запуска ультрозвуковых датчиков
#define Work_ON 1
#define Work_OFF 0

//для ультрозвуковых датчиков
#define HC_SR04_Vert_ON 1
#define HC_SR04_Vert_OFF 0
#define HC_SR04_Horiz_ON 1
#define HC_SR04_Horiz_OFF 0


#define work_in_progress 1   //означает, что таймер еще считывает импульс
#define not_work_in_progress 0 // работа таймера завершена


/* For DMA - USART*/
#define BAUD 115200

#define DMA_CH2_ON() DMA1_Channel2->CCR |= DMA_CCR_EN   // включение канала по усарт
#define DMA_CH2_OFF() DMA1_Channel2->CCR &= ~DMA_CCR_EN

#define ready_transmit 1   //флаг для проверки идет ли передача по усарт или еще нет
#define not_ready_transmit 0


#define BUZZER_ON() GPIOB->ODR|=GPIO_ODR_2
#define BUZZER_OFF() GPIOB->ODR&=~GPIO_ODR_2



#endif /* GLOBAL_H_ */
