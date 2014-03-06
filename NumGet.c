/*!
 * \file NumGet.c
 * \brief Plik źródłowy obsługi pobieracza rozmiaru
 */
#include"NumGet.h"
//#include<string.h>

void newng(NumGet* ng, int now, int minn, int maxx, int w){
	ng->now = now;
	ng->minn = minn;
	ng->maxx = maxx;
	ng->w = w;
}

void incrng(NumGet* ng){
	ng->now++;
	if(ng->now > ng->maxx) ng->now=ng->minn;
}

void decrng(NumGet* ng){
	ng->now--;
	if(ng->now < ng->minn) ng->now=ng->maxx;
}

void incr10ng(NumGet* ng){
	if(ng->now == ng->maxx){
		ng->now = ng->minn;
		return;
	}
	
	ng->now+=10;
	if(ng->now > ng->maxx){
		ng->now-=ng->maxx;
		ng->now+=ng->minn;
	}
}

void decr10ng(NumGet* ng){
	if(ng->now == ng->minn){
		ng-> now = ng->maxx;
		return;
	}
	
	ng->now-=10;
	if(ng->now < ng->minn){
		ng->now-=ng->minn;
		ng->now+=ng->maxx;
	}
}

void drawng(NumGet* ng){
	WINDOW* w = ng->win;
	delwin(w);
	int l,c;
	getmaxyx(stdscr,l,c);
	c = (c-ng->w)/2;
	l = (l - 3)/2;
	ng->win = newwin(3,ng->w,l,c);
	w = ng->win;

	mvwprintw(w,1,0,">");
	mvwprintw(w,1,2,"%d",ng->now);
	
	updateng(ng);
}

void updateng(NumGet *ng){
	WINDOW* w = ng->win;
	mvwchgat(w,1, 0, -1, A_STANDOUT, 1, NULL);
	
	wrefresh(w);
}
