/*!
 * \file main.c
 * \brief Plik główny
 * 
 * W tym pliku zaimplementowany jest główny mechanizm gry:
 * 	- główna pętla gry
 * 	- funkcje wywoływane po nastąpieniu danego stanu
 * 	- obsługa mechanizmu turowego
 * 
 */

#include<ncurses.h>
#include<stdio.h>
#include"Screen.h"
#include"Menu.h"
#include"ResHan.h"
#ifndef BOARDVIEWER_H
#include"BoardViewer.h"
#define BOARDVIEWER_H
#endif
#include"Brain.h"
#include"BoardSetter.h"
#include"NumGet.h"
#include"StringGet.h"
#ifndef LOGGER_H
#include"Logger.h"
#define LOGGER_H
#endif
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<signal.h>
//! Obiekt reprezentujący menu główne
Menu mainmenu;
//! Obiekt reprezentujący menu wyboru poziomu trudności
Menu levelmenu;
//! Obiekt reprezentujący menu wyboru sposobu generowania planszy
Menu genmenu;
//! Obiekt reprezentujący menu "Czy jesteś pewien?"
Menu suremenu;
//! Obiekt reprezentujący menu "Czy chcesz wyeksportować przebieg rozgrywki?"
Menu logexpmenu;
//! Obiekt reprezentujący menu wyboru typu planszy (kwadrat/torus)
Menu torusmenu;
//! Obiekt reprezentujący menu wyboru przeciwnika (czlowiek/komputer)
Menu oppmenu;
//! Obiekt reprezentujący ekran wprowadzania rozmiaru planszy
NumGet sizeng;
//! Obiekt reprezentujący ekran wprowadzania nazwy pliku
StringGet sg;
//! Obiekt reprezentujący moduł zapisujący przebieg rozgrywki
Logger lg;

////! Wskaźnik na aktualnie wyświetlane menu
extern Menu* aktmenu;

//! Aktualny stan programu (co jest aktualnie wyświetlane na ekranie)
enum {BYE, MENU, QUIT, LEVEL, GEN, GRA, BLAD, OPPMENU, TORMENU, METAST, RECZNIE, LOSOWO, ZPLIKU, GSIZE} stan;

//! Wybrany poziom trudności
enum {LATWY=0, SREDNI=1, TRUDNY=2} trud;

//! Wybrany przeciwnik
enum {VSCPU, VSP2} opp;

//! Numer aktualnie obsługiwanego gracza w menu
int menuplayer;

//! Wybrany typ planszy
VIEWT brdt;

int brdsize=10;

/** @name Napisy
 * używane w górnym i dolnym pasku
 */
//@{
char* mennag = "Statki ULTIMATE, Konrad Sikorski 2014";
char* levnag = "Wybierz poziom trudnosci:";
char* gennag = "Wybierz sposob generowania planszy:";
char* oppnag = "Wybierz przeciwnika:";
char* tornag = "Wybierz typ planszy:";
char* surenag = "Czy na pewno chcesz zakonczyc gre?";
char* logexpnag = "Czy chcesz wyeksportowac zapis przebiegu rozgrywki?";

char* err = "Wystapil blad.";
char* exported = "Wyeksportowano pomyslnie!";

char* ngnag = "Wybierz rozmiar planszy:";
char* ngstop = "Uzyj strzalek aby zwiekszyc lub zmniejszyc wartosc.";

char* pname1 = "Gracz 1";
char* pname2 = "Gracz 2";

char* forbid = "Ruch zabroniony.";

char* usermiss = "Spudlowales!";
char* usershot = "Trafiony!";
char* userdest = "Trafiony-zatopiony!";
char* userwin = "Wygrales!";

char* cpumiss = "Na szczescie, komputer spudlowal!";
char* cpushot = "Doznales obrazen!";
char* cpuwin = "Przegrales!";

char* author = "Konrad Sikorski";
char* pusty = "";
char* menstop = "Poruszanie sie po menu: strzalki / ENTER / BACKSPACE";
char* steering = "Uzyj strzalek, aby wybrac pole do zaatakowania.";
char* steering2 = "Wcisnij ENTER, aby zaatakowac.";
char* anybut = "Nacisnij dowolny klawisz, aby kontynuowac.";

