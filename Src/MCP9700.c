/*
 * MCP9700.c
 *
 *  Created on: Oct 15, 2021
 *      Author: Ekaterina
 */

#include "MCP9700.h"


#define send

//режим работы сенсоров
extern uint8_t mode_HC_SR04_Vert;
extern uint8_t mode_HC_SR04_Horiz;
extern uint8_t modeMCP9700;
extern uint8_t Work_Allowed; //разрешение работы ультразвуковых датчиков
extern uint8_t status_get_data;
extern uint8_t status_convertion;

float temp=0;

void MCP9700_init(){
	ADC_init();
}

void MCP9700_On(){
	if (modeMCP9700==MCP9700_OFF){ // датчик не включен, поэтому уходим
			return;
	}
	if (status_convertion == convertion_no){ // если датчик включен и конвертация по ацп еще не началась
		start_convert(); //запускаем адц и дма
		status_convertion = convertion_yes;
		return;
	}
	if(status_get_data==ready_data && status_convertion == convertion_no){ //как только
		//данные получены, считываем температуру
		MCP9700_get_temp();
		Work_Allowed = Work_ON; //разрешает работу ультразвуковых
		modeMCP9700=MCP9700_OFF; //не включаем больше MCP9700
	}
}

void MCP9700_get_temp(){
	uint32_t sr_data=0;
	sr_data = ADC_middle_value();//считаем среднее значение, для точности
	MCP9700_Get_temperature(sr_data); //вычисляем температуру
}

void MCP9700_Convert_And_USART(){
	if (status_convertion == convertion_no && status_get_data==not_ready_data){ // если датчик включен и конвертация по ацп еще не началась
		start_convert(); //запускаем адц и дма
		status_convertion = convertion_yes;
		return;
	}
	if(status_get_data==ready_data && status_convertion == convertion_no){ //как только
		//данные получены, считываем температуру
		MCP9700_get_temp();
		send_temp(temp);
		status_convertion = convertion_no;
		status_get_data = not_ready_data;
	}
}


// принимает значение с ацп
void MCP9700_Get_temperature(uint32_t data){
	temp = ((data - MIN_ADC)*ADC_TEMP) - 40;
}

// задержка, в мкс
void delay(uint32_t tick){
	tick*=(SystemCoreClock / 1000000)/5;
	while (tick--);
}
