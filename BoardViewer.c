/*!
 * \file BoardViewer.c
 * \brief Plik źródłowy obsługi wyświetlania planszy
 */

#include "BoardViewer.h"

//! Szerokość wyświetlanego pola
const int WIDTH=4;
//! Wysokość wyświetlanego pola
const int HEIGHT=2;

extern int brdsize, brdt;

//! Tablica kolorów do wyświetlania planszy
static const int colors[3][4]={{4,2,4,3},{5,5,4,3},{0,0,0,0}};

void newbv(BoardViewer* bv, Plansza* p, int curs, int mode, int pan){
	bv -> cur = curs;
	if(brdt==TORUS){
		bv -> cx = 5;
		bv -> cy = 5;
	}
	else{
		bv -> cx = 1;
		bv -> cy = 1;
	}
	bv -> mode = mode;
	bv -> brd = p;
	bv -> pan = pan;
	
	bv->vpl = 1;
	bv->vpc = 1;
}

void drawbv(BoardViewer* bv){
	WINDOW *w = bv->win;
	delwin(w);
	int l,c;
	l = (getmaxy(stdscr)-11*HEIGHT)/2;

	int maxx=getmaxx(stdscr)/2 - 11;
	c = (maxx-WIDTH*11)/2;
	if(bv->pan==RIGHT) c += 22+  maxx;
	bv->win = newwin(11*HEIGHT,11*WIDTH,l,c);
	w = bv->win;
	
	updatebv(bv);
	
}

//! Tablica znaków kursora
/** Zawiera znaki, które są rysowane w miejscu, gdzie znajduje się kursor. */
static chtype cursor[2][4] = {{'/',' ',' ','\\'},{'\\',' ',' ','/'}};

//! Funkcja rysująca pojedyncze pole planszy
static void fillgap(WINDOW* w, int sl, int sc, int con, int mode, int cur){
	int i,j;
	chtype ch = con/2 ? A_STANDOUT|' ' : ACS_CKBOARD;
	for(i=0; i<HEIGHT; i++){
		for(j=0; j<WIDTH; j++){
			if(cur) ch = A_STANDOUT|cursor[i][j];
			mvwaddch(w,sl+i, sc+j,ch|COLOR_PAIR(colors[mode][con]));
		}
	}
}

//! Funkcja pisząca pojedynczą współrzędną przy wyświetlaczu
static void fillnum(WINDOW* w, int sl, int sc, int con){
	mvwprintw(w,sl+1,sc,"%2.d",con);
}

void updatebv(BoardViewer* bv){
	WINDOW* w = bv->win;
	Plansza* p = bv->brd;
	
	int i,j;
	int n=brdsize;
	for(i=0; i<10; i++){
		fillnum(w,0,(i+1)*WIDTH,((bv->vpc+i-1+n)%n)+1);
		fillnum(w,(i+1)*HEIGHT,0,((bv->vpl+i-1+n)%n)+1);
		for(j=0; j<10; j++){
			fillgap(w,	(i+1)*HEIGHT,		(j+1)*WIDTH,		p->tab[((bv->vpl+i-1)%n)+1][((bv->vpc+j-1)%n)+1],	bv->mode,	(bv->cur && ((bv->vpl+i-1)%n)+1==bv->cy && ((bv->vpc+j-1)%n)+1==bv->cx));
		}
	}
	
	wrefresh(w);
}

void bvup(BoardViewer* bv){
	
	if(bv->pan == LEFT){
		bv->vpl--;
		if(brdt == KWADRAT && bv->vpl == 0) bv->vpl=brdsize-9;
		if(brdt == TORUS && bv->vpl == 0) bv->vpl=brdsize;
		return;
	}
	
	bv->cy--;
	if(brdt==KWADRAT){
		if(!bv->cy){
			bv->cy=brdsize;
			bv->vpl=brdsize-10+1;
		}
		else if(bv->cy < bv-> vpl) bv->vpl = bv->cy;
	}
	if(brdt==TORUS){
		if(!bv->cy) bv->cy=brdsize;
		bv->vpl--;
		if(!bv->vpl) bv->vpl=brdsize;
	}
}

void bvdown(BoardViewer* bv){
	if(bv->pan == LEFT){
		bv->vpl++;
		if(brdt == TORUS && bv->vpl == brdsize+1) bv->vpl=1;
		if(brdt == KWADRAT && bv->vpl+9 == brdsize+1) bv->vpl=1;
		return;
	}
	
	bv->cy++;
	if(brdt==KWADRAT){
		if(bv->cy==brdsize+1){
			bv->cy=1;
			bv->vpl=1;
		}
		else if(bv->cy >= bv->vpl+10) bv->vpl++;
	}
	if(brdt==TORUS){
		if(bv->cy == brdsize+1) bv->cy=1;
		bv->vpl++;
		if(bv->vpl == brdsize+1) bv->vpl=1;
	}
}

void bvleft(BoardViewer* bv){
	if(bv->pan == LEFT){
		bv->vpc--;
		if(brdt == KWADRAT && bv->vpc == 0) bv->vpc=brdsize-9;
		if(brdt == TORUS && bv->vpc == 0) bv->vpc=brdsize;
		return;
	}
	
	bv->cx--;
	if(brdt==KWADRAT){
		if(!bv->cx){
			bv->cx=brdsize;
			bv->vpc=brdsize-10+1;
		}
		else if(bv->cx < bv-> vpc) bv->vpc = bv->cx;
	}
	if(brdt==TORUS){
		if(!bv->cx) bv->cx=brdsize;
		bv->vpc--;
		if(!bv->vpc) bv->vpc=brdsize;
	}
}

void bvright(BoardViewer* bv){	
	if(bv->pan == LEFT){
		bv->vpc++;
		if(brdt == TORUS && bv->vpc == brdsize+1) bv->vpc=1;
		if(brdt == KWADRAT && bv->vpc+9 == brdsize+1) bv->vpc=1;
		return;
	}
	
	bv->cx++;
	if(brdt==KWADRAT){
		if(bv->cx==brdsize+1){
			bv->cx=1;
			bv->vpc=1;
		}
		else if(bv->cx >= bv->vpc+10) bv->vpc++;
	}
	if(brdt==TORUS){
		if(bv->cx == brdsize+1) bv->cx=1;
		bv->vpc++;
		if(bv->vpc == brdsize+1) bv->vpc=1;
	}
}
