/*!
 * \file Statki.c
 * \brief Plik źródłowy obsługi planszy
 */
#include"Statki.h"
#include"Util.h"

#include<ncurses.h>

extern int brdsize, brdt;

int nowystatek(Statek* s, int l, int k, int len, orient o){
	s->lin=l;
	s->col=k;
	s->len=len;
	s->ori=o;
	return 0;
}

/** Funkcja ustawia stan wszystkich pól na puste. */
int nowaplansza(Plansza* p){
	p->shot=0;
	p->ile=0;
	int i,j;
	
	if(p->sizebefore){
		for(i=0; i<p->sizebefore+2; i++) free(p->tab[i]);
		free(p->tab);
	}
	
	p->tab = malloc((brdsize+2) * sizeof(pole*));
	for(i=0; i<brdsize+2; i++){
		p->tab[i] = malloc((brdsize+2)*sizeof(pole));
		for(j=0; j<brdsize+2; j++){
			p->tab[i][j]=NONE;
		}
	}
	p->sizebefore = brdsize;
	return 0;
}

/** Funkcja zakłada, że podany przez argument statek mieści się na planszy.
 * Funkcja sprawdza, czy dany statek można ustawić w danym miejscu - sprawdza, czy na polach zajmowanych przez niego i sąsiadujących do nich (również po skosie) nie stoi żaden segment statku.
 * Funkcja zwraca:
 * - -1, jeśli statek nie może zostać wstawiony na planszę
 * - 0, gdy statek spełnia warunki wstawienia na planszę (oraz wstawia statek na planszę) */
int wrzucnaplansze(Statek *s, Plansza *p){
	// zakladamy, ze statek miesci sie na planszy
	// zwraca -1, gdy pozycja jest niedozwolona (konflikt z innym statkiem)
	
	int n=brdsize;
	
	int lb = s->lin;
	int cb = s->col;
	int le,ce;
	if(s->ori == HOR){
		le = lb;
		ce = cb+(s->len)-1;
	}
	else{
		le = lb+(s->len)-1;
		ce = cb;
	}
	lb--;
	cb--;
	le++;
	ce++;
	int i,j;
	for(i=lb; i<=le; i++){
		for(j=cb; j<=ce; j++){
			if(brdt==KWADRAT) if(p->tab[i][j] != NONE) return -1;
			if(brdt==TORUS) if(p->tab[((i-1+n)%n)+1][((j-1+n)%n)+1] != NONE) return -1;
		}
	}
	if(s->ori == HOR) for(i=0; i<s->len; i++) p->tab[s->lin][((s->col+i-1+n)%n)+1]=STAT;
	else for(i=0; i<s->len; i++) p->tab[((s->lin+i-1+n)%n)+1][s->col]=STAT;
	p->kolejne[p->ile]=*s;
	p->ile++;
	return 0;
}

int cofnijzplanszy(Plansza *p){
	if(p->ile == 0) return -1;
	p->ile--;
	int n=brdsize;
	Statek *s=&(p->kolejne[p->ile]);
	int i;
	if(s->ori == HOR) for(i=0; i<s->len; i++) p->tab[s->lin][((s->col+i-1)%n)+1]=NONE;
	else for(i=0; i<s->len; i++) p->tab[((s->lin+i-1)%n)+1][s->col]=NONE;
	return 0;
}

//! Funkcja przeszukiwania grafu wgłąb - do sprawdzania, czy statek zajmujący pole (l,c) jest zatopiony.
/** Argumenty przyjmowane przez funkcję to kolejno:
 * - plansza, na której ma być wykonywane sprawdzenie
 * - linia i kolumna aktualnie sprawdzanego pola
 * - linia i kolumna pola, z którego aktualna instancja funkcji została wywołana - zapobiega wejściu do tego samego pola 2 razy */
