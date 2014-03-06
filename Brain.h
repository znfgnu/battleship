/*!
 * \file Brain.h
 * \brief Plik nagłówkowy obsługi sztucznej inteligencji
 */

#ifndef STATKI_H
#include"Statki.h"
#define STATKI_H
#endif

//! Skruktura reprezentująca pojedyncze pole (współrzędne na planszy)
typedef struct{
	int x,y;
}Pole;

//! Struktura reprezentująca jednostkę sztucznej inteligencji
/** Jej schemat działania jest następujący: przy inicjalizacji do tablicy ok wpisywane są pola planszy przeciwnika, na których znajdują się segmenty statków, a do tablicy bad pola, na których ich nie ma.
 * Podczas rozgrywki AI podejmuje decyzję, czy wybrać pole ze statkiem, czy nie (z określonym prawdopodobieństwem: 10%/20%/40% odpowiednio dla łatwego/średniego/trudnego poziomu trudności), a następnie
 * wybiera kolejne pole z odpowiedniej tablicy. Jeśli wszystkie pola w tablicy zostały wykorzystane (jest to mało prawdopodobne, ale jednak), wtedy wybierane jest pole z tablicy, w której jeszcze są pola.*/
typedef struct{
	//! Tablica z polami, na których znajdują się segmenty statków
	Pole ok[20];
	//! Indeks tablicy ok, który wskazuje na pierwsze niewykorzystane z niej pole
	int oki;
	//! Tablica z polami, na których nie ma statków
	Pole* bad;
	//! Indeks tablicy bad, który wskazuje na pierwsze niewykorzystane z niej pole
	int badi;
	//! Prawdopodobieństwo (w promilach) wybrania pola z segmentem statku
	int prop;	// 0-10000
	//! Rozmiar poprzedniej planszy, dla której AI było inicjowane
	int prevbrdsize;
}Brain;

//! Funkcja inicjująca AI dla danej planszy
void newbrain(Brain*,Plansza*,int);

//! Funkcja pobierająca kolejny ruch AI
void getmove(Brain*, int*, int*);
