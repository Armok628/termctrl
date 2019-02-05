#include "map.h"
int world_pos=-1;
int world_xscroll=0;
int world_yscroll=0;
void scroll_map(int pos)
{
	if (WORLD_HEIGHT>TERM_HEIGHT||WORLD_WIDTH>TERM_WIDTH) {
		world_xscroll=TERM_WIDTH/2-pos%WORLD_WIDTH;
		world_yscroll=TERM_HEIGHT/2-pos/WORLD_WIDTH;
	}
}
void draw_world_pos(struct worldtile *w,int pos)
{
	int x=pos%WORLD_WIDTH,y=pos/WORLD_WIDTH;
	x+=world_xscroll;
	y+=world_yscroll;
	if (x>=0&&x<TERM_WIDTH&&y>=0&&y<TERM_HEIGHT) {
		// ^ If x,y is on-screen
		move(y,x);
		draw_worldtile(&w[pos]);
	}
}
void draw_world(struct worldtile *w)
{
	for (int x=0;x<TERM_WIDTH;x++)
	for (int y=0;y<TERM_HEIGHT;y++) {
		int x2=x-world_xscroll,y2=y-world_yscroll;
		move(y,x);
		if (x2<0||x2>=WORLD_WIDTH||y2<0||y2>=WORLD_HEIGHT)
			addch(' '|color(BLACK,BLACK));
		else
			draw_worldtile(&w[x2+y2*WORLD_WIDTH]);
	}
}
bool legal_world_move(int from,int to)
{
	int dx=to%WORLD_WIDTH-from%WORLD_WIDTH;
	return -1<=dx&&dx<=1&&0<=to&&to<WORLD_AREA;
}
#define REPORT_SORTED_FACTIONS(f,memb) \
do {\
	int indices[num_factions]; \
	for (int i=0;i<num_factions;i++) \
		indices[i]=i; \
	insertion_sort(indices,num_factions,&f); \
	for (int i=0;i<num_factions;i++) { \
		next_report(); \
		addch(' '|color(BLACK,factions[indices[i]]->color)); \
		printw(" %s\t",factions[indices[i]]->name); \
		printw("(%d)",factions[indices[i]]->memb); \
	} \
} while (0);
bool uncivilized_town(struct worldtile *w,int p)
{
	return w[p].town&&!w[p].faction;
}
void open_map(struct worldtile *w)
{
	erase();
	report_height=WORLD_HEIGHT>TERM_HEIGHT?TERM_HEIGHT:WORLD_HEIGHT;
	draw_world(w);
	int dt=1;
	while (/**/!exit_req/**/) {
		move(report_height,0);
		if (w[world_pos].faction) {
			struct faction *f=w[world_pos].faction;
			report_here("%s (Size: %d, Stagnation: %d, Age: %d)",
					f->name,f->size,f->stagnation,f->age);
		} else
			report_here("Unoccupied territory");
		enum color fc=w[world_pos].faction?w[world_pos].faction->color:BLACK;
		int x=world_pos%WORLD_WIDTH+world_xscroll;
		int y=world_pos/WORLD_WIDTH+world_yscroll;
		move(y,x);
		addch('@'|color(LIGHT_RED,fc));
		int c=key();
		clear_reports();
		draw_world_pos(w,world_pos);
		int to=world_pos+input_offset(c,WORLD_WIDTH);
		if (to!=world_pos&&legal_world_move(to,world_pos)) {
			world_pos=to;
			scroll_map(world_pos);
		}
		/**/
		if (c=='\n') { // Add new faction at position
			struct faction *f=w[world_pos].faction;
			if (f)
				f->size--;
			cull_dead_factions();
			f=random_faction();
			w[world_pos].faction=f;
			f->size++;
		} else if (c==' ') { // Spread all factions
			static int turn=0;
			clock_t t=clock();
			for (int i=0;i<dt;i++)
				advance_world(w);
			t=clock()-t;
			report("Turn: %d",turn+=dt);
			report("dt=%d, %fms",dt,1000.0*t/CLOCKS_PER_SEC);
		} else if (c=='#') {
			move(report_height,0);
			clrtoeol();
			noraw();
			echo();
			curs_set(true);
			printw("dt=");
			scanw("%d",&dt);
			if (dt<0)
				dt=1;
			curs_set(false);
			noecho();
			raw();
		} else if (c=='A')
			REPORT_SORTED_FACTIONS(descending_age,age)
		else if (c=='S')
			REPORT_SORTED_FACTIONS(descending_size,size)
		else if (c=='T') {
			int i=rand_loc(w,&uncivilized_town);
			if (!i)
				continue;
			struct faction *f=random_faction();
			w[i].faction=f;
			f->size++;
			draw_world_pos(w,i);
		}
		draw_world(w);
		/**/
	}
}
