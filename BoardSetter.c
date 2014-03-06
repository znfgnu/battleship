/*!
 * \file BoardSetter.c
 * \brief Plik źródłowy ustawiacza statków
 */

#include"BoardSetter.h"

extern const int WIDTH, HEIGHT;
extern int brdsize, brdt;

//! Funkcja ustawiająca widok na planszę tak, aby statek był możliwie jak najbardziej na środku
static void centerview(BoardSetter* bs){
	if(brdt==KWADRAT){
		bs->vpc = bs->s[bs->akt].col-3;
		bs->vpl = bs->s[bs->akt].lin-3;
		if(bs->vpc+9 > brdsize) bs->vpc=brdsize-9;
		if(bs->vpl+9 > brdsize) bs->vpl=brdsize-9;
		
		if(bs->vpc<=0) bs->vpc=1;
		if(bs->vpl<=0) bs->vpl=1;
	}
	if(brdt==TORUS){
		bs->vpc = ((bs->s[bs->akt].col-3-1+brdsize)%brdsize)+1;
		bs->vpl = ((bs->s[bs->akt].lin-3-1+brdsize)%brdsize)+1;	
	}
}

/** Funkcja wypełnia tablicę kolejnych statków do wstawienia od najdłuższego do najkrótszego */
void newbs(BoardSetter* bs, Plansza* p){
	bs -> p = p;
	bs->akt=0;
	
	
	int k=0,i;
	for(i=1; i<=4; i++){
		int len=5-i;
		int j;
		for(j=0; j<i; j++){
			nowystatek(&(bs->s[k++]),1,1,len,VERT);
		}
	}
	
	if(brdt==KWADRAT){
		bs->vpl=1;
		bs->vpc=1;
	}
	if(brdt==TORUS){
		centerview(bs);
	}
}

/** Funkcja zwraca:
 * - -1, gdy ustawienie jest zabronione
 * - 0, gdy wstawiła statek na planszę
 * - 1, gdy wstawiła ostatni statek na planszę
 * */
int nextstat(BoardSetter* bs){
	if(wrzucnaplansze(&(bs->s[bs->akt]),bs->p)) return -1;
	bs->akt++;
	if(bs->akt==10) return 1;
	
	//ustawienie pozycji aktualnego statku na identyczna z ostatnia
	bs->s[bs->akt].lin = bs->s[bs->akt-1].lin;
	bs->s[bs->akt].col = bs->s[bs->akt-1].col;
	
	
	if(brdt==TORUS) centerview(bs);
	
	return 0;
}

/** Funkcja zwraca:
 * - -1, gdy na planszy nie znajduje się jeszcze żaden statek
 * - 0, gdy ostatnio wstawiony statek został usunięty z planszy */
int prevstat(BoardSetter* bs){
	if(cofnijzplanszy(bs->p)) return -1;
	bs->akt--;
	centerview(bs);
	return 0;
	
}

void drawbs(BoardSetter* bs){
	WINDOW *w = bs->win;
	delwin(w);
	int l,c;
	l = (getmaxy(stdscr)-11*HEIGHT)/2;
	c = (getmaxx(stdscr)-WIDTH*11)/2;
	
	bs->win = newwin(11*HEIGHT,11*WIDTH,l,c);
	w = bs->win;
	
	updatebs(bs);
}

//! Enumeracja używana w oznaczeniu stanu pola przy ustawianiu planszy (segment statku znajdujący się na planszy / puste pole / aktualnie wstawiany statek / jego okolica / konfliktowe pole - pole, które powoduje, że dane ustawienie jest nieprawidłowe)
enum{ST=0,EM,NW,OKO,KON};	// statek, puste, nowy, okolica, konflikt

//! Numery kolorów (ncurses), używane do kolorowania pól ustawiacza
static const int colors[5] = {2,4,2,4,3};

//! Funkcja rysująca pojedyncze pole ustawiacza
static void fillgap(WINDOW* w, int sl, int sc, int con){
	int i,j;
	chtype ch = con/2 ? A_STANDOUT|' ' : ACS_CKBOARD;
	for(i=0; i<HEIGHT; i++){
		for(j=0; j<WIDTH; j++){
			mvwaddch(w,sl+i, sc+j,ch|COLOR_PAIR(colors[con]));
		}
	}
}

//! Funkcja pisząca pojedynczą współrzędną przy ustawiaczu
static void fillnum(WINDOW* w, int sl, int sc, int con){
	mvwprintw(w,sl+1,sc,"%2.d",con);
}

