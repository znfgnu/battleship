/*!
 * \file Brain.c
 * \brief Plik źródłowy obsługi sztucznej inteligencji
 */

#include "Brain.h"
#include<stdlib.h>

extern int brdsize, brdt;

//! Pomocnicza funkcja tworząca nowe pole z danych podanych poprzez parametry
static Pole newpole(int a, int b){
	Pole p;
	p.y=a;
	p.x=b;
	return p;
}

//! Funkcja, która losowo permutuje tablicę pól
static void pomieszaj(Pole* t, int n){
	int i;
	for(i=1; i<n; i++){
		int pos = rand()%(i+1);
		Pole p=t[pos];
		t[pos]=t[i];
		t[i]=p;
	}
}

void newbrain(Brain* br, Plansza* p, int trud){
	if(br->prevbrdsize) free(br->bad);
	br->bad = malloc((brdsize*brdsize-20) * sizeof(Pole));
	
	br->oki=0;
	br->badi=0;
	br->prop=200000/brdsize/brdsize;
	switch(trud){
		case 0:
			br->prop/=2;
			break;
		case 1:
			br->prop*=1;//200000/brdsize/brdsize;
			break;
		case 2:
			br->prop*=2;
			break;
	}
	
	int i,j;
	for(i=1; i<=brdsize; i++){
		for(j=1; j<=brdsize; j++){
			if(p->tab[i][j] == STAT) br->ok[br->oki++]=newpole(i,j);
			else br->bad[br->badi++]=newpole(i,j);
		}
	}
	pomieszaj(br->ok,20);
	pomieszaj(br->bad,brdsize*brdsize-20);
	br->badi=0;
	br->oki=0;	
	br->prevbrdsize=brdsize;
}

void getmove(Brain* br, int* y, int* x){
	if(br->oki == 20){
		*y = br->bad[br->badi].y;
		*x = br->bad[br->badi].x;
		br->badi++;
		return;
	}
	if(br->badi == brdsize*brdsize-20){
		*y = br->ok[br->oki].y;
		*x = br->ok[br->oki].x;
		br->oki++;
		return;
	}
	int gen = rand()%10000;
	if(gen<br->prop){
		*y = br->ok[br->oki].y;
		*x = br->ok[br->oki].x;
		br->oki++;
		return;
	}
	else{
		*y = br->bad[br->badi].y;
		*x = br->bad[br->badi].x;
		br->badi++;
		return;
	}
}