static int dfs(Plansza* p, int l, int c, int ll, int lc){
	int n=brdsize;
	if(brdt==KWADRAT){
		if(p->tab[l][c] == STAT ) return 0;
		if(p->tab[l+1][c] == STAT || (p->tab[l+1][c] == STCH && (l+1!=ll || c !=lc ) && !dfs(p,l+1,c,l,c))) return 0;
		if(p->tab[l-1][c] == STAT || (p->tab[l-1][c] == STCH && (l-1!=ll || c !=lc ) && !dfs(p,l-1,c,l,c))) return 0;
		if(p->tab[l][c+1] == STAT || (p->tab[l][c+1] == STCH && (l!=ll || c+1 !=lc ) && !dfs(p,l,c+1,l,c))) return 0;
		if(p->tab[l][c-1] == STAT || (p->tab[l][c-1] == STCH && (l!=ll || c-1 !=lc ) && !dfs(p,l,c-1,l,c))) return 0;
	}
	if(brdt==TORUS){
		if(p->tab[l][c] == STAT ) return 0;
		if(p->tab[((l+1+n-1)%n)+1][c] == STAT || (p->tab[((l+1+n-1)%n)+1][c] == STCH && (((l+1+n-1)%n)+1!=ll || c !=lc ) && !dfs(p,((l+1+n-1)%n)+1,c,l,c))) return 0;
		if(p->tab[((l-1+n-1)%n)+1][c] == STAT || (p->tab[((l-1+n-1)%n)+1][c] == STCH && (((l-1+n-1)%n)+1!=ll || c !=lc ) && !dfs(p,((l-1+n-1)%n)+1,c,l,c))) return 0;
		if(p->tab[l][((c+1+n-1)%n)+1] == STAT || (p->tab[l][((c+1+n-1)%n)+1] == STCH && (l!=ll || ((c+1+n-1)%n)+1 !=lc ) && !dfs(p,l,((c+1+n-1)%n)+1,l,c))) return 0;
		if(p->tab[l][((c-1+n-1)%n)+1] == STAT || (p->tab[l][((c-1+n-1)%n)+1] == STCH && (l!=ll || ((c-1+n-1)%n)+1 !=lc ) && !dfs(p,l,((c-1+n-1)%n)+1,l,c))) return 0;
	}
	return 1;
}

//! Funkcja ustawiająca pole na sprawdzone jeśli było puste
static void mkchkd(Plansza* p, int l, int c){
	if(p->tab[l][c] == NONE) p->tab[l][c] = NOCH;
}

//! Funkcja odsłaniająca pola wokół zadanego pola
/** Funkcja wywołuje się rekurencyjnie dla każdego segmentu statku,
 * dzięki czemu podając jej jeden segment zatopionego statku odsłaniają
 * się pola wokół niego */
static void dfscol(Plansza* p, int l, int c, int ll, int lc){
	if(p->tab[l][c] != STCH) return;
	
	int i,j;
	for(i=-1; i<=1; i++){
		for(j=-1; j<=1; j++){
			if(abs(i)!=abs(j)){
				int nl = ((l+i-1+brdsize)%brdsize)+1;
				int nc = ((c+j-1+brdsize)%brdsize)+1;
				if(nl!=ll || nc!=lc) dfscol(p,nl,nc,l,c);
			}
			
			if(brdt == TORUS) mkchkd(p,((l+i-1+brdsize)%brdsize)+1,((c+j-1+brdsize)%brdsize)+1);
			if(brdt == KWADRAT) mkchkd(p,l+i,c+j);
		}
	}
	
}

/** Funkcja zwraca:
	- -1, gdy strzał nastąpił w miejsce, które już było ostrzeliwane
	- 0, gdy w puste
	- 1, gdy trafia w segment
	- 2, gdy trafia w segment i zatapia statek
	- 3, gdy po wykonaniu strzału na planszy nie został żaden segment
 */
int strzal(Plansza *p, int l, int c){
	
	if(p->tab[l][c]==NONE){
		p->tab[l][c]=NOCH;
		return 0;
	}
	else if(p->tab[l][c]==STAT){
		p->tab[l][c]=STCH;
		p->shot++;
		
		if(czykoniec(p)) return 3;
		if(dfs(p,l,c,l,c)){
			dfscol(p,l,c,l,c);
			return 2;
		}
		return 1;
	}
	else return -1;
}

int czykoniec(Plansza *p){
	return p->shot==20;
}

/** Funkcja ustawia seed do generatora liczb pseudolosowych jako liczbę podaną w parametrze.
 * Następnie, dla każdej długości statku (w kolejności: 4-masztowiec, 2 3-masztowce, 3 2-masztowce, 4 1-masztowce)
 * losuje jego orientację, później pozycję na planszy i wykonuje te czynności dopóki funkcja wstawiająca statek na planszę
 * nie potwierdzi jego wstawienia (dopóki miejsce, na które statek miał zostać wstawiony, będzie zajęte). */
int generujplansze(Plansza *p, int seed){
	srand(seed);
	int i;
	for(i=1; i<=4; i++){
		int len=5-i;
		int j;
		for(j=0; j<i; j++){
			Statek s;
			do{
				orient o=rand()%2;
				if(brdt==KWADRAT){
					if(o==VERT) nowystatek(&s,myrand(1,brdsize+1-len),myrand(1,brdsize),len,o);
					else nowystatek(&s,myrand(1,brdsize),myrand(1,brdsize+1-len),len,o);
				}
				if(brdt==TORUS){
					nowystatek(&s,myrand(1,brdsize),myrand(1,brdsize),len,o);
				}
				
			}while(wrzucnaplansze(&s,p));
		}
	}
	return 0;
}