void updatebs(BoardSetter* bs){
	WINDOW* w = bs->win;
	Plansza* p = bs->p;
	
	int n = brdsize;
	
	Statek* s = &(bs->s[bs->akt]);
	int tab[brdsize+2][brdsize+2];
	int i,j;
	for(i=0; i<brdsize+2; i++){
		for(j=0; j<brdsize+2; j++) tab[i][j]=0;
	}

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
	for(i=lb; i<=le; i++){
		for(j=cb; j<=ce; j++){
			if(brdt == TORUS) tab[((i-1+n)%n)+1][((j-1+n)%n)+1]=2;	// okolica
			else tab[i][j]=2;
		}
	}
	if(s->ori == HOR) for(i=0; i<s->len; i++) tab[s->lin][((s->col+i-1)%n)+1]=1;	// statek
	else for(i=0; i<s->len; i++) tab[((s->lin+i-1)%n)+1][s->col]=1;
	int ii,jj;
	for(ii=1; ii<=10; ii++){
		fillnum(w, 0, (ii)*WIDTH, ((bs->vpc+-1+brdsize+ii-1)%brdsize)+1);// (s->col-4+ii+brdsize-1)%brdsize+1);
		fillnum(w, (ii)*HEIGHT, 0, ((bs->vpl-1+brdsize+ii-1)%brdsize)+1); //(s->lin-4+ii+brdsize-1)%brdsize+1);
		for(jj=1; jj<=10; jj++){
			i=(bs->vpl-1+ii+brdsize-1)%brdsize+1;
			j=(bs->vpc-1+jj+brdsize-1)%brdsize+1;
			int con;//=p->tab[i][j];
			if(tab[i][j]==2){	// okolica
				if(p->tab[i][j] == STAT) con=KON;
				else con=OKO;
			}
			else if(tab[i][j]==1){	// statek
				if(p->tab[i][j] == STAT) con=KON;
				else con=NW;
			}
			else{
				if(p->tab[i][j]==STAT) con=ST;
				else con=EM;
			}
			fillgap(w, (ii)*HEIGHT, (jj)*WIDTH, con);
		}
	}
	wrefresh(w);
}

void bsup(BoardSetter* bs){
	bs->s[bs->akt].lin--;
	if(brdt==KWADRAT){
		if(bs->s[bs->akt].lin==0){
			bs->vpl=brdsize-9;
			if(bs->s[bs->akt].ori==HOR) bs->s[bs->akt].lin=brdsize;
			else bs->s[bs->akt].lin=brdsize+1-bs->s[bs->akt].len;
		}
		else if(bs->s[bs->akt].lin < bs->vpl) bs->vpl--;
	}
	else{
		bs->vpl--;
		if(bs->vpl == 0) bs->vpl=brdsize;
		if(bs->s[bs->akt].lin==0) bs->s[bs->akt].lin=brdsize;
	}
}
void bsdown(BoardSetter* bs){
	bs->s[bs->akt].lin++;
	if(brdt == KWADRAT){
		if(bs->s[bs->akt].ori==HOR){
			if(bs->s[bs->akt].lin==brdsize+1){
				bs->s[bs->akt].lin=1;
				bs->vpl=1;
			}
			else if(bs->vpl+10 == bs->s[bs->akt].lin) bs->vpl++;
		}
		else{
			if(bs->s[bs->akt].lin==brdsize+2-bs->s[bs->akt].len){
				bs->s[bs->akt].lin=1;
				bs->vpl=1;
			}
			else if(bs->vpl+10+1-bs->s[bs->akt].len == bs->s[bs->akt].lin) bs->vpl++;
		}
	}
	else{
		bs->vpl++;
		if(bs->vpl == brdsize+1) bs->vpl=1;
		if(bs->s[bs->akt].lin==brdsize+1) bs->s[bs->akt].lin=1;
	}
}
void bsleft(BoardSetter* bs){
	bs->s[bs->akt].col--;
	if(brdt==KWADRAT){
		if(bs->s[bs->akt].col==0){
			bs->vpc=brdsize-9;
			if(bs->s[bs->akt].ori==VERT) bs->s[bs->akt].col=brdsize;
			else bs->s[bs->akt].col=brdsize+1-bs->s[bs->akt].len;
		}
		else if(bs->s[bs->akt].col < bs->vpc) bs->vpc--;
	}
	else{
		bs->vpc--;
		if(bs->vpc == 0) bs->vpc=brdsize;
		if(bs->s[bs->akt].col==0) bs->s[bs->akt].col=brdsize;
	}
}
void bsright(BoardSetter* bs){
	bs->s[bs->akt].col++;
	if(brdt == KWADRAT){
		if(bs->s[bs->akt].ori==VERT){
			if(bs->s[bs->akt].col==brdsize+1){
				bs->s[bs->akt].col=1;
				bs->vpc=1;
			}
			else if(bs->vpc+10 == bs->s[bs->akt].col) bs->vpc++;
		}
		else{
			if(bs->s[bs->akt].col==brdsize+2-bs->s[bs->akt].len){
				bs->s[bs->akt].col=1;
				bs->vpc=1;
			}
			else if(bs->vpc+10+1-bs->s[bs->akt].len == bs->s[bs->akt].col) bs->vpc++;
		}
	}
	else{
		bs->vpc++;
		if(bs->vpc == brdsize+1) bs->vpc=1;
		if(bs->s[bs->akt].col==brdsize+1) bs->s[bs->akt].col=1;
	}
}

//! Funkcja określająca, czy dany statek wystaje poza planszę
/** Wykorzystywana przy funkcji obracającej statek */
static int isok(Statek *s){	// czy nie wystaje poza plansze
	if(s->ori==VERT && s->len+s->lin > brdsize+1) return 0;
	if(s->ori==HOR && s->len+s->col > brdsize+1) return 0;
	return 1;
}

/** Statek jest obracany o 90 stopni względem jego skrajnie lewego/górnego segmentu. Jeśli obrót spowoduje wystawanie statku poza planszę, obrót nie jest wykonywany */
void bsrot(BoardSetter* bs){
	Statek* s = &(bs->s[bs->akt]);
	s->ori^=1;
	if(brdt==KWADRAT && !isok(s)) s->ori^=1;
	else if(brdt==KWADRAT){
		switch(s->ori){
			case HOR:
				if(s->col+s->len-1 > bs->vpc+9) bs->vpc = s->col+s->len-10;
				break;
			case VERT:
				if(s->lin+s->len-1 > bs->vpl+9) bs->vpl = s->lin+s->len-10;
				break;
		}
	}
	
}
