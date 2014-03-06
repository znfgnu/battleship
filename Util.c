/*!
 * \file Util.c
 * \brief Plik źródłowy funkcji pomocniczych
 */
#include"Util.h"

void swap(int *a, int *b){
	int c=*a;
	*a=*b;
	*b=c;
}

int myrand(int a, int b){
	if(b<a) swap(&a,&b);
	return a+rand()%(b-a+1);
}
