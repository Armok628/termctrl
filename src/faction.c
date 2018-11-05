#include "faction.h"
int spread_influence(struct worldtile *w,int pos)
{
	struct faction *f=w[pos].faction;
	int choices[9],n=0;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++) {
			int i=pos+dx+dy*W_WIDTH;
			if (!legal_world_move(pos,i))
				continue;
			if (w[i].elev>=500&&w[i].faction!=f)
				choices[n++]=i;
		}
	if (!n)
		return pos;
	return choices[rand()%n];
}
void spread_faction(struct worldtile *w,struct faction *f)
{
	struct faction *t[W_AREA]; // Territory map
	for (int i=0;i<W_AREA;i++)
		t[i]=w[i].faction;
	for (int i=0;i<W_AREA;i++) {
		if (t[i]!=f)
			continue;
		int target=spread_influence(w,i);
		if (target==i)
			continue;
		if (!t[i]&&rand()%2)
			w[target].faction=f;
		else
			switch (rand()%3) {
			case 0:
				w[target].faction=f;
				break;
			case 1:
				w[target].faction=NULL;
				break;
			case 2:
				break;
			}
	}
}
#define MAX_FACTIONS 64
static struct faction *factions[MAX_FACTIONS];
static int num_factions=0;
void spread_all_factions(struct worldtile *w)
{
	for (int i=0;i<num_factions;i++)
		spread_faction(w,factions[i]);
}
void recolor_faction(struct faction *f)
{
	static range_t faction_colors={RED,LIGHT_GRAY};
	f->color=rrand(faction_colors);
}
struct faction *random_faction(void)
{
	if (num_factions==MAX_FACTIONS)
		return factions[rand()%num_factions];
	struct faction *f=malloc(sizeof(struct faction));
	f->name=random_word(3+rand()%3);
	f->name[0]+='A'-'a';
	recolor_faction(f);
	factions[num_factions++]=f;
	return f;
}