char* niema = "Nie ma statkow na planszy.";
char* notable = "Nie mozna tu umiescic statku.";

char* steering3 = "Uzyj strzalek, aby wybrac miejsce.";
char* steering4 = "Spacja - obrot / Enter - akceptacja";
//@}

////! Wskaźnik na napis, który ma być wyświetlany w nagłówku
extern char* naglowek;
////! Wskaźnik na napis, który ma być wyświetlany w nagłówku
extern char* stopka;

//! Obiekt reprezentujący planszę użytkownika (Gracza 1)
Plansza plansza1;
//! Obiekt reprezentujący planszę komputera / gracza drugiego
Plansza plansza2;
//! Wskaźnik na aktualnie ustawianą planszę
Plansza* aktplansza;
//! Obiekt reprezentujący wyświetlacz planszy gracza (dla Gracza 1)
BoardViewer lewa1;
//! Obiekt reprezentujący wyświetlacz planszy przeciwnika (dla Gracza 1)
BoardViewer prawa1;
//! Obiekt reprezentujący wyświetlacz planszy gracza (dla Gracza 2)
BoardViewer lewa2;
//! Obiekt reprezentujący wyświetlacz planszy przeciwnika (dla Gracza 2)
BoardViewer prawa2;
//! Wskaźnik na aktualnie wyświetlany wyświetlacz po lewej stronie
BoardViewer *aktlewa;
//! Wskaźnik na aktualnie wyświetlany wyświetlacz po prawej stronie
BoardViewer *aktprawa;
//! Obiekt reprezentujący narzędzie do ustawiania statków
BoardSetter ustawiacz;
//! Obiekt reprezentujący sztuczną inteligencję komputera
Brain mozg;

//! Napisy do wyświetlenia w głównym menu
char* maincontent[3] = {"Rozpocznij gre", "Wyjscie"};
//! Napisy do wyświetlenia w menu wyboru poziomu trudności
char* levelcontent[3] = {"Latwy", "Sredni", "Trudny"};
//! Napisy do wyświetlenia w menu wyboru sposobu generowania planszy
char* gencontent[3] = {"Recznie", "Losowo", "Z pliku"};
//! Napisy do wyświetlenia w menu "Czy jesteś pewien?"
char* surecontent[2] = {"Nie, wracam do gry", "Tak, chce zakonczyc gre"};
//! Napisy do wyświetlenia w menu "Czy chcesz wyeksportować przebieg rozgrywki?"
char* logexpcontent[2] = {"Nie, dziekuje","Tak, poprosze"};
//! Napisy do wyświetlenia w menu wyboru typu planszy (kwadrat/torus)
char* toruscontent[2] = {"Kwadrat", "Torus"};
//! Napisy do wyświetlenia w menu wyboru przeciwnika (człowiek/komputer)
char* oppcontent[2] = {"Komputer", "Czlowiek"};

//! Funkcja inicjalizująca wszystkie menu, które będą wyświetlane w programie
void genmenus(){
	newmenu(&mainmenu,2,maincontent,30);
	newmenu(&levelmenu,3,levelcontent,30);
	newmenu(&genmenu,3,gencontent,30);
	newmenu(&suremenu,2,surecontent,30);
	newmenu(&torusmenu,2,toruscontent,30);
	newmenu(&oppmenu,2,oppcontent,30);
	newmenu(&logexpmenu,2,logexpcontent,30);
	
	newng(&sizeng,10,10,100,30);
}

//! Funkcja reprezentująca stan programu przed wyjściem (QUIT)
int quit(){
	endwin();
	puts("Do zobaczenia!\n");
	return BYE;
}

