/*
 * Buzzer.c
 *
 *  Created on: 23 окт. 2021 г.
 *      Author: Ekaterina
 */


#include "Buzzer.h"
extern float Distance_metr_Horiz;
extern uint8_t modeBuzzer;
extern void delay(uint32_t tick);
uint8_t ColvoMetr;

// инциализация
void Buzzer_init(){
	GPIOB_Buzzer_init();
}

void GPIOB_Buzzer_init(){
	RCC_Buzzer_ON(); //тактирование
	Perif_Buzzer->MODER|=Buzzer_output;
	Perif_Buzzer->PUPDR|=Buzzer_pull_down;
	BUZZER_OFF();
}

void Buzzer_Work_On(){
	if(modeBuzzer==BuzzerOFF){
		return;
	}
	ColvoMetr=(int)Distance_metr_Horiz; // считывает расстояние и округляет его до целого, без учета дробной части
	if(modeBuzzer==BuzzerON_PIT || ColvoMetr==0){ // если обнаружена яма или же близко препятствие
		BuzzerON_delay_OFF(3000000);//3s
		modeBuzzer=BuzzerOFF;
		delay(3000000);//3s
		modeBuzzer=BuzzerOFF;
		return;
	}
	else { // если 1 или 2 метра впереди
		for(int i=0;i<ColvoMetr;i++){
			BuzzerON_delay_OFF(200000);
		}
	}
	modeBuzzer=BuzzerOFF;
	delay(3000000);//3s, задержка для того, чтобы не было беспрерывного сигнала
}

// на вход подается время задержки между 0 и 1
void BuzzerON_delay_OFF(uint32_t time){
	BUZZER_ON();
	delay(time);
	BUZZER_OFF();
	delay(time);
}

