/*!
 * \file Statki.h
 * \brief Plik nagłówkowy obsługi planszy
 */

//! Enumeracja reprezentująca orientację statku (pionowa/pozioma)
typedef enum{VERT=0,HOR=1} orient;
//! Enumeracja reprezentująca stan pola planszy (puste / segment statku / puste sprawdzone / zniszczony segment statku)
typedef enum{NONE=0, STAT=1, NOCH=2, STCH=3} pole;
//! Enumeracja oznaczająca tryb wyświetlania planszy, w zależności od tego, czy rozgrywka odbywa się na zwykłej planszy, czy na torusie
typedef enum{TORUS, KWADRAT} VIEWT;

//! Struktura reprezentująca statek (używana podczas ustawiania planszy)
typedef struct{
	//! Linia w której znajduje się lewy/górny segment statku
	int lin;
	//! Kolumna w której znajduje się lewy/górny segment statku
	int col;
	//! Orientacja statku (pionowa/pozioma)
	orient ori;
	//! Długość statku
	int len;
} Statek;

//! Struktura reprezentująca planszę
typedef struct{
	//! Tablica statków, w której przechowywane są kolejno wstawiane na planszę statki, aby móc cofnąć wstawiony już statek podczas ustawiania
	Statek kolejne[10];
	//! Aktualna liczba statków na planszy (podczas wstawiania statków na planszę)
	int ile;
	//! Tablica pól planszy (z dodatkowymi kolumnami granicznymi)
	/** Zawartość prawdziwej planszy znajduje się od pola o indeksach (1,1) do (brdsize,brdsize) włącznie. Zostało to tak zaprojektowane, aby ułatwić sprawdzanie sąsiadujących do statka pól. */
	pole** tab;
	//! Liczba aktualnie zestrzelonych segmentów statków (do sprawdzania, czy gra już się skończyła)
	int shot;
	//! Poprzedno ustawiony rozmiar planszy
	int sizebefore;
} Plansza;

//! Funkcja inicjująca statek
int nowystatek(Statek*, int, int, int, orient);
//! Funkcja inicjująca planszę
int nowaplansza(Plansza*);

//! Funkcja wstawiająca statek na planszę
int wrzucnaplansze(Statek*, Plansza*);
//! Funkcja usuwająca z planszy ostatni wstawiony na nią statek
int cofnijzplanszy(Plansza*);

//! Funkcja realizująca strzał w dane pole na planszy
int strzal(Plansza*, int, int);
//! Funkcja zwracająca wartość logiczną, czy na danej planszy gra się już skończyła (nie ma już segmentów statków do zestrzelenia)
int czykoniec(Plansza*);

//! Funkcja generująca planszę na podstawie podanej wartości
int generujplansze(Plansza*, int);