//! Funkcja wyświetlająca menu "Czy jesteś pewien?"
int suref(){
	aktmenu = &suremenu;
	naglowek = surenag;
	stopka = pusty;
	signal(SIGWINCH,everytimerh);//rhmenu);
	myclear();
	drawmenu(aktmenu);
	
	int x;
	while(1){
		chtype c = getch();
		switch(c){
			case KEY_UP:
				prevchoice(aktmenu);
				break;
			case KEY_DOWN:
				nextchoice(aktmenu);
				break;
			case KEY_BACKSPACE:
				aktmenu->now = 0;
				return 0;
			case '\n':
				x =aktmenu->now;
				aktmenu->now = 0;
				return x;
		}
		updatemenu(aktmenu);
	}
	return 1;
}

//! Funkcja odpowiadająca za obsługę wyboru użytkownika w menu "Czy jesteś pewien?" (uruchamiana podczas rozgrywki)
int sure(){
	int res = suref();
	if(res) return 1;
	signal(SIGWINCH,everytimerh);//rhgame);
	naglowek = steering;
	stopka = steering2;
	myclear();
	drawbv(&lewa1);
	drawbv(&prawa1);
	return 0;
}

//! Funkcja odpowiadająca za obsługę wyboru użytkownika w menu "Czy jesteś pewien?" (uruchamiana podczas ustawiania planszy)
int sure2(){
	int res = suref();
	if(res) return 1;
	signal(SIGWINCH,everytimerh);//rhbs);
	//naglowek = steering3;
	//stopka = steering4;
	myclear();
	drawbs(&ustawiacz);
	return 0;
}

//! Funkcja obsługująca pojedynczy ruch na zadanej planszy
/** Zwraca stan gry po wykonaniu ruchu */
int ruch(Plansza *p, int l, int c){
	int t = strzal(p,l,c);
	switch(t){
		case -1: return FORB;
		case 0: return MISS;
		case 1: return SHOT;
		case 2: return DESTROYED;
		case 3: return WIN;
	}
	return ERR;
}

//! Funkcja obsługująca przebieg tury
/** Zwraca wartość logiczną, oznaczającą, czy po wykonaniu danej tury rozgrywka ma być nadal kontynuowana */
int tura(Plansza* p, int l, int c, Logger* lg, int pid){
	int x = ruch(p,l,c);	
	if(x != FORB) addmove(lg,l,c,pid,x);
	
	switch(x){
		case FORB:
			stopka = forbid;
			return 1;
		case MISS:
			stopka = usermiss;
			return -1;
		case SHOT:
			stopka = usershot;
			return 1;
		case DESTROYED:
			stopka = userdest;
			return 1;
		case WIN:
			return 0;
	}
	return BLAD;
}

//! Funkcja obsługująca wyświetlanie ekranu zmiany gracza
void chscreen(int id){
	if(id==1) naglowek = "Pudlo! Oczekiwanie na Gracza 1";
	if(id==2) naglowek = "Pudlo! Oczekiwanie na Gracza 2";
	stopka=anybut;
	myclear();
	getch();
}

//! Funkcja obsługująca wyświetlanie ekranu końca rozgrywki
void winscreen(int id){
	if(id==1) naglowek = "Wygral Gracz 1!";
	if(id==2) naglowek = "Wygral Gracz 2!";
	stopka=anybut;
	myclear();
	getch();
}

//! Funkcja wyświetlająca menu "Czy chcesz wyeksportować przebieg rozgrywki?"
int logexpf(){
	aktmenu = &logexpmenu;
	naglowek = logexpnag;
	stopka = pusty;
	signal(SIGWINCH,everytimerh);//rhmenu);
	myclear();
	drawmenu(aktmenu);
	
	int x;
	while(1){
		chtype c = getch();
		switch(c){
			case KEY_UP:
				prevchoice(aktmenu);
				break;
			case KEY_DOWN:
				nextchoice(aktmenu);
				break;
			case KEY_BACKSPACE:
				aktmenu->now = 0;
				return 0;
			case '\n':
				x =aktmenu->now;
				aktmenu->now = 0;
				return x;
		}
		updatemenu(aktmenu);
	}
	return 1;
}

//! Funkcja obsługująca zapytanie użytkownika o eksport przebiegu rozgrywki
void logexp(Logger* lg){
	int czy = logexpf();
	if(czy){
		if(exportlog(lg)) printscr(err,anybut);
		else printscr(exported,anybut);
	}
}

