/*!
 * \file Logger.h
 * \brief Plik nagłówkowy obsługi modułu zapisującego przebieg rozgrywki
 */

#include"Screen.h"
#include<time.h>

//! Enumeracja oznaczająca odpowiedzi po wykonaniu ruchu
enum{WIN, SHOT, DESTROYED, MISS, FORB};
//! Enumeracja oznaczająca powód zakończenia rozgrywki (Gracz 1 / Gracz 2 / Przerwane)
enum{P1, P2, IRR};

//! Struktura reprezentująca pojedynczy wpis do modułu logującego przebieg rozgrywki
typedef struct{
	//! Linia
	int l;
	//! Kolumna
	int c;
	//! Id gracza
	int pid;
	//! Wynik ruchu (trafiony/pudło/zatopiony)
	int res;
}LogMove;

//! Funkcja inicjująca nowy wpis
void newlm(LogMove*, int, int, int, int);

//! Struktura reprezentująca moduł logujący przebieg rozgrywki
typedef struct{
	//! Czas rozpoczęcia rozgrywki
	time_t begin;
	//! Czas zakończenia rozgrywki
	time_t end;
	//! Powód zakończenia rozgrywki
	int reason;
	//! Tablica kolejnych ruchów
	LogMove* data;
	//! Rozmiar tablicy kolejnych ruchów (2*rozmiar*rozmiar)
	int size;
	//! Liczba ruchów dotychczas już wykonanych
	int wsk;
	//! Szerokość rysowanego okna
	int width;
	//! Wysokość rysowanego okna, liczba ostatnich ruchów do wypisania
	int height;
	//! Wskaźnik na okno (z ncurses), którym logger ma być rysowany
	WINDOW* win;
}Logger;

//! Funkcja inicjująca nowy Logger na podstawie wartości przekazanych poprzez parametry
void newlog(Logger*, int, int, int);
//! Funkcja dodająca nowy wpis do Loggera na podstawie wartości przekazanych poprzez parametry
void addmove(Logger*, int, int, int, int);
//! Funkcja eksportująca zapis rozgrywki do pliku (sama generuje nazwę pliku)
/** Zwraca 0, gdy wyeksportowano pomyślnie */
int exportlog(Logger*);
//! Funkcja kończąca rozgrywkę
void endgame(Logger*, int);
//! Funkcja rysująca Logger
void drawlog(Logger*);
//! Funkcja uaktualniająca Logger
void updatelog(Logger*);
