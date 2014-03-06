/*!
 * \file ResHan.c
 * \brief Plik źródłowy funkcji przerwań (zmiana rozmiaru terminala)
 */

#include "ResHan.h"
#include "Menu.h"
#ifndef BOARDVIEWER_H
#include"BoardViewer.h"
#define BOARDVIEWER_H
#endif
#include "BoardSetter.h"

#include<stdio.h>
#include<stdlib.h>

//! Wskaźnik na aktualnie wyświetlane menu
Menu* aktmenu;
extern BoardViewer *aktlewa, *aktprawa;
extern BoardSetter ustawiacz;

//! Mininalna liczba linii terminala
static const int MINLINES=28;
//! Minimalna liczba kolumn terminala
static const int MINCOLS=0;//125;

void everytimerh(){
	endwin();
	int my = getmaxy(stdscr);
	int mx = getmaxx(stdscr);
	if( my < MINLINES || mx < MINCOLS){
		fprintf(stderr,"Za mały rozmiar terminala, minimalny rozmiar to linii: %d i kolumn: %d.\n",MINLINES,MINCOLS);
		fprintf(stderr,"Rozmiar Twojego terminala to linii: %d i kolumn: %d.\n",my,mx);
		exit(1);
	}
	refresh();
	myclear();
}
/*
void rhmenu(){
	everytime();
	drawmenu(aktmenu);
}

void rhgame(){
	everytime();
	drawbv(aktlewa);
	drawbv(aktprawa);
}

void rhbs(){
	everytime();
	drawbs(&ustawiacz);
}
*/
