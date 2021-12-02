/*
 * HC_SR04.c
 *
 *  Created on: 30 сент. 2021 г.
 *      Author: Ekaterina
 */

#include "HC_SR04.h"

extern uint8_t mode_HC_SR04_Vert;
extern uint8_t mode_HC_SR04_Horiz;
extern uint8_t modeMCP9700;
extern uint8_t Work_Allowed;
extern uint8_t modeBuzzer;
extern uint8_t work;

extern float temp;
extern void delay(uint32_t tick);

float Distance_metr_Horiz=0;
float Distance_metr_Vert=0;

// буфер необходим для того, чтобы сравнивать предыдущие значения и текущее для обнаружения ям или ступенек
float data_for_distance[5]={0,0,0,0,0};
Buffer Buf_Distance_Vert;


//функция инциализации
void HC_SR04_init(){
	HC_SR04_GPIOC_Init(); // настройка пинов
	Struct_For_HC_SR04_Init(); //создание структуры
	delay(10000); // для того, чтобы таймер не зашел в прерывание по изменению сигнала
	TIM2_For_HC_SR04_Horiz_init(); // 2 таймера для подсчета, через сколько мкс придет ответная волна от датчиков
	TIM3_For_HC_SR04_Vert_init();
}

void HC_SR04_GPIOC_Init(){
	Perif_HC_SR04_ON_for_trig(); //тактирование
	Perif_HC_SR04_for_trig->MODER |= Trig_Vertical_Init_Output | Trig_Horizontal_Init_Output; //настройка на выход
	//GPIOA->PUPDR |= GPIO_PUPDR_PUPDR2_0;
	pinTRIG_Horiz_LOW(); // низкий сигнал
	pinTRIG_Vert_LOW();

	Perif_HC_SR04_ON_for_echo();
	Perif_HC_SR04_for_echo->MODER |= Echo_Horizontal_Init_AF| Echo_Vertical_Init_AF;
	Perif_HC_SR04_for_echo->PUPDR |= Echo_Vert_PULL_down | Echo_Horiz_PULL_down;
	AF_init_echo();
}

void Struct_For_HC_SR04_Init(){
	Buffer_Init(&Buf_Distance_Vert,&data_for_distance[0], 5); // создание структуры для данных расстояния с вертикального датчика
}


// функция работы датчиков
void HC_SR04_ON(){
	if(Work_Allowed==Work_OFF || work == work_in_progress || modeBuzzer==BuzzerON){ // если работа не разрешена, таймеры работают или работает зуммер, выходим
		return;
	}
	if(mode_HC_SR04_Vert==HC_SR04_Vert_OFF && mode_HC_SR04_Horiz == HC_SR04_Horiz_OFF){ // если ни один из датчиков не работает
		mode_HC_SR04_Vert=HC_SR04_Vert_ON; // включаем вертикальный, чтобы сначала узнать, если ли ямы
		HC_SR04_Vert_Work_ON(); // включаем считывание вертикального датчика
		return;
	}
	if(mode_HC_SR04_Vert==HC_SR04_Vert_OFF && mode_HC_SR04_Horiz == HC_SR04_Horiz_ON){
		HC_SR04_Horiz_Work_ON(); // запуск горизонтального датчика
		return;
	}
}

void HC_SR04_Horiz_Work_ON(){
	//условие на старт - на пине TRIG установить сигнал 1 и держать 10 мкс
	TIM2_CH1_EN(); /* разрешение захвата*/
	TIM2_CH2_EN(); /* разрешение захвата*/
	TIM2_for_Horiz_ON(); //запускаем таймер
	pinTRIG_Horiz_HIGH();
	delay(10);
	pinTRIG_Horiz_LOW();
	work = work_in_progress;
}

void HC_SR04_Vert_Work_ON(){
	work = work_in_progress;
	// запуск на старт аналогично горизонтальному
	TIM3_CH1_EN(); /* разрешение захвата*/
	TIM3_CH2_EN(); /* разрешение захвата*/
	TIM3_for_Vert_ON(); //запускаем таймер
	pinTRIG_Vert_HIGH();
	delay(10);
	pinTRIG_Vert_LOW();
}

// функция для проверки датчика и отправку результатов на комп по усарт
void CHECK_HC_SR04_ON(){
	// будем использовать только один датчик, горизонтальный
	temp = 25; //для расчета расстояния необходимо знать температуру
	if(work == work_in_progress){
		return;
	}
	HC_SR04_Horiz_Work_ON();
}
