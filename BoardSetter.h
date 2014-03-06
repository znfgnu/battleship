/*!
 * \file BoardSetter.h
 * \brief Plik nagłówkowy ustawiacza statków
 */

#ifndef STATKI_H
#include"Statki.h"
#define STATKI_H
#endif
#include<ncurses.h>
#ifndef BOARDVIEWER_H
#include"BoardViewer.h"
#define BOARDVIEWER_H
#endif
//! Struktura reprezentująca ustawiacz statków
typedef struct{
	//! Wskaźnik na okno, w którym ma być rysowany ustawiacz (ncurses)
	WINDOW* win;
	//! Wskaźnik na ustawianą planszę
	Plansza* p;
	//! Tablica kolejno ustawianych statków (od najdłuższego do najkrótszego)
	Statek s[10];
	//! Indeks w tablicy s aktualnie ustawianego statku
	int akt;
	
	int vpl;
	int vpc;
}BoardSetter;

//! Funkcja inicjująca nowy ustawiacz
void newbs(BoardSetter*, Plansza*);
//! Funkcja rysująca ustawiacz
void drawbs(BoardSetter*);
//! Funkcja uaktualniająca ustawiacz, aby nie tworzyć nowego okna za każdym razem, gdy użytkownik wciśnie klawisz
void updatebs(BoardSetter*);

//! Funkcja przesuwająca aktualnie ustawiany statek w górę
void bsup(BoardSetter*);
//! Funkcja przesuwająca aktualnie ustawiany statek w dół
void bsdown(BoardSetter*);
//! Funkcja przesuwająca aktualnie ustawiany statek w lewo
void bsleft(BoardSetter*);
//! Funkcja przesuwająca aktualnie ustawiany statek w prawo
void bsright(BoardSetter*);
//! Funkcja obracająca (jeśli się da) aktualnie ustawiany statek
void bsrot(BoardSetter*);
//! Funkcja akceptująca ustawienie aktualnie ustawianego statku 
int nextstat(BoardSetter*);
//! Funkcja cofająca ustawienie ostatnio ustawionego statku
int prevstat(BoardSetter*);
