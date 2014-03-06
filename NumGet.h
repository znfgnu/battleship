/*!
 * \file NumGet.h
 * \brief Plik nagłówkowy obsługi pobieracza rozmiaru planszy
 */
#include"Screen.h"

//! Struktura reprezentująca pobieracz rozmiaru
typedef struct{
	//! Wskaźnik na okno (z ncurses), którym pobieracz ma być rysowany
	WINDOW* win;
	//! Szerokość pobieracza (w znakach)
	int w;
	//! Dolne ograniczenie
	int minn;
	//! Górne ograniczenie
	int maxx;
	//! Aktualna wartość
	int now;
} NumGet;

//! Funkcja inicjująca pobieracz wartościami przekazanymi w parametrach
void newng(NumGet*, int, int, int, int);
//! Funkcja powiększająca aktualną wartość o 1
void incrng(NumGet*);
//! Funkcja zmniejszająca aktualną wartość o 1
void decrng(NumGet*);
//! Funkcja powiększająca aktualną wartość o 10
void incr10ng(NumGet*);
//! Funkcja zmniejszająca aktualną wartość o 10
void decr10ng(NumGet*);

//! Funkcja wykonująca ustalanie pozycji pobieracza oraz rysowanie go.
void drawng(NumGet*);
//! Funkcja uaktualniająca wyświetlanie pobieracza
void updateng(NumGet*);

