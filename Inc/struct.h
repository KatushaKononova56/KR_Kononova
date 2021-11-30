#ifndef _struct_
#define _struct_

#include "stm32f0xx.h"

struct RingBuffer{
	float * buf;
	int len; //длина буфера
	int ptrBegin; //индекс массива, указывающий на начало данных
	int ptrEnd; //индекс массива после последнего элемента данных(ячейка после последнего - пустая)
	int datalen;//количество данных между ptrBegin и ptrEnd
};

typedef struct RingBuffer Buffer;

void  Buffer_AddtoEnd(Buffer *B, float el);
void Buffer_Init(Buffer *B, float *data, int size);
int bufferGetFromFront(Buffer *B, float *el);
void Buffer_flush(Buffer *B);



#endif
