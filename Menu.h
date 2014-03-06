/*!
 * \file Menu.h
 * \brief Plik nagłówkowy obsługi menu
 */
#include"Screen.h"

//! Struktura reprezentująca menu
typedef struct{
	//! Wskaźnik na okno (z ncurses), którym menu ma być rysowane
	WINDOW* win;
	//! Szerokość menu (w znakach)
	int w;
	//! Liczba elementów w menu
	int n;
	//! Tablica napisów do wyświetlenia w menu
	char choices[10][20];
	//! Numer aktualnie wybranego menu
	int now;
} Menu;

//! Funkcja inicjująca menu wartościami przekazanymi w parametrach
void newmenu(Menu*, int, char*[], int);
//! Funkcja realizująca zmianę aktualnie wybranej opcji na następną
void nextchoice(Menu*);
//! Funkcja realizująca zmianę aktualnie wybranej opcji na poprzednią
void prevchoice(Menu*);

//! Funkcja wykonująca ustalanie pozycji menu oraz rysowanie go.
void drawmenu(Menu*);
//! Funkcja uaktualniająca wyświetlanie menu
void updatemenu(Menu*);
