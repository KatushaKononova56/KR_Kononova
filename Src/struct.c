#include "struct.h"

// на вход подается буфер, массив и размер массива
void Buffer_Init(Buffer *B, float *data, int size) {
	B->buf = data;
	B->len = size;
	B->ptrBegin = 0;
	B->ptrEnd = 0;
	B->datalen = 0;
}
//Функция, извлекающая данные из нашего буфера.
// на вход буфер, из которого извлечь инфу, и адрес элемента, куда записать
int bufferGetFromFront(Buffer *B, float *el) {
	if (B->datalen == 0)
		return 0;
	*el =B->buf[B->ptrBegin];
	++B->ptrBegin;
	--B->datalen;
	//убедимся, что указатель не вышел за пределы нашего массива:
	if (B->ptrBegin > B->len)
		B->ptrBegin = 0;
	return 1;
}

// на вход буфер и элемент
void  Buffer_AddtoEnd(Buffer *B, float el) {
	B->buf[B->ptrEnd] = el;
	++B->ptrEnd;
	++B->datalen;
	if(B->datalen >= B->len){
		B->datalen=5;
	}
//убедимся, что указатель не вышел за пределы нашего массива:
	if (B->ptrEnd > B->len-1)
		B->ptrEnd = 0;
}

//Ф-я очищения, на вход только буфер
void Buffer_flush(Buffer *B) {
	B->ptrBegin = 0;
	B->ptrEnd = 0;
	B->datalen = 0;
}