//! Funkcja reprezentująca stan programu, oznaczający rozgrywkę (GRA)
int gra(){	// zaklada, ze plansza1 jest ustawiona
	signal(SIGWINCH,everytimerh);//rhgame);
	if(opp == VSP2) printscr("Oczekiwanie na Gracza 1",anybut);
		
	newbv(&lewa1,&plansza1,0,SELF,LEFT);
	newbv(&prawa1,&plansza2,1,OPP,RIGHT);
	
	newbv(&lewa2,&plansza2,0,SELF,LEFT);
	newbv(&prawa2,&plansza1,1,OPP,RIGHT);
	
	newlog(&lg,brdsize,22,22);
	
	int aktgracz=1;
	
	aktlewa=&lewa1;
	aktprawa=&prawa1;
	
	int isrunning=1;
	Plansza* oppbrd;
	naglowek = pname1;
	stopka = pusty;
	while(isrunning){	
		int l,c;
		if(opp==VSCPU && aktgracz==2){
			oppbrd = &plansza1;
			getmove(&mozg,&l,&c);
		}
		else{
			myclear();
			drawbv(aktlewa);
			drawbv(aktprawa);
			drawlog(&lg);
			while(1){
				updatebv(aktlewa);
				updatebv(aktprawa);

				chtype ch = getch();
				switch(ch){
					case KEY_UP:
						bvup(aktprawa);
						break;
					case KEY_DOWN:
						bvdown(aktprawa);
						break;
					case KEY_LEFT:
						bvleft(aktprawa);
						break;
					case KEY_RIGHT:
						bvright(aktprawa);
						break;
					case 'w':
						bvup(aktlewa);
						break;
					case 's':
						bvdown(aktlewa);
						break;
					case 'a':
						bvleft(aktlewa);
						break;
					case 'd':
						bvright(aktlewa);
						break;
					case KEY_BACKSPACE:
						if(sure()){
							endgame(&lg,IRR);
							logexp(&lg);
							return MENU;
						}
						break;
					case 'H':
						aktprawa->mode^=1;
						break;
					case ' ':
					case '\n':
						l=aktprawa->cy;
						c=aktprawa->cx;
						oppbrd = aktprawa->brd;
						break;
				}
				if(ch=='\n') break;
				//myclear();
				//updatebv(aktlewa);
				//updatebv(aktprawa);
				//updatelog(&lg);
			}
		}
		isrunning=tura(oppbrd,l,c,&lg,aktgracz);
		if(isrunning==-1){
			if(aktgracz==1){
				aktgracz=2;
				if(opp==VSP2)chscreen(aktgracz);
				aktlewa = &lewa2;
				aktprawa = &prawa2;
				naglowek = pname2;
				stopka = pusty;
			}
			else if(aktgracz==2){
				aktgracz=1;
				if(opp==VSP2)chscreen(aktgracz);
				aktlewa = &lewa1;
				aktprawa = &prawa1;
				naglowek = pname1;
				stopka = pusty;
			}
		}
		
	}
	if(aktgracz==1) endgame(&lg,P1);
	if(aktgracz==2) endgame(&lg,P2);
	winscreen(aktgracz);
	logexp(&lg);
	return MENU;
}

//! Funkcja reprezentująca stan programu, oznaczający ręczne ustawianie statków (RECZNIE)
int recznie(){	// ustawia plansza1 TODOOOOOO
	signal(SIGWINCH,everytimerh);//rhbs);
	naglowek = steering3;
	stopka = steering4;
	nowaplansza(aktplansza);
	newbs(&ustawiacz,aktplansza);
	
	myclear();
	
	int isrunning=1;
	
	while(isrunning){
		drawbs(&ustawiacz);
		chtype c = getch();
		switch(c){
			case KEY_UP:
				bsup(&ustawiacz);
				break;
			case KEY_DOWN:
				bsdown(&ustawiacz);
				break;
			case KEY_LEFT:
				bsleft(&ustawiacz);
				break;
			case KEY_RIGHT:
				bsright(&ustawiacz);
				break;
			case KEY_BACKSPACE:
				switch(prevstat(&ustawiacz)){
					case -1:
						return GEN;
					case 0:
						naglowek = pusty;
						stopka = pusty;
						break;
				}
				
				break;
			case ' ':
				bsrot(&ustawiacz);
				break;
			case '\n':
				switch(nextstat(&ustawiacz)){
					case -1:
						naglowek=notable;
						stopka=pusty;
						break;
					case 0:
						naglowek=pusty;
						stopka=pusty;
						break;
					case 1:
						isrunning=0;
						break;
				}
				break;
		}
		myclear();
	}
	fprintf(stderr,"halo\n");	
	return METAST;
}

