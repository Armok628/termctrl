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
void decr_size(struct faction *f)
{
	if (!f)
		return;
	f->size--;
	if (f->size)
		return;
	int i=0;
	for (;i<num_factions;i++)
		if (factions[i]==f)
			break;
	report("s s",f->name,"has been obliterated!");
	free(f->name);
	free(f);
	factions[i]=factions[--num_factions];
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
		if (!t[i]&&rand()%2) {
			w[target].faction=f;
			incr_size(f);
		} else {
			struct faction *t=w[target].faction;
			switch (rand()%3) {
			case 0:
				decr_size(t);
				incr_size(f);
				w[target].faction=f;
				break;
			case 1:
				decr_size(t);
				w[target].faction=NULL;
				break;
			case 2:
				break;
			}
		}
	}
}
struct faction *factions[MAX_FACTIONS];
int num_factions=0;
void spread_all_factions(struct worldtile *w)
{
	for (int i=0;i<num_factions;i++)
		spread_faction(w,factions[i]);
	if (num_factions>1&&!(rand()%20)) {
		struct faction *f=factions[rand()%num_factions];
		report("s s",f->name,"has a resurgence!");
		for (int i=0;i<10;i++)
			spread_faction(w,f);
	}
}
void recolor_faction(struct faction *f)
{
	static range_t faction_colors={RED,LIGHT_GRAY};
	if (f) {
		color_t c=f->color;
		while (f->color==c)
			f->color=rrand(faction_colors);
	}
}
struct faction *random_faction(void)
{
	if (num_factions==MAX_FACTIONS)
		return factions[rand()%num_factions];
	struct faction *f=malloc(sizeof(struct faction));
	f->name=random_word(3+rand()%3);
	f->size=0;
	f->name[0]+='A'-'a';
	f->color=BLACK;
	recolor_faction(f);
	factions[num_factions++]=f;
	return f;
}
