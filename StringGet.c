/*!
 * \file StringGet.c
 * \brief Plik źródłowy obsługi pobieracza łańcucha znakowego
 */

#include"StringGet.h"
#include<stdlib.h>

void newsg(StringGet* sg, int maxlen, int w){
	if(sg->maxlen) free(sg->data);
	sg->maxlen = maxlen;
	sg->data = malloc(sizeof(char)*(sg->maxlen+1));
	sg->w = w;
	
	sg->akt=0;
	sg->data[0]='\0';
}

void keyhandler(StringGet* sg, chtype c){
	if(c >= 32 && c<=127){	//czy drukowalny
		if(sg->akt != sg->maxlen){	//jesli jest jeszcze miejsce
			sg->data[sg->akt++]=c;
			sg->data[sg->akt]='\0';
		}
	}
	else if(c == KEY_BACKSPACE){
		if(sg->akt) sg->data[--sg->akt]='\0';
	}
}

void drawsg(StringGet* sg){
	WINDOW* w = sg->win;
	delwin(w);
	int l,c;
	getmaxyx(stdscr,l,c);
	c = (c-sg->w)/2;
	l = (l - 3)/2;
	sg->win = newwin(3,sg->w,l,c);
	w = sg->win;

	/*int i;
	for(i=0; i<m->n; i++){
		mvwprintw(w,2*i+1,0,">");
		mvwprintw(w,2*i+1,2,m->choices[i]);
	}*/
	
	
	updatesg(sg);
}

/** Funkcja jest wywoływana, gdy zmieni się aktualny wybór w danym menu lub gdy menu jest przerysowywane. Stworzona, aby nie przerysowywać całego menu za każdym razem, gdy użytkownik zmieni aktualnie wybraną opcję */
void updatesg(StringGet *sg){
	WINDOW* w = sg->win;
	//mvwchgat(w,1, 0, -1, A_STANDOUT, 1, NULL);
	int szer = sg->w - 3;	// liczba wyswietlanych znakow
	char czywczes = '*';
	int start = sg->akt - szer;
	if(start<0){
		start=0;
		czywczes=' ';
	}
	
	mvwprintw(w,1,0,">%c",czywczes);
	mvwprintw(w,1,2,"%s",sg->data+start);
	mvwchgat(w,1, 0, -1, A_STANDOUT, 1, NULL);
	
	wrefresh(w);
}

