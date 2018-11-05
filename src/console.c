#include "console.h"
extern int player_pos;
void command(struct tile *z,int pos)
{
	char buf[128],cmd[128];
	next_report();
	fputs("Enter command: ",stdout);
	set_cursor_visible(true);
	set_canon(true);
	fgets(buf,128,stdin);
	set_cursor_visible(false);
	set_canon(false);
	int n=1;
	if (sscanf(buf,"%d%*c%128s",&n,cmd)!=2)
		if (!sscanf(buf,"%128[^\n]",cmd))
			return;
	if (!strcmp(cmd,"think")) {
		report("Thought: %c",think(z,target(z,pos)));
	} else if (!strcmp(cmd,"swap")) {
		int p=target(z,pos);
		if (z[p].e)
			player_pos=p;
	} else if (!strcmp(cmd,"kill")) {
		int p=target(z,pos);
		if (z[p].e)
			kill_entity(z,p);
		draw_pos(z,p);
	} else if (!strcmp(cmd,"resurrect")) {
		int p=target(z,pos);
		if (z[p].c&&!z[p].e) {
			z[p].e=z[p].c;
			z[p].c=NULL;
			z[p].e->hp=z[p].e->maxhp;
		}
		draw_pos(z,p);
	} else if (!strcmp(cmd,"teleport")) {
		move_entity(z,pos,target(z,pos));
	} else if (!strcmp(cmd,"rand_room")) {
		for (int i=0;i<n;i++)
			rand_room(z);
		draw_zone(z);
	} else if (!strcmp(cmd,"fix_rooms")) {
		for (int i=0;i<n;i++)
			fix_rooms(z);
		draw_zone(z);
	} else if (!strcmp(cmd,"fix_gaps")) {
		for (int i=0;i<n;i++)
			fix_gaps(z);
		draw_zone(z);
	} else if (!strcmp(cmd,"rand_path")) {
		for (int i=0;i<n;i++)
			rand_path(z);
		draw_zone(z);
	} else if (!strcmp(cmd,"snow")) {
		draw_zone(z);
		weather=&snow;
	} else if (!strcmp(cmd,"rain")) {
		draw_zone(z);
		weather=&rain;
	} else if (!strcmp(cmd,"clear")) {
		draw_zone(z);
		weather=NULL;
	}
}
