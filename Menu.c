/*!
 * \file Menu.c
 * \brief Plik źródłowy obsługi menu
 */

#include"Menu.h"
#include<string.h>
/** Funkcja inicjuje liczbę wyborów, napisy oznaczające wybory oraz szerokość menu */
void newmenu(Menu* m, int n, char* choices[], int w){
	m->n = n;
	m->w = w;
	int i;
	for(i=0; i<n; i++) strcpy(m->choices[i],choices[i]);
}

/** Gdy osiągnięty zostanie koniec menu, aktualnie wybraną staje się pierwsza opcja */
void nextchoice(Menu* m){
	m->now++;
	if(m->now == m->n) m->now=0;
}
/** Gdy osiągnięty zostanie początek menu, aktualnie wybraną staje się ostatnia opcja */
void prevchoice(Menu* m){
	m->now--;
	if(m->now==-1) m->now=m->n-1;
}

/** Funkcja jest wywoływana, gdy menu jest otwierane (zmiana stanu) gry oraz za każdym razem, gdy zmieniony zostanie rozmiar terminala.
 * Tworzy okno (ncurses), w którym będzie rysowane menu - na środku ekranu zarówno w pionie, jak i w poziomie.
 * Następnie wypisuje opcje menu oraz wywołuje funkcję, która podświetli aktualnie wybraną opcję i odświeży okno.
 */ 
void drawmenu(Menu* m){
	WINDOW* w = m->win;
	delwin(w);
	int l,c;
	getmaxyx(stdscr,l,c);
	c = (c-m->w)/2;
	l = (l - 2*(m->n)-1)/2;
	m->win = newwin(2*m->n+1,m->w,l,c);
	w = m->win;

	int i;
	for(i=0; i<m->n; i++){
		mvwprintw(w,2*i+1,0,">");
		mvwprintw(w,2*i+1,2,m->choices[i]);
	}
	
	updatemenu(m);
}

/** Funkcja jest wywoływana, gdy zmieni się aktualny wybór w danym menu lub gdy menu jest przerysowywane. Stworzona, aby nie przerysowywać całego menu za każdym razem, gdy użytkownik zmieni aktualnie wybraną opcję */
void updatemenu(Menu *m){
	int i;
	WINDOW* w = m->win;
	for(i=0; i<m->n; i++){
		if(i==m->now) mvwchgat(w,2*i+1, 0, -1, A_STANDOUT, 1, NULL);
		else mvwchgat(w,2*i+1, 0, -1, A_NORMAL, 1, NULL);
	}
	wrefresh(w);
}
