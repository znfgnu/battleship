/*!
 * \file Logger.c
 * \brief Plik źródłowy obsługi modułu zapisującego przebieg rozgrywki
 */

#ifndef LOGGER_H
#include"Logger.h"
#define LOGGER_H
#endif

#include<stdlib.h>
#include<string.h>

void newlm(LogMove* lm, int l, int c, int pid, int res){
	lm->l=l;
	lm->c=c;
	lm->pid=pid;
	lm->res=res;
}

void newlog(Logger* lg, int size, int width, int height){
	if(lg->size) free(lg->data);
	lg->size = size;
	lg->data = malloc(2*size*size*sizeof(LogMove));
	lg->wsk = 0;
	lg->width=width;
	lg->height=height;
	lg->begin = time(NULL);
}

void addmove(Logger* lg, int l, int c, int pid, int res){
	newlm(&(lg->data[lg->wsk]),l,c,pid,res);
	lg->wsk++;
}

void endgame(Logger* lg, int reason){
	lg->reason = reason;
	lg->end = time(NULL);
}

extern int brdt, brdsize;

//jesli ok - zwroc 0
int exportlog(Logger* lg){
	//TODO: format wyjscia
	
	char* pref = "gra-";
	char* suff = ".html";
	int len = strlen(pref)+4+2+2+2+2+2+5+strlen(suff);
	char filename[len+1];
	
	struct tm *s;
	s = localtime(&(lg->begin));
	
	sprintf(filename,"%s%4.d-%2.d-%2.d-%2.d-%2.d-%2.d%s",pref, s->tm_year+1900, s->tm_mon+1, s->tm_mday, s->tm_hour, s->tm_min, s->tm_sec  ,suff);
	int i;
	for(i=0; i<len+1; i++) if(filename[i]==' ') filename[i]='0';
	
	FILE* f = fopen(filename,"w");
	if(!f) return 1;
	
	char tytul[256];
	
	sprintf(tytul,"Zapis rozgrywki z %d.%d%d.%d, %d:%d%d:%d%d",
		s->tm_mday,
		
		(s->tm_mon+1)/10,
		(s->tm_mon+1)%10,
		
		s->tm_year+1900,
		
		s->tm_hour,
		(s->tm_min)/10,
		(s->tm_min)%10,
		(s->tm_sec)/10,
		(s->tm_sec)%10);
	
	fprintf(f,"<html><head><title>%s</title></head>",tytul);
	fprintf(f,"<body bgcolor=\"silver\"><p>%s</p><p>Rozmiar planszy: %d<br>Typ planszy: %s</p><table border=\"1\" cellpadding=\"10\">",tytul,brdsize,brdt ? "normalna" : "torus");
	fprintf(f,"<tr><td>Nr ruchu</td><td>Gracz</td><td>Linia</td><td>Kolumna</td><td>Rezultat</td></tr>");
	
	for(i=0; i<lg->wsk; i++){
		LogMove* lm = &(lg->data[i]);
		lm->pid--;
		
		char* wyn;
		char* kolortla;
		
		switch(lm->res){
			case MISS:
				wyn = "Pudlo";
				kolortla = lm->pid ? "maroon" : "green";
				break;
			case SHOT:
				wyn = "Trafiony";
				kolortla = lm->pid ? "maroon" : "green";
				break;
			case DESTROYED:
			case WIN:
				wyn = "Zatopiony";
				kolortla = lm->pid ? "red" : "lime";
				break;
		}
		fprintf(f,"<tr bgcolor=\"%s\"><td>%d</td><td>%s</td><td>%d</td><td>%d</td><td>%s</td></tr>",kolortla,i+1,lm->pid ? "P2" : "P1",lm->l, lm->c, wyn);
		
		lm->pid++;
	}
	
	fprintf(f,"</table><p>");
	
	switch(lg->reason){
		case P1:
			fprintf(f,"Wygral gracz 1.\n");
			break;
		case P2:
			fprintf(f,"Wygral gracz 2.\n");
			break;
		case IRR:
			fprintf(f,"Rozgrywka niezakonczona.\n");
			break;
	}
	
	fprintf(f,"</p></body></html>");
	
	fclose(f);
	return 0;
}

void drawlog(Logger* lg){
	WINDOW *w = lg->win;
	delwin(w);
	int l,c;
	l = (getmaxy(stdscr)-lg->height)/2;
	int maxx=getmaxx(stdscr);
	c = (maxx-lg->width)/2;
	lg->win = newwin(lg->height,lg->width,l,c);
	w = lg->win;
	
	updatelog(lg);	
}

void updatelog(Logger* lg){
	WINDOW* w = lg->win;
	int i=0;
	for(i=0; i<lg->height; i++){
		if(lg->wsk-lg->height+i >=0){
			LogMove* lm = &(lg->data[lg->wsk-lg->height+i]);
			lm->pid--;
			
			char* wyn;
			switch(lm->res){
				case MISS:
					wyn = "Pudlo";
					break;
				case SHOT:
					wyn = "Trafiony";
					break;
				case DESTROYED:
				case WIN:
					wyn = "Zatopiony";
					break;
			}
			
			mvwprintw(w,i,0," %s %3.d %3.d %s",lm->pid ? "P2" : "P1",lm->l, lm->c, wyn);
			switch(lm->pid){
				case 0:
					mvwchgat(w,i, 0, -1, A_STANDOUT, 1, NULL);
					break;
				case 1:
					mvwchgat(w,i, 0, -1, A_NORMAL, 1, NULL);
					break;
			}
			lm->pid++;
		}
	}
	wrefresh(w);
}
