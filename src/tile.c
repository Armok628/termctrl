#include "tile.h"
#ifdef SCROLL
int z_offset_x=0;
int z_offset_y=0;
void scroll_zone(int pos)
{
	z_offset_x=G_WIDTH/2-pos%Z_WIDTH;
	z_offset_y=G_HEIGHT/2-pos/Z_WIDTH;
}
#endif
void draw_tile(struct tile *t)
{
	if (t->e)
		draw_entity(t->e);
	else if (t->c)
		draw_entity(t->c);
	else if (t->fg)
		draw(t->fg,t->fg_c,BLACK);
	else if (t->bg)
		draw(t->bg,t->bg_c,BLACK);
}
void free_zone(struct tile *z)
{
	for (int i=0;i<Z_AREA;i++) {
		if (z[i].e)
			free_entity(z[i].e);
		if (z[i].c)
			free_entity(z[i].c);
	}
	free(z);
}
void draw_pos(struct tile *z,int pos)
{
	int x=pos%Z_WIDTH,y=pos/Z_WIDTH;
#ifdef SCROLL
	x+=z_offset_x;
	y+=z_offset_y;
	if (x>=0&&x<G_WIDTH&&y>=0&&y<G_HEIGHT) // If x,y is on-screen
#endif
	{
		next_draw(x,y);
		draw_tile(&z[pos]);
	}
}
void draw_zone(struct tile *z)
{
#ifdef SCROLL
	for (int x=0;x<G_WIDTH;x++)
		for (int y=0;y<G_HEIGHT;y++) {
			int x2=x-z_offset_x,y2=y-z_offset_y;
			int i=x2+y2*Z_WIDTH;
			next_draw(x,y);
			if (x2<0||x2>=Z_WIDTH||y2<0||y2>=Z_HEIGHT) {
				sgr(RESET);
				putchar(' ');
			} else
				draw_tile(&z[i]);
		}
#else
	for (int i=0;i<Z_AREA;i++)
		draw_pos(z,i);
#endif
}
bool legal_move(int from,int to)
{
	int dx=to%Z_WIDTH-from%Z_WIDTH;
	return -1<=dx&&dx<=1&&0<=to&&to<Z_AREA;
}