//! Błędy możliwe do zwrócenia przez funkcję wczytującą planszę z pliku
enum loaderr{NOERR=0, SYNERR, SHIPERR, READERR, SIZERR};

//! Funkcja wczytująca planszę z (wcześniej otwartego) pliku
/** Funkcja nie zamyka pliku, trzeba to obsłużyć poza funkcją. */
int loadboard(Plansza* p, FILE* f){
	// 4, 3, 3, 2, 2, 2, 1, 1, 1, 1
	// 10 linii
	// kazda linia po 3 zmienne - linia, kolumna, orientacja (V/H);
	char buf[11];

	int i;
	for(i=1; i<=4; i++){
		int len=5-i;
		int j;
		for(j=0; j<i; j++){
			Statek s;
			
			if(!fgets(buf,10,f)) return READERR;
			if(buf[strlen(buf)-1] != '\n') return READERR;
			int l,c;
			char o;
			if(sscanf(buf,"%d%d %c ",&l,&c,&o) != 3) return SYNERR;
			if(o!='v' && o!='h') return SYNERR;
			int ori = o=='v' ? VERT : HOR;
			
			if(brdt == TORUS) if(l < 1 || l > brdsize || c < 1 || c > brdsize) return SIZERR;
			if(brdt == KWADRAT){
				if(l<1 || c<1) return SIZERR;
				
				if(ori == VERT){
					if(l+len-1 > brdsize) return SIZERR;
				}
				else if(c+len-1 > brdsize) return SIZERR;
			}
			s.lin = l;
			s.col = c;
			s.ori = ori;
			s.len = len;
			if(wrzucnaplansze(&s,p)) return SHIPERR;
		}
	}
	return NOERR;
}

//! Funkcja reprezentująca stan programu, oznaczający wczytywanie planszy z pliku
int zpliku(){
	naglowek = "Podaj nazwe pliku:";
	stopka = "Aby potwierdzic, wcisnij ENTER";
	nowaplansza(aktplansza);
	newsg(&sg,FILENAME_MAX,30);
	
	myclear();
	drawsg(&sg);
	
	int isrunning=1;
	
	while(isrunning){
		chtype c = getch();
		switch(c){
			case '\n':
				isrunning=0;
				break;
			case KEY_BACKSPACE:
				if(sg.akt==0) return GEN;
			default:
				keyhandler(&sg,c);
				break;
		}
		myclear();
		drawsg(&sg);
	}
	
	FILE* f = fopen(sg.data,"r");
	if(f==NULL){
		printscr("Nie ma takiego pliku!", anybut);
		return ZPLIKU;
	}
	
	int res =loadboard(aktplansza, f);
	if(res){
		switch(res){
			case READERR:
				printscr("Blad odczytu!", anybut);
				break;
			case SIZERR:
				printscr("Statek poza plansza!", anybut);
				break;
			case SHIPERR:
				printscr("Niepoprawne ustawienie!", anybut);
				break;
			case SYNERR:
				printscr("Blad skladni!", anybut);
				break;
		}
		fclose(f);
		return ZPLIKU;
	}
	fclose(f);
	return METAST;
}

//! Funkcja reprezentująca stan programu, oznaczający losowe rozstawienie statków na planszy użytkownika (LOSOWO)
/** W przeciwieństwie do innych stanów nie wchodzi w interakcję z użytkownikiem, ani nic nie wyświetla.*/
int losowo(){	// ustawia plansza1
	nowaplansza(aktplansza);
	generujplansze(aktplansza,rand());
	return METAST;
}

