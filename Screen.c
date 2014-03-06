/*!
 * \file Screen.c
 * \brief Plik źródłowy obsługi ekranu
 */
#include"Screen.h"
#include<string.h>

//! Wskaźnik na napis, który ma być wyświetlony w górnej belce
char* naglowek;
//! Wskaźnik na napis, który ma byc wyświetlony w dolnej belce
char* stopka;

extern char* mennag;

void init(){
	initscr();
	noecho();
	cbreak();
	keypad(stdscr,TRUE);
	start_color();
	curs_set(0);
	refresh();
}

void myclear(){
	clear();
	mvprintw(0,1,mennag);//"%s",naglowek);
	mvprintw(2,2,"%s",naglowek);
	
	mvchgat(0,0,-1,A_STANDOUT,1,NULL);
	int nlen=strlen(naglowek);
	if(nlen) mvchgat(2,1,nlen+2,A_STANDOUT,1,NULL);
	
	int l=getmaxy(stdscr)-1;
	int slen = strlen(stopka);
	int c=getmaxx(stdscr)-slen-2;
	
	mvprintw(l-1,c,"%s",stopka);
	if(slen) mvchgat(l-1,c-1,slen+2,A_STANDOUT,1,NULL);
	
	//mvchgat(l,0,slen+2,A_STANDOUT,1,NULL);
	
	refresh();
}

void printscr(char* p1, char* p2){
	naglowek = p1;
	stopka = p2;
	myclear();
	getch();
}
