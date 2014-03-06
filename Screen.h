/*!
 * \file Screen.h
 * \brief Plik nagłówkowy obsługi ekranu
 */

#include<ncurses.h>

//! Funkcja inicjująca obsługę ekranu
void init();
//! Funkcja czyszcząca ekran i wyświetlająca belki
void myclear();
//! Funkcja wyświetlająca pusty ekran z zadanymi przez argument napisami ustawianymi w nagłówku i stopce
/** Ekran znika gdy użytkownik wciśnie dowolny klawisz */
void printscr(char*, char*);
