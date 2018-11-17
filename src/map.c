#include "map.h"
int world_pos=-1;
#ifdef SCROLL
int w_offset_x=0;
int w_offset_y=0;
void scroll_map(int pos)
{
	w_offset_x=G_WIDTH/2-pos%W_WIDTH;
	w_offset_y=G_HEIGHT/2-pos/W_WIDTH;
}
#endif
void draw_world_pos(struct worldtile *w,int pos)
{
	int x=pos%W_WIDTH,y=pos/W_WIDTH;
#ifdef SCROLL
	x+=w_offset_x;
	y+=w_offset_y;
	if (x>=0&&x<G_WIDTH&&y>=0&&y<G_HEIGHT) // If x,y is on-screen
#endif
	{
		next_draw(x,y);
		draw_worldtile(&w[pos]);
	}
}
void draw_world(struct worldtile *w)
{
#ifdef SCROLL
	for (int x=0;x<G_WIDTH;x++)
		for (int y=0;y<G_HEIGHT;y++) {
			int x2=x-w_offset_x,y2=y-w_offset_y;
			next_draw(x,y);
			if (x2<0||x2>=W_WIDTH||y2<0||y2>=W_HEIGHT)
				draw(' ',RESET,RESET);
			else
				draw_worldtile(&w[x2+y2*W_WIDTH]);
		}
#else
	for (int i=0;i<W_AREA;i++)
		draw_world_pos(w,i);
#endif
}
bool legal_world_move(int from,int to)
{
	int dx=to%W_WIDTH-from%W_WIDTH;
	return -1<=dx&&dx<=1&&0<=to&&to<W_AREA;
}
#define REPORT_SORTED_FACTIONS(f,memb) \
do {\
	int indices[num_factions]; \
	for (int i=0;i<num_factions;i++) \
		indices[i]=i; \
	sort(indices,num_factions,&f); \
	for (int i=0;i<num_factions;i++) { \
		next_report(); \
		set_bg(factions[indices[i]]->color); \
		putchar(' '); \
		sgr(RESET); \
		printf(" %s\t",factions[indices[i]]->name); \
		printf("(%d)",factions[indices[i]]->memb); \
	} \
} while (0);
void open_map(struct worldtile *w)
{
	clear_screen();
#ifdef SCROLL
	report_height=G_HEIGHT;
#else
	report_height=W_HEIGHT;
#endif
	draw_world(w);
	int dt=1;
	while (/**/!exit_req/**/) {
		move_cursor(0,report_height);
		if (w[world_pos].faction) {
			struct faction *f=w[world_pos].faction;
			report_here("%s (Size: %d, Stagnation: %d, Age: %d)",
					f->name,f->size,f->stagnation,f->age);
		} else
			report_here("Unoccupied territory");
		color_t fc=w[world_pos].faction?w[world_pos].faction->color:BLACK;
#ifdef SCROLL
		int x=world_pos%W_WIDTH+w_offset_x;
		int y=world_pos/W_WIDTH+w_offset_y;
		next_draw(x,y);
#else
		next_draw(world_pos%W_WIDTH,world_pos/W_WIDTH);
#endif
		draw('@',LIGHT_RED,fc);
		char c=key();
		clear_reports();
		draw_world_pos(w,world_pos);
		int to=world_pos+input_offset_width(c,W_WIDTH);
		if (to!=world_pos&&legal_world_move(world_pos,to)) {
			world_pos=to;
#ifdef SCROLL
			clock_t t=clock();
			scroll_map(world_pos);
			draw_world(w);
			t=clock()-t;
			report("Scroll took %fms",1000.0*t/CLOCKS_PER_SEC);
#endif
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
			report("dt=%d, %fms",dt,1000.0*t/CLOCKS_PER_SEC);
			report("Turn: %d",turn+=dt);
		} else if (c=='#') {
			char s[1000];
			move_cursor(0,report_height);
			clear_line();
			report_here("dt=");
			set_canon(true);
			set_cursor_visible(true);
			fgets(s,100,stdin);
			sscanf(s,"%d",&dt);
			if (dt<0)
				dt=1;
			set_cursor_visible(false);
			set_canon(false);
		} else if (c=='R')
			redraw();
		else if (c=='A')
			REPORT_SORTED_FACTIONS(descending_age,age)
		else if (c=='S')
			REPORT_SORTED_FACTIONS(descending_size,size)
		/**/
	}
}