//! Funkcja reprezentująca stan programu, oznaczający menu wyboru sposobu generowania planszy
int gen(){
	aktmenu = &genmenu;
	naglowek = gennag;
	stopka = pusty;
	if(opp == VSP2){
		switch(menuplayer){
			case 1: stopka = pname1; break;
			case 2: stopka = pname2; break;
		}
	}
	signal(SIGWINCH,everytimerh);//rhmenu);
	myclear();
	drawmenu(aktmenu);
	
	switch(menuplayer){
		case 1: aktplansza = &plansza1; break;
		case 2: aktplansza = &plansza2; break;
	}
	
	while(1){
		int c = getch();
		switch(c){
			case KEY_UP:
				prevchoice(aktmenu);
				break;
			case KEY_DOWN:
				nextchoice(aktmenu);
				break;
			case KEY_BACKSPACE:
				if(opp==VSP2){
					if(menuplayer==1) return OPPMENU;
					if(menuplayer==2){
						menuplayer=3;
						return METAST;
					}
				}
				if(opp==VSCPU) return LEVEL;
			case '\n':
				switch(aktmenu->now){
					case 0: return RECZNIE;
					case 1: return LOSOWO;
					case 2: return ZPLIKU;
				}
		}
		updatemenu(aktmenu);
	}
	return QUIT;
}

//! Funkcja reprezentująca stan programu, oznaczający menu wyboru typu planszy (kwadrat/torus)
int tormenu(){
	aktmenu = &torusmenu;
	naglowek = tornag;
	stopka = pusty;
	signal(SIGWINCH,everytimerh);//rhmenu);
	myclear();
	drawmenu(aktmenu);
	
	while(1){
		int c = getch();
		switch(c){
			case KEY_UP:
				prevchoice(aktmenu);
				break;
			case KEY_DOWN:
				nextchoice(aktmenu);
				break;
			case KEY_BACKSPACE:
				return GSIZE;
			case '\n':
				switch(aktmenu->now){
					case 0: brdt=KWADRAT; return OPPMENU;
					case 1: brdt=TORUS; return OPPMENU;
				}
		}
		updatemenu(aktmenu);
	}
	return QUIT;
}

//! Funkcja reprezentująca stan programu, obsługujący niuanse związane z opcją wyboru drugiego gracza
/** W przeciwieństwie do innych stanów nie wchodzi w interakcję z użytkownikiem, ani nic nie wyświetla.*/
int metaf(){
	if(opp==VSP2){
		if(menuplayer==1){
			menuplayer=2;
			return GEN;
		}
		if(menuplayer==2) return GRA;
		if(menuplayer==3){	// menuplayer jest ustawiany na 3, gdy użytkownik chce wrócić z menu wyboru ustawiania planszy dla gracza 2
			int res=suref();
			if(res) return MENU;
			else{
				menuplayer=2;
				return GEN;
			}
		}
	}
	if(opp==VSCPU){
		nowaplansza(&plansza2);
		generujplansze(&plansza2,rand());
		newbrain(&mozg,&plansza1,trud);
		return GRA;
	}
	return BLAD;
}

//! Funkcja reprezentująca stan programu, oznaczający menu główne
int menu(){
	aktmenu = &mainmenu;
	naglowek = "";
	if(stopka!=menstop) stopka = pusty;
	signal(SIGWINCH,everytimerh);//rhmenu);
	everytimerh();//rhmenu();
	myclear();
	drawmenu(aktmenu);
	
	while(1){
		int c = getch();
		switch(c){
			case KEY_UP:
				prevchoice(aktmenu);
				break;
			case KEY_DOWN:
				nextchoice(aktmenu);
				break;
			case KEY_BACKSPACE:
				aktmenu->now=1;
				break;
			case '\n':
				switch(aktmenu->now){
					case 0: return GSIZE;
					case 1: return QUIT;
					default: return BLAD;
				}
		}
		updatemenu(aktmenu);
	}
}

