/*!
 * \file StringGet.h
 * \brief Plik nagłówkowy obsługi pobieracza łańcucha znakowego
 */

#include"Screen.h"

//! Struktura reprezentująca pobieracz łańcucha znakowego
typedef struct{
	//! Bufor danych
	char* data;
	//! Maksymalna długość przechowywanego łańcucha znakowego
	int maxlen;
	//! Liczba aktualnie wczytanych znaków
	int akt;
	//! Wskaźnik na okno (z ncurses), którym pobieracz ma być rysowany
	WINDOW* win;
	//! Szerokość pola do wprowadzania łańcucha
	int w;
}StringGet;

//! Funkcja inicjująca nowy pobieracz znaków
void newsg(StringGet*, int, int);
//! Funkcja obsługująca wciśnięcia klawiszy
void keyhandler(StringGet*, chtype);
//! Funkcja rysująca pobieracz
void drawsg(StringGet*);
//! Funkcja aktualizująca pobieracz
void updatesg(StringGet*);
