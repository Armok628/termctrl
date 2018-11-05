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
	if (!f)
		return;
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
	if (!(rand()%20)) {
		struct faction *f=factions[rand()%num_factions];
		report("s s",f->name,"surges forth!");
		for (int i=0;i<5;i++)
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
void place_uprising(struct worldtile *w,int i,struct faction *r)
{
	decr_size(w[i].faction);
	w[i].faction=r;
	incr_size(r);
	for (int i=0;i<20;i++)
		spread_faction(w,r);
}
static struct faction *territory_search=NULL;
// ^ Static variable gets around callback limitations
bool in_territory(struct worldtile *w,int i)
{
	return w[i].faction==territory_search;
}
void random_rebellion(struct worldtile *w,struct faction *f)
{
	if (!f)
		return;
	report("s ss","A rebellion occurs under",f->name,"!");
	struct faction *r=random_faction();
	r->color=f->color;
	recolor_faction(r);
	territory_search=f;
	int p=rand_loc(w,&in_territory);
	place_uprising(w,p,r);
}
bool coastal(struct worldtile *w,int p)
{
	if (w[p].elev<500)
		return false;
	for (int dx=-1;dx<=1;dx++)
		for (int dy=-1;dy<=1;dy++)
			if ((dx||dy)&&w[p+dx+dy*W_HEIGHT].elev<500)
				return true;
	return false;
}
bool raidable(struct worldtile *w,int p)
{
	return coastal(w,p)&&!in_territory(w,p);
}
void coastal_raid(struct worldtile *w,struct faction *f)
{
	struct faction *raid_party=malloc(sizeof(struct faction));
	territory_search=f;
	int landing=rand_loc(w,&raidable);
	if (!landing)
		return;
	place_uprising(w,landing,raid_party);
	for (int i=0;i<W_AREA;i++) {
		if (w[i].faction==raid_party) {
			w[i].faction=f;
			incr_size(f);
		}
	}
	free(raid_party);
}