//! Funkcja reprezentująca stan programu, oznaczający menu wyboru przeciwnika
int oppm(){
	aktmenu = &oppmenu;
	naglowek = oppnag;
	stopka = pusty;
	signal(SIGWINCH,everytimerh);//rhmenu);
	myclear();
	drawmenu(aktmenu);
	
	menuplayer=1;
	
	while(1){
		int c = getch();
		switch(c){
			case KEY_UP:
				prevchoice(aktmenu);
				break;
			case KEY_DOWN:
				nextchoice(aktmenu);
				break;
			case KEY_BACKSPACE:
				return TORMENU;
				break;
			case '\n':
				switch(aktmenu->now){
					case 0: opp=VSCPU; return LEVEL;
					case 1: opp=VSP2; return GEN;
					default: return BLAD;
				}
		}
		updatemenu(aktmenu);
	}
}

//! Funkcja reprezentująca stan programu, oznaczający menu wyboru poziomu trudności
int level(){
	aktmenu = &levelmenu;
	naglowek = levnag;
	stopka = pusty;
	signal(SIGWINCH,everytimerh);//rhmenu);
	myclear();
	drawmenu(aktmenu);
	
	while(1){
		int c = getch();
		switch(c){
			case KEY_UP:
				prevchoice(aktmenu);
				break;
			case KEY_DOWN:
				nextchoice(aktmenu);
				break;
			case KEY_BACKSPACE:
				return OPPMENU;
			case '\n':
				trud = aktmenu->now;
				return GEN;
		}
		updatemenu(aktmenu);
	}
	
	return QUIT;
}

//! Funkcja reprezentująca stan programu, oznaczający ekran pobierania od użytkownika wielkości planszy
int gsize(){
	naglowek = ngnag;
	stopka = ngstop;
	myclear();
	drawng(&sizeng);
	
	while(1){
		int c = getch();
		switch(c){
			case KEY_UP:
				incrng(&sizeng);
				break;
			case KEY_RIGHT:
				incr10ng(&sizeng);
				break;
			case KEY_DOWN:
				decrng(&sizeng);
				break;
			case KEY_LEFT:
				decr10ng(&sizeng);
				break;
			case KEY_BACKSPACE:
				return MENU;
			case '\n':
				brdsize = sizeng.now;
				return TORMENU;
		}
		myclear();
		drawng(&sizeng);
	}
	
	return QUIT;
}

//! Główna funkcja
/** Zawiera inicjalizację ncurses, kolorów, wszystkich menu oraz napisu w dolnej belce, a następnie pętlę gry. Mechanizm przechodzenia pomiędzy różnymi stanami gry odbywa się następująco: w głównej pętli gry zapamiętywany jest aktualny stan gry. Następnie uruchamiana jest funkcja reprezentująca dany stan. Wartość przez nią zwrócona określa stan, do którego należy przejść. */
int main(){
	srand(time(NULL));
	init();
	init_pair(1,COLOR_GREEN,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_RED,COLOR_BLACK);
	init_pair(4,COLOR_BLUE,COLOR_BLACK);
	init_pair(5,COLOR_WHITE,COLOR_BLACK);
	
	genmenus();
	stopka=menstop;
	stan=MENU;
	
	while(1){
		switch(stan){
			case MENU:
				stan=menu();
				break;
			case QUIT:
				stan=quit();
				break;
			case GRA:
				stan=gra();
				break;
			case LEVEL:
				stan=level();
				break;
			case GEN:
				stan=gen();
				break;
			case RECZNIE:
				stan=recznie();
				break;
			case LOSOWO:
				stan=losowo();
				break;
			case ZPLIKU:
				stan=zpliku();
				break;
			case OPPMENU:
				stan=oppm();
				break;
			case TORMENU:
				stan=tormenu();
				break;
			case METAST:
				stan=metaf();
				break;
			case GSIZE:
				stan=gsize();
				break;
			case BYE:
				return 0;
			default:
				move(1,1);
				printw("ERROR");
				getch();
				endwin();
				return 1;
		}
	}

}
