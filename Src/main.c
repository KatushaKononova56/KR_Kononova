/* программа была написана 17.11.2021
 * автор: Ekaterina
 * описание работы: данный код выполняет задачу сканирования пространства с целью узнать наличие преград для  того, чтобы
 * слабовидящие могли спокойно передвигаться
 *
 * в работе использовано: 2 датчика расстояния HC_SR04, датчик температуры MCP9700
 */



#include "main.h"


uint8_t mode_HC_SR04_Vert=HC_SR04_Vert_OFF; //вертикальный HC_SR04 выключен
uint8_t mode_HC_SR04_Horiz=HC_SR04_Horiz_OFF; //горизонтальный HC_SR04 выключен
uint8_t modeButton=Button_OFF; // кнопка
uint8_t modeMCP9700=MCP9700_OFF; // датчик температуры
uint8_t Work_Allowed=Work_OFF; //разрешение работы ультразвуковых датчиков ( нужен для того,
								//чтобы сначала считать температуру, а потом запустить датчики расстояния
uint8_t modeBuzzer=BuzzerOFF; // зуммер

//для датчика температуры
uint8_t status_get_data = not_ready_data; //получены ли данные
uint8_t status_convertion=convertion_no; //идет ли конвертация
// для ультразвукового датчика
uint8_t work=not_work_in_progress; //считывания не идет

void InitALL();

int main(void)
{
#ifdef CHECK_HC_SR04 //исследуем ультразвуковые датчики HC_SR04
	HC_SR04_init();
	USART1_DMA_Init();
#endif

#ifdef CHECK_MCP9700 //исследуем датчик температуры MCP9700
	MCP9700_init();
	USART1_DMA_Init();
#endif

#ifdef WORK // основной код программы
	InitALL();
#endif

	while(1){

#ifdef CHECK_HC_SR04
		CHECK_HC_SR04_ON();
#endif

#ifdef CHECK_MCP9700
		MCP9700_Convert_And_USART();
#endif

#ifdef WORK
		MCP9700_On();
		HC_SR04_ON();
		Buzzer_Work_On();
#endif
	}
}

void InitALL(){
	SYSCFG_Clock(); //включение тактирования для прерываний по кнопке
	Button_init();
	HC_SR04_init();
	Buzzer_init();
	MCP9700_init();
}
