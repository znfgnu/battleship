/*!
 * \file BoardViewer.h
 * \brief Plik nagłówkowy obsługi wyświetlania planszy
 */

#ifndef STATKI_H
#include"Statki.h"
#define STATKI_H
#endif
#include<ncurses.h>

//! Enumeracja oznaczająca tryb wyświetlania planszy (czy niesprawdzone dotąd pola mają być widoczne)
typedef enum{SELF=0, OPP=1} MODE;
//! Enumeracja oznaczająca stronę ekranu, po której ma być narysowany wyświetlacz
typedef enum{LEFT, RIGHT} PAN;

//! Struktura reprezentująca "wyświetlacz" planszy
typedef struct{
	//! Wartość logiczna, oznaczająca widoczność kursora wyboru pola na planszy
	int cur;
	//! Liczba, oznaczająca linię aktualnie wyświetlanego obszaru (względną), w której znajduje się pole wskazywane przez kursor
	int cy;
	//! Liczba, oznaczająca kolumnę aktualnie wyświetlanego obszaru (względną), w której znajduje się pole wskazywane przez kursor
	int cx;
	//! Zmienna, oznaczająca tryb wyświetlania planszy
	MODE mode;
	//! Zmienna, oznaczająca stronę ekranu, po której ma być narysowany wyświetlacz
	PAN pan;
	//! Wskaźnik na rysowaną planszę
	Plansza* brd;
	//! Wskaźnik na okno, w którym ma być rysowany wyświetlacz (ncurses)
	WINDOW* win;
	//! Współrzędna onaczająca linię lewego górnego rogu aktualnie wyświetlanego obszaru (viewport)
	int vpl;
	//! Współrzędna onaczająca kolumnę lewego górnego rogu aktualnie wyświetlanego obszaru (viewport)
	int vpc;
}BoardViewer;

//! Funkcja inicjująca nowy wyświetlacz
void newbv(BoardViewer*, Plansza*, int, int, int);
//! Funkcja rysująca wyświetlacz
void drawbv(BoardViewer*);
//! Funkcja uaktualniająca wyświetlacz
void updatebv(BoardViewer*);

//! Funkcja przesuwająca kursor w górę
void bvup(BoardViewer*);
//! Funkcja przesuwająca kursor w dół
void bvdown(BoardViewer*);
//! Funkcja przesuwająca kursor w lewo
void bvleft(BoardViewer*);
//! Funkcja przesuwająca kursor w prawo
void bvright(BoardViewer*);
