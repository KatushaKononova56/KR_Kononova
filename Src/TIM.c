/*
 * TIM.c
 *
 *  Created on: 16 нояб. 2021 г.
 *      Author: Ekaterina
 */

#include "TIM.h"


float SpeedOfSound=0; // скорость звука
float distance_UM=0; // дистанция в сантиметрах
uint16_t time_micro_tick=0;

extern uint8_t work;
extern float Distance_metr_Horiz;
extern float Distance_metr_Vert;
extern float data_for_distance[5];

extern uint8_t mode_HC_SR04_Vert;
extern uint8_t mode_HC_SR04_Horiz;
extern uint8_t modeBuzzer;
extern float temp;
extern Buffer Buf_Distance_Vert;

void TIM2_IRQHandler(){
	if ((TIM2->SR & TIM_SR_CC2IF) != 0){ // срабатывания прерывание для горизонтального датчика
		time_micro_tick = TIM2->CCR2;
		if(time_micro_tick <Three_cm){ // нужно в случае ошибки (если больше 4х метров или меньше 3см датчик выдаст 0)
			time_micro_tick=0;
			TIM2->CNT = 0;
			work=not_work_in_progress;
			mode_HC_SR04_Horiz = HC_SR04_Horiz_OFF; // отключаем датчик
			TIM2_work_off();
			return;
		}
		if(time_micro_tick<Three_metr){
			Distance_metr_Horiz = Get_Distance(time_micro_tick); // получаем расстояние в метрах
#ifdef CHECK_HC_SR04
#ifdef with_temp
			send_dist(Distance_metr_Horiz);
#endif
#ifdef no_temp
			float dist = Get_Distance_without_temp(time_micro_tick);
			send_dist(dist);
#endif
#endif
		}
		else{
			Distance_metr_Horiz = 3;
#ifdef CHECK_HC_SR04
			send_dist(3);
#endif
		}
		time_micro_tick=0;
		TIM2->CNT = 0;
		work=not_work_in_progress;
		modeBuzzer=BuzzerON; // инциализируем работу зуммера
		mode_HC_SR04_Horiz = HC_SR04_Horiz_OFF; // отключаем датчик
		TIM2_work_off();
		return;
	}
}


void TIM2_For_HC_SR04_Horiz_init(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 |= TIM_CR1_OPM;
	TIM2->PSC = 9-1;
	TIM2->ARR = 23200-1;//1 tact - 1 us

	TIM2->CCMR1 |= TIM_CCMR1_CC1S_1; /* 2 канал активен*/

	TIM2->CCMR1 |= TIM_CCMR1_CC2S_0; /* 2 канал активен (на ту же ножку, как и для первого канала)*/
	TIM2->CCER |= TIM_CCER_CC2P; // срабатывание по заднему фронту импульса
	TIM2->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_1 //110: Фильтрованный вход таймера 2 (TI2FP2))
	| TIM_SMCR_SMS_2; //100: Режим сброса - Возрастающий фронт выбранного входа триггера (TRGI) повторно инициализирует счетчик
						//и генерирует обновление регистров.
	TIM2->DIER |= TIM_DIER_CC2IE;

	NVIC_SetPriority(TIM2_IRQn, 3);
	NVIC_EnableIRQ(TIM2_IRQn);
}


void TIM3_IRQHandler(){
	if ((TIM3->SR & TIM_SR_CC2IF) != 0){ // срабатывания прерывание для горизонтального датчика
		time_micro_tick = TIM3->CCR2;
		if(time_micro_tick <Three_cm){// так как минимальное расстояние 3см, то любое значение меньше это 0
			modeBuzzer=BuzzerON_PIT; // настраиваем зуммер на сигнал о яме
			mode_HC_SR04_Vert = HC_SR04_Vert_OFF; //отключаем датчик
			work=not_work_in_progress;
			TIM3_work_off();
			return;
		}
		if(time_micro_tick<Three_metr){
			//но так как максимальное 4 м, то разумнее больше 3х метров не считывать
			Distance_metr_Vert = Get_Distance(time_micro_tick); // получаем расстояние в метрах
			Buffer_AddtoEnd(&Buf_Distance_Vert, Distance_metr_Vert);
		}
		else{
			Distance_metr_Vert = 3;
		}
		TIM3->CNT = 0;
		if(Pit_YES_or_NO(Distance_metr_Vert)){ // если есть яма или ступенка
			modeBuzzer=BuzzerON_PIT; // настраиваем зуммер на сигнал о яме
			Buffer_flush(&Buf_Distance_Vert);//очищаем буффер, так как человек может наступить на яму или обогнуть ее, расстояние измениться
			mode_HC_SR04_Vert = HC_SR04_Vert_OFF; //отключаем датчик
			work=not_work_in_progress;
			TIM3_work_off();
			return;
		}
		// если ямы нет, разрешаем работу горизонтального датчика
		mode_HC_SR04_Vert = HC_SR04_Vert_OFF;
		mode_HC_SR04_Horiz = HC_SR04_Horiz_ON;
		work=not_work_in_progress;
		TIM3_work_off();
		return;
	}
}


void TIM3_For_HC_SR04_Vert_init(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 |= TIM_CR1_OPM;
	TIM3->PSC = 9-1;
	TIM3->ARR = 23200-1;//1 tact - 1 us

	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0; /* 1 канал активен*/

	TIM3->CCMR1 |= TIM_CCMR1_CC2S_1; /* 2 канал активен (на ту же ножку, как и для первого канала)*/
	TIM3->CCER |= TIM_CCER_CC2P; // срабатывание по заднему фронту импульса
	TIM3->DIER |= TIM_DIER_CC2IE;
	TIM3->SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0 //101: Фильтрованный вход таймера 1 (TI1FP1)
	| TIM_SMCR_SMS_2; //100: Режим сброса - Возрастающий фронт выбранного входа триггера (TRGI) повторно инициализирует счетчик
						//и генерирует обновление регистров.

	NVIC_SetPriority(TIM3_IRQn, 3);
	NVIC_EnableIRQ(TIM3_IRQn);
}

// на вход время, которое пришло на таймер
float Get_Distance(uint16_t time){
	SpeedOfSound = 20.042 * sqrt(temp+273.15);// в м/с
	distance_UM = (SpeedOfSound * (time))/2;// микрометры
	float distance = distance_UM / 1000000; //metr
	return distance;
}


// на вход время
float Get_Distance_without_temp(uint16_t time){
	SpeedOfSound = 331;// в м/с
	distance_UM = (SpeedOfSound * (time))/2;// микрометры
	float distance = distance_UM / 1000000; //metr
	return distance;
}

// на вход данные, с которыми надо сравнить данные из буфера
uint8_t Pit_YES_or_NO(float data){
	for(int i=0;i<Buf_Distance_Vert.datalen;i++){ // запускаем цикл по всем данным из буфера
		if(Buf_Distance_Vert.buf[i]!=0){ // если расстояние не нулевое (нулевое в случае только начала цикла)
			if(fabs(Buf_Distance_Vert.buf[i]-data)>0.3) // сравниваем, больше ли модуль 30 см, и если да, возвращаем 1
				return 1;
		}
	}
	return 0;
}

void TIM2_work_off(){
	TIM2->CR1 &=~TIM_CR1_CEN;
	TIM2->CCER &= ~TIM_CCER_CC1E;
	TIM2->CCER &= ~TIM_CCER_CC2E;
}

void TIM3_work_off(){
	TIM3->CR1 &=~TIM_CR1_CEN;
	TIM3->CCER &= ~TIM_CCER_CC1E;
	TIM3->CCER &= ~TIM_CCER_CC2E;
}
